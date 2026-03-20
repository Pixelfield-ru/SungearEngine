//
// Created by stuka on 02.02.2024.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct SGCORE_EXPORT SphereGizmosUpdater : public ISystem
    {
        sg_implement_type_id(SphereGizmosUpdater, 24)

        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}
