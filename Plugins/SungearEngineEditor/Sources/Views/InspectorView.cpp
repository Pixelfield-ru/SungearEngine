//
// Created by ilya on 23.06.24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include "InspectorView.h"
#include <SGCore/Render/RenderPipelinesManager.h>

bool SGE::InspectorView::begin()
{
    return IView::begin();
}

void SGE::InspectorView::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);
    
    ImGui::Begin("InspectorView");

    ImGui::Text("Profiler (TO REMOVE)");

    auto currentRenderPipeline = SGCore::RenderPipelinesManager::getCurrentRenderPipeline();

    ImGui::Text("Render (render pipeline '%s'):", typeid(*currentRenderPipeline).name());

    for(const auto& renderPass : currentRenderPipeline->m_renderPasses)
    {
        ImGui::Text("Render pass '%s' execution time: %f", typeid(*renderPass).name(), renderPass->getExecutionTime());
    }

    ImGui::End();
    
    ImGui::PopStyleVar(2);
}

void SGE::InspectorView::end()
{
    IView::end();
}
