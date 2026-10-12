//
// Created by stuka on 29.09.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_pass.h"

#include "pixelfield/rhi/vulkan/vulkan_context.h"
#include "pixelfield/rhi/vulkan/vulkan_errors.h"
#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_types.h"

pf::rhi::VulkanPass::VulkanPass(VulkanPass&& other) noexcept :
    context_(std::exchange(other.context_, nullptr)),
    logical_device_(std::exchange(other.logical_device_, nullptr)),
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE)),
    color_attachments_(std::exchange(other.color_attachments_, {})),
    attachments_refs_(std::exchange(other.attachments_refs_, {}))
{
}

pf::rhi::VulkanPass::~VulkanPass()
{
    destroy();
}

bool pf::rhi::VulkanPass::valid() const noexcept
{
    return handle_ != nullptr;
}

pf::rhi::VulkanPass& pf::rhi::VulkanPass::operator=(VulkanPass&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    destroy();

    context_ = std::exchange(other.context_, nullptr);
    logical_device_ = std::exchange(other.logical_device_, nullptr);
    handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);
    color_attachments_ = std::exchange(other.color_attachments_, {});
    attachments_refs_ = std::exchange(other.attachments_refs_, {});

    return *this;
}

void pf::rhi::VulkanPass::destroy() noexcept
{
    if(handle_)
    {
        vkDestroyRenderPass(logical_device_->handle_, handle_, context_->allocator_.get());
        handle_ = VK_NULL_HANDLE;
    }

    color_attachments_.clear();
    attachments_refs_.clear();
}

std::error_code pf::rhi::VulkanPass::create(const PassCreateInfo& create_info) noexcept
{
    const auto* vulkan_context = static_cast<const VulkanContext*>(create_info.context);
    const auto* vulkan_logical_device = static_cast<const VulkanLogicalDevice*>(create_info.logical_device);

    context_ = vulkan_context;
    logical_device_ = vulkan_logical_device;

    color_attachments_ = std::vector<VkAttachmentDescription>(create_info.color_attachments.size());
    attachments_refs_ = std::vector<VkAttachmentReference>(create_info.color_attachments.size());

    for(size_t i = 0; i < create_info.color_attachments.size(); ++i)
    {
        const auto& color_attachment = create_info.color_attachments[i];

        VkAttachmentDescription vk_color_attachment{};
        vk_color_attachment.format = toVkFormat(color_attachment.format);
        vk_color_attachment.samples = toVkSampleCount(color_attachment.storage_sample_count);
        vk_color_attachment.loadOp = toVkAttachmentLoadOp(color_attachment.load_op);
        vk_color_attachment.storeOp = toVkAttachmentStoreOp(color_attachment.store_op);
        vk_color_attachment.stencilLoadOp = toVkAttachmentLoadOp(color_attachment.stencil_load_op);
        vk_color_attachment.stencilStoreOp = toVkAttachmentStoreOp(color_attachment.stencil_store_op);
        vk_color_attachment.initialLayout = toVkImageLayout(color_attachment.initial_layout);
        vk_color_attachment.finalLayout = toVkImageLayout(color_attachment.final_layout);

        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.attachment = i;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        color_attachments_[i] = vk_color_attachment;
        attachments_refs_[i] = color_attachment_ref;
    }

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = attachments_refs_.size();
    subpass.pColorAttachments = attachments_refs_.data();

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = color_attachments_.size();
    render_pass_info.pAttachments = color_attachments_.data();
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;

    if(vkCreateRenderPass(vulkan_logical_device->handle_, &render_pass_info, vulkan_context->allocator_.get(), &handle_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreateRenderPass;
    }

    return {};
}
