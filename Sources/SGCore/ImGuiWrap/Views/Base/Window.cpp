//
// Created by stuka on 28.11.2023.
//

#include "Window.h"
#include "imgui.h"

bool SGCore::ImGuiWrap::Window::begin() noexcept
{
    ImGui::Begin(m_name.getName().c_str());

    return true;
}

void SGCore::ImGuiWrap::Window::renderBody()
{
    onRender();
}

void SGCore::ImGuiWrap::Window::end() noexcept
{
    ImGui::End();
}
