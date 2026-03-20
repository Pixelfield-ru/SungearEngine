//
// Created by stuka on 21.07.2025.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct SGCORE_EXPORT InstancingUpdater : ISystem
    {
        sg_implement_type_id(InstancingUpdater, 36)

        void update(const double& dt, const double& fixedDt) noexcept final;
    };
}
