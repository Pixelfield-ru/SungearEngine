//
// Created by Ilya on 18.11.2023.
//

#pragma once

#include <sgcore_export.h>

#include <imgui.h>

#include "SGCore/Utils/Macroses.h"

namespace SGCore::ImGuiWrap
{
    // TODO: ADD ON EXIT CLEAR
    struct SGCORE_EXPORT ImGuiLayer
    {
        static void init() noexcept;
        static void destroy() noexcept;
        
        static void reload() noexcept;

        static void beginFrame() noexcept;

        static void endFrame() noexcept;

        SG_NOINLINE static ImGuiContext* getCurrentContext() noexcept;

        SG_NOINLINE static void destroyDeviceObjects() noexcept;

    private:
        static inline ImGuiContext* m_currentContext = nullptr;
    };
}
