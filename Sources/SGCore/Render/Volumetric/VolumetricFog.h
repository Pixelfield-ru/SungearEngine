//
// Created by stuka on 26.01.2026.
//

#pragma once

#include "SGCore/Render/MeshBase.h"

namespace SGCore
{
    struct VolumetricFog : ECS::Component<VolumetricFog, const VolumetricFog>
    {
        VolumetricFog();

        float m_density = 1.0f;
        float m_coverage = 1.0f;

        MeshBase m_meshBase;
    };
}
