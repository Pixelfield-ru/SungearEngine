//
// Created by stuka on 23.12.2024.
//
#include "EntitiesManipulator.h"

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
    deltaScale = updatedScale / originalScale;

    deltaRotation = updatedRotation * glm::inverse(originalRotation);
    // deltaRotation = glm::inverse(updatedRotation) * originalRotation;
}

void SGE::EntitiesManipulator::manipulateEntities(const SGCore::Scene& forScene,
                                                  const SGCore::entity_t& camera3DEntity) const noexcept
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
    std::vector<SGCore::EntityBaseInfo::reg_t*> entitiesBaseInfo;

    for(const auto& entity : m_manipulatingEntities)
    {
        auto* tmpTransform = forScene.getECSRegistry()->try_get<SGCore::Ref<SGCore::Transform>>(entity);
        auto* tmpBaseInfo = forScene.getECSRegistry()->try_get<SGCore::EntityBaseInfo::reg_t>(entity);

        if(!tmpTransform || !tmpBaseInfo) continue;

        entitiesTransforms.push_back(*tmpTransform);

        entitiesBaseInfo.push_back(tmpBaseInfo);
    }

    // manipulating each entity base first entity
    if(!entitiesTransforms.empty() && !entitiesBaseInfo.empty())
    {
        SGCore::Ref<SGCore::Transform> firstEntityParentTransform;

        auto tmpMatrix = entitiesTransforms[0]->m_finalTransform.m_modelMatrix;

        if(entitiesBaseInfo[0]->getParent() != entt::null)
        {
            {
                auto* tmp = forScene.getECSRegistry()->try_get<SGCore::Ref<SGCore::Transform>>(
                        entitiesBaseInfo[0]->getParent());

                if(tmp)
                {
                    firstEntityParentTransform = *tmp;
                }
            }
        }

        if(firstEntityParentTransform)
        {
            // tmpMatrix = glm::inverse(firstEntityParentTransform->m_finalTransform.m_scaleMatrix) * tmpMatrix;
        }

        ImGuizmo::OPERATION usedOperation;
        if(ImGuizmo::Manipulate(
                glm::value_ptr(camera3DRenderingBase->m_viewMatrix),
                glm::value_ptr(camera3DRenderingBase->m_projectionMatrix),
                m_manipulationOperation,                                                // current manipulation operation
                m_mode,                                                                 // local or world mode
                glm::value_ptr(tmpMatrix),
                &usedOperation
        ))
        {
            if(firstEntityParentTransform)
            {
                tmpMatrix = glm::inverse(firstEntityParentTransform->m_finalTransform.m_modelMatrix) * tmpMatrix;
                // tmpMatrix = glm::inverse(firstEntityParentTransform->m_finalTransform.m_rotationMatrix * firstEntityParentTransform->m_finalTransform.m_translationMatrix) * tmpMatrix;
            }

            glm::mat4 deltaMatrix;

            bool isTranslated = usedOperation == ImGuizmo::OPERATION::TRANSLATE;
            bool isRotated = usedOperation == ImGuizmo::OPERATION::ROTATE;
            bool isScaled = usedOperation == ImGuizmo::OPERATION::SCALE;

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

                std::cout << "translation: " << glm::to_string(deltaTranslation)
                          << ", rotation: " << glm::to_string(deltaRotation)
                          << ", scale: " << fmt::format("{}, {}, {} ", deltaScale.x, deltaScale.y, deltaScale.z) << std::endl;

                deltaMatrix = glm::translate(glm::mat4(1.0), deltaTranslation);
                deltaMatrix *= glm::toMat4(deltaRotation);
                deltaMatrix *= glm::scale(glm::mat4(1.0), deltaScale);;
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

                    // for translation
                    if(isTranslated)
                    {
                        if(firstEntityParentTransform)
                        {
                            finalMat = firstEntityParentTransform->m_finalTransform.m_scaleMatrix * finalMat;
                            finalMat = firstEntityParentTransform->m_finalTransform.m_rotationMatrix * finalMat;
                        }

                        if(childEntityParent)
                        {
                            finalMat = glm::inverse(childEntityParent->m_finalTransform.m_scaleMatrix) *
                                       finalMat;
                            finalMat = glm::inverse(childEntityParent->m_finalTransform.m_rotationMatrix) *
                                       finalMat;
                        }
                    }

                    // for scale
                    if(isScaled)
                    {
                        if(firstEntityParentTransform)
                        {
                            finalMat = finalMat * glm::inverse(firstEntityParentTransform->m_finalTransform.m_rotationMatrix);
                        }

                        if(childEntityParent)
                        {
                            finalMat = finalMat * childEntityParent->m_finalTransform.m_rotationMatrix;
                        }
                    }

                    if(isRotated)
                    {
                        // for rotation
                        if(firstEntityParentTransform)
                        {
                            finalMat = (firstEntityParentTransform->m_finalTransform.m_rotationMatrix *
                                        finalMat *
                                        glm::inverse(firstEntityParentTransform->m_finalTransform.m_rotationMatrix));
                        }

                        finalMat = (glm::inverse(transform->m_finalTransform.m_rotationMatrix) *
                                    finalMat *
                                    transform->m_finalTransform.m_rotationMatrix);
                    }

                    glm::vec3 updatedTranslation, updatedScale, skew;
                    glm::quat updatedRotation;
                    glm::vec4 perspective;
                    glm::decompose(finalMat, updatedScale, updatedRotation, updatedTranslation, skew, perspective);

                    if(isTranslated)
                    {
                        transform->m_ownTransform.m_position += updatedTranslation;
                        std::cout << "translating" << std::endl;
                    }
                    if(isRotated)
                    {
                        transform->m_ownTransform.m_rotation =
                                transform->m_ownTransform.m_rotation * updatedRotation;
                        std::cout << "rotating" << std::endl;
                    }
                    if(isScaled)
                    {
                        transform->m_ownTransform.m_scale *= updatedScale;
                        std::cout << "scaling" << std::endl;
                    }
                }
            }
        }
    }
}

/*void SGE::EntitiesManipulator::rotateAroundWorldAxis(glm::mat4& localMatrix, const glm::mat4& parentMatrix,
                                                     const glm::vec3& worldAxis, float angle) noexcept
{
// Вычисление мировой матрицы
    glm::mat4 worldMatrix = parentMatrix * localMatrix;

    // Матрица вращения вокруг мировой оси
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), worldAxis);

    // Применяем вращение в мировом пространстве
    glm::mat4 worldMatrixRotated = rotationMatrix * worldMatrix;

    // Возвращаем матрицу в локальное пространство
    glm::mat4 parentInverse = glm::inverse(parentMatrix);
    localMatrix = parentInverse * worldMatrixRotated;
}*/

