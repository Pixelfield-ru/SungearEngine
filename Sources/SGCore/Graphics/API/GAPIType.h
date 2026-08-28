//
// Created by stuka on 06.07.2023.
//

#ifndef SUNGEARENGINE_APITYPE_H
#define SUNGEARENGINE_APITYPE_H

#include <optional>
#include <string_view>

namespace SGCore
{
    enum GAPIType
    {
        SG_API_TYPE_UNKNOWN,

        SG_API_TYPE_GL4,
        SG_API_TYPE_GL46,

        SG_API_TYPE_GLES2,
        SG_API_TYPE_GLES3,

        SG_API_TYPE_VULKAN,
        SG_API_TYPE_DX12
    };

    /// True for every OpenGL-family backend (desktop GL and GLES). These backends share
    /// the GLFW/EGL context path and are the permanent fallback of the engine.
    [[nodiscard]] constexpr bool isOpenGLAPI(GAPIType apiType) noexcept
    {
        return apiType == SG_API_TYPE_GL4 || apiType == SG_API_TYPE_GL46 ||
               apiType == SG_API_TYPE_GLES2 || apiType == SG_API_TYPE_GLES3;
    }

    /// True for explicit (command-list based) backends that create their own surface/swapchain
    /// and require GLFW_NO_API window creation.
    [[nodiscard]] constexpr bool isExplicitAPI(GAPIType apiType) noexcept
    {
        return apiType == SG_API_TYPE_VULKAN || apiType == SG_API_TYPE_DX12;
    }

    [[nodiscard]] constexpr std::string_view gapiTypeToString(GAPIType apiType) noexcept
    {
        switch(apiType)
        {
            case SG_API_TYPE_UNKNOWN: return "unknown";
            case SG_API_TYPE_GL4: return "gl4";
            case SG_API_TYPE_GL46: return "gl46";
            case SG_API_TYPE_GLES2: return "gles2";
            case SG_API_TYPE_GLES3: return "gles3";
            case SG_API_TYPE_VULKAN: return "vulkan";
            case SG_API_TYPE_DX12: return "dx12";
        }

        return "unknown";
    }

    /// Accepts the same identifiers that gapiTypeToString() produces (case-sensitive).
    [[nodiscard]] constexpr std::optional<GAPIType> gapiTypeFromString(std::string_view name) noexcept
    {
        if(name == "gl4") return SG_API_TYPE_GL4;
        if(name == "gl46") return SG_API_TYPE_GL46;
        if(name == "gles2") return SG_API_TYPE_GLES2;
        if(name == "gles3") return SG_API_TYPE_GLES3;
        if(name == "vulkan") return SG_API_TYPE_VULKAN;
        if(name == "dx12") return SG_API_TYPE_DX12;

        return std::nullopt;
    }
}

#endif //SUNGEARENGINE_APITYPE_H
