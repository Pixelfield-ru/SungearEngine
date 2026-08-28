//
// Created by 8bitniksis on 17.08.2026.
//

#include "VulkanDevice.h"

#include <iterator>

#include "SGCore/Logger/Logger.h"
#include "VulkanCommandList.h"
#include "VulkanDescriptorSet.h"
#include "VulkanGPUBuffer.h"
#include "VulkanPipelineState.h"
#include "VulkanShaderProgram.h"
#include "VulkanTexture.h"

SGCore::VulkanDevice::VulkanDevice(VulkanContext& context, GLFWwindow* window) noexcept : m_context(context)
{
    m_properties.m_apiType = SG_API_TYPE_VULKAN;
    m_properties.m_originBottomLeft = false;
    m_properties.m_depthZeroToOne = !context.m_depthClipControl;
    m_properties.m_ndcYFlipRequired = true;
    m_properties.m_framesInFlight = VulkanSwapchain::frames_in_flight;
    m_properties.m_pushConstantsMaxSize = context.m_physicalDeviceProperties.limits.maxPushConstantsSize;
    m_properties.m_maxTexelBufferElements = context.m_physicalDeviceProperties.limits.maxTexelBufferElements;
    m_properties.m_supportsExplicitBarriers = true;
    m_properties.m_supportsMultithreadedRecording = true;
    m_properties.m_supportsBindless = false;

    VkCommandPoolCreateInfo poolInfo { };
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = context.m_graphicsQueueFamily;
    if(!SG_VK_CHECK(vkCreateCommandPool(context.m_device, &poolInfo, nullptr, &m_commandPool))) return;

    m_descriptorPools.push_back(createDescriptorPool());
    if(m_descriptorPools.back() == VK_NULL_HANDLE) return;

    m_swapchain = std::make_unique<VulkanSwapchain>(*this, window);
    if(!m_swapchain->isValid())
    {
        SG_LOG_E("VulkanDevice: swapchain creation failed.");
        return;
    }

    m_ready = true;
}

SGCore::VulkanDevice::~VulkanDevice()
{
    if(m_context.m_device == VK_NULL_HANDLE) return;

    vkDeviceWaitIdle(m_context.m_device);
    retire(true);
    m_deferredDestroy.clear();
    m_pipelineCache.clear();
    m_swapchain.reset();

    for(const auto pool : m_descriptorPools) vkDestroyDescriptorPool(m_context.m_device, pool, nullptr);
    for(const auto fence : m_freeFences) vkDestroyFence(m_context.m_device, fence, nullptr);
    if(m_commandPool != VK_NULL_HANDLE) vkDestroyCommandPool(m_context.m_device, m_commandPool, nullptr);
}

// ---- IDevice ------------------------------------------------------------------------------------

SGCore::Ref<SGCore::IGPUBuffer> SGCore::VulkanDevice::createBuffer(const GPUBufferDesc& desc) noexcept
{
    auto buffer = MakeRef<VulkanGPUBuffer>(*this, desc);
    return buffer->isValid() ? buffer : nullptr;
}

SGCore::Ref<SGCore::IShaderProgram> SGCore::VulkanDevice::createShaderProgram(const ShaderProgramDesc& desc) noexcept
{
    return MakeRef<VulkanShaderProgram>(*this, desc);
}

SGCore::Ref<SGCore::IDescriptorSet> SGCore::VulkanDevice::createDescriptorSet() noexcept
{
    return MakeRef<VulkanDescriptorSet>(*this);
}

SGCore::Ref<SGCore::IPipelineState> SGCore::VulkanDevice::getOrCreatePipeline(const PipelineStateDesc& desc) noexcept
{
    auto& bucket = m_pipelineCache[desc.hash()];
    for(const auto& existing : bucket)
    {
        if(existing->getDesc().equalsForCache(desc)) return existing;
    }
    auto pipeline = MakeRef<VulkanPipelineState>(*this, desc);
    bucket.push_back(pipeline);
    return pipeline;
}

SGCore::Ref<SGCore::ICommandList> SGCore::VulkanDevice::createCommandList() noexcept
{
    return MakeRef<VulkanCommandList>(*this);
}

