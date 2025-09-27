//
// Created by stuka on 27.09.2025.
//

#pragma once

#include "SGCore/Scene/ISystem.h"

namespace SGCore::UI
{
    struct UIInputListener : ISystem
    {
        sg_implement_type_id(UIInputListener, 53)

        void update(const double& dt, const double& fixedDt) final;
    };
}
