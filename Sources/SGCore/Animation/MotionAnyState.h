//
// Created by stuka on 03.04.2025.
//

#ifndef MOTIONANYSTATE_H
#define MOTIONANYSTATE_H

#include "MotionPlannerConnection.h"

namespace SGCore
{
    struct MotionAnyState
    {
        Ref<MotionPlannerConnection> m_toRootConnection;
        std::vector<Weak<MotionPlannerConnection>> m_lastActivatedConnections;
        bool m_isSomeNodeInTreeExceptRootActive = false;
    };
}

#endif //MOTIONANYSTATE_H
