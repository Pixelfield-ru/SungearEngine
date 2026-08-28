//
// Created by 8bitniksis on 16.08.2026.
//

#pragma once

#include <optional>
#include <vector>
#include <sgcore_export.h>

#include "GAPIType.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IRenderer;

    /**
     * Chooses which graphics backend the engine runs on.
     *
     * Order of resolution:
     *   1. Backend forced through the environment variable SG_GAPI
     *      (values are the identifiers of gapiTypeToString(): "gl4", "gl46", "vulkan", "dx12" ...).
     *   2. The preference list (setPreference() or the platform default).
     *
     * A backend that is not implemented in this build is skipped with a log message,
     * so the preference list may already contain backends that arrive in later stages.
     */
    struct SGCORE_EXPORT GAPISelector
    {
        /// Name of the environment variable used to force a backend.
        static constexpr std::string_view forced_gapi_env_var = "SG_GAPI";

        /// Platform default order. Backends unavailable in the current build are skipped at selection.
        [[nodiscard]] static std::vector<GAPIType> getDefaultPreference() noexcept;

        [[nodiscard]] static const std::vector<GAPIType>& getPreference() noexcept;
        /// Must be called before CoreMain::init() to take effect.
        static void setPreference(std::vector<GAPIType> preference) noexcept;

        /// Backend forced by SG_GAPI, if the variable is set and holds a known identifier.
        [[nodiscard]] static std::optional<GAPIType> getForcedFromEnvironment() noexcept;

        /// Creates the renderer for the given backend. Returns nullptr if the backend
        /// is not implemented in this build.
        [[nodiscard]] static Ref<IRenderer> createRenderer(GAPIType apiType) noexcept;

        /// Resolves the backend (forced → preference) and creates its renderer.
        /// Returns nullptr only if no backend from the list is available.
        [[nodiscard]] static Ref<IRenderer> selectRenderer() noexcept;

    private:
        static std::vector<GAPIType> s_preference;
    };
}
