//
// Created by stuka on 26.01.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct EnableDecalPass : ECS::Component<EnableDecalPass, const EnableDecalPass>
    {
    private:
        bool m_dummy = false;
    };
}