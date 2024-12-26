//
// Created by stuka on 23.12.2024.
//

#ifndef SUNGEARENGINEEDITOR_ENTITIESMANIPULATOR_H
#define SUNGEARENGINEEDITOR_ENTITIESMANIPULATOR_H

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Render/RenderingBase.h>
#include <SGCore/Transformations/Transform.h>
#include <SGCore/Scene/EntityBaseInfo.h>

namespace SGE
{
    struct EntitiesManipulator
    {
        bool m_isOrtho = false;
        ImVec2 m_rectPos { };
        ImVec2 m_rectSize { };

        ImGuizmo::MODE m_mode = ImGuizmo::MODE::WORLD;

        ImGuizmo::OPERATION m_manipulationOperation =
                ImGuizmo::OPERATION::TRANSLATE |
                ImGuizmo::OPERATION::ROTATE |
                ImGuizmo::OPERATION::SCALE;


        template<typename ContainerT>
        void manipulateEntities(const SGCore::Scene& forScene,
                                const SGCore::entity_t& camera3DEntity,
                                const ContainerT& manipulatingEntities) noexcept
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

            for(const auto& entity : manipulatingEntities)
            {
                auto* tmpTransform = forScene.getECSRegistry()->try_get<SGCore::Ref<SGCore::Transform>>(entity);
                auto* tmpBaseInfo = forScene.getECSRegistry()->try_get<SGCore::EntityBaseInfo>(entity);

                if(!tmpTransform || !tmpBaseInfo) continue;

                entitiesTransforms.push_back(*tmpTransform);

                entitiesBaseInfo.push_back(tmpBaseInfo);
            }

            // manipulating each entity base first entity
            if(!entitiesTransforms.empty() && !entitiesBaseInfo.empty())
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

                /*glm::mat4 deltaM;
                glm::mat4 tmpMatrix2 = tmpMatrix;
                if(ImGuizmo::Manipulate(
                        glm::value_ptr(camera3DRenderingBase->m_viewMatrix),
                        glm::value_ptr(camera3DRenderingBase->m_projectionMatrix),
                        m_manipulationOperation,                                                // current manipulation operation
                        m_mode,                                                                 // local or world mode
                        glm::value_ptr(tmpMatrix2),
                        glm::value_ptr(deltaM)
                ))
                {
                    for(size_t i = 0; i < entitiesTransforms.size(); ++i)
                    {
                        auto& transform = entitiesTransforms[i];
                        auto* baseInfo = entitiesBaseInfo[i];

                        SGCore::Ref<SGCore::Transform> childEntityParent;

                        if(baseInfo->getParent() != entt::null)
                        {
                            {
                                auto* tmp = forScene.getECSRegistry()->try_get<SGCore::Ref<SGCore::Transform>>(
                                        baseInfo->getParent());

                                if(tmp)
                                {
                                    childEntityParent = *tmp;
                                }
                            }
                        }

                        tmpMatrix = deltaM * transform->m_finalTransform.m_modelMatrix;
                        if(childEntityParent)
                        {
                            tmpMatrix = glm::inverse(childEntityParent->m_finalTransform.m_modelMatrix) * tmpMatrix;
                        }

                        glm::vec3 deltaTranslation;
                        glm::quat deltaRotation;
                        glm::vec3 deltaScale;
                        getDeltaBetweenMatrices(transform->m_ownTransform.m_modelMatrix, tmpMatrix, deltaTranslation, deltaRotation, deltaScale);

                        transform->m_ownTransform.m_position += deltaTranslation;

                        transform->m_ownTransform.m_rotation =
                                deltaRotation * transform->m_ownTransform.m_rotation;

                        transform->m_ownTransform.m_scale *= deltaScale;
                    }
                }*/

