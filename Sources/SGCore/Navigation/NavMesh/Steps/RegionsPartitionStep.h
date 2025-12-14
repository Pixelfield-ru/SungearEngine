//
// Created by stuka on 08.12.2025.
//

#pragma once

#include <vector>
#include <unordered_set>

#include "INavMeshBuildStep.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::Navigation
{
    struct NavVoxel;
    struct VoxelizationStep;

    struct Region
    {
        std::vector<size_t> m_voxelsIndices;
        std::vector<size_t> m_contourVoxelsIndices;
    };

    struct RegionsPartitionStep : INavMeshBuildStep
    {
        void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept override;

    private:
        void floodFill(const Ref<VoxelizationStep>& voxelizationStep,
                       const NavVoxel& voxel,
                       size_t voxelIndex,
                       std::unordered_set<size_t>& visitedVoxels) noexcept;
    };
}
