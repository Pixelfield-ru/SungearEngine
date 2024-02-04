//
// Created by stuka on 28.11.2023.
//

#include "SceneHierarchy.h"
#include "imgui.h"
#include "SGCore/Scene/Scene.h"

bool SGEditor::SceneHierarchy::begin() noexcept
{
    ImGui::TreePush(m_name.c_str());

    return true;
}

void SGEditor::SceneHierarchy::renderBody()
{
    auto curScene = SGCore::Scene::getCurrentScene();

    if(curScene)
    {
        /*for(const auto& entity : curScene->m_entities)
        {
            renderEntity(entity);
        }*/
    }

    (*m_onRenderEvent)();
}

void SGEditor::SceneHierarchy::end() noexcept
{
    ImGui::TreePop();
}

void SGEditor::SceneHierarchy::renderEntity(const entt::entity& entity) noexcept
{
    /*if(ImGui::TreeNode(entity->getName().c_str()))
    {
        for(const auto& child : entity->getChildren())
        {
            renderEntity(child);
        }

        ImGui::TreePop();
    }*/
}