void SGCore::VulkanDevice::submit(const Ref<ICommandList>& commandList) noexcept
{
    auto* list = static_cast<VulkanCommandList*>(commandList.get());
    if(!list) return;
    if(list->isRecording()) list->end();
    if(!list->hasRecordedWork()) return;

    const bool touchedSwapchain = list->touchedSwapchain();
    auto submission = list->takeSubmission();
    if(submission.m_commandBuffers.empty()) return;

    if(touchedSwapchain)
    {
        // the first work on the acquired image waits for the presentation engine to release it
        if(const auto acquireSemaphore = m_swapchain->takeAcquireSemaphore(); acquireSemaphore != VK_NULL_HANDLE)
        {
            submission.m_waitSemaphores.push_back(acquireSemaphore);
            submission.m_waitStages.push_back(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT);
        }
    }
    submission.m_keepAlive.push_back(commandList);
    submitRaw(std::move(submission));
}

void SGCore::VulkanDevice::destroyDeferred(Ref<IGPUObject> object) noexcept
{
    if(!object) return;
    // released with the next submission's fence: attaches to whatever gets submitted next, or on waitIdle()
    m_deferredDestroy.push_back(std::move(object));
}

void SGCore::VulkanDevice::waitIdle() noexcept
{
    vkDeviceWaitIdle(m_context.m_device);
    retire(true);
    m_deferredDestroy.clear();
}

// ---- backend services -----------------------------------------------------------------------------

VkCommandBuffer SGCore::VulkanDevice::acquireCommandBuffer() noexcept
{
    if(m_freeCommandBuffers.empty())
    {
        VkCommandBufferAllocateInfo allocateInfo { };
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = m_commandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 8;
        VkCommandBuffer buffers[8] = { };
        if(!SG_VK_CHECK(vkAllocateCommandBuffers(m_context.m_device, &allocateInfo, buffers))) return VK_NULL_HANDLE;
        m_freeCommandBuffers.insert(m_freeCommandBuffers.end(), buffers, buffers + 8);
    }
    const VkCommandBuffer buffer = m_freeCommandBuffers.back();
    m_freeCommandBuffers.pop_back();
    return buffer;
}

VkFence SGCore::VulkanDevice::acquireFence() noexcept
{
    if(!m_freeFences.empty())
    {
        const VkFence fence = m_freeFences.back();
        m_freeFences.pop_back();
        return fence;
    }
    VkFenceCreateInfo fenceInfo { };
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    VkFence fence = VK_NULL_HANDLE;
    SG_VK_CHECK(vkCreateFence(m_context.m_device, &fenceInfo, nullptr, &fence));
    return fence;
}

std::uint64_t SGCore::VulkanDevice::submitRaw(VulkanSubmission&& submission) noexcept
{
    std::vector<VkCommandBufferSubmitInfo> commandBufferInfos;
    for(const auto commandBuffer : submission.m_commandBuffers)
    {
        VkCommandBufferSubmitInfo info { };
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
        info.commandBuffer = commandBuffer;
        commandBufferInfos.push_back(info);
    }
    std::vector<VkSemaphoreSubmitInfo> waitInfos;
    for(std::size_t i = 0; i < submission.m_waitSemaphores.size(); ++i)
    {
        VkSemaphoreSubmitInfo info { };
        info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        info.semaphore = submission.m_waitSemaphores[i];
        info.stageMask = i < submission.m_waitStages.size() ? submission.m_waitStages[i] : VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        waitInfos.push_back(info);
    }
    std::vector<VkSemaphoreSubmitInfo> signalInfos;
    for(const auto semaphore : submission.m_signalSemaphores)
    {
        VkSemaphoreSubmitInfo info { };
        info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        info.semaphore = semaphore;
        info.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        signalInfos.push_back(info);
    }

    VkSubmitInfo2 submitInfo { };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submitInfo.commandBufferInfoCount = static_cast<std::uint32_t>(commandBufferInfos.size());
    submitInfo.pCommandBufferInfos = commandBufferInfos.data();
    submitInfo.waitSemaphoreInfoCount = static_cast<std::uint32_t>(waitInfos.size());
    submitInfo.pWaitSemaphoreInfos = waitInfos.data();
    submitInfo.signalSemaphoreInfoCount = static_cast<std::uint32_t>(signalInfos.size());
    submitInfo.pSignalSemaphoreInfos = signalInfos.data();

    PendingSubmission pending;
    pending.m_fence = acquireFence();
    pending.m_id = ++m_submissionCounter;
    pending.m_payload = std::move(submission);
    // objects queued for deferred destruction ride along with this submission
    for(auto& object : m_deferredDestroy) pending.m_payload.m_keepAlive.push_back(std::move(object));
    m_deferredDestroy.clear();

    SG_VK_CHECK(vkQueueSubmit2(m_context.m_graphicsQueue, 1, &submitInfo, pending.m_fence));
    m_pending.push_back(std::move(pending));
    return m_submissionCounter;
}

