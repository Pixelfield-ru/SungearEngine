//
// Created by stuka on 02.05.2023.
//

#include "TransformationsUpdater.h"
#include "TransformBase.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "SGCore/Scene/Scene.h"
#include "GLFW/glfw3.h"
#include "TransformationsUtils.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/ECSObservers/Flags/ModelMatrixChangedFlag.h"

/*double curT = 0.0;
double accum = 0.0;
size_t fps = 0;*/

void SGCore::TransformationsUpdater::fixedUpdate
(const Ref<Scene>& scene)
{
    auto transformsView = scene->getECSRegistry().view<Transform>();

    transformsView.each([&scene](const entt::entity& entity, Transform& transform) {
        entt::registry& registry = scene->getECSRegistry();

        EntityBaseInfo* entityBaseInfo = registry.try_get<EntityBaseInfo>(entity);
        Transform* parentTransform = nullptr;

        if(entityBaseInfo)
        {
            parentTransform = registry.try_get<Transform>(entityBaseInfo->m_parent);
        }

        //TransformationsUtils::updateTransform(transform, parentTransform);

        bool translationChanged = false;
        bool rotationChanged = false;
        bool scaleChanged = false;

        TransformBase& ownTransform = transform.m_ownTransform;
        TransformBase& finalTransform = transform.m_finalTransform;

        // translation ==============================================

        if(ownTransform.m_lastPosition != ownTransform.m_position)
        {
            ownTransform.m_translationMatrix = glm::translate(ownTransform.m_translationMatrix,
                                                              ownTransform.m_position -
                                                              ownTransform.m_lastPosition
            );
            ownTransform.m_lastPosition = ownTransform.m_position;

            translationChanged = true;
        }

        if(parentTransform && transform.m_followParentTRS.x)
        {
            finalTransform.m_translationMatrix = parentTransform->m_finalTransform.m_translationMatrix * ownTransform.m_translationMatrix;
        }
        else
        {
            finalTransform.m_translationMatrix = ownTransform.m_translationMatrix;
        }

        // rotation ================================================

        if(ownTransform.m_lastRotation != ownTransform.m_rotation)
        {
            const float rotDifX = ownTransform.m_rotation.x - ownTransform.m_lastRotation.x;
            const float rotDifY = ownTransform.m_rotation.y - ownTransform.m_lastRotation.y;
            const float rotDifZ = ownTransform.m_rotation.z - ownTransform.m_lastRotation.z;

            ownTransform.m_rotationMatrix = glm::rotate(ownTransform.m_rotationMatrix,
                                                        -glm::radians(rotDifX),
                                                        glm::vec3(1, 0, 0));

            ownTransform.m_rotationMatrix = glm::rotate(ownTransform.m_rotationMatrix,
                                                        -glm::radians(rotDifY),
                                                        glm::vec3(0, 1, 0));
            ownTransform.m_rotationMatrix = glm::rotate(ownTransform.m_rotationMatrix,
                                                        -glm::radians(rotDifZ),
                                                        glm::vec3(0, 0, 1));

            // rotating directions vectors
            ownTransform.m_left = glm::rotate(ownTransform.m_left,
                                              -glm::radians(rotDifY), glm::vec3(0, 1, 0));
            ownTransform.m_left = glm::rotate(ownTransform.m_left,
                                              -glm::radians(rotDifZ), glm::vec3(0, 0, 1));

            ownTransform.m_forward = glm::rotate(ownTransform.m_forward,
                                                 -glm::radians(rotDifX), glm::vec3(1, 0, 0));
            ownTransform.m_forward = glm::rotate(ownTransform.m_forward,
                                                 -glm::radians(rotDifY), glm::vec3(0, 1, 0));

            ownTransform.m_up = glm::rotate(ownTransform.m_up,
                                            -glm::radians(rotDifX), glm::vec3(1, 0, 0));
            ownTransform.m_up = glm::rotate(ownTransform.m_up,
                                            -glm::radians(rotDifZ), glm::vec3(0, 0, 1));

            //transformComponent->m_rotationMatrix = glm::toMat4(rotQuat);

            ownTransform.m_lastRotation = ownTransform.m_rotation;

            rotationChanged = true;
        }

        if(parentTransform && transform.m_followParentTRS.y)
        {
            finalTransform.m_rotationMatrix = parentTransform->m_finalTransform.m_rotationMatrix * ownTransform.m_rotationMatrix;
        }
        else
        {
            finalTransform.m_rotationMatrix = ownTransform.m_rotationMatrix;
        }

        // scale ========================================================

        if(ownTransform.m_lastScale != ownTransform.m_scale)
        {
            ownTransform.m_scaleMatrix = glm::scale(ownTransform.m_scaleMatrix,
                                                    ownTransform.m_scale -
                                                    ownTransform.m_lastScale
            );
            ownTransform.m_lastScale = ownTransform.m_scale;

            scaleChanged = true;
        }

        if(parentTransform && transform.m_followParentTRS.z)
        {
            finalTransform.m_scaleMatrix = parentTransform->m_finalTransform.m_scaleMatrix * ownTransform.m_scaleMatrix;
        }
        else
        {
            finalTransform.m_scaleMatrix = ownTransform.m_scaleMatrix;
        }

        // model matrix =================================================

        bool modelMatrixChanged = translationChanged || rotationChanged || scaleChanged;
        bool parentModelMatrixChanged = entityBaseInfo && registry.all_of<ModelMatrixChangedFlag>(entityBaseInfo->m_parent);
        bool finalModelMatrixChanged = parentModelMatrixChanged || modelMatrixChanged;

        if(modelMatrixChanged)
        {
            ownTransform.m_modelMatrix =
                    ownTransform.m_translationMatrix * ownTransform.m_rotationMatrix * ownTransform.m_scaleMatrix;
        }

        if(parentTransform)
        {
            if(finalModelMatrixChanged)
            {
                finalTransform.m_modelMatrix =
                        parentTransform->m_finalTransform.m_modelMatrix * ownTransform.m_modelMatrix;
            }
        }
        else
        {
            if(modelMatrixChanged) finalTransform.m_modelMatrix = ownTransform.m_modelMatrix;
        }

        if(finalModelMatrixChanged && !registry.all_of<ModelMatrixChangedFlag>(entity))
        {
            registry.emplace<ModelMatrixChangedFlag>(entity);
        }

        if(parentModelMatrixChanged)
        {
            registry.erase<ModelMatrixChangedFlag>(entityBaseInfo->m_parent);
        }

        if(finalModelMatrixChanged)
        {
            scene->invokeFlagObserverSystems<ModelMatrixChangedFlag>(entity);
        }
    });
}