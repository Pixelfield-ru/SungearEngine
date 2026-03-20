//
// Created by stuka on 03.04.2025.
//

#pragma once

#include <vector>
#include <sgcore_export.h>

#include "MotionPlannerConnection.h"

namespace SGCore
{
    struct SGCORE_EXPORT MotionAnyState
    {
        Ref<MotionPlannerConnection> m_toRootConnection;
        std::vector<Weak<MotionPlannerNode>> m_lastActiveNodes;
        // std::vector<Weak<MotionPlannerConnection>> m_lastActivatedConnections;
        bool m_isSomeNodeInTreeExceptRootActive = false;
    };
}
