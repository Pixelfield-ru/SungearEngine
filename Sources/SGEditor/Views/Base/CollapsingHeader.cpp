//
// Created by stuka on 28.11.2023.
//

#include "CollapsingHeader.h"
#include "imgui.h"

void SGEditor::CollapsingHeader::render() noexcept
{
    if(ImGui::CollapsingHeader(m_name.c_str()))
    {
        renderSubViews();

        (*m_onRenderEvent)();
    }
}
