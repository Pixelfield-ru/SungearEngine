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
    struct FrameAnimationsProcessor : ISystem
    {
        void update(const double& dt, const double& fixedDt) noexcept override;
    };
}
