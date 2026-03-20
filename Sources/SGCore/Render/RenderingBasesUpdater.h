//
// Created by stuka on 03.02.2024.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct SGCORE_EXPORT RenderingBasesUpdater : public ISystem
    {
        sg_implement_type_id(RenderingBasesUpdater, 28)

        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}
