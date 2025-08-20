//
// Created by stuka on 21.08.2025.
//

#ifndef SUNGEARENGINE_IKROOTJOINT_H
#define SUNGEARENGINE_IKROOTJOINT_H

#include "IKJoint.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct IKRootJoint : ECS::Component<IKRootJoint, const IKRootJoint>
    {
    private:
        bool m_dummy = true;
    };
}

#endif // SUNGEARENGINE_IKROOTJOINT_H
