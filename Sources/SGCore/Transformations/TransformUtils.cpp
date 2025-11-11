//
// Created by stuka on 06.02.2025.
//

#include "TransformUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Transform.h"
#include "SGCore/Logger/Logger.h"

bool SGCore::TransformUtils::calculateTransform(Transform& childTransform,
                                                const Transform* parentTransform) noexcept
{
    auto& childOwnTransform = childTransform.m_ownTransform;
    auto& childFinalTransform = childTransform.m_finalTransform;

    bool translationChanged = false;
    bool rotationChanged = false;
    bool scaleChanged = false;

    // ============================================== translation calc

    if(childOwnTransform.m_position != childOwnTransform.m_lastPosition)
    {
        LOG_I(SGCORE_TAG, "Updating position")

        childOwnTransform.m_translationMatrix = glm::translate(glm::mat4(1.0), childOwnTransform.m_position);

        // std::cout << "pos : " << ownTransform.m_position.x << ", " << ownTransform.m_position.y << ", " << ownTransform.m_position.z << std::endl;

        childOwnTransform.m_lastPosition = childOwnTransform.m_position;

        translationChanged = true;
    }

    if(parentTransform && childTransform.m_followParentTRS.x)
    {
        // std::cout << "dfdff" << std::endl;
        childFinalTransform.m_translationMatrix =
                parentTransform->m_finalTransform.m_translationMatrix * childOwnTransform.m_translationMatrix;
    }
    else
    {
        childFinalTransform.m_translationMatrix = childOwnTransform.m_translationMatrix;
    }

    // ============================================== rotation calc

    if(childOwnTransform.m_lastRotation != childOwnTransform.m_rotation)
    {
        LOG_I(SGCORE_TAG, "Updating rotation")

        childOwnTransform.m_rotationMatrix = glm::toMat4(childOwnTransform.m_rotation);

        const glm::vec3& column1 = childOwnTransform.m_rotationMatrix[0];
        const glm::vec3& column2 = childOwnTransform.m_rotationMatrix[1];
        const glm::vec3& column3 = childOwnTransform.m_rotationMatrix[2];

        childOwnTransform.m_right = column1;             // X ось
        childOwnTransform.m_up = column2;             // Y ось
        childOwnTransform.m_forward = -column3;

        /*childOwnTransform.m_up = glm::vec3(column1.y, column2.y, column3.y);
        childOwnTransform.m_forward = -glm::vec3(column1.z, column2.z, column3.z);
        childOwnTransform.m_right = glm::vec3(column1.x, column2.x, column3.x);*/

        childOwnTransform.m_lastRotation = childOwnTransform.m_rotation;

        rotationChanged = true;
    }

    if(parentTransform && childTransform.m_followParentTRS.y)
    {
        childFinalTransform.m_rotationMatrix =
                parentTransform->m_finalTransform.m_rotationMatrix * childOwnTransform.m_rotationMatrix;
    }
    else
    {
        childFinalTransform.m_rotationMatrix = childOwnTransform.m_rotationMatrix;
    }

    // ============================================== scale calc

    if(childOwnTransform.m_lastScale != childOwnTransform.m_scale)
    {
        LOG_I(SGCORE_TAG, "Updating scale")

        childOwnTransform.m_scaleMatrix = glm::scale(glm::mat4(1.0),
                                                childOwnTransform.m_scale
        );
        childOwnTransform.m_lastScale = childOwnTransform.m_scale;

        scaleChanged = true;
    }

    if(parentTransform && childTransform.m_followParentTRS.z)
    {
        childFinalTransform.m_scaleMatrix =
                parentTransform->m_finalTransform.m_scaleMatrix * childOwnTransform.m_scaleMatrix;
    }
    else
    {
        childFinalTransform.m_scaleMatrix = childOwnTransform.m_scaleMatrix;
    }

    // ================================================= model matrix calc

    bool modelMatrixChanged = translationChanged || rotationChanged || scaleChanged;

    // О ТАК ВЕРНО
    childTransform.m_transformChanged =
            modelMatrixChanged || (parentTransform && parentTransform->m_transformChanged) || childTransform.m_isAnimated;

    // todo: FIX! MAY PRODUCE INCORRECT RESULTS WHEN TransformationsUpdater WORKS IN PARALLEL.
    // I THINK THAT PARENT UPDATES FIRST AND THEN WHEN CHILD WAS ADD AND IT HAS POSITION = BASIC POSITION (0.0), ROTATION = BASIC ROTATION (0.0) AND
    // SCALE = BASIC SCALE (1.0) CHILD IS NOT UPDATED BECAUSE PARENT WAS UPDATED EARLIER AND CHILD HAS UNCHANGED TRANSFORMATIONS
    // if(childTransform.m_transformChanged)
    {
        childOwnTransform.m_animatedModelMatrix =
            childOwnTransform.m_translationMatrix * childOwnTransform.m_rotationMatrix * childOwnTransform.m_scaleMatrix;
        childOwnTransform.m_modelMatrix = childOwnTransform.m_animatedModelMatrix;

        if(parentTransform)
        {
            childFinalTransform.m_animatedModelMatrix =
                parentTransform->m_finalTransform.m_animatedModelMatrix * childTransform.m_boneMatrix * childOwnTransform.m_animatedModelMatrix;

            childFinalTransform.m_modelMatrix = parentTransform->m_finalTransform.m_modelMatrix * childOwnTransform.m_modelMatrix;
        }
        else
        {
            childFinalTransform.m_animatedModelMatrix = childTransform.m_boneMatrix * childOwnTransform.m_animatedModelMatrix;
            childFinalTransform.m_modelMatrix = childOwnTransform.m_modelMatrix;
        }

        // ================================================= getting final TRS

        glm::vec3 finalScale;
        glm::quat finalRotation;
        glm::vec3 finalTranslation;
        glm::vec3 finalSkew;
        glm::vec4 finalPerspective;

        glm::decompose(childFinalTransform.m_animatedModelMatrix, finalScale, finalRotation, finalTranslation, finalSkew,
                       finalPerspective);

        childFinalTransform.m_lastPosition = childFinalTransform.m_position;
        childFinalTransform.m_lastRotation = childFinalTransform.m_rotation;
        childFinalTransform.m_lastScale = childFinalTransform.m_scale;

        childFinalTransform.m_position = finalTranslation;
        childFinalTransform.m_rotation = finalRotation;
        childFinalTransform.m_scale = finalScale;

        childFinalTransform.m_forward = finalRotation * MathUtils::forward3;
        childFinalTransform.m_right = finalRotation * MathUtils::right3;
        childFinalTransform.m_up = finalRotation * MathUtils::up3;
    }

    return childTransform.m_transformChanged;
}

glm::mat4 SGCore::TransformUtils::calculateModelMatrix(const glm::vec3& position,
                                                       const glm::quat& rotation,
                                                       const glm::vec3& scale) noexcept
{
    auto modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(glm::mat4(1.0), position);
    modelMatrix *= glm::toMat4(rotation);
    modelMatrix = glm::scale(modelMatrix, scale);

    return modelMatrix;
}
