//
// Created by stuka on 26.01.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct EnableMeshPass : ECS::Component<EnableMeshPass, const EnableMeshPass>
    {
    private:
        bool m_dummy = false;
    };
}
