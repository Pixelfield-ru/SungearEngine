//
// Created by stuka on 21.03.2026.
//

#pragma once

#include "SGCore/Actions/AlwaysTrueAction.h"
#include "SGCore/Actions/IAction.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct IAnimationNode;

    struct SGCORE_EXPORT AnimationNodeConnection
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

        Ref<IAnimationNode> m_nextNode;
        Weak<IAnimationNode> m_previousNode;
    };
}
