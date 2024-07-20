//
// Created by Ilya on 17.07.2024.
//

#include "EngineSettingsView.h"
#include <imgui_internal.h>

SGE::EngineSettingsView::EngineSettingsView()
{
    m_bodyPadding = { 0, 0 };

    const auto buttonsSize = ImVec2(75, 0);

    addButton({
                      .m_text = "OK",
                      .m_name = "OKButton",
                      .isFastClicked = [](auto& self) -> bool {
                          return ImGui::IsKeyPressed(ImGuiKey_Enter);
                      },
                      .onClicked = [this](auto& self) {
                          // submit();
                      },
                      .m_color = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1),
                      .m_hoveredColor = ImVec4(0 / 255.0f, 70 / 255.0f, 110 / 255.0f, 1),
                      .m_borderColor = { 0, 0, 0, 0 },
                      .m_borderShadowColor = { 0, 0, 0, 0 },
                      .m_size = buttonsSize
              });

    addButton({
                      .m_text = "Cancel",
                      .m_name = "CancelButton",
                      .isFastClicked = [](auto& self) -> bool {
                          return ImGui::IsKeyPressed(ImGuiKey_Escape);
                      },
                      .onClicked = [this](auto& self) {
                          // cancel();
                      },
                      .m_size = buttonsSize
              });
}

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

    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoUndocking;
    ImGui::SetNextWindowClass(&windowClass);


    if(m_currentTreeSize.x < 100.0f)
    {
    }
    ImGui::SetNextWindowSize({ 100.0f, m_currentTreeSize.y });

    ImGui::Begin("LeftParametersTree", nullptr,
                 ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("sdsdfsfsdfsdfsdfsdfsdfsdf sdg sdfgsd fgd");
    m_currentTreeSize = ImGui::GetWindowSize();
    ImGui::End();

    m_enableDocking = true;
}

void SGE::EngineSettingsView::postRenderBody()
{

}
