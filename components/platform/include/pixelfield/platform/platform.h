//
// Created by stuka on 13.10.2026.
//

#pragma once

#include <system_error>

#include "pixelfield/core/log.h"

namespace pf::platform
{
    struct Platform final
    {
        friend Platform& platform() noexcept;

        ~Platform() noexcept;
        Platform(const Platform&) = delete;
        Platform(Platform&&) = delete;

        [[nodiscard]] std::error_code init() noexcept;
        [[nodiscard]] bool initialized() const noexcept;

        void setDebugOutput(core::LogCallback callback) noexcept;

    private:
        Platform() noexcept = default;

        core::LogCallback debug_output_;
        bool initialized_ {};

        static void glfwErrorCallback(int level, const char* description) noexcept;
    };

    Platform& platform() noexcept;
}
