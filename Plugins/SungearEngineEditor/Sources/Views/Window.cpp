//
// Created by Ilya on 18.07.2024.
//

#include "Window.h"

bool SGE::Window::begin()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();

    ImGui::SetNextWindowPos(ImVec2(center.x - m_size.x / 2.0f, center.y - m_size.y / 2.0f));

    if(m_isPopupWindow)
    {
        if(isActive())
        {
            ImGui::OpenPopup(m_name.getName().c_str());
        }
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    if(m_isPopupWindow)
    {
        ImGui::BeginPopupModal(m_name.getName().c_str(), nullptr,
                               ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                               ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
    }
    else
    {
        ImGui::Begin(m_name.getName().c_str(), nullptr,
                     ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
    }

    m_size = ImGui::GetWindowSize();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 8));
    ImGui::BeginChild(ImGui::GetID("DialogName"), ImVec2(ImGui::GetContentRegionAvail().x, 0),
                      ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY,
                      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7, 8));
    ImGui::Text(m_name.getName().c_str());
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().WindowPadding.x - 2);
    ImGui::Separator();
    ImGui::PopStyleVar();

    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7, 8));

    ImGui::BeginChild(ImGui::GetID((m_name.getName() + "_Body").c_str()), ImVec2(0, 0),
                      ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                      ImGuiChildFlags_AlwaysUseWindowPadding,
                      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
                      ImGuiWindowFlags_NoSavedSettings);

    return isActive();
}

void SGE::Window::end()
{
    ImGui::EndChild();

    ImGui::PopStyleVar(2);

    if(m_isPopupWindow)
    {
        ImGui::EndPopup();
    }
    else
    {
        ImGui::End();
    }

    ImGui::PopStyleVar(2);
}
