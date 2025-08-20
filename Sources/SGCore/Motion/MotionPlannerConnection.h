//
// Created by stuka on 07.01.2025.
//

#ifndef SUNGEARENGINE_MOTIONPLANNERCONNECTION_H
#define SUNGEARENGINE_MOTIONPLANNERCONNECTION_H

#include "SGCore/Actions/AlwaysTrueAction.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct MotionPlannerNode;

    struct MotionPlannerConnection
    {
        float m_currentBlendTime = 0.0f;
        float m_blendTime = 1.0f;

        float m_blendSpeed = 1.0f;

        // calls if:
        // 1) if connected to other node as child, then calls if
        // parent node is playing (m_isPlaying == true) and active (m_isActive == true)
        // 2) if not connected to other node as child (it means that this node is root),
        // then calls if this node is playing (m_isPlaying == true) and active (m_isActive == true)
        Ref<IAction<bool()>> m_activationAction = MakeRef<AlwaysTrueAction>();

        bool m_doNotInterruptWhenInactive = true;

        // bool m_stopPreviousNodeIfThisStarted = true;

        // IGNORED IN ANY STATE.
        Weak<MotionPlannerNode> m_previousNode;
        Ref<MotionPlannerNode> m_nextNode;

        [[nodiscard]] MotionPlannerConnection copyStructure(const Ref<MotionPlannerNode>& baseNode) const noexcept;
    };
}

#endif //SUNGEARENGINE_MOTIONPLANNERCONNECTION_H
