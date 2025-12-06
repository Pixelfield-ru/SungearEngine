//
// Created by stuka on 07.12.2025.
//

#pragma once

#include <limits>

#include "INavMeshBuildStep.h"

namespace SGCore::Navigation
{
    struct NavVoxel
    {
        bool m_isSolid {};
        bool m_isWalkable {};
        float m_distanceToEdge = std::numeric_limits<float>::max();
        std::int32_t m_regionID {};
    };

    struct VoxelizationStep : INavMeshBuildStep
    {
        void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept override;
    };
}