                glm::mat4 deltaM;
                if(ImGuizmo::Manipulate(
                        glm::value_ptr(camera3DRenderingBase->m_viewMatrix),
                        glm::value_ptr(camera3DRenderingBase->m_projectionMatrix),
                        m_manipulationOperation,                                                // current manipulation operation
                        m_mode,                                                                 // local or world mode
                        glm::value_ptr(tmpMatrix),
                        glm::value_ptr(deltaM)
                ))
                {
                    if(parentTransform)
                    {
                        tmpMatrix = glm::inverse(parentTransform->m_finalTransform.m_modelMatrix) * tmpMatrix;
                    }

                    // glm::mat4 deltaMatrix = tmpMatrix * glm::inverse(entitiesTransforms[0]->m_ownTransform.m_modelMatrix);
                    glm::mat4 deltaMatrix;

                    {
                        glm::mat4 originalMatrix = entitiesTransforms[0]->m_ownTransform.m_modelMatrix;

                        glm::vec3 deltaTranslation;
                        glm::quat deltaRotation;
                        glm::vec3 deltaScale;
                        getDeltaBetweenMatrices(originalMatrix, tmpMatrix, deltaTranslation, deltaRotation, deltaScale);

                        entitiesTransforms[0]->m_ownTransform.m_position += deltaTranslation;

                        entitiesTransforms[0]->m_ownTransform.m_rotation =
                                deltaRotation * entitiesTransforms[0]->m_ownTransform.m_rotation;

                        entitiesTransforms[0]->m_ownTransform.m_scale *= deltaScale;

                        deltaMatrix = glm::translate(glm::mat4(1.0), deltaTranslation);
                        deltaMatrix *= glm::toMat4(deltaRotation);
                        deltaMatrix *= glm::scale(glm::mat4(1.0), deltaScale);
                    }

                    if(entitiesTransforms.size() > 1)
                    {
                        for(size_t i = 1; i < entitiesTransforms.size(); ++i)
                        {
                            auto& transform = entitiesTransforms[i];
                            auto* baseInfo = entitiesBaseInfo[i];

                            SGCore::Ref<SGCore::Transform> childEntityParent;

                            if(baseInfo->getParent() != entt::null)
                            {
                                {
                                    auto* tmp = forScene.getECSRegistry()->try_get<SGCore::Ref<SGCore::Transform>>(
                                            baseInfo->getParent());

                                    if(tmp)
                                    {
                                        childEntityParent = *tmp;
                                    }
                                }
                            }

                            auto finalMat = deltaMatrix;
                            if(parentTransform)
                            {
                                finalMat = parentTransform->m_finalTransform.m_scaleMatrix * finalMat;
                                finalMat = parentTransform->m_finalTransform.m_rotationMatrix * finalMat;
                            }

                            if(childEntityParent)
                            {
                                finalMat = glm::inverse(childEntityParent->m_finalTransform.m_scaleMatrix) * finalMat;
                                finalMat = glm::inverse(childEntityParent->m_finalTransform.m_rotationMatrix) * finalMat;
                            }

                            glm::vec3 updatedTranslation, updatedScale, skew;
                            glm::quat updatedRotation;
                            glm::vec4 perspective;
                            glm::decompose(finalMat, updatedScale, updatedRotation, updatedTranslation, skew, perspective);

                            std::cout << "translation: " << glm::to_string(updatedTranslation)
                            << ", rotation: " << glm::to_string(updatedRotation)
                            << ", scale: " << glm::to_string(updatedScale) << std::endl;

                            transform->m_ownTransform.m_position += updatedTranslation;
                            // transform->m_ownTransform.m_rotation = updatedRotation * transform->m_ownTransform.m_rotation;
                            // transform->m_ownTransform.m_scale *= updatedScale;
                        }
                    }
                }
            }
        }

        void rotateAroundWorldQuaternion(glm::quat& localRotation, const glm::quat& worldRotation) noexcept;

    private:
        static void getDeltaBetweenMatrices(const glm::mat4& originalMatrix, const glm::mat4& updatedMatrix,
                                            glm::vec3& deltaTranslation, glm::quat& deltaRotation, glm::vec3& deltaScale) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_ENTITIESMANIPULATOR_H
