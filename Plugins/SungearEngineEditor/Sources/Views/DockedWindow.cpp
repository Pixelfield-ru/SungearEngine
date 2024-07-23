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
    ImGui::Begin(m_name.getName().c_str(), nullptr,
                 ImGuiWindowFlags_NoTitleBar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, m_itemsSpacing);

    return true;
}

void SGE::DockedWindow::end()
{
    m_currentSize = ImGui::GetWindowSize();

    ImGui::PopStyleVar();
    ImGui::End();
    ImGui::PopStyleVar();

    // TODO: WAITING FOR IMGUI IMPLEMENTATION OF MIN SIZE OF DOCKED WINDOW...(
    /*if(m_currentSize.x < m_minSize.x)
    {
        std::printf("name: %s. current size: %f, %f\n", m_name.getName().c_str(), m_currentSize.x, m_currentSize.y);
        ImGui::DockBuilderSetNodeSize(m_thisDockNodeID, { m_minSize.x, m_currentSize.y });
        ImGui::DockBuilderFinish(m_thisDockNodeID);
    }

    if(m_currentSize.y < m_minSize.y)
    {
        ImGui::DockBuilderSetNodeSize(m_thisDockNodeID, { m_currentSize.x, m_minSize.y });
        ImGui::DockBuilderFinish(m_thisDockNodeID);
    }*/
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
            ImGui::DockBuilderDockWindow(outDockedWindowAtDir->m_name.getName().c_str(), outDockedWindowAtDir->m_thisDockNodeID);
        }

        if(outDockedWindowAtOppositeDir)
        {
            ImGui::DockBuilderDockWindow(outDockedWindowAtOppositeDir->m_name.getName().c_str(),
                                         outDockedWindowAtOppositeDir->m_thisDockNodeID);
        }

        ImGui::DockBuilderFinish(m_thisDockNodeID);
    }
}
