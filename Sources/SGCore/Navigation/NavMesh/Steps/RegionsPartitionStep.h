//
// Created by stuka on 08.12.2025.
//

#pragma once

#include "INavMeshBuildStep.h"

namespace SGCore::Navigation
{
    struct RegionsPartitionStep : INavMeshBuildStep
    {
        void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept override;
    };
}
