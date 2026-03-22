//
// Created by stuka on 29.09.2025.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore::UI
{
    struct EventsHandlingSystem : ISystem
    {
        void update(double dt, double fixedDt) noexcept final;
    };
}
