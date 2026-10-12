//
// Created by stuka on 11.09.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.h>

#include "pixelfield/rhi/vulkan/vulkan_buffer.h"
#include "pixelfield/rhi/vulkan/vulkan_command_buffer.h"
#include "pixelfield/rhi/vulkan/vulkan_context.h"
#include "pixelfield/rhi/vulkan/vulkan_descriptor_set.h"
#include "pixelfield/rhi/vulkan/vulkan_framebuffer.h"
#include "pixelfield/rhi/vulkan/vulkan_image.h"
#include "pixelfield/rhi/vulkan/vulkan_image_view.h"
#include "pixelfield/rhi/vulkan/vulkan_pass.h"
#include "pixelfield/rhi/vulkan/vulkan_physical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_pass.h"
#include "pixelfield/rhi/vulkan/vulkan_pipeline_state.h"
#include "pixelfield/rhi/vulkan/vulkan_sampler.h"
#include "pixelfield/rhi/vulkan/vulkan_shader.h"
#include "pixelfield/rhi/vulkan/vulkan_swapchain.h"
#include "pixelfield/rhi/vulkan/vulkan_types.h"

pf::rhi::VulkanLogicalDevice::VulkanLogicalDevice(VulkanLogicalDevice&& other) noexcept :
    context_(std::exchange(other.context_, nullptr)),
    physical_device_(std::exchange(other.physical_device_, nullptr)),
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE)),
    queues_(std::exchange(other.queues_, {})),
    command_pools_(std::exchange(other.command_pools_, {})),
    descriptor_pool_(std::exchange(other.descriptor_pool_, VK_NULL_HANDLE)),
    staging_buffer_(std::exchange(other.staging_buffer_, {})),
    staging_command_buffer_(std::exchange(other.staging_command_buffer_, {})),
    staging_buffer_min_size_(std::exchange(other.staging_buffer_min_size_, {}))
{

}

pf::rhi::VulkanLogicalDevice::~VulkanLogicalDevice()
{
    destroy();
}

bool pf::rhi::VulkanLogicalDevice::valid() const noexcept
{
    return handle_ != nullptr;
}

