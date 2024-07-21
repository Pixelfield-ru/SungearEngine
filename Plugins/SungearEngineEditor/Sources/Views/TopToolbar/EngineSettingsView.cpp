//
// Created by Ilya on 17.07.2024.
//

#include "EngineSettingsView.h"
#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"
#include <imgui_internal.h>

SGE::EngineSettingsView::EngineSettingsView()
{
    m_isPopupWindow = true;

    m_bodyPadding = { 0, 1 };
    m_bodyMinSize = { 700, 400 };
    m_name = "Engine Settings";

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

    TreeNode toolchainsNode {
            .m_text = "Toolchains",
            .onClicked = [this](TreeNode& self) {
                onSettingsContentDraw = []() {
                    ImGui::Text("Hello from Toolchains!");
                };
            }
    };

    TreeNode devAndSupportNode {
            .m_text = "Development and Support",
            .onClicked = [this](TreeNode& self) {
                onSettingsContentDraw = []() {
                    ImGui::Text("Hello from Development and Support!");
                };
            },
            .m_children = { toolchainsNode },
            .m_isTree = true
    };

    m_treeNodes.push_back(devAndSupportNode);
}

void SGE::EngineSettingsView::renderBody()
{
    if(m_enableDocking && m_firstTime)
    {
        m_firstTime = false;

        ImGui::DockBuilderRemoveNode(m_dockspaceID);
        ImGui::DockBuilderAddNode(m_dockspaceID, m_dockspaceFlags);
        ImGui::DockBuilderSetNodeSize(m_dockspaceID, ImGui::GetWindowSize());

        ImGui::DockBuilderSplitNode(m_dockspaceID, ImGuiDir_Left, 0.25f, &m_treeDockNodeID, &m_settingsContentDockNodeID);

        ImGui::DockBuilderDockWindow("LeftParametersTree", m_treeDockNodeID);
        ImGui::DockBuilderDockWindow("SettingsContent", m_settingsContentDockNodeID);

        ImGui::DockBuilderFinish(m_dockspaceID);

        ImGui::DockBuilderSetNodeSize(m_treeDockNodeID, { m_minTreeWindowWidth, m_currentTreeSize.y });
    }

    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoUndocking;
    ImGui::SetNextWindowClass(&windowClass);

    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::Begin("LeftParametersTree", nullptr,
                     ImGuiWindowFlags_NoTitleBar);

        // tree =========================================

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5, 7 });
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5, 7 });

        ImGui::BeginChildFrame(ImGui::GetID("EngineSettingsTree"),
                               ImGui::GetContentRegionAvail(),
                               ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);

        if (m_drawTreeSelectedNodeRect)
        {
            ImGui::GetWindowDrawList()->AddRectFilled(m_clickedTreeRowRectMin, m_clickedTreeRowRectMax,
                                                      ImGui::ColorConvertFloat4ToU32(
                                                              ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1)), 3.0f);
        }

        m_treeWindowCursorPos = ImGui::GetCursorScreenPos();
        m_treeWindowContentRegionMax = ImGui::GetContentRegionAvail();

        m_drawTreeSelectedNodeRect = false;

        renderTreeNodes(m_treeNodes);

        ImGui::EndChildFrame();
        ImGui::PopStyleVar(2);

        // ==============================================

        m_currentTreeSize = ImGui::GetWindowSize();

        ImGui::End();
        ImGui::PopStyleVar(2);

        if (m_currentTreeSize.x < m_minTreeWindowWidth)
        {
            ImGui::DockBuilderSetNodeSize(m_treeDockNodeID, { m_minTreeWindowWidth, m_currentTreeSize.y });
        }
    }

    ImGui::SetNextWindowClass(&windowClass);

    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 7));
        ImGui::Begin("SettingsContent", nullptr,
                     ImGuiWindowFlags_NoTitleBar);

        if(onSettingsContentDraw)
        {
            onSettingsContentDraw();
        }

        m_currentSettingsContentSize = ImGui::GetWindowSize();
        // std::printf("m_currentSettingsContentSize: %f, %f\n", m_currentSettingsContentSize.x, m_currentSettingsContentSize.y);

        ImGui::End();
        ImGui::PopStyleVar();

        if (m_currentSettingsContentSize.x < m_minSettingsContentWindowWidth)
        {
            ImGui::DockBuilderSetNodeSize(m_settingsContentDockNodeID,
                                          { m_minSettingsContentWindowWidth, m_currentSettingsContentSize.y });
        }
    }

    m_enableDocking = true;
}

void SGE::EngineSettingsView::renderTreeNodes(std::vector<TreeNode>& nodes) noexcept
{
    for(auto& node : nodes)
    {
        bool arrowBtnClicked = false;

        auto style = StylesManager::getCurrentStyle();

        if(node.m_isTree)
        {
            if (node.m_isOpened)
            {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
                arrowBtnClicked = ImGuiUtils::ImageButton(
                        style->m_chevronDownIcon->getSpecialization(16, 16)->getTexture()->getTextureNativeHandler(),
                        ImVec2((16 + 6) * m_UIScale.x, (16 + 6) * m_UIScale.y),
                        ImVec2(16 * m_UIScale.x, 16 * m_UIScale.y)).m_isLMBClicked;
            }
            else
            {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
                arrowBtnClicked = ImGuiUtils::ImageButton(
                        style->m_chevronRightIcon->getSpecialization(16, 16)->getTexture()->getTextureNativeHandler(),
                        ImVec2((16 + 6) * m_UIScale.x, (16 + 6) * m_UIScale.y),
                        ImVec2(16 * m_UIScale.x, 16 * m_UIScale.y)).m_isLMBClicked;
            }

            ImGui::SameLine();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        }

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);

        ImGui::Text(node.m_text.c_str());

        auto mouseScreenPos = ImGui::GetCursorScreenPos();

        auto rectMin = ImVec2(m_treeWindowCursorPos.x, mouseScreenPos.y - 25);
        auto rectMax = ImVec2(m_treeWindowCursorPos.x + ImGui::GetScrollX() + m_treeWindowContentRegionMax.x + 3,
                              mouseScreenPos.y - 3);

        bool rowDoubleClicked = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) &&
                                ImGui::IsWindowHovered();

        bool rowClicked = ImGui::IsMouseHoveringRect(rectMin, rectMax) &&
                          ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered();

        if(rowClicked && !arrowBtnClicked)
        {
            m_chosenTreeNodeName = node.m_name;

            node.onClicked(node);
        }

        if(m_chosenTreeNodeName == node.m_name)
        {
            m_clickedTreeRowRectMin = rectMin;
            m_clickedTreeRowRectMax = rectMax;
            m_drawTreeSelectedNodeRect = true;
        }

        if((rowDoubleClicked || arrowBtnClicked) && node.m_isTree)
        {
            node.m_isOpened = !node.m_isOpened;
        }

        if(node.m_isOpened)
        {
            ImGui::Indent(40);
            renderTreeNodes(node.m_children);
            ImGui::Unindent();
        }
    }
}

void SGE::EngineSettingsView::postRenderBody()
{

}
