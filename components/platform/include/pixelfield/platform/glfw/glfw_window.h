//
// Created by stuka on 13.10.2026.
//

#pragma once

#include <system_error>
#include <GLFW/glfw3.h>

#include "pixelfield/core/log.h"
#include "pixelfield/platform/window.h"

namespace pf::platform
{
    struct GLFWWindow final : IWindow
    {
        GLFWWindow() noexcept = default;
        GLFWWindow(const GLFWWindow&) = delete;
        GLFWWindow(GLFWWindow&& other) noexcept;
        ~GLFWWindow() noexcept override;

        void setTitle(std::string_view title) noexcept override;

        void position(std::int32_t& x, std::int32_t& y) const noexcept override;
        void setPosition(std::int32_t x, std::int32_t y) noexcept override;

        void size(std::int32_t& width, std::int32_t& height) const noexcept override;
        void setSize(std::int32_t width, std::int32_t height) noexcept override;

        GLFWWindow& operator=(const GLFWWindow&) = delete;
        GLFWWindow& operator=(GLFWWindow&& other) noexcept;

        static std::error_code create(const WindowCreateInfo& create_info, GLFWWindow& output) noexcept;

    private:
        GLFWwindow* handle_ {};

        core::LogCallback debug_output_;

        void destroy() noexcept;

        static void nativeCloseCallback(GLFWwindow* window) noexcept;
        static void nativeIconifyCallback(GLFWwindow* window, int iconified) noexcept;
        static void nativeFramebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept;
    };
}
