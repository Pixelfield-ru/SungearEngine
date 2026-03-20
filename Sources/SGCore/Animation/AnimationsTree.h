//
// Created by stuka on 21.03.2026.
//

#pragma once

#include <vector>

#include "SGCore/Main/CoreMain.h"

namespace SGCore
{
    struct IAnimationNode;

    struct AnimationsTree
    {
        std::vector<Ref<IAnimationNode>> m_rootNodes;
    };
}
