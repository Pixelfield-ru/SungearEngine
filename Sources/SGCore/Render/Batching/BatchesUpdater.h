//
// Created by stuka on 18.07.2025.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct BatchesUpdater : ISystem
    {
        sg_implement_type_id(BatchesUpdater, 35)

        void update(const double& dt, const double& fixedDt) noexcept final;
    };
}
