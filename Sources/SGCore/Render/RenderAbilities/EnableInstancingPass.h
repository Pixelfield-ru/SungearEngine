//
// Created by stuka on 26.01.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SGCORE_EXPORT EnableInstancingPass : ECS::Component<EnableInstancingPass, const EnableInstancingPass>
    {
    private:
        bool m_dummy = false;
    };
}