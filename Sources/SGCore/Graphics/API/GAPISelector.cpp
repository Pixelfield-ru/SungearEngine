//
// Created by 8bitniksis on 16.08.2026.
//

#include "GAPISelector.h"

#include <algorithm>
#include <cstdlib>

#include "SGCore/Logger/Logger.h"
#include "SGCore/Utils/Platform.h"

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"
#include "SGCore/Graphics/API/Vulkan/VkRenderer.h"

#if defined(_WIN32)
#include "SGCore/Graphics/API/DX12/DX12Renderer.h"
#endif

std::vector<SGCore::GAPIType> SGCore::GAPISelector::s_preference = SGCore::GAPISelector::getDefaultPreference();

std::vector<SGCore::GAPIType> SGCore::GAPISelector::getDefaultPreference() noexcept
{
    // Order is the design intent (see docs/RHI_DESIGN.md): explicit APIs first, OpenGL as
    // the permanent fallback with GL46 as its primary implementation (decision of 2026-08-17).
    // Backends that are not implemented yet are skipped at selection. Note that a failed
    // GL46Renderer::confirmSupport() still closes the window instead of falling back to GL4
    // (task 1.3a).
    // Vulkan is buildable but its legacy facades are incomplete (stage 2 in progress): it stays out
    // of the default order and is reached through SG_GAPI=vulkan or setPreference() until the smoke
    // test passes on it. Then it moves in front of GL46 here.
    // DX12 (stage 3) is at an earlier point still — the device comes up but nothing renders — so it
    // is likewise reachable only through SG_GAPI=dx12. Listing it here would make an unfinished
    // backend the default on Windows and give every app an empty frame.
#if SG_PLATFORM_OS_WINDOWS
    return { SG_API_TYPE_GL46, SG_API_TYPE_GL4 };
#elif SG_PLATFORM_OS_ANDROID
    return { SG_API_TYPE_GL4 };
#else
    return { SG_API_TYPE_GL46, SG_API_TYPE_GL4 };
#endif
}

const std::vector<SGCore::GAPIType>& SGCore::GAPISelector::getPreference() noexcept
{
    return s_preference;
}

void SGCore::GAPISelector::setPreference(std::vector<GAPIType> preference) noexcept
{
    s_preference = std::move(preference);
}

std::optional<SGCore::GAPIType> SGCore::GAPISelector::getForcedFromEnvironment() noexcept
{
    const char* forcedValue = std::getenv(forced_gapi_env_var.data());
    if(!forcedValue || *forcedValue == '\0')
    {
        return std::nullopt;
    }

    const auto forcedType = gapiTypeFromString(forcedValue);
    if(!forcedType)
    {
        SG_LOG_W("Environment variable {} holds unknown graphics API '{}'. It will be ignored.",
                 forced_gapi_env_var, forcedValue);
    }

    return forcedType;
}

SGCore::Ref<SGCore::IRenderer> SGCore::GAPISelector::createRenderer(GAPIType apiType) noexcept
{
    switch(apiType)
    {
        case SG_API_TYPE_GL4:
            return GL4Renderer::getInstance();
        case SG_API_TYPE_GL46:
            return GL46Renderer::getInstance();
        // GLES on Android is currently served by GL4Renderer under the GL4 identifier
        // (see GL4Renderer::getInstance()); GLES2/GLES3 get their own entries when that is untangled.
        case SG_API_TYPE_VULKAN:
            return VkRenderer::getInstance();
        case SG_API_TYPE_DX12:
#if defined(_WIN32)
            return DX12Renderer::getInstance();
#else
            return nullptr;
#endif
        case SG_API_TYPE_UNKNOWN:
        case SG_API_TYPE_GLES2:
        case SG_API_TYPE_GLES3:
            return nullptr;
    }

    return nullptr;
}

SGCore::Ref<SGCore::IRenderer> SGCore::GAPISelector::selectRenderer() noexcept
{
    std::vector<GAPIType> candidates;
    candidates.reserve(s_preference.size() + 1);

    if(const auto forcedType = getForcedFromEnvironment())
    {
        SG_LOG_I("Graphics API '{}' is forced by environment variable {}.",
                 gapiTypeToString(*forcedType), forced_gapi_env_var);
        candidates.push_back(*forcedType);
    }

    for(const auto candidate : s_preference)
    {
        if(std::find(candidates.begin(), candidates.end(), candidate) == candidates.end())
        {
            candidates.push_back(candidate);
        }
    }

    for(std::size_t i = 0; i < candidates.size(); ++i)
    {
        const auto candidate = candidates[i];
        auto renderer = createRenderer(candidate);

        if(!renderer)
        {
            SG_LOG_I("Graphics API '{}' is not available in this build. Trying next one...",
                     gapiTypeToString(candidate));
            continue;
        }

        if(i > 0)
        {
            SG_LOG_W("Graphics API '{}' selected as fallback. Preferred API '{}' is not available.",
                     gapiTypeToString(candidate), gapiTypeToString(candidates[0]));
        }
        else
        {
            SG_LOG_I("Graphics API '{}' selected.", gapiTypeToString(candidate));
        }

        return renderer;
    }

    SG_LOG_C("No graphics API from the preference list is available in this build.");

    return nullptr;
}
