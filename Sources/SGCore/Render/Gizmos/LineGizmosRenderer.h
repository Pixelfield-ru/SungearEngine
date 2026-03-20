//
// Created by stuka on 02.02.2024.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct SGCORE_EXPORT LineGizmosRenderer : public ISystem
    {
        sg_implement_type_id(LineGizmosRenderer, 22)

        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}
