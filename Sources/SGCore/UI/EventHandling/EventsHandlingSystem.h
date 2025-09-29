//
// Created by stuka on 29.09.2025.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore::UI
{
    struct EventsHandlingSystem : ISystem
    {
        void update(const double& dt, const double& fixedDt) noexcept final;
    };
}
