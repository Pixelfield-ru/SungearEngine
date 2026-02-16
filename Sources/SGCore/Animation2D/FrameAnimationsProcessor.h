//
// Created by stuka on 16.02.2026.
//

#pragma once

#include "SGCore/Scene/ISystem.h"
#include "SGCore/Coro/Task.h"

namespace SGCore
{
    struct FrameAnimationsProcessor : ISystem
    {
        void update(const double& dt, const double& fixedDt) noexcept override;
    };
}
