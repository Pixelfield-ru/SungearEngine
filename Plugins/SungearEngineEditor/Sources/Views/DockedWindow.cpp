//
// Created by Ilya on 22.07.2024.
//

#include "DockedWindow.h"
#include "imgui_internal.h"

bool SGE::DockedWindow::begin()
{
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoUndocking;
    ImGui::SetNextWindowClass(&windowClass);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, m_padding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, m_itemsSpacing);

    ImGui::Begin(m_name.c_str(), nullptr,
                 ImGuiWindowFlags_NoTitleBar);

    return true;
}

void SGE::DockedWindow::end()
{
    m_currentSize = ImGui::GetWindowSize();

    ImGui::End();
    ImGui::PopStyleVar(2);

    if(m_currentSize.x < m_minSize.x)
    {
        ImGui::DockBuilderSetNodeSize(m_thisDockNodeID, { m_minSize.x, m_currentSize.y });
    }

    if(m_currentSize.y < m_minSize.y)
    {
        ImGui::DockBuilderSetNodeSize(m_thisDockNodeID, { m_currentSize.x, m_minSize.y });
    }
}

void SGE::DockedWindow::split(ImGuiDir splitDir, float sizeRatioForNodeAtDir, SGE::DockedWindow* outDockedWindowAtDir,
                              SGE::DockedWindow* outDockedWindowAtOppositeDir)
{
    if (ImGui::DockBuilderGetNode(m_thisDockNodeID))
    {
        ImGui::DockBuilderSplitNode(m_thisDockNodeID, splitDir, sizeRatioForNodeAtDir,
                                    outDockedWindowAtDir ? &outDockedWindowAtDir->m_thisDockNodeID : nullptr,
                                    outDockedWindowAtOppositeDir ? &outDockedWindowAtOppositeDir->m_thisDockNodeID : nullptr);

        if(outDockedWindowAtDir)
        {
            ImGui::DockBuilderDockWindow(outDockedWindowAtDir->m_name.c_str(), outDockedWindowAtDir->m_thisDockNodeID);
        }

        if(outDockedWindowAtOppositeDir)
        {
            ImGui::DockBuilderDockWindow(outDockedWindowAtOppositeDir->m_name.c_str(),
                                         outDockedWindowAtOppositeDir->m_thisDockNodeID);
        }

        ImGui::DockBuilderFinish(m_thisDockNodeID);
    }
}
