//
// Created by stuka on 19.04.2025.
//

#ifndef SUNGEARENGINE_TERRAIN_H
#define SUNGEARENGINE_TERRAIN_H

#include "TesselationParams.h"

namespace SGCore
{
    struct Terrain : ECS::Component<Terrain, const Terrain>
    {
        TesselationParams m_tesselationParams;
    };
}

#endif // SUNGEARENGINE_TERRAIN_H
