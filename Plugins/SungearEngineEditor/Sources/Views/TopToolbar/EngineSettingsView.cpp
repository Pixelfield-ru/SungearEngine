//
// Created by Ilya on 17.07.2024.
//

#include "EngineSettingsView.h"
#include <imgui_internal.h>

void SGE::EngineSettingsView::renderBody()
{
    // m_isPopupWindow = true;

    // ImGui::SetWindowSize(ImVec2(100, 100));

    if(m_enableDocking && m_firstTime)
    {
        m_firstTime = false;

        ImGui::DockBuilderRemoveNode(m_dockspaceID);
        // Add empty node
        ImGui::DockBuilderAddNode(m_dockspaceID, m_dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
        // Main node should cover entire window
        ImGui::DockBuilderSetNodeSize(m_dockspaceID, ImGui::GetWindowSize());
        // get id of main dock space area
        ImGuiID dockspaceMainID = m_dockspaceID;
        // Create a dock node for the right docked window
        ImGuiID left = ImGui::DockBuilderSplitNode(dockspaceMainID, ImGuiDir_Left, 0.25f, nullptr, &dockspaceMainID);

        // ImGui::DockBuilderDockWindow("Content One", dockspaceMainID);
        // ImGui::DockBuilderDockWindow("Content Two", dockspaceMainID);
        ImGui::DockBuilderDockWindow("LeftParametersTree", left);
        ImGui::DockBuilderFinish(m_dockspaceID);
    }

    ImGui::Begin("LeftParametersTree", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("sdsdfsfsdfsdfsdfsdfsdfsdf sdg sdfgsd fgd");
    ImGui::End();

    m_enableDocking = true;
}

void SGE::EngineSettingsView::postRenderBody()
{
}
