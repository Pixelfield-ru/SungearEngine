//
// Created by stuka on 13.10.2026.
//

#pragma once

#include <string_view>

#include "pixelfield/core/log.h"

namespace pf::platform
{
    enum class WindowClientApi
    {
        kNoApi,
        kOpenGL,
        kOpenGLES
    };

    struct WindowCreateInfo final
    {
        core::LogCallback debug_output;

        WindowClientApi client_api = WindowClientApi::kNoApi;

        std::int32_t width {};
        std::int32_t height {};

        bool fullscreen {};

        std::string_view title;
    };

    struct IWindow
    {
        virtual ~IWindow() = default;

        virtual void setTitle(std::string_view title) = 0;

        virtual void position(std::int32_t& x, std::int32_t& y) const = 0;
        virtual void setPosition(std::int32_t x, std::int32_t y) = 0;

        virtual void size(std::int32_t& width, std::int32_t& height) const = 0;
        virtual void setSize(std::int32_t width, std::int32_t height) = 0;
    };
}