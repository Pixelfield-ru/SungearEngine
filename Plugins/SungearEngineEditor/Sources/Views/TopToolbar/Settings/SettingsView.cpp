//
// Created by Ilya on 17.07.2024.
//

#include "SettingsView.h"
#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"
#include <Plugins/SungearEngineEditor/cmake-build-release/vcpkg_installed/x64-windows/include/imgui_internal.h>

SGE::SettingsView::SettingsView()
{
    m_isPopupWindow = true;

    m_bodyPadding = { 0, 1 };
    m_bodyMinSize = { 700, 400 };

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
                      .m_text = "Apply",
                      .m_name = "ApplyButton",
                      .onClicked = [this](auto& self) {
                          // cancel();
                      },
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

bool SGE::SettingsView::begin()
{
    bool began = Window::begin();
    if(!began) return false;

    m_dockspaceID = ImGui::GetID((m_name.getName() + "_BodyDockspace").c_str());

    ImGui::DockSpace(m_dockspaceID, ImVec2(0.0f, 0.0f), m_dockspaceFlags);

    if (m_firstTime)
    {
        m_firstTime = false;

        m_settingsTreeDockedWindow = SGCore::MakeRef<DockedWindow>();
        m_settingsTreeDockedWindow->m_name = m_name.getName() + "SettingsTree";
        m_settingsTreeDockedWindow->m_minSize = { 300.0f, 0.0f };
        m_settingsTreeDockedWindow->m_padding = { 0.0f, 0.0f };
        m_settingsTreeDockedWindow->m_itemsSpacing = { 0.0f, 0.0f };

        m_settingsContentDockedWindow = SGCore::MakeRef<DockedWindow>();
        m_settingsContentDockedWindow->m_name = m_name.getName() + "SettingsContent";
        m_settingsContentDockedWindow->m_minSize = { 300.0f, 0.0f };
        m_settingsContentDockedWindow->m_padding = { 5.0f, 7.0f };

        ImGui::DockBuilderRemoveNode(m_dockspaceID);
        ImGui::DockBuilderAddNode(m_dockspaceID, m_dockspaceFlags);
        ImGui::DockBuilderSetNodeSize(m_dockspaceID, ImGui::GetWindowSize());

        ImGui::DockBuilderSplitNode(m_dockspaceID, ImGuiDir_Left, 0.25f, &m_settingsTreeDockedWindow->m_thisDockNodeID,
                                    &m_settingsContentDockedWindow->m_thisDockNodeID);

        ImGui::DockBuilderDockWindow(m_settingsTreeDockedWindow->m_name.getName().c_str(),
                                     m_settingsTreeDockedWindow->m_thisDockNodeID);
        ImGui::DockBuilderDockWindow(m_settingsContentDockedWindow->m_name.getName().c_str(),
                                     m_settingsContentDockedWindow->m_thisDockNodeID);

        ImGui::DockBuilderFinish(m_dockspaceID);

        ImGui::DockBuilderSetNodeSize(m_settingsTreeDockedWindow->m_thisDockNodeID,
                                      { m_settingsTreeDockedWindow->m_minSize.x, 0 });

        ImGui::DockBuilderFinish(m_settingsTreeDockedWindow->m_thisDockNodeID);
    }

    if (m_settingsTreeDockedWindow->begin())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5, 7 });
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 7 });

        ImGui::BeginChildFrame(ImGui::GetID("SettingsTreeContent"),
                               ImGui::GetContentRegionAvail(),
                               ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);

        m_settingsTree.draw({ m_UIScale.x, m_UIScale.y });

        ImGui::EndChildFrame();
        ImGui::PopStyleVar(2);

        m_settingsTreeDockedWindow->end();
    }

    if (m_settingsContentDockedWindow->begin())
    {
        if (onSettingsContentDraw)
        {
            onSettingsContentDraw();
        }

        m_settingsContentDockedWindow->end();
    }

    return true;
}
