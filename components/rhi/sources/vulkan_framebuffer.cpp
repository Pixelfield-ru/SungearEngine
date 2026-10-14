//
// Created by stuka on 20.09.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_framebuffer.h"

#include "pixelfield/rhi/vulkan/vulkan_context.h"
#include "pixelfield/rhi/vulkan/vulkan_image_view.h"
#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_pass.h"
#include "pixelfield/rhi/vulkan/vulkan_pipeline_state.h"

pf::rhi::VulkanFramebuffer::VulkanFramebuffer(VulkanFramebuffer&& other) noexcept :
    context_(std::exchange(other.context_, nullptr)),
    logical_device_(std::exchange(other.logical_device_, nullptr)),
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE))
{

}

pf::rhi::VulkanFramebuffer::~VulkanFramebuffer()
{
    destroy();
}

pf::rhi::VulkanFramebuffer& pf::rhi::VulkanFramebuffer::operator=(VulkanFramebuffer&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    destroy();

    context_ = std::exchange(other.context_, nullptr);
    logical_device_ = std::exchange(other.logical_device_, nullptr);
    handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);

    return *this;
}

void pf::rhi::VulkanFramebuffer::destroy() noexcept
{
    if(handle_)
    {
        vkDestroyFramebuffer(logical_device_->handle_, handle_, context_->allocator_.get());
        handle_ = VK_NULL_HANDLE;
    }
}

std::error_code pf::rhi::VulkanFramebuffer::create(const FramebufferCreateInfo& create_info) noexcept
{
    const auto* vulkan_context = static_cast<const VulkanContext*>(create_info.context);
    const auto* vulkan_logical_device = static_cast<const VulkanLogicalDevice*>(create_info.logical_device);
    const auto* vulkan_pass = static_cast<const VulkanPass*>(create_info.pass);

    context_ = vulkan_context;
    logical_device_ = vulkan_logical_device;

    std::vector<VkImageView> vulkan_attachments(create_info.images.size());
    for(size_t i = 0; i < create_info.images.size(); ++i)
    {
        const auto* vulkan_image_view = static_cast<const VulkanImageView*>(create_info.images[i]);

        vulkan_attachments[i] = vulkan_image_view->handle_;
    }

    VkFramebufferCreateInfo framebuffer_info{};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = vulkan_pass->handle_;
    framebuffer_info.attachmentCount = vulkan_attachments.size();
    framebuffer_info.pAttachments = vulkan_attachments.data();
    framebuffer_info.width = create_info.size.x;
    framebuffer_info.height = create_info.size.y;
    framebuffer_info.layers = 1;

    if(vkCreateFramebuffer(vulkan_logical_device->handle_, &framebuffer_info, vulkan_context->allocator_.get(), &handle_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreateFramebuffer;
    }

    return {};
}
