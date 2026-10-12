//
// Created by stuka on 14.09.2026.
//

#pragma once

#include "pixelfield/rhi/image_view.h"
#include "vulkan_errors.h"

#include <vulkan/vulkan.h>

namespace pf::rhi
{
    struct VulkanLogicalDevice;
    struct VulkanContext;

    struct VulkanImageView final : IImageView
    {
        friend struct VulkanLogicalDevice;
        friend struct VulkanFramebuffer;
        friend struct VulkanSwapchain;
        friend struct VulkanDescriptorSet;
        friend struct VulkanImage;

        VulkanImageView() = default;
        VulkanImageView(const VulkanImageView&) = delete;
        VulkanImageView(VulkanImageView&& other) noexcept;
        ~VulkanImageView() noexcept override;

        [[nodiscard]] bool valid() const noexcept override;

        VulkanImageView& operator=(const VulkanImageView&) = delete;
        VulkanImageView& operator=(VulkanImageView&& other) noexcept;

    private:
        const VulkanContext* context_ {};
        const VulkanLogicalDevice* logical_device_ {};

        VkImageView handle_ {};

        void destroy() noexcept override;
        [[nodiscard]] std::error_code create(const ImageViewCreateInfo& create_info) noexcept override;
    };
}
