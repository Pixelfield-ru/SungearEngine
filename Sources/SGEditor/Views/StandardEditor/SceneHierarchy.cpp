//
// Created by stuka on 28.11.2023.
//

#include "SceneHierarchy.h"
#include "imgui.h"
#include "SGCore/ECS/Scene.h"

void SGEditor::SceneHierarchy::render()
{
    ImGui::TreePush(m_name.c_str());
    {
        auto curScene = SGCore::Scene::getCurrentScene();

        if(curScene)
        {
            for(const auto& entity : curScene->m_entities)
            {
                renderEntity(entity);
            }
        }

        renderSubViews();

        (*m_onRenderEvent)();
    }
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
