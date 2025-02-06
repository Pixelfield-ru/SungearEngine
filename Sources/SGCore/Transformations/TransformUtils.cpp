//
// Created by stuka on 06.02.2025.
//

#include "TransformUtils.h"

#include "Transform.h"

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
        childOwnTransform.m_rotationMatrix = glm::toMat4(childOwnTransform.m_rotation);

        const glm::vec3& column1 = childOwnTransform.m_rotationMatrix[0];
        const glm::vec3& column2 = childOwnTransform.m_rotationMatrix[1];
        const glm::vec3& column3 = childOwnTransform.m_rotationMatrix[2];

        childOwnTransform.m_up = glm::vec3(column1.y, column2.y, column3.y);
        childOwnTransform.m_forward = -glm::vec3(column1.z, column2.z, column3.z);
        childOwnTransform.m_right = glm::vec3(column1.x, column2.x, column3.x);

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
            modelMatrixChanged || (parentTransform && parentTransform->m_transformChanged);

    if(childTransform.m_transformChanged)
    {
        childOwnTransform.m_modelMatrix =
                childOwnTransform.m_translationMatrix * childOwnTransform.m_rotationMatrix * childOwnTransform.m_scaleMatrix;

        if(parentTransform)
        {
            childFinalTransform.m_modelMatrix =
                    parentTransform->m_finalTransform.m_modelMatrix * childOwnTransform.m_modelMatrix;
        }
        else
        {
            childFinalTransform.m_modelMatrix = childOwnTransform.m_modelMatrix;
        }

        // ================================================= getting final TRS

        glm::vec3 finalScale;
        glm::quat finalRotation;
        glm::vec3 finalTranslation;
        glm::vec3 finalSkew;
        glm::vec4 finalPerspective;

        glm::decompose(childFinalTransform.m_modelMatrix, finalScale, finalRotation, finalTranslation, finalSkew,
                       finalPerspective);

        childFinalTransform.m_position = finalTranslation;
        childFinalTransform.m_rotation = finalRotation;
        childFinalTransform.m_scale = finalScale;
    }

    return childTransform.m_transformChanged;
}