std::error_code pf::rhi::VulkanLogicalDevice::submit(const SubmitInfo& submit_info) noexcept
{
    auto* vulkan_swapchain = static_cast<VulkanSwapchain*>(submit_info.swapchain);
    const auto frame_slot = vulkan_swapchain->current_frame_idx_;
    auto& frame_fence = vulkan_swapchain->busy_frames_fences_[frame_slot];
    auto& image_available = vulkan_swapchain->image_available_semaphores_[frame_slot];
    const auto queue = queues_[std::to_underlying(submit_info.queue_family)];

    const VkPipelineStageFlags wait_stages[] { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    std::vector<VkCommandBuffer> command_buffers(submit_info.command_buffers.size());
    for(size_t i = 0; i < submit_info.command_buffers.size(); ++i)
    {
        command_buffers[i] = static_cast<const VulkanCommandBuffer*>(submit_info.command_buffers[i])->handle_;
    }

    VkSubmitInfo vulkan_submit_info{};
    vulkan_submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    vulkan_submit_info.waitSemaphoreCount = 1;
    vulkan_submit_info.pWaitSemaphores = &image_available;
    vulkan_submit_info.pWaitDstStageMask = wait_stages;
    vulkan_submit_info.commandBufferCount = static_cast<std::uint32_t>(command_buffers.size());
    vulkan_submit_info.pCommandBuffers = command_buffers.data();
    vulkan_submit_info.signalSemaphoreCount = 1;
    // present waits on this semaphore until the image is re-acquired — index by image, not frame
    vulkan_submit_info.pSignalSemaphores =
        &vulkan_swapchain->render_finished_semaphores_[submit_info.wait_for_frame->image_index];

    // Reset immediately before submit so a failed acquire never leaves the fence unsignaled.
    vkResetFences(handle_, 1, &frame_fence);

    if(vkQueueSubmit(queue, 1, &vulkan_submit_info, frame_fence) != VK_SUCCESS)
    {
        // Submit did not take the fence; it stays unsignaled after reset.
        // image_available is still signaled from acquire — consume it and signal the fence.
        const VkPipelineStageFlags recover_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        VkSubmitInfo recover_info{};
        recover_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        recover_info.waitSemaphoreCount = 1;
        recover_info.pWaitSemaphores = &image_available;
        recover_info.pWaitDstStageMask = &recover_stage;

        if(vkQueueSubmit(queue, 1, &recover_info, frame_fence) != VK_SUCCESS)
        {
            // Last resort: recreate fence in signaled state so the next wait cannot hang.
            vkDestroyFence(handle_, frame_fence, context_->allocator_.get());
            VkFenceCreateInfo fence_info{};
            fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            if(vkCreateFence(handle_, &fence_info, context_->allocator_.get(), &frame_fence) != VK_SUCCESS)
            {
                frame_fence = VK_NULL_HANDLE;
            }
            // image_available may still be signaled; force GPU drain before next acquire.
            vkDeviceWaitIdle(handle_);
        }

        return VulkanError::kCannotSubmit;
    }

    return {};
}

std::error_code pf::rhi::VulkanLogicalDevice::waitIdle() noexcept
{
    if(vkDeviceWaitIdle(handle_) != VK_SUCCESS)
    {
        return VulkanError::kCannotWaitDeviceIdle;
    }

    return {};
}

std::expected<std::unique_ptr<pf::rhi::ISwapchain>, std::error_code> pf::rhi::VulkanLogicalDevice::createSwapchain(
    const SwapchainCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanSwapchain>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::IImage>, std::error_code> pf::rhi::VulkanLogicalDevice::createImage(
    const ImageCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanImage>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::IImageView>, std::error_code> pf::rhi::VulkanLogicalDevice::createImageView(
    const ImageViewCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanImageView>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::ISampler>, std::error_code> pf::rhi::VulkanLogicalDevice::createSampler(
    const SamplerCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanSampler>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::IShader>, std::error_code> pf::rhi::VulkanLogicalDevice::createShader(
    const ShaderCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanShader>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::IPass>, std::error_code> pf::rhi::VulkanLogicalDevice::createPass(
    const PassCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanPass>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::IPipelineState>, std::error_code> pf::rhi::VulkanLogicalDevice::createPipelineState(
    const PipelineStateCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanPipelineState>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::IFramebuffer>, std::error_code> pf::rhi::VulkanLogicalDevice::createFramebuffer(
    const FramebufferCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanFramebuffer>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::ICommandBuffer>, std::error_code> pf::rhi::VulkanLogicalDevice::createCommandBuffer(
    const CommandBufferCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanCommandBuffer>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::IBuffer>, std::error_code> pf::rhi::VulkanLogicalDevice::createBuffer(
    const BufferCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanBuffer>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

std::expected<std::unique_ptr<pf::rhi::IDescriptorSet>, std::error_code> pf::rhi::VulkanLogicalDevice::createDescriptorSet(
    const DescriptorSetCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanDescriptorSet>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

pf::rhi::VulkanLogicalDevice& pf::rhi::VulkanLogicalDevice::operator=(VulkanLogicalDevice&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    destroy();

    context_ = std::exchange(other.context_, nullptr);
    physical_device_ = std::exchange(other.physical_device_, nullptr);
    handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);
    queues_ = std::exchange(other.queues_, {});
    command_pools_ = std::exchange(other.command_pools_, {});
    descriptor_pool_ = std::exchange(other.descriptor_pool_, VK_NULL_HANDLE);
    staging_buffer_ = std::exchange(other.staging_buffer_, {});
    staging_command_buffer_ = std::exchange(other.staging_command_buffer_, {});
    staging_buffer_min_size_ = std::exchange(other.staging_buffer_min_size_, {});

    return *this;
}

std::expected<VkCommandPool, std::error_code> pf::rhi::VulkanLogicalDevice::commandPool(QueueFamilyType queue_type) noexcept
{
    auto*& existing_pool = command_pools_[std::to_underlying(queue_type)];
    if(existing_pool) return existing_pool;

    VkCommandPool command_pool;

    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = *physical_device_->queue_families_indices_[std::to_underlying(queue_type)];
    // Allow vkBeginCommandBuffer to reset individual buffers (e.g. after swapchain recreate).
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if(vkCreateCommandPool(handle_, &pool_info, context_->allocator_.get(), &command_pool) != VK_SUCCESS)
    {
        return std::unexpected(VulkanError::kCannotCreateCommandPool);
    }

    existing_pool = command_pool;

    return existing_pool;
}

std::expected<VkDescriptorPool, std::error_code> pf::rhi::VulkanLogicalDevice::descriptorPool() noexcept
{
    if(descriptor_pool_) return descriptor_pool_;

    // todo: make configurable
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
    VkDescriptorPoolCreateInfo pool_info {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 4096;
    pool_info.poolSizeCount = static_cast<std::uint32_t>(std::size(sizes));
    pool_info.pPoolSizes = sizes;

    if(vkCreateDescriptorPool(handle_, &pool_info, context_->allocator_.get(), &descriptor_pool_) != VK_SUCCESS)
    {
        return std::unexpected(VulkanError::kCannotCreateDescriptorPool);
    }

    return descriptor_pool_;
}

std::error_code pf::rhi::VulkanLogicalDevice::assureStagingBuffer(size_t size) noexcept
{
    if(staging_buffer_.valid() && staging_buffer_.size() >=  size) return {};

    BufferCreateInfo create_info;
    create_info.context_ = context_;
    create_info.logical_device_ = this;
    create_info.type = BufferType::kTransferSrc;
    create_info.host_coherent = true;
    create_info.size = staging_buffer_.valid() ? size : staging_buffer_min_size_;

    staging_buffer_.destroy();
    if(const auto res = staging_buffer_.create(create_info))
    {
        return res;
    }

    return {};
}

std::error_code pf::rhi::VulkanLogicalDevice::assureStagingCommandBuffer() noexcept
{
    if(staging_command_buffer_.valid()) return {};

    CommandBufferCreateInfo create_info;
    create_info.context = context_;
    create_info.logical_device = this;
    create_info.queue_family = QueueFamilyType::kGraphics;

    staging_command_buffer_.destroy();
    if(const auto res = staging_command_buffer_.create(create_info))
    {
        return res;
    }

    return {};
}

std::error_code pf::rhi::VulkanLogicalDevice::updateBuffer(void* data, VulkanBuffer& dst_buffer, size_t dst_offset,
                                                       size_t size) noexcept
{
    // creating staging buffer
    if(const auto res = assureStagingBuffer(size))
    {
        return res;
    }

    // creating command buffer
    if(const auto res = assureStagingCommandBuffer())
    {
        return res;
    }

    // copying data to staging buffer
    void* gpu_mem {};
    if(vkMapMemory(handle_, staging_buffer_.memory_, 0, size, 0, &gpu_mem) != VK_SUCCESS)
    {
        return VulkanError::kCannotMapMemory;
    }
    std::memcpy(gpu_mem, data, size);
    vkUnmapMemory(handle_, staging_buffer_.memory_);

    if(const auto res = staging_command_buffer_.begin())
    {
        return res;
    }

    // copying buffer
    staging_command_buffer_.copyBuffer(staging_buffer_, dst_buffer, 0, dst_offset, size);

    if(const auto res = staging_command_buffer_.end())
    {
        return res;
    }

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &staging_command_buffer_.handle_;

    // submit copy command
    const auto& graphics_queue = queues_[static_cast<std::int32_t>(QueueFamilyType::kGraphics)];
    if(vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        return VulkanError::kCannotSubmit;
    }

    // waiting until the device copies the data
    if(const auto res = waitIdle())
    {
        return res;
    }

    return {};
}

std::error_code pf::rhi::VulkanLogicalDevice::updateImage(IImage& dst_image, const ImageUpdateInfo& update_info) noexcept
{
    auto& vulkan_image = static_cast<VulkanImage&>(dst_image);

    const std::uint32_t bpp = formatBytesPerPixel(vulkan_image.format());
    if(bpp == 0)
    {
        return VulkanError::kInvalidArgument;
    }

    const std::uint32_t row_texels = update_info.buffer_row_length ? update_info.buffer_row_length : update_info.extent.x;
    const std::uint32_t row_count = update_info.buffer_image_height ? update_info.buffer_image_height : update_info.extent.y;
    const VkDeviceSize staging_size =
        static_cast<VkDeviceSize>(update_info.layer_count) * update_info.extent.z * row_count * row_texels * bpp;

    // creating staging buffer
    if(const auto res = assureStagingBuffer(staging_size))
    {
        return res;
    }

    // creating command buffer
    if(const auto res = assureStagingCommandBuffer())
    {
        return res;
    }

    // mapping staging buffer memory
    void* mapped = nullptr;
    if(vkMapMemory(handle_, staging_buffer_.memory_, 0, staging_size, 0, &mapped) != VK_SUCCESS)
    {
        return VulkanError::kCannotMapMemory;
    }
    std::memcpy(mapped, update_info.data, staging_size);
    vkUnmapMemory(handle_, staging_buffer_.memory_);

    if(const auto res = staging_command_buffer_.begin())
    {
        return res;
    }

    // copying buffer to texture
    const VkImageSubresourceRange range {
        toVkImageAspect(update_info.aspect),
        update_info.mip_level,
        1,
        update_info.base_array_layer,
        update_info.layer_count
    };

    // wait until all operations with image are completed and then change layout to transfer dst
    VkImageMemoryBarrier to_dst{};
    to_dst.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    to_dst.oldLayout = toVkImageLayout(vulkan_image.layout_); // current layout
    to_dst.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    to_dst.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    to_dst.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    to_dst.image = vulkan_image.handle_;
    to_dst.subresourceRange = range;
    to_dst.srcAccessMask = toVkAccessMask(vulkan_image.layout_);
    to_dst.dstAccessMask = toVkAccessMask(ImageLayout::kTransferDst);
    vkCmdPipelineBarrier(
        staging_command_buffer_.handle_,
        toVkPipelineStageFlags(vulkan_image.layout_),
        toVkPipelineStageFlags(ImageLayout::kTransferDst),
        0, 0, nullptr, 0, nullptr, 1, &to_dst);

    // copy region
    VkBufferImageCopy copy{};
    copy.bufferOffset = 0;
    copy.bufferRowLength = update_info.buffer_row_length;
    copy.bufferImageHeight = update_info.buffer_image_height;
    copy.imageSubresource.aspectMask = range.aspectMask;
    copy.imageSubresource.mipLevel = update_info.mip_level;
    copy.imageSubresource.baseArrayLayer = update_info.base_array_layer;
    copy.imageSubresource.layerCount = update_info.layer_count;
    copy.imageOffset = { update_info.offset.x, update_info.offset.y, update_info.offset.z };
    copy.imageExtent = { update_info.extent.x, update_info.extent.y, update_info.extent.z };
    vkCmdCopyBufferToImage(
        staging_command_buffer_.handle_,
        staging_buffer_.handle_,
        vulkan_image.handle_,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &copy);

    // transfer image to new layout
    VkImageMemoryBarrier to_final = to_dst;
    to_final.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    to_final.newLayout = toVkImageLayout(update_info.final_layout);
    to_final.srcAccessMask = toVkAccessMask(ImageLayout::kTransferDst);
    to_final.dstAccessMask = toVkAccessMask(update_info.final_layout);
    vkCmdPipelineBarrier(
        staging_command_buffer_.handle_,
        toVkPipelineStageFlags(ImageLayout::kTransferDst),
        toVkPipelineStageFlags(update_info.final_layout),
        0, 0, nullptr, 0, nullptr, 1, &to_final);

    if(const auto res = staging_command_buffer_.end())
    {
        return res;
    }

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &staging_command_buffer_.handle_;

    // submit copy command
    const auto& graphics_queue = queues_[static_cast<std::int32_t>(QueueFamilyType::kGraphics)];
    if(vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        return VulkanError::kCannotSubmit;
    }

    if(const auto res = waitIdle())
    {
        return res;
    }

    vulkan_image.layout_ = update_info.final_layout;
    return {};
}

void pf::rhi::VulkanLogicalDevice::destroy() noexcept
{
    staging_buffer_.destroy();
    staging_command_buffer_.destroy();

    for(auto*& pool : command_pools_)
    {
        if(!pool) continue;

        vkDestroyCommandPool(handle_, pool, context_->allocator_.get());
        pool = VK_NULL_HANDLE;
    }

    if(descriptor_pool_)
    {
        vkDestroyDescriptorPool(handle_, descriptor_pool_, context_->allocator_.get());
        descriptor_pool_ = VK_NULL_HANDLE;
    }

    if(handle_)
    {
        waitIdle();
        vkDestroyDevice(handle_, context_->allocator_.get());
        handle_ = VK_NULL_HANDLE;
    }
}

std::error_code pf::rhi::VulkanLogicalDevice::create(const LogicalDeviceCreateInfo& create_info) noexcept
{
    const auto* vulkan_context = static_cast<const VulkanContext*>(create_info.context);
    const auto* vulkan_physical_device = static_cast<const VulkanPhysicalDevice*>(create_info.physical_device);

    context_ = vulkan_context;
    physical_device_ = vulkan_physical_device;

    const float queue_priority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queues_create_info;
    std::unordered_set<std::uint32_t> unique_queue_family_indices;

    // creating queues for all found families
    for(const auto& family_index : vulkan_physical_device->queue_families_indices_)
    {
        if(!family_index || unique_queue_family_indices.contains(*family_index)) continue;

        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = *family_index;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;

        queues_create_info.push_back(queue_create_info);
        unique_queue_family_indices.insert(*family_index);
    }

    VkPhysicalDeviceVulkan11Features features11 {};
    features11.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
    features11.shaderDrawParameters = VK_TRUE;

    VkPhysicalDeviceFeatures2 features2 {};
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features2.pNext = &features11;
    // enabling all device features
    features2.features = vulkan_physical_device->features_;

    std::vector<const char*> device_extensions;
    device_extensions.reserve(vulkan_context->enabled_extensions_.size());
    for(const auto& ext : vulkan_context->enabled_extensions_)
    {
        device_extensions.push_back(ext.c_str());
    }

    VkDeviceCreateInfo handle_create_info{};
    handle_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    handle_create_info.pNext = &features2;
    handle_create_info.pQueueCreateInfos = queues_create_info.data();
    handle_create_info.queueCreateInfoCount = queues_create_info.size();
    handle_create_info.pEnabledFeatures = nullptr;
    // Device layers are deprecated; validation is enabled on the instance.
    handle_create_info.enabledLayerCount = 0;
    handle_create_info.ppEnabledLayerNames = nullptr;
    handle_create_info.ppEnabledExtensionNames = device_extensions.data();
    handle_create_info.enabledExtensionCount = device_extensions.size();

    if(vkCreateDevice(vulkan_physical_device->handle_, &handle_create_info, vulkan_context->allocator_.get(), &handle_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreateDevice;
    }

    vkGetDeviceQueue(handle_, *vulkan_physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kGraphics)], 0, &queues_[std::to_underlying(QueueFamilyType::kGraphics)]);
    vkGetDeviceQueue(handle_, *vulkan_physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kPresent)], 0, &queues_[std::to_underlying(QueueFamilyType::kPresent)]);

    return {};
}
