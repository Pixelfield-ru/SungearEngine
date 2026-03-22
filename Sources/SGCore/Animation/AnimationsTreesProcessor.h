//
// Created by stuka on 21.03.2026.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct IAnimationNode;

    struct SGCORE_EXPORT AnimationsTreesProcessor : ISystem
    {
        void update(double dt, double fixedDt) noexcept override;

    private:
        void interpolateAndCollectNodes(double dt, const Ref<IAnimationNode>& currentNode, std::vector<Ref<IAnimationNode>>& nodesToActivate) noexcept;
    };
}
