//
// Created by stuka on 24.07.2026.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct WheeledVehiclesUpdater final : ISystem
    {
        void update(double dt, double fixedDt) noexcept override;
    };
}
