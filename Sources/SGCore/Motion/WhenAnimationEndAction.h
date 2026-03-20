//
// Created by stuka on 29.05.2025.
//

#pragma once

#include <sgcore_export.h>

#include "SGCore/Actions/IAction.h"

namespace SGCore
{
    struct MotionPlannerNode;

    struct SGCORE_EXPORT WhenAnimationEndAction : IAction<bool()>
    {
        Ref<MotionPlannerNode> m_animationNode;

        bool execute() noexcept final;

        Ref<IAction> copy() noexcept final;
    };
}
