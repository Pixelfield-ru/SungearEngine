//
// Created by stuka on 02.02.2024.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct SGCORE_EXPORT SphereGizmosUpdater : public ISystem
    {
        sg_implement_type_id(SGCore::SphereGizmosUpdater)

        void fixedUpdate(double dt, double fixedDt) final;
    };
}
