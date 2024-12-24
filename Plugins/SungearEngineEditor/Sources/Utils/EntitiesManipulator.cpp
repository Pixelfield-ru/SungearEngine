//
// Created by stuka on 23.12.2024.
//
#include "EntitiesManipulator.h"

#include <SGCore/Render/RenderingBase.h>
#include <SGCore/Transformations/Transform.h>
#include <SGCore/Scene/EntityBaseInfo.h>

void SGE::EntitiesManipulator::getDeltaBetweenMatrices(const glm::mat4& originalMatrix, const glm::mat4& updatedMatrix,
                                                       glm::vec3& deltaTranslation, glm::quat& deltaRotation,
                                                       glm::vec3& deltaScale) noexcept
{
    glm::vec3 originalTranslation, originalScale, skew;
    glm::quat originalRotation;
    glm::vec4 perspective;
    glm::decompose(originalMatrix, originalScale, originalRotation, originalTranslation, skew, perspective);

    glm::vec3 updatedTranslation, updatedScale;
    glm::quat updatedRotation;
    glm::decompose(updatedMatrix, updatedScale, updatedRotation, updatedTranslation, skew, perspective);

    deltaTranslation = updatedTranslation - originalTranslation;
    deltaScale = updatedScale - originalScale;

    deltaRotation = updatedRotation * glm::inverse(originalRotation);
}

void SGE::EntitiesManipulator::manipulateEntities(const SGCore::Scene& forScene,
                                                  const SGCore::entity_t& camera3DEntity,
                                                  const SGCore::Camera3D& forCamera3D) noexcept
{
    ImGuizmo::SetOrthographic(m_isOrtho);
    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(m_rectPos.x, m_rectPos.y, m_rectSize.x, m_rectSize.y);

    SGCore::Ref<SGCore::RenderingBase> camera3DRenderingBase;
    {
        auto* tmp = forScene.getECSRegistry()->try_get<SGCore::Ref<SGCore::RenderingBase>>(camera3DEntity);
        if(!tmp) return;

        camera3DRenderingBase = *tmp;
    }

    std::vector<SGCore::Ref<SGCore::Transform>> entitiesTransforms;
    std::vector<SGCore::EntityBaseInfo*> entitiesBaseInfo;

    for(const auto& entity : forCamera3D.m_pickedEntities)
    {
        {
            auto* tmp = forScene.getECSRegistry()->try_get<SGCore::Ref<SGCore::Transform>>(entity);
            if(!tmp) continue;

            entitiesTransforms.push_back(*tmp);
        }

        {
            auto* tmp = forScene.getECSRegistry()->try_get<SGCore::EntityBaseInfo>(entity);
            if(!tmp) continue;

            entitiesBaseInfo.push_back(tmp);
        }
    }

    // manipulating 1 object
    if(entitiesTransforms.size() == 1 && entitiesBaseInfo.size() == 1)
    {
        SGCore::Ref<SGCore::Transform> parentTransform;

        auto tmpMatrix = entitiesTransforms[0]->m_finalTransform.m_modelMatrix;

        if(entitiesBaseInfo[0]->getParent() != entt::null)
        {
            {
                auto* tmp = forScene.getECSRegistry()->try_get<SGCore::Ref<SGCore::Transform>>(
                        entitiesBaseInfo[0]->getParent());

                if(tmp)
                {
                    parentTransform = *tmp;
                }
            }
        }

        if(ImGuizmo::Manipulate(
                glm::value_ptr(camera3DRenderingBase->m_viewMatrix),
                glm::value_ptr(camera3DRenderingBase->m_projectionMatrix),
                m_manipulationOperation,                                                // current manipulation operation
                m_mode,                                                                 // local or world mode
                glm::value_ptr(tmpMatrix)
        ))
        {
            if(parentTransform)
            {
                tmpMatrix = glm::inverse(parentTransform->m_finalTransform.m_modelMatrix) * tmpMatrix;
            }

            glm::mat4 originalMatrix = entitiesTransforms[0]->m_ownTransform.m_modelMatrix;

            glm::vec3 deltaTranslation;
            glm::quat deltaRotation;
            glm::vec3 deltaScale;
            getDeltaBetweenMatrices(originalMatrix, tmpMatrix, deltaTranslation, deltaRotation, deltaScale);

            entitiesTransforms[0]->m_ownTransform.m_position += deltaTranslation;

            entitiesTransforms[0]->m_ownTransform.m_rotation = deltaRotation * entitiesTransforms[0]->m_ownTransform.m_rotation;

            entitiesTransforms[0]->m_ownTransform.m_scale += deltaScale;
        }
    }
    else if(entitiesTransforms.size() > 1) // manipulating group of objects
    {

    }
}

