//
// Created by stuka on 21.08.2025.
//

#ifndef SUNGEARENGINE_IKJOINT_H
#define SUNGEARENGINE_IKJOINT_H

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct IKJoint : ECS::Component<IKJoint, const IKJoint>
    {
        std::optional<glm::vec3> m_targetPosition { };
        bool m_isEndJoint = false;
    };
}

#endif // SUNGEARENGINE_IKJOINT_H
