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
        std::vector<Region> m_regions;

        void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept override;

    private:
        void floodFill(Region& region,
                       const Ref<VoxelizationStep>& voxelizationStep,
                       const NavVoxel& voxel,
                       size_t voxelIndex,
                       std::unordered_set<size_t>& visitedVoxels,
                       const NavMeshConfig& config) noexcept;
    };
}
