//
// Created by ilya on 23.06.24.
//

#include <imgui.h>
#include <imgui_internal.h>
#include "InspectorView.h"

#include <SGCore/Render/DebugDraw.h>
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

    if(ImGui::CollapsingHeader("Profiler (TO REMOVE)"))
    {
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
    }

    ImGui::Separator();

    if(ImGui::CollapsingHeader("Engine Settings"))
    {
        auto debugRenderPass = SGCore::RenderPipelinesManager::getCurrentRenderPipeline()->getRenderPass<SGCore::DebugDraw>();

        static const char* debugModes[] {
            "No debug", "Wireframe"
        };

        ImGui::Combo("Debug Draw Mode", (int*) &debugRenderPass->m_mode, debugModes, 2);
    }

    ImGui::Separator();

    if(m_currentChosenEntity != entt::null && SGCore::Scene::getCurrentScene())
    {
        auto ecsRegistry = SGCore::Scene::getCurrentScene()->getECSRegistry();

        ImGui::Text("Entity: %d", m_currentChosenEntity);

        ImGui::Text("Components");

        auto& entityBaseInfo = ecsRegistry->get<SGCore::EntityBaseInfo>(m_currentChosenEntity);
        if(ImGui::CollapsingHeader("Entity Base Info"))
        {
            ImGui::Text("Name: %s", entityBaseInfo.getName().c_str());
            if(entityBaseInfo.getParent() != entt::null)
            {
                auto& parentBaseInfo = ecsRegistry->get<SGCore::EntityBaseInfo>(entityBaseInfo.getParent());
                ImGui::Text("Parent: %llu (name: %s)", entityBaseInfo.getParent(), parentBaseInfo.getName().c_str());
            }
            else
            {
                ImGui::Text("Parent: null");
            }
        }

        auto* tmpTransform = ecsRegistry->tryGet<SGCore::Transform>(m_currentChosenEntity);
        if(tmpTransform != nullptr && ImGui::CollapsingHeader("Transform"))
        {
            auto& transform = *tmpTransform;

            ImGui::DragFloat3("Position", &transform->m_ownTransform.m_position.x);
        }
    }

    ImGui::End();

    ImGui::PopStyleVar(2);
}

void SGE::InspectorView::end()
{
    IView::end();
}
