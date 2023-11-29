//
// Created by stuka on 28.11.2023.
//

#include "SceneHierarchy.h"
#include "imgui.h"
#include "SGCore/ECS/Scene.h"

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
        for(const auto& entity : curScene->m_entities)
        {
            renderEntity(entity);
        }
    }

    (*m_onRenderEvent)();
}

void SGEditor::SceneHierarchy::end() noexcept
{
    ImGui::TreePop();
}

void SGEditor::SceneHierarchy::renderEntity(const SGCore::Ref<SGCore::Entity>& entity) noexcept
{
    size_t sameNameIdx = entity->getSceneSameNameIndex();
    std::string finalEntityName = entity->m_name + (sameNameIdx > 0 ? " (" + std::to_string(sameNameIdx) + ") " : "");

    if(ImGui::TreeNode(finalEntityName.c_str()))
    {
        for(const auto& child : entity->m_children)
        {
            renderEntity(child);
        }

        ImGui::TreePop();
    }
}
