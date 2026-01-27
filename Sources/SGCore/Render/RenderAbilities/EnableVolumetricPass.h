//
// Created by stuka on 26.01.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct EnableVolumetricPass : ECS::Component<EnableVolumetricPass, const EnableVolumetricPass>
    {
    private:
        bool m_dummy = false;
    };
}