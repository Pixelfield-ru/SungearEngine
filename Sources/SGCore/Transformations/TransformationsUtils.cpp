//
// Created by stuka on 02.02.2024.
//

#include "TransformationsUtils.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

void SGCore::TransformationsUtils::updateTransform(SGCore::Transform& transform, Transform* parent) noexcept
{
    updateTranslationMatrix(transform, parent);
    updateRotationMatrix(transform, parent);
    updateScaleMatrix(transform, parent);
    updateModelMatrix(transform, parent);
}

void SGCore::TransformationsUtils::updateTranslationMatrix(SGCore::Transform& transform, Transform* parent) noexcept
{
    TransformBase& ownTransform = transform.m_ownTransform;
    TransformBase& finalTransform = transform.m_finalTransform;

    ownTransform.m_translationMatrix = glm::translate(ownTransform.m_translationMatrix,
                                                      ownTransform.m_position -
                                                      ownTransform.m_lastPosition
    );
    ownTransform.m_lastPosition = ownTransform.m_position;

    if(parent && transform.m_followParentTRS.x)
    {
        finalTransform.m_translationMatrix = parent->m_finalTransform.m_translationMatrix * ownTransform.m_translationMatrix;
    }
    else
    {
        finalTransform.m_translationMatrix = ownTransform.m_translationMatrix;
    }
}

void SGCore::TransformationsUtils::updateRotationMatrix(SGCore::Transform& transform, Transform* parent) noexcept
{
    TransformBase& ownTransform = transform.m_ownTransform;
    TransformBase& finalTransform = transform.m_finalTransform;

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

    if(parent && transform.m_followParentTRS.y)
    {
        finalTransform.m_rotationMatrix = parent->m_finalTransform.m_rotationMatrix * ownTransform.m_rotationMatrix;
    }
    else
    {
        finalTransform.m_rotationMatrix = ownTransform.m_rotationMatrix;
    }
}

void SGCore::TransformationsUtils::updateScaleMatrix(SGCore::Transform& transform, Transform* parent) noexcept
{
    TransformBase& ownTransform = transform.m_ownTransform;
    TransformBase& finalTransform = transform.m_finalTransform;

    ownTransform.m_scaleMatrix = glm::scale(ownTransform.m_scaleMatrix,
                                            ownTransform.m_scale -
                                            ownTransform.m_lastScale
    );
    ownTransform.m_lastScale = ownTransform.m_scale;

    if(parent && transform.m_followParentTRS.z)
    {
        finalTransform.m_scaleMatrix = parent->m_finalTransform.m_scaleMatrix * ownTransform.m_scaleMatrix;
    }
    else
    {
        finalTransform.m_scaleMatrix = ownTransform.m_scaleMatrix;
    }
}

void SGCore::TransformationsUtils::updateModelMatrix(SGCore::Transform& transform, Transform* parent) noexcept
{
    TransformBase& ownTransform = transform.m_ownTransform;
    TransformBase& finalTransform = transform.m_finalTransform;

    ownTransform.m_modelMatrix =
            ownTransform.m_translationMatrix * ownTransform.m_rotationMatrix * ownTransform.m_scaleMatrix;

    if(parent)
    {
        finalTransform.m_modelMatrix = parent->m_finalTransform.m_modelMatrix * ownTransform.m_modelMatrix;
    }
    else
    {
        finalTransform.m_modelMatrix = ownTransform.m_modelMatrix;
    }
}
