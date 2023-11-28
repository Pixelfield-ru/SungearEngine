//
// Created by stuka on 28.11.2023.
//

#include "Window.h"
#include "imgui.h"

void SGEditor::Window::render() noexcept
{
    ImGui::Begin(m_name.c_str());
    {
        renderSubViews();

        (*m_onRenderEvent)();
    }
    ImGui::End();
}
