//
// Created by stuka on 06.02.2025.
//

#include "TransformUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <glm/gtx/quaternion.hpp>

#include "Transform.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Physics/Rigidbody3D.h"

bool SGCore::TransformUtils::calculateTransform(Transform& childTransform,
                                                const Transform* parentTransform) noexcept
{
    auto& childLocalTransform = childTransform.m_localTransform;
    auto& childWorldTransform = childTransform.m_worldTransform;

    bool translationChanged = false;
    bool rotationChanged = false;
    bool scaleChanged = false;

    // ============================================== translation calc

    if(childLocalTransform.m_position != childLocalTransform.m_lastPosition)
    {
        childLocalTransform.m_translationMatrix = glm::translate(glm::mat4(1.0), childLocalTransform.m_position);

        // std::cout << "pos : " << localTransform.m_position.x << ", " << localTransform.m_position.y << ", " << localTransform.m_position.z << std::endl;

        childLocalTransform.m_lastPosition = childLocalTransform.m_position;

        translationChanged = true;
    }

    if(parentTransform && childTransform.m_followParentTRS.x)
    {
        // std::cout << "dfdff" << std::endl;
        childWorldTransform.m_translationMatrix =
                parentTransform->m_worldTransform.m_translationMatrix * childLocalTransform.m_translationMatrix;
    }
    else
    {
        childWorldTransform.m_translationMatrix = childLocalTransform.m_translationMatrix;
    }

    // ============================================== rotation calc

    if(childLocalTransform.m_lastRotation != childLocalTransform.m_rotation)
    {
        childLocalTransform.m_rotationMatrix = glm::toMat4(childLocalTransform.m_rotation);

        const glm::vec3& column1 = childLocalTransform.m_rotationMatrix[0];
        const glm::vec3& column2 = childLocalTransform.m_rotationMatrix[1];
        const glm::vec3& column3 = childLocalTransform.m_rotationMatrix[2];

        childLocalTransform.m_right = column1;             // X ось
        childLocalTransform.m_up = column2;             // Y ось
        childLocalTransform.m_forward = -column3;

        /*childLocalTransform.m_up = glm::vec3(column1.y, column2.y, column3.y);
        childLocalTransform.m_forward = -glm::vec3(column1.z, column2.z, column3.z);
        childLocalTransform.m_right = glm::vec3(column1.x, column2.x, column3.x);*/

        childLocalTransform.m_lastRotation = childLocalTransform.m_rotation;

        rotationChanged = true;
    }

    if(parentTransform && childTransform.m_followParentTRS.y)
    {
        childWorldTransform.m_rotationMatrix =
                parentTransform->m_worldTransform.m_rotationMatrix * childLocalTransform.m_rotationMatrix;
    }
    else
    {
        childWorldTransform.m_rotationMatrix = childLocalTransform.m_rotationMatrix;
    }

    // ============================================== scale calc

    if(childLocalTransform.m_lastScale != childLocalTransform.m_scale)
    {
        childLocalTransform.m_scaleMatrix = glm::scale(glm::mat4(1.0),
                                                childLocalTransform.m_scale
        );
        childLocalTransform.m_lastScale = childLocalTransform.m_scale;

        scaleChanged = true;
    }

    if(parentTransform && childTransform.m_followParentTRS.z)
    {
        childWorldTransform.m_scaleMatrix =
                parentTransform->m_worldTransform.m_scaleMatrix * childLocalTransform.m_scaleMatrix;
    }
    else
    {
        childWorldTransform.m_scaleMatrix = childLocalTransform.m_scaleMatrix;
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
        childLocalTransform.m_animatedModelMatrix =
            childLocalTransform.m_translationMatrix * childLocalTransform.m_rotationMatrix * childLocalTransform.m_scaleMatrix;
        childLocalTransform.m_modelMatrix = childLocalTransform.m_animatedModelMatrix;

        if(parentTransform)
        {
            childWorldTransform.m_animatedModelMatrix =
                parentTransform->m_worldTransform.m_animatedModelMatrix * childTransform.m_boneMatrix * childLocalTransform.m_animatedModelMatrix;

            childWorldTransform.m_modelMatrix = parentTransform->m_worldTransform.m_modelMatrix * childLocalTransform.m_modelMatrix;
        }
        else
        {
            childWorldTransform.m_animatedModelMatrix = childTransform.m_boneMatrix * childLocalTransform.m_animatedModelMatrix;
            childWorldTransform.m_modelMatrix = childLocalTransform.m_modelMatrix;
        }

        // ================================================= getting final TRS

        /*glm::vec3 finalScale;
        glm::quat finalRotation;
        glm::vec3 finalTranslation;
        glm::vec3 finalSkew;
        glm::vec4 finalPerspective;

        // DECOMPOSE IS VERY HEAVY
        glm::decompose(childWorldTransform.m_animatedModelMatrix, finalScale, finalRotation, finalTranslation, finalSkew,
                       finalPerspective);*/

        childWorldTransform.m_lastPosition = childWorldTransform.m_position;
        childWorldTransform.m_lastRotation = childWorldTransform.m_rotation;
        childWorldTransform.m_lastScale = childWorldTransform.m_scale;

        auto finalTranslation = childLocalTransform.m_position;
        auto finalRotation = childLocalTransform.m_rotation;
        auto finalScale = childLocalTransform.m_scale;

        if(parentTransform)
        {
            const auto childAnimatedMatrix = parentTransform->m_worldTransform.m_animatedModelMatrix * childTransform.m_boneMatrix;

            finalTranslation = childAnimatedMatrix * glm::vec4(finalTranslation, 1.0f);
            finalRotation = parentTransform->m_worldTransform.m_rotation * finalRotation;
            finalScale = parentTransform->m_worldTransform.m_scale * finalScale;
        }

        childWorldTransform.m_position = finalTranslation;
        childWorldTransform.m_rotation = finalRotation;
        childWorldTransform.m_scale = finalScale;

        childWorldTransform.m_forward = finalRotation * MathUtils::forward3;
        childWorldTransform.m_right = finalRotation * MathUtils::right3;
        childWorldTransform.m_up = finalRotation * MathUtils::up3;
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

glm::vec3 SGCore::TransformUtils::calculateLocalPosition(const Transform& parentTransform,
                                                         const glm::vec3& childWorldPosition) noexcept
{
    return glm::inverse(parentTransform.m_worldTransform.m_rotation) * ((childWorldPosition - parentTransform.m_worldTransform.m_position) / parentTransform.m_worldTransform.m_scale);
    // return (glm::inverse(parentTransform.m_worldTransform.m_rotation) * (childWorldPosition - parentTransform.m_worldTransform.m_position)) / parentTransform.m_worldTransform.m_scale;
}

glm::quat SGCore::TransformUtils::calculateLocalRotation(const Transform& parentTransform,
                                                         const glm::quat& childWorldRotation) noexcept
{
    return glm::inverse(parentTransform.m_worldTransform.m_rotation) * childWorldRotation;
}

glm::vec3 SGCore::TransformUtils::calculateWorldPosition(const Transform& parentTransform,
                                                         const glm::vec3& childLocalPosition) noexcept
{
    return parentTransform.m_worldTransform.m_position + parentTransform.m_worldTransform.m_rotation * (childLocalPosition * parentTransform.m_worldTransform.m_scale);
    // return parentTransform.m_worldTransform.m_position + (parentTransform.m_worldTransform.m_rotation * childLocalPosition) * parentTransform.m_worldTransform.m_scale;
}

glm::quat SGCore::TransformUtils::calculateWorldRotation(const Transform& parentTransform,
                                                         const glm::quat& childLocalRotation) noexcept
{
    return parentTransform.m_worldTransform.m_rotation * childLocalRotation;
}

glm::vec3 SGCore::TransformUtils::screenToWorld(const glm::vec2& screenPosition,
                                                const glm::vec2& screenSize,
                                                const glm::mat4& cameraProjection,
                                                const glm::mat4& cameraView,
                                                float defaultZ) noexcept
{
    // normalize to [-1, 1]
    const float x = (2.0f * screenPosition.x) / screenSize.x - 1.0f;
    const float y = 1.0f - (2.0f * screenPosition.y) / screenSize.y;
    const float z = defaultZ;

    const glm::vec4 rayNDC { x, y, z, 1.0f };

    const glm::vec4 rayClip = glm::inverse(cameraProjection) * rayNDC;
    const auto rayEye = glm::vec4(rayClip.x, rayClip.y, -1.0f, 0.0f);
    const glm::vec4 rayWorld = glm::inverse(cameraView) * rayEye;

    const auto rayDir = glm::normalize(glm::vec3(rayWorld));

    // camera position
    const glm::vec3 cameraPos = glm::inverse(cameraView)[3];

    // find intersection with plane
    const float t = -cameraPos.y / rayDir.y;

    return cameraPos + rayDir * t;
}
