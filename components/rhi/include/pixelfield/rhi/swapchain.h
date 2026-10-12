//
// Created by stuka on 11.09.2026.
//

#pragma once

#include <expected>
#include <system_error>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include "types.h"

namespace pf::rhi
{
    struct IContext;
    struct ILogicalDevice;
    struct IPhysicalDevice;
    struct IImageView;

    struct SwapchainCreateInfo final
    {
        const IContext* context {};
        const ILogicalDevice* logical_device {};
        IPhysicalDevice* physical_device {};
        GLFWwindow* window_handle {};
        Format preferred_format = Format::kBgra8Unorm;
        ColorSpace preferred_color_space = ColorSpace::kSrgbNonlinear;
        std::int8_t max_frames_in_flight = 2;
    };

    struct AcquiredFrame final
    {
        std::uint32_t image_index {};
        IImageView* image_view {};
    };

    struct PresentInfo final
    {
        const AcquiredFrame* frame {};
        QueueFamilyType queue_family = QueueFamilyType::kGraphics;
    };

    struct ISwapchain
    {
        virtual ~ISwapchain() = default;

        [[nodiscard]] std::error_code recreate(const SwapchainCreateInfo& create_info) noexcept
        {
            destroy();
            return create(create_info);
        }

        [[nodiscard]] virtual bool valid() const noexcept = 0;
        [[nodiscard]] virtual glm::ivec2 extent() const noexcept = 0;
        [[nodiscard]] virtual Format format() const noexcept = 0;
        [[nodiscard]] virtual std::uint16_t imagesCount() const noexcept = 0;
        [[nodiscard]] virtual IImageView* imageView(std::uint16_t idx) noexcept = 0;
        [[nodiscard]] const IImageView* imageView(std::uint16_t idx) const noexcept
        {
            return const_cast<ISwapchain*>(this)->imageView(idx);
        }
        [[nodiscard]] virtual std::expected<AcquiredFrame, std::error_code> acquireNextFrame() = 0;
        [[nodiscard]] virtual std::error_code present(const PresentInfo& present_info) = 0;

    private:
        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const SwapchainCreateInfo& create_info) = 0;
    };
}
