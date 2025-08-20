//
// Created by stuka on 29.05.2025.
//

#ifndef SUNGEARENGINE_WHENANIMATIONENDACTION_H
#define SUNGEARENGINE_WHENANIMATIONENDACTION_H

#include "SGCore/Actions/IAction.h"

namespace SGCore
{
    struct MotionPlannerNode;

    struct WhenAnimationEndAction : IAction<bool()>
    {
        Ref<MotionPlannerNode> m_animationNode;

        bool execute() noexcept final;

        Ref<IAction> copy() noexcept final;
    };
}

#endif // SUNGEARENGINE_WHENANIMATIONENDACTION_H
