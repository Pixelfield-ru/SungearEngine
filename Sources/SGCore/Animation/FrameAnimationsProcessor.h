//
// Created by stuka on 16.02.2026.
//

#pragma once

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Coro/Task.h"

namespace SGCore
{
    /**
     * Updates all \link FrameAnimation components of all entities.
     */
    struct SGCORE_EXPORT FrameAnimationsProcessor : ISystem
    {
        void update(double dt, double fixedDt) noexcept override;
    };
}
