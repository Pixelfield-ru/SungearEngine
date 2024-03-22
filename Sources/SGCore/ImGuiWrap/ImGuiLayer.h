//
// Created by Ilya on 18.11.2023.
//

#ifndef SUNGEARENGINE_IMGUILAYER_H
#define SUNGEARENGINE_IMGUILAYER_H

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include <sgcore_export.h>

namespace SGCore::ImGuiWrap
{
    // TODO: ADD ON EXIT CLEAR
    struct SGCORE_EXPORT ImGuiLayer
    {
        static void initImGui() noexcept;

        static void beginFrame() noexcept;

        static void endFrame() noexcept;
    };
}

#endif // SUNGEARENGINE_IMGUILAYER_H
