//
// Created by stuka on 14.09.2026.
//

#pragma once

#include <system_error>

#include <vulkan/vulkan.h>

#include "vulkan_image_view.h"
#include "pixelfield/rhi/image.h"

namespace pf::rhi
{
    struct VulkanImage final : IImage
    {
        friend struct VulkanLogicalDevice;
        friend struct VulkanImageView;

        VulkanImage() = default;
        VulkanImage(VkImage handle) noexcept;
        VulkanImage(const VulkanImage&) = delete;
        VulkanImage(VulkanImage&&) noexcept;
        ~VulkanImage() noexcept override;

        [[nodiscard]] std::error_code update(const ImageUpdateInfo& update_info) noexcept override;

        [[nodiscard]] IImageView const *const view() const noexcept override;
        [[nodiscard]] IImageView *const view() noexcept override;

        [[nodiscard]] bool valid() const noexcept override;

        VulkanImage& operator=(const VulkanImage&) = delete;
        VulkanImage& operator=(VulkanImage&&) noexcept;

    private:
        const VulkanContext* context_ {};
        VulkanLogicalDevice* logical_device_ {};

        VkImage handle_ = VK_NULL_HANDLE;
        VkDeviceMemory memory_ = VK_NULL_HANDLE;
        VulkanImageView view_;
        bool owns_image_ = true;

        void destroy() noexcept override;
        [[nodiscard]] std::error_code create(const ImageCreateInfo& create_info) noexcept override;
    };
}
