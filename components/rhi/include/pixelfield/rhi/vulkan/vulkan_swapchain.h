//
// Created by stuka on 11.09.2026.
//

#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "pixelfield/rhi/swapchain.h"
#include "vulkan_errors.h"

namespace pf::rhi
{
    struct VulkanLogicalDevice;
    struct VulkanContext;
    struct VulkanImageView;

    struct VulkanSwapchain final : ISwapchain
    {
        friend struct VulkanLogicalDevice;

        VulkanSwapchain() noexcept = default;
        VulkanSwapchain(const VulkanSwapchain&) = delete;
        VulkanSwapchain(VulkanSwapchain&& other) noexcept;
        ~VulkanSwapchain() noexcept override;

        [[nodiscard]] bool valid() const noexcept override;
        [[nodiscard]] glm::ivec2 extent() const noexcept override;
        [[nodiscard]] Format format() const noexcept override;
        [[nodiscard]] std::uint16_t imagesCount() const noexcept override;
        [[nodiscard]] IImageView* imageView(std::uint16_t idx) noexcept override;
        [[nodiscard]] std::expected<AcquiredFrame, std::error_code> acquireNextFrame() noexcept override;
        [[nodiscard]] std::error_code present(const PresentInfo& present_info) noexcept override;

        VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
        VulkanSwapchain& operator=(VulkanSwapchain&& other) noexcept;

    private:
        const VulkanContext* context_ {};
        const VulkanLogicalDevice* logical_device_ {};

        VkSwapchainKHR handle_ {};

        VkSurfaceFormatKHR surface_format_ {};
        VkPresentModeKHR present_mode_ = VK_PRESENT_MODE_MAILBOX_KHR;
        VkExtent2D extent_ {};

        std::vector<VulkanImageView> images_view_;

        std::vector<VkSemaphore> image_available_semaphores_;
        std::vector<VkSemaphore> render_finished_semaphores_;
        std::vector<VkFence> busy_frames_fences_;

        std::uint32_t current_frame_idx_ {};

        void destroy() noexcept override;
        [[nodiscard]] std::error_code create(const SwapchainCreateInfo& create_info) noexcept override;
    };
}
