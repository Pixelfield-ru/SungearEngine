//
// Created by stuka on 08.12.2025.
//

#pragma once

#include <sgcore_export.h>

#include "INavMeshBuildStep.h"

namespace SGCore::Navigation
{
    struct SGCORE_EXPORT FilterErosionStep : INavMeshBuildStep
    {
        void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept override;
    };
}
