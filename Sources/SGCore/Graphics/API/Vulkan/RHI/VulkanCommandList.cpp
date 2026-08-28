//
// Created by 8bitniksis on 17.08.2026.
//

#include "VulkanCommandList.h"

#include <algorithm>
#include <cstring>

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/Vulkan/VkTexture2D.h"
#include "SGCore/Logger/Logger.h"
#include "VulkanDescriptorSet.h"
#include "VulkanGPUBuffer.h"
#include "VulkanShaderProgram.h"

SGCore::VulkanCommandList::VulkanCommandList(VulkanDevice& device) noexcept : m_device(device)
{
    m_debugName = "command_list";
}

SGCore::VulkanCommandList::~VulkanCommandList()
{
    // command buffers of an un-submitted recording go back to the device untouched: they are reset
    // by the next vkBeginCommandBuffer
    if(!m_submitted && m_commandBuffer != VK_NULL_HANDLE)
    {
        VulkanSubmission drop;
        drop.m_commandBuffers.push_back(m_commandBuffer);
        if(m_uploadCommandBuffer != VK_NULL_HANDLE) drop.m_commandBuffers.push_back(m_uploadCommandBuffer);
        drop.m_transientSets = std::move(m_submission.m_transientSets);
        // an empty submit retires them through the normal path
        m_device.submitRaw(std::move(drop));
    }
}

void SGCore::VulkanCommandList::resetState() noexcept
{
    m_inPass = false;
    m_targetIsSwapchain = false;
    m_touchedSwapchain = false;
    m_passFormats = { };
    m_passExtent = { };
    m_passColorTextures.clear();
    m_passDepthTexture.reset();
    m_pipeline.reset();
    m_boundPipeline = VK_NULL_HANDLE;
    m_boundLayout = VK_NULL_HANDLE;
    for(auto& slot : m_sets) slot = { };
    m_viewportExplicit = false;
    m_scissorExplicit = false;
    m_viewportDirty = true;
    m_pendingPushConstants.clear();
}

void SGCore::VulkanCommandList::begin() noexcept
{
    // Someone else is still recording into this list — the legacy facades share one, and a pass
    // binds its framebuffer without unbinding the previous one. Finish and SUBMIT that work:
    // vkBeginCommandBuffer below resets the buffer, which would silently discard everything
    // recorded so far (observed as an empty geometry framebuffer on the smoke scene).
    if(m_recording) end();
    if(m_ended && !m_submitted) flushRecordedWork();

    if(m_submitted || m_commandBuffer == VK_NULL_HANDLE)
    {
        m_commandBuffer = m_device.acquireCommandBuffer();
        m_uploadCommandBuffer = m_device.acquireCommandBuffer();
        m_submission = { };
    }
    m_submitted = false;
    m_ended = false;
    m_uploadUsed = false;
    resetState();

    VkCommandBufferBeginInfo beginInfo { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    SG_VK_CHECK(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
    SG_VK_CHECK(vkBeginCommandBuffer(m_uploadCommandBuffer, &beginInfo));
    m_recording = true;
}

void SGCore::VulkanCommandList::endUploadRecording() noexcept
{
    if(m_uploadUsed)
    {
        // uploads land before anything the main buffer reads
        VkMemoryBarrier2 barrier { };
        barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2;
        barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
        barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
        barrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        barrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT | VK_ACCESS_2_MEMORY_WRITE_BIT;
        VkDependencyInfo dependency { };
        dependency.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
        dependency.memoryBarrierCount = 1;
        dependency.pMemoryBarriers = &barrier;
        vkCmdPipelineBarrier2(m_uploadCommandBuffer, &dependency);
    }
    SG_VK_CHECK(vkEndCommandBuffer(m_uploadCommandBuffer));
}

void SGCore::VulkanCommandList::end() noexcept
{
    if(!m_recording) return;
    if(m_inPass) endRenderPass();

    endUploadRecording();
    SG_VK_CHECK(vkEndCommandBuffer(m_commandBuffer));
    m_recording = false;
    m_ended = true;
}

void SGCore::VulkanCommandList::flushRecordedWork() noexcept
{
    const bool touchedSwapchain = m_touchedSwapchain;
    auto submission = takeSubmission();
    if(submission.m_commandBuffers.empty()) return;

    if(touchedSwapchain)
    {
        if(const auto acquireSemaphore = m_device.getVulkanSwapchain().takeAcquireSemaphore(); acquireSemaphore != VK_NULL_HANDLE)
        {
            submission.m_waitSemaphores.push_back(acquireSemaphore);
            submission.m_waitStages.push_back(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT);
        }
    }
    m_device.submitRaw(std::move(submission));
}

SGCore::VulkanSubmission SGCore::VulkanCommandList::takeSubmission() noexcept
{
    VulkanSubmission submission = std::move(m_submission);
    m_submission = { };
    if(!m_ended || m_submitted)
    {
        return { };
    }
    submission.m_commandBuffers.clear();
    if(m_uploadUsed) submission.m_commandBuffers.push_back(m_uploadCommandBuffer);
    else
    {
        // an unused upload buffer still has to be recycled: hand it over as a no-op
        submission.m_commandBuffers.push_back(m_uploadCommandBuffer);
    }
    submission.m_commandBuffers.push_back(m_commandBuffer);
    m_submitted = true;
    return submission;
}

// ---- render pass -------------------------------------------------------------------------------

void SGCore::VulkanCommandList::beginRenderPass(const RenderPassBeginDesc& desc) noexcept
{
    if(!m_recording) begin();
    if(m_inPass) endRenderPass();

    m_passColorTextures.clear();
    m_passDepthTexture.reset();
    m_passFormats = { };
    m_targetIsSwapchain = desc.m_frameBuffer == nullptr;

    if(m_targetIsSwapchain)
    {
        auto& swapchain = m_device.getVulkanSwapchain();
        if(!swapchain.ensureAcquired())
        {
            SG_LOG_E("VulkanCommandList: can not begin a render pass on the window, no swapchain image is available.");
            return;
        }
        auto texture = swapchain.getCurrentTexture();
        m_passColorTextures.push_back(texture);
        m_passExtent = texture->getExtent();
        m_touchedSwapchain = true;
    }
    else
    {
        std::vector<SGFrameBufferAttachmentType> colorTypes = desc.m_colorAttachments;
        if(colorTypes.empty())
        {
            for(const auto& [type, texture] : desc.m_frameBuffer->getAttachments())
            {
                if(isColorAttachment(type)) colorTypes.push_back(type);
            }
            std::sort(colorTypes.begin(), colorTypes.end());
        }
        for(const auto type : colorTypes)
        {
            const auto attachment = desc.m_frameBuffer->getAttachment(type);
            const auto* vkAttachment = dynamic_cast<VkTexture2D*>(attachment.get());
            if(!vkAttachment || !vkAttachment->getVulkanTexture())
            {
                SG_LOG_E("VulkanCommandList: framebuffer attachment {} is not a Vulkan texture.", static_cast<int>(type));
                continue;
            }
            m_passColorTextures.push_back(vkAttachment->getVulkanTexture());
        }
        for(const auto& [type, texture] : desc.m_frameBuffer->getAttachments())
        {
            if(!isDepthAttachment(type) && !isDepthStencilAttachment(type)) continue;
            const auto* vkAttachment = dynamic_cast<VkTexture2D*>(texture.get());
            if(vkAttachment && vkAttachment->getVulkanTexture())
            {
                m_passDepthTexture = vkAttachment->getVulkanTexture();
                break;
            }
        }
        m_passExtent = { static_cast<std::uint32_t>(desc.m_frameBuffer->getWidth()), static_cast<std::uint32_t>(desc.m_frameBuffer->getHeight()) };
        if(!m_passColorTextures.empty()) m_passExtent = m_passColorTextures.front()->getExtent();
        else if(m_passDepthTexture) m_passExtent = m_passDepthTexture->getExtent();
    }

    if(m_passColorTextures.empty() && !m_passDepthTexture)
    {
        SG_LOG_E("VulkanCommandList: render pass without attachments.");
        return;
    }

    // layouts (barriers are illegal inside dynamic rendering, so all of them go here)
    std::vector<VkRenderingAttachmentInfo> colorInfos;
    for(const auto& texture : m_passColorTextures)
    {
        texture->recordTransition(m_commandBuffer, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        m_passFormats.m_colorFormats.push_back(texture->getFormat());

        VkRenderingAttachmentInfo info { };
        info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        info.imageView = texture->getView();
        info.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        info.loadOp = desc.m_colorLoadOp == LoadOp::SGG_CLEAR ? VK_ATTACHMENT_LOAD_OP_CLEAR
                      : desc.m_colorLoadOp == LoadOp::SGG_DONT_CARE ? VK_ATTACHMENT_LOAD_OP_DONT_CARE : VK_ATTACHMENT_LOAD_OP_LOAD;
        info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        info.clearValue.color = { { desc.m_clearColor.r, desc.m_clearColor.g, desc.m_clearColor.b, desc.m_clearColor.a } };
        colorInfos.push_back(info);
    }

    VkRenderingAttachmentInfo depthInfo { };
    VkRenderingAttachmentInfo stencilInfo { };
    if(m_passDepthTexture)
    {
        const bool hasStencil = (m_passDepthTexture->getAspect() & VK_IMAGE_ASPECT_STENCIL_BIT) != 0;
        m_passDepthTexture->recordTransition(m_commandBuffer, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        m_passFormats.m_depthFormat = m_passDepthTexture->getFormat();
        if(hasStencil) m_passFormats.m_stencilFormat = m_passDepthTexture->getFormat();

        depthInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        depthInfo.imageView = m_passDepthTexture->getView();
        depthInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthInfo.loadOp = desc.m_depthLoadOp == LoadOp::SGG_CLEAR ? VK_ATTACHMENT_LOAD_OP_CLEAR
                           : desc.m_depthLoadOp == LoadOp::SGG_DONT_CARE ? VK_ATTACHMENT_LOAD_OP_DONT_CARE : VK_ATTACHMENT_LOAD_OP_LOAD;
        depthInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depthInfo.clearValue.depthStencil = { desc.m_clearDepth, 0 };
        stencilInfo = depthInfo;
    }

    VkRenderingInfo renderingInfo { };
    renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea.offset = { 0, 0 };
    renderingInfo.renderArea.extent = m_passExtent;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = static_cast<std::uint32_t>(colorInfos.size());
    renderingInfo.pColorAttachments = colorInfos.data();
    renderingInfo.pDepthAttachment = m_passDepthTexture ? &depthInfo : nullptr;
    renderingInfo.pStencilAttachment = m_passDepthTexture && m_passFormats.m_stencilFormat != VK_FORMAT_UNDEFINED ? &stencilInfo : nullptr;

    // a named region per pass: without it a capture is one flat list of draws
    {
        std::string label = m_targetIsSwapchain ? "swapchain pass" : "framebuffer pass";
        label += " " + std::to_string(m_passExtent.width) + "x" + std::to_string(m_passExtent.height) +
                 ", colors " + std::to_string(colorInfos.size()) + (m_passDepthTexture ? ", depth" : "");
        m_device.getContext().beginDebugLabel(m_commandBuffer, label);
    }

    vkCmdBeginRendering(m_commandBuffer, &renderingInfo);
    m_inPass = true;

    // pipeline variants depend on the pass: rebind at the next draw
    m_boundPipeline = VK_NULL_HANDLE;
    m_boundLayout = VK_NULL_HANDLE;
    m_viewportExplicit = false;
    m_scissorExplicit = false;
    m_viewportDirty = true;
    for(auto& slot : m_sets) slot.m_materializedLayout = VK_NULL_HANDLE;
    for(const auto& texture : m_passColorTextures) m_submission.m_keepAlive.push_back(texture);
    if(m_passDepthTexture) m_submission.m_keepAlive.push_back(m_passDepthTexture);
}

void SGCore::VulkanCommandList::endRenderPass() noexcept
{
    if(!m_inPass) return;
    vkCmdEndRendering(m_commandBuffer);
    m_device.getContext().endDebugLabel(m_commandBuffer);
    m_inPass = false;

    // offscreen targets rest in SHADER_READ_ONLY so later passes can sample them without a barrier
    // inside their render pass; the swapchain image is transitioned by present()
    if(!m_targetIsSwapchain)
    {
        for(const auto& texture : m_passColorTextures)
        {
            texture->recordTransition(m_commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        if(m_passDepthTexture)
        {
            m_passDepthTexture->recordTransition(m_commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
    }
    m_passColorTextures.clear();
    m_passDepthTexture.reset();
}

// ---- state ------------------------------------------------------------------------------------

void SGCore::VulkanCommandList::bindPipeline(const Ref<IPipelineState>& pipeline) noexcept
{
    m_pipeline = std::static_pointer_cast<VulkanPipelineState>(pipeline);
    m_boundPipeline = VK_NULL_HANDLE;
    if(m_pipeline) m_submission.m_keepAlive.push_back(m_pipeline);
}

void SGCore::VulkanCommandList::bindDescriptorSet(std::uint32_t setIndex, const Ref<IDescriptorSet>& set) noexcept
{
    if(setIndex >= max_descriptor_sets)
    {
        SG_LOG_E("VulkanCommandList: descriptor set index {} exceeds the supported {} sets.", setIndex, max_descriptor_sets);
        return;
    }
    auto& slot = m_sets[setIndex];
    slot.m_set = std::static_pointer_cast<VulkanDescriptorSet>(set);
    slot.m_materializedVersion = ~0ull;
    slot.m_vkSet = VK_NULL_HANDLE;
    if(slot.m_set) m_submission.m_keepAlive.push_back(slot.m_set);
}

void SGCore::VulkanCommandList::pushConstants(const void* data, std::uint32_t size, std::uint32_t offset) noexcept
{
    if(!data || size == 0) return;
    PendingPushConstants pending;
    pending.m_data.assign(static_cast<const std::uint8_t*>(data), static_cast<const std::uint8_t*>(data) + size);
    pending.m_offset = offset;
    m_pendingPushConstants.push_back(std::move(pending));
}

void SGCore::VulkanCommandList::bindVertexBuffer(std::uint32_t slot, const Ref<IGPUBuffer>& buffer, std::uint64_t offset) noexcept
{
    const auto* vkBuffer = static_cast<const VulkanGPUBuffer*>(buffer.get());
    if(!vkBuffer || !vkBuffer->isValid() || !m_recording) return;
    const VkBuffer handle = vkBuffer->getHandle();
    const VkDeviceSize vkOffset = offset;
    vkCmdBindVertexBuffers(m_commandBuffer, slot, 1, &handle, &vkOffset);
    m_submission.m_keepAlive.push_back(buffer);
}

void SGCore::VulkanCommandList::bindIndexBuffer(const Ref<IGPUBuffer>& buffer, SGIndexType indexType, std::uint64_t offset) noexcept
{
    const auto* vkBuffer = static_cast<const VulkanGPUBuffer*>(buffer.get());
    if(!vkBuffer || !vkBuffer->isValid() || !m_recording) return;
    vkCmdBindIndexBuffer(m_commandBuffer, vkBuffer->getHandle(), offset,
                         indexType == SGIndexType::SGG_UINT16 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);
    m_submission.m_keepAlive.push_back(buffer);
}

void SGCore::VulkanCommandList::setViewport(const Viewport& viewport) noexcept
{
    m_viewport = viewport;
    m_viewportExplicit = true;
    m_viewportDirty = true;
}

void SGCore::VulkanCommandList::setScissor(const Scissor& scissor) noexcept
{
    m_scissor = scissor;
    m_scissorExplicit = true;
    m_viewportDirty = true;
}

void SGCore::VulkanCommandList::applyViewportScissor() noexcept
{
    const float targetHeight = static_cast<float>(m_passExtent.height);

    Viewport viewport = m_viewport;
    if(!m_viewportExplicit)
    {
        viewport = { 0.0f, 0.0f, static_cast<float>(m_passExtent.width), targetHeight, 0.0f, 1.0f };
    }
    Scissor scissor = m_scissor;
    if(!m_scissorExplicit)
    {
        // GL default: no scissor -> the viewport rectangle
        scissor = { static_cast<std::int32_t>(viewport.m_x), static_cast<std::int32_t>(viewport.m_y),
                    static_cast<std::int32_t>(viewport.m_width), static_cast<std::int32_t>(viewport.m_height) };
    }

    VkViewport vkViewport { };
    VkRect2D vkScissor { };
    if(m_targetIsSwapchain)
    {
        // window: flip so the picture is upright on screen; GL (x, y from bottom, w, h) becomes a
        // negative-height Vulkan viewport with y at the rectangle's bottom edge in top-down coordinates
        vkViewport.x = viewport.m_x;
        vkViewport.y = targetHeight - viewport.m_y;
        vkViewport.width = viewport.m_width;
        vkViewport.height = -viewport.m_height;
        vkScissor.offset = { scissor.m_x, static_cast<std::int32_t>(targetHeight) - scissor.m_y - scissor.m_height };
    }
    else
    {
        // offscreen: no flip, memory layout matches GL (row 0 = NDC y -1)
        vkViewport.x = viewport.m_x;
        vkViewport.y = viewport.m_y;
        vkViewport.width = viewport.m_width;
        vkViewport.height = viewport.m_height;
        vkScissor.offset = { scissor.m_x, scissor.m_y };
    }
    vkViewport.minDepth = viewport.m_minDepth;
    vkViewport.maxDepth = viewport.m_maxDepth;
    vkScissor.extent = { static_cast<std::uint32_t>(std::max(scissor.m_width, 0)), static_cast<std::uint32_t>(std::max(scissor.m_height, 0)) };
    // clamp the scissor into the target
    if(vkScissor.offset.x < 0) { vkScissor.extent.width = static_cast<std::uint32_t>(std::max<std::int32_t>(0, static_cast<std::int32_t>(vkScissor.extent.width) + vkScissor.offset.x)); vkScissor.offset.x = 0; }
    if(vkScissor.offset.y < 0) { vkScissor.extent.height = static_cast<std::uint32_t>(std::max<std::int32_t>(0, static_cast<std::int32_t>(vkScissor.extent.height) + vkScissor.offset.y)); vkScissor.offset.y = 0; }
    vkScissor.extent.width = std::min(vkScissor.extent.width, m_passExtent.width - std::min<std::uint32_t>(m_passExtent.width, static_cast<std::uint32_t>(vkScissor.offset.x)));
    vkScissor.extent.height = std::min(vkScissor.extent.height, m_passExtent.height - std::min<std::uint32_t>(m_passExtent.height, static_cast<std::uint32_t>(vkScissor.offset.y)));

    vkCmdSetViewport(m_commandBuffer, 0, 1, &vkViewport);
    vkCmdSetScissor(m_commandBuffer, 0, 1, &vkScissor);
    m_viewportDirty = false;
}

VkDescriptorSet SGCore::VulkanCommandList::materializeSet(SetSlot& slot, std::uint32_t setIndex) noexcept
{
    const auto* program = m_pipeline->getProgram();
    const auto& layouts = program->getSetLayouts();
    if(setIndex >= layouts.size()) return VK_NULL_HANDLE;

    const auto transient = m_device.allocateTransientDescriptorSet(layouts[setIndex]);
    if(transient.m_set == VK_NULL_HANDLE) return VK_NULL_HANDLE;
    m_submission.m_transientSets.push_back(transient);

    std::vector<VkWriteDescriptorSet> writes;
    std::vector<VkDescriptorBufferInfo> bufferInfos;
    std::vector<VkDescriptorImageInfo> imageInfos;
    bufferInfos.reserve(slot.m_set->getEntries().size());
    imageInfos.reserve(slot.m_set->getEntries().size());

    for(const auto& [key, entry] : slot.m_set->getEntries())
    {
        const auto [binding, arrayIndex] = key;
        const auto* reflected = program->findBinding(setIndex, binding);
        if(!reflected) continue; // bound but not used by this program: fine, as on GL

        VkWriteDescriptorSet write { };
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = transient.m_set;
        write.dstBinding = binding;
        write.dstArrayElement = arrayIndex;
        write.descriptorCount = 1;
        write.descriptorType = VulkanShaderProgram::descriptorTypeToVk(reflected->m_type);

        if(entry.m_buffer)
        {
            const auto* buffer = static_cast<const VulkanGPUBuffer*>(entry.m_buffer.get());
            if(!buffer->isValid()) continue;
            VkDescriptorBufferInfo info { };
            info.buffer = buffer->getHandle();
            info.offset = entry.m_offset;
            info.range = entry.m_range == 0 ? VK_WHOLE_SIZE : entry.m_range;
            bufferInfos.push_back(info);
            write.pBufferInfo = &bufferInfos.back();
            m_submission.m_keepAlive.push_back(entry.m_buffer);
        }
        else if(entry.m_texelBufferView != VK_NULL_HANDLE)
        {
            write.pTexelBufferView = &entry.m_texelBufferView;
        }
        else if(entry.m_vulkanTexture || entry.m_texture)
        {
            const auto* vkTexture = entry.m_texture ? dynamic_cast<const VkTexture2D*>(entry.m_texture.get()) : nullptr;
            auto texture = entry.m_vulkanTexture ? entry.m_vulkanTexture
                                                 : (vkTexture ? vkTexture->getVulkanTexture() : nullptr);
            if(!texture || texture->getView() == VK_NULL_HANDLE || texture->getSampler() == VK_NULL_HANDLE) continue;

            // The passes bind every attachment of a framebuffer as a texture up front and only then
            // pick which one to draw into (PostProcessPass does exactly this), so a descriptor can end
            // up pointing at the very image this pass renders to. Sampling it is undefined on GL and a
            // validation error here, and the descriptor still has to be written or the driver drops the
            // draw — so it gets the dummy. No shader of the engine reads what it writes; if one ever
            // does, it will read white and this is the place to look.
            if(isPassAttachment(texture))
            {
                const auto& dummy = m_device.getDummyTexture();
                if(!dummy || dummy->getView() == VK_NULL_HANDLE) continue;
                texture = dummy;
            }

            if(texture->getLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            {
                if(m_inPass)
                {
                    SG_LOG_W("VulkanCommandList: texture '{}' is sampled while not in SHADER_READ_ONLY layout (bound inside a render pass).", texture->getDebugName());
                }
                else
                {
                    texture->recordTransition(m_commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                }
            }
            VkDescriptorImageInfo info { };
            info.sampler = texture->getSampler();
            info.imageView = texture->getView();
            info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfos.push_back(info);
            write.pImageInfo = &imageInfos.back();
            m_submission.m_keepAlive.push_back(texture);
        }
        else
        {
            // cubemaps arrive with the legacy texture facades in the next step
            continue;
        }
        writes.push_back(write);
    }

    if(!writes.empty())
    {
        vkUpdateDescriptorSets(m_device.getContext().m_device, static_cast<std::uint32_t>(writes.size()), writes.data(), 0, nullptr);
    }
    return transient.m_set;
}

bool SGCore::VulkanCommandList::isPassAttachment(const Ref<VulkanTexture>& texture) const noexcept
{
    if(!m_inPass || !texture) return false;
    if(m_passDepthTexture == texture) return true;
    return std::find(m_passColorTextures.begin(), m_passColorTextures.end(), texture) != m_passColorTextures.end();
}

bool SGCore::VulkanCommandList::flushState() noexcept
{
    if(!m_inPass)
    {
        SG_LOG_E("VulkanCommandList: draw outside of a render pass.");
        return false;
    }
    if(!m_pipeline)
    {
        SG_LOG_E("VulkanCommandList: draw without a bound pipeline.");
        return false;
    }

    // pipeline variant for this pass
    if(m_boundPipeline == VK_NULL_HANDLE)
    {
        const VkPipeline pipeline = m_pipeline->getOrCreate(m_passFormats);
        if(pipeline == VK_NULL_HANDLE) return false;
        vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        m_boundPipeline = pipeline;
        m_boundLayout = m_pipeline->getLayout();

        // window passes flip the viewport, which inverts the winding
        VkFrontFace frontFace = m_pipeline->getFrontFace();
        if(m_targetIsSwapchain)
        {
            frontFace = frontFace == VK_FRONT_FACE_CLOCKWISE ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
        }
        vkCmdSetFrontFace(m_commandBuffer, frontFace);

        // the pipeline's debug name identifies the draw in a capture
        if(const auto& name = m_pipeline->getDebugName(); !name.empty())
        {
            m_device.getContext().insertDebugLabel(m_commandBuffer, name);
        }
    }

    // descriptor sets
    for(std::uint32_t i = 0; i < max_descriptor_sets; ++i)
    {
        auto& slot = m_sets[i];
        if(!slot.m_set) continue;
        const bool stale = slot.m_vkSet == VK_NULL_HANDLE ||
                           slot.m_materializedVersion != slot.m_set->getVersion() ||
                           slot.m_materializedLayout != m_boundLayout;
        if(!stale) continue;

        slot.m_vkSet = materializeSet(slot, i);
        slot.m_materializedVersion = slot.m_set->getVersion();
        slot.m_materializedLayout = m_boundLayout;
        if(slot.m_vkSet != VK_NULL_HANDLE)
        {
            vkCmdBindDescriptorSets(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_boundLayout, i, 1, &slot.m_vkSet, 0, nullptr);
        }
    }

    // push constants
    if(!m_pendingPushConstants.empty())
    {
        const auto& ranges = m_pipeline->getProgram()->getReflection().m_pushConstants;
        for(const auto& pending : m_pendingPushConstants)
        {
            VkShaderStageFlags stages = VK_SHADER_STAGE_ALL_GRAPHICS;
            if(!ranges.empty()) stages = VulkanShaderProgram::stageMaskToVk(ranges.front().m_stages);
            vkCmdPushConstants(m_commandBuffer, m_boundLayout, stages, pending.m_offset,
                               static_cast<std::uint32_t>(pending.m_data.size()), pending.m_data.data());
        }
        m_pendingPushConstants.clear();
    }

    if(m_viewportDirty) applyViewportScissor();
    return true;
}

// ---- draws ------------------------------------------------------------------------------------

void SGCore::VulkanCommandList::draw(std::uint32_t vertexCount, std::uint32_t instanceCount,
                                     std::uint32_t firstVertex, std::uint32_t firstInstance) noexcept
{
    if(!flushState()) return;
    vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void SGCore::VulkanCommandList::drawIndexed(std::uint32_t indexCount, std::uint32_t instanceCount,
                                            std::uint32_t firstIndex, std::int32_t vertexOffset,
                                            std::uint32_t firstInstance) noexcept
{
    if(!flushState()) return;
    vkCmdDrawIndexed(m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

// ---- clears / transitions / uploads --------------------------------------------------------------

void SGCore::VulkanCommandList::clearColorAttachment(std::uint32_t colorIndex, const glm::vec4& color) noexcept
{
    if(!m_inPass || colorIndex >= m_passColorTextures.size()) return;

    VkClearAttachment clear { };
    clear.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    clear.colorAttachment = colorIndex;
    clear.clearValue.color = { { color.r, color.g, color.b, color.a } };
    VkClearRect rect { };
    rect.rect.extent = m_passExtent;
    rect.layerCount = 1;
    vkCmdClearAttachments(m_commandBuffer, 1, &clear, 1, &rect);
}

void SGCore::VulkanCommandList::clearDepthStencil(float depth, bool clearStencil, std::uint32_t stencil) noexcept
{
    if(!m_inPass || !m_passDepthTexture) return;

    VkClearAttachment clear { };
    clear.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    if(clearStencil && (m_passDepthTexture->getAspect() & VK_IMAGE_ASPECT_STENCIL_BIT)) clear.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    clear.clearValue.depthStencil = { depth, stencil };
    VkClearRect rect { };
    rect.rect.extent = m_passExtent;
    rect.layerCount = 1;
    vkCmdClearAttachments(m_commandBuffer, 1, &clear, 1, &rect);
}

void SGCore::VulkanCommandList::transition(const Ref<IGPUObject>& resource, GPUResourceState newState) noexcept
{
    auto texture = std::dynamic_pointer_cast<VulkanTexture>(resource);
    if(!texture || !m_recording || m_inPass) return;

    VkImageLayout layout = VK_IMAGE_LAYOUT_GENERAL;
    switch(newState)
    {
        case GPUResourceState::SGG_STATE_RENDER_TARGET: layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; break;
        case GPUResourceState::SGG_STATE_DEPTH_WRITE: layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; break;
        case GPUResourceState::SGG_STATE_SHADER_READ: layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; break;
        case GPUResourceState::SGG_STATE_TRANSFER_SRC: layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL; break;
        case GPUResourceState::SGG_STATE_TRANSFER_DST: layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL; break;
        case GPUResourceState::SGG_STATE_PRESENT: layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; break;
        case GPUResourceState::SGG_STATE_UNDEFINED: return;
    }
    texture->recordTransition(m_commandBuffer, layout);
    m_submission.m_keepAlive.push_back(texture);
}

void SGCore::VulkanCommandList::uploadData(const Ref<IGPUBuffer>& dst, const void* data, std::uint64_t size, std::uint64_t offset) noexcept
{
    auto* buffer = static_cast<VulkanGPUBuffer*>(dst.get());
    if(!buffer || !buffer->isValid() || !data || size == 0) return;

    if(buffer->isHostVisible())
    {
        buffer->write(data, size, offset);
        return;
    }
    if(!m_recording)
    {
        buffer->write(data, size, offset);
        return;
    }

    auto staging = m_device.createStagingBuffer(size, "upload_staging");
    if(!staging || !staging->isValid()) return;
    std::memcpy(staging->getMappedPointer(), data, size);

    VkBufferCopy region { };
    region.srcOffset = 0;
    region.dstOffset = offset;
    region.size = size;
    vkCmdCopyBuffer(m_uploadCommandBuffer, staging->getHandle(), buffer->getHandle(), 1, &region);
    m_uploadUsed = true;
    m_submission.m_keepAlive.push_back(staging);
    m_submission.m_keepAlive.push_back(dst);
}
