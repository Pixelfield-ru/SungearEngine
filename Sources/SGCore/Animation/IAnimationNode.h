//
// Created by stuka on 21.03.2026.
//

#pragma once

#include <vector>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct AnimationNodeConnection;

    struct IAnimationNode
    {
        std::vector<Ref<AnimationNodeConnection>> m_connections;
    };
}
