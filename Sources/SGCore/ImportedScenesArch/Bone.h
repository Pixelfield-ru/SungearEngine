//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_BONE_H
#define SUNGEARENGINE_BONE_H

#include "SGCore/Main/CoreGlobals.h"

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <string>
#include <vector>

namespace SGCore
{
    struct KeyPosition
    {
        glm::vec3 m_position { };
        float m_timeStamp = 0.0f;
    };

    struct KeyRotation
    {
        glm::quat m_rotation = glm::identity<glm::quat>();
        float m_timeStamp = 0.0f;
    };

    struct KeyScale
    {
        glm::vec3 m_scale { };
        float m_timeStamp = 0.0f;
    };

    struct Bone
    {
        std::int32_t m_id = -1;
        std::string m_name;

        std::vector<KeyPosition> m_positionKeys;
        std::vector<KeyRotation> m_rotationKeys;
        std::vector<KeyScale> m_scaleKeys;

        glm::mat3 m_localMatrix = glm::identity<glm::mat4>();

        std::vector<Ref<Bone>> m_children;
    };
}

#endif //SUNGEARENGINE_BONE_H
