//
// Created by stuka on 02.05.2023.
//

#pragma once

#pragma once

#include "SGCore/Math/MathUtils.h"
#include "SGCore/Math/AABB.h"

namespace SGCore
{
    struct TransformBase
    {
        friend struct TransformationsUpdater;

        bool m_blockTranslation = false;
        bool m_blockRotation = false;
        bool m_blockScale = false;

        AABB<> m_aabb;

        glm::vec3 m_position { 0.0 };
        glm::vec3 m_scale { 1.0 };

        glm::vec3 m_right = MathUtils::right3;
        glm::vec3 m_forward = MathUtils::forward3;
        glm::vec3 m_up = MathUtils::up3;

        glm::mat4 m_translationMatrix = glm::mat4(1);
        glm::mat4 m_rotationMatrix = glm::mat4(1);
        glm::mat4 m_scaleMatrix = glm::mat4(1);

        bool m_positionChanged = false;
        bool m_rotationChanged = false;
        bool m_scaleChanged = false;

        glm::mat4 m_animatedModelMatrix = glm::mat4(1);
        glm::mat4 m_modelMatrix = glm::mat4(1);

        glm::mat4 m_boneAnimatedMatrix = glm::identity<glm::mat4>();

        glm::vec3 m_lastPosition { 0.0 };
        // from Controllables3DUpdater, where x - yaw, y - pitch, z - roll
        glm::vec3 m_yawPitchRoll { 0 };
        glm::vec3 m_lastScale = glm::vec3(0);

        glm::quat m_rotation = glm::identity<glm::quat>();
        glm::quat m_lastRotation = glm::identity<glm::quat>();
    };
}
