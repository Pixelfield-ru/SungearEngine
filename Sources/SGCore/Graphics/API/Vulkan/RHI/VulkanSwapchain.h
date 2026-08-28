//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/ISwapchain.h"
#include "SGCore/Main/CoreGlobals.h"
#include "VulkanContext.h"
#include "VulkanTexture.h"

struct GLFWwindow;

namespace SGCore
{
    class VulkanDevice;

    /// VkSwapchainKHR over the GLFW window surface. FIFO present, images acquired lazily: the first
    /// render pass of a frame that targets the window acquires one (ensureAcquired()), present()
    /// transitions it to PRESENT_SRC and hands it to the presentation engine.
    class SGCORE_EXPORT VulkanSwapchain final : public ISwapchain
    {
    public:
        static constexpr std::uint32_t frames_in_flight = 2;

        VulkanSwapchain(VulkanDevice& device, GLFWwindow* window) noexcept;
        ~VulkanSwapchain() override;

        [[nodiscard]] bool isValid() const noexcept { return m_swapchain != VK_NULL_HANDLE; }

        // ---- ISwapchain
        bool beginFrame() noexcept override;
        void present() noexcept override;
        [[nodiscard]] std::uint32_t getWidth() const noexcept override { return m_extent.width; }
        [[nodiscard]] std::uint32_t getHeight() const noexcept override { return m_extent.height; }
        [[nodiscard]] std::uint32_t getFrameIndex() const noexcept override { return m_frameSlot; }
        [[nodiscard]] std::uint64_t getFrameNumber() const noexcept override { return m_frameNumber; }

        // ---- backend
        /// Acquires the next image unless one is already held for this frame. False when the
        /// swapchain is unusable (minimized window, lost surface).
        bool ensureAcquired() noexcept;
        [[nodiscard]] bool isAcquired() const noexcept { return m_acquired; }
        /// The acquired image (nullptr when none is held).
        [[nodiscard]] Ref<VulkanTexture> getCurrentTexture() const noexcept;
        [[nodiscard]] VkFormat getFormat() const noexcept { return m_format; }
        /// The acquire semaphore of the current frame, handed out once: the first submission that
        /// touches the swapchain image waits on it. VK_NULL_HANDLE afterwards.
        [[nodiscard]] VkSemaphore takeAcquireSemaphore() noexcept;

        /// Recreates the swapchain for the current framebuffer size (also used on OUT_OF_DATE).
        bool recreate() noexcept;

    private:
        struct FrameSlot
        {
            VkSemaphore m_acquireSemaphore = VK_NULL_HANDLE;
            bool m_acquireSemaphorePending { };
            /// submission id of this slot's last present; waited on before the slot is reused
            std::uint64_t m_lastSubmission { };
        };

        void destroySwapchainObjects() noexcept;
        [[nodiscard]] bool queryWindowExtent(VkExtent2D& outExtent) const noexcept;

        VulkanDevice& m_device;
        GLFWwindow* m_window { };

        VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
        VkFormat m_format = VK_FORMAT_B8G8R8A8_UNORM;
        VkColorSpaceKHR m_colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        VkExtent2D m_extent { };
        std::vector<Ref<VulkanTexture>> m_images;
        std::vector<VkSemaphore> m_renderFinishedSemaphores; // one per swapchain image

        FrameSlot m_frameSlots[frames_in_flight] { };
        std::uint32_t m_frameSlot { };
        std::uint64_t m_frameNumber { };

        bool m_acquired { };
        std::uint32_t m_imageIndex { };
        bool m_needsRecreate { };
    };
}
