//
// Created by stuka on 02.02.2024.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct SGCORE_EXPORT LineGizmosRenderer : public ISystem
    {
        sg_implement_type_id(SGCore::LineGizmosRenderer)

        void fixedUpdate(double dt, double fixedDt) final;
    };
}
