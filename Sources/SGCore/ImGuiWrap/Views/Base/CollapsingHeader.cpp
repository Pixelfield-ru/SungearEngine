//
// Created by stuka on 28.11.2023.
//

#include "CollapsingHeader.h"
#include "imgui.h"

bool SGCore::ImGuiWrap::CollapsingHeader::begin() noexcept
{
    return ImGui::CollapsingHeader(m_name.getName().c_str());
}

void SGCore::ImGuiWrap::CollapsingHeader::renderBody()
{
    onRender();
}

void SGCore::ImGuiWrap::CollapsingHeader::end() noexcept
{

}