void SGCore::VulkanDevice::waitForSubmission(std::uint64_t id) noexcept
{
    if(id == 0) return;
    for(const auto& pending : m_pending)
    {
        if(pending.m_id > id) break;
        vkWaitForFences(m_context.m_device, 1, &pending.m_fence, VK_TRUE, UINT64_MAX);
    }
    retire(false);
}

void SGCore::VulkanDevice::retire(bool waitAll) noexcept
{
    while(!m_pending.empty())
    {
        auto& pending = m_pending.front();
        if(waitAll)
        {
            vkWaitForFences(m_context.m_device, 1, &pending.m_fence, VK_TRUE, UINT64_MAX);
        }
        else if(vkGetFenceStatus(m_context.m_device, pending.m_fence) != VK_SUCCESS)
        {
            break;
        }

        vkResetFences(m_context.m_device, 1, &pending.m_fence);
        m_freeFences.push_back(pending.m_fence);
        for(const auto commandBuffer : pending.m_payload.m_commandBuffers)
        {
            vkResetCommandBuffer(commandBuffer, 0);
            m_freeCommandBuffers.push_back(commandBuffer);
        }
        for(const auto& set : pending.m_payload.m_transientSets)
        {
            vkFreeDescriptorSets(m_context.m_device, set.m_pool, 1, &set.m_set);
        }
        // keepAlive references drop here
        m_pending.pop_front();
    }
}

void SGCore::VulkanDevice::immediateSubmit(const std::function<void(VkCommandBuffer)>& record) noexcept
{
    const VkCommandBuffer commandBuffer = acquireCommandBuffer();
    if(commandBuffer == VK_NULL_HANDLE) return;

    VkCommandBufferBeginInfo beginInfo { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    record(commandBuffer);
    vkEndCommandBuffer(commandBuffer);

    VulkanSubmission submission;
    submission.m_commandBuffers.push_back(commandBuffer);
    const auto id = submitRaw(std::move(submission));
    waitForSubmission(id);
}

VkDescriptorPool SGCore::VulkanDevice::createDescriptorPool() noexcept
{
    const VkDescriptorPoolSize sizes[] = {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4096 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1024 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 8192 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 512 },
        { VK_DESCRIPTOR_TYPE_SAMPLER, 512 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 256 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 512 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 128 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 64 },
    };
    VkDescriptorPoolCreateInfo poolInfo { };
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 4096;
    poolInfo.poolSizeCount = static_cast<std::uint32_t>(std::size(sizes));
    poolInfo.pPoolSizes = sizes;
    VkDescriptorPool pool = VK_NULL_HANDLE;
    SG_VK_CHECK(vkCreateDescriptorPool(m_context.m_device, &poolInfo, nullptr, &pool));
    return pool;
}

SGCore::VulkanTransientDescriptorSet SGCore::VulkanDevice::allocateTransientDescriptorSet(VkDescriptorSetLayout layout) noexcept
{
    VulkanTransientDescriptorSet result;
    if(layout == VK_NULL_HANDLE) return result;

    for(std::size_t attempt = 0; attempt < 2; ++attempt)
    {
        // round-robin over the pools starting with the current one
        for(std::size_t i = 0; i < m_descriptorPools.size(); ++i)
        {
            const std::size_t index = (m_currentDescriptorPool + i) % m_descriptorPools.size();
            VkDescriptorSetAllocateInfo allocateInfo { };
            allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocateInfo.descriptorPool = m_descriptorPools[index];
            allocateInfo.descriptorSetCount = 1;
            allocateInfo.pSetLayouts = &layout;
            VkDescriptorSet set = VK_NULL_HANDLE;
            const VkResult vkResult = vkAllocateDescriptorSets(m_context.m_device, &allocateInfo, &set);
            if(vkResult == VK_SUCCESS)
            {
                m_currentDescriptorPool = index;
                result.m_pool = m_descriptorPools[index];
                result.m_set = set;
                return result;
            }
            if(vkResult != VK_ERROR_OUT_OF_POOL_MEMORY && vkResult != VK_ERROR_FRAGMENTED_POOL)
            {
                SG_VK_CHECK(vkResult);
                return result;
            }
        }
        // every pool is full: grow
        const VkDescriptorPool pool = createDescriptorPool();
        if(pool == VK_NULL_HANDLE) return result;
        m_descriptorPools.push_back(pool);
        m_currentDescriptorPool = m_descriptorPools.size() - 1;
    }
    return result;
}

