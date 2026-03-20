//
// Created by stuka on 21.08.2025.
//

#pragma once

#include <glm/vec3.hpp>
#include <optional>

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SGCORE_EXPORT IKJoint : ECS::Component<IKJoint, const IKJoint>
    {
        std::optional<glm::vec3> m_targetPosition { };
        bool m_isEndJoint = false;
    };
}
