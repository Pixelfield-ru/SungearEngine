//
// Created by stuka on 21.08.2025.
//

#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/detail/type_quat.hpp>

#include <optional>

#include "SGCore/ECS/Component.h"
#include "SGCore/Math/MathUtils.h"

namespace SGCore
{
    struct SGCORE_EXPORT IKJoint : ECS::Component<IKJoint, const IKJoint>
    {
        bool m_useRotationConstraints = false;
        glm::vec3 m_constraintMinRotation {};
        glm::vec3 m_constraintMaxRotation {};
        glm::vec3 m_rotationDirectionReference = MathUtils::up3;

        std::optional<glm::vec3> m_targetPosition { };
        bool m_isEndJoint = false;
    };
}
