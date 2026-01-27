//
// Created by stuka on 26.01.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct EnableTerrainPass : ECS::Component<EnableTerrainPass, const EnableTerrainPass>
    {
    private:
        bool m_dummy = false;
    };
}