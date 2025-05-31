//
// Created by ilya on 23.06.24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include "InspectorView.h"
#include <SGCore/Render/RenderPipelinesManager.h>
#include <SGCore/Scene/Scene.h>

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

    if(SGCore::Scene::getCurrentScene())
    {
        ImGui::Separator();

        ImGui::Text("Scene systems");

        for(const auto& system : SGCore::Scene::getCurrentScene()->getAllSystems())
        {
            ImGui::Text("System '%s'. 'update' execution time: %f, 'parallelUpdate' execution time: %f, 'fixedUpdate' execution time: %f",
                typeid(*system).name(),
                system->m_executionTimes["update"],
                system->m_executionTimes["parallelUpdate"],
                system->m_executionTimes["fixedUpdate"]);
        }
    }

    ImGui::End();

    ImGui::PopStyleVar(2);
}

void SGE::InspectorView::end()
{
    IView::end();
}
