//
// Created by stuka on 21.08.2025.
//

#pragma once

#include "IKJoint.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SGCORE_EXPORT IKRootJoint : ECS::Component<IKRootJoint, const IKRootJoint>
    {
    private:
        bool m_dummy = true;
    };
}