void SGCore::VulkanDevice::rotateUniformArena() noexcept
{
    m_currentUniformRegion = (m_currentUniformRegion + 1) % uniform_arena_regions;
    auto& region = m_uniformArena[m_currentUniformRegion];
    region.m_currentChunk = 0;
    region.m_usedInChunk = 0;
}

SGCore::UniformSlice SGCore::VulkanDevice::allocateTransientUniforms(std::uint64_t size) noexcept
{
    if(size == 0) return { };

    // a uniform buffer can only be bound at an offset the device allows
    const std::uint64_t alignment = std::max<std::uint64_t>(
        m_context.m_physicalDeviceProperties.limits.minUniformBufferOffsetAlignment, 16);

    auto& region = m_uniformArena[m_currentUniformRegion];

    while(region.m_currentChunk < region.m_chunks.size())
    {
        const auto& chunk = region.m_chunks[region.m_currentChunk];
        const std::uint64_t offset = (region.m_usedInChunk + alignment - 1) / alignment * alignment;
        if(chunk.m_mapped && offset + size <= chunk.m_size)
        {
            region.m_usedInChunk = offset + size;
            return { chunk.m_buffer, offset, static_cast<std::uint8_t*>(chunk.m_mapped) + offset };
        }
        // this chunk is full: continue in the next one, adding it below if there is none
        ++region.m_currentChunk;
        region.m_usedInChunk = 0;
    }

    constexpr std::uint64_t min_chunk_size = 256ull * 1024;

    UniformArenaChunk chunk;
    chunk.m_size = std::max(min_chunk_size, size);

    GPUBufferDesc desc;
    desc.m_size = chunk.m_size;
    desc.m_usage = GPUBufferUsage::SGG_UNIFORM_BUFFER;
    desc.m_access = GPUMemoryAccess::SGG_HOST_VISIBLE;
    desc.m_debugName = "transient_uniforms";

    chunk.m_buffer = MakeRef<VulkanGPUBuffer>(*this, desc);
    if(!chunk.m_buffer->isValid()) return { };
    chunk.m_mapped = chunk.m_buffer->getMappedPointer();
    if(!chunk.m_mapped) return { };

    region.m_chunks.push_back(chunk);
    region.m_currentChunk = region.m_chunks.size() - 1;
    region.m_usedInChunk = size;
    return { chunk.m_buffer, 0, chunk.m_mapped };
}

SGCore::Ref<SGCore::IGPUObject> SGCore::VulkanDevice::getDummyBackendTexture() noexcept
{
    return getDummyTexture();
}

const SGCore::Ref<SGCore::VulkanTexture>& SGCore::VulkanDevice::getDummyTexture() noexcept
{
    if(m_dummyTexture) return m_dummyTexture;

    VulkanTextureDesc desc;
    desc.m_width = 1;
    desc.m_height = 1;
    desc.m_format = VK_FORMAT_R8G8B8A8_UNORM;
    desc.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    desc.m_debugName = "dummy_white";
    m_dummyTexture = VulkanTexture::create(m_context, desc);
    if(!m_dummyTexture) return m_dummyTexture;

    const std::uint8_t white[4] = { 255, 255, 255, 255 };
    auto staging = createStagingBuffer(sizeof(white), "dummy_white_upload");
    if(!staging) return m_dummyTexture;
    std::memcpy(staging->getMappedPointer(), white, sizeof(white));

    immediateSubmit([&](VkCommandBuffer commandBuffer) {
        m_dummyTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        VkBufferImageCopy region { };
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.layerCount = 1;
        region.imageExtent = { 1, 1, 1 };
        vkCmdCopyBufferToImage(commandBuffer, staging->getHandle(), m_dummyTexture->getImage(),
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        m_dummyTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    });

    return m_dummyTexture;
}

SGCore::Ref<SGCore::VulkanGPUBuffer> SGCore::VulkanDevice::createStagingBuffer(std::uint64_t size, const char* debugName) noexcept
{
    GPUBufferDesc desc;
    desc.m_size = size;
    desc.m_usage = GPUBufferUsage::SGG_TRANSFER_SRC | GPUBufferUsage::SGG_TRANSFER_DST;
    desc.m_access = GPUMemoryAccess::SGG_HOST_VISIBLE;
    desc.m_debugName = debugName ? debugName : "staging";
    auto buffer = MakeRef<VulkanGPUBuffer>(*this, desc);
    return buffer->isValid() ? buffer : nullptr;
}
