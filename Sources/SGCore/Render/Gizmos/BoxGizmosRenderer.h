//
// Created by stuka on 02.02.2024.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class SGCORE_EXPORT BoxGizmosRenderer : public ISystem
    {
        sg_implement_type_id(BoxGizmosRenderer, 21)

        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}
