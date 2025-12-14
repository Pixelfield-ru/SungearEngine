//
// Created by stuka on 08.12.2025.
//

#include "RegionsPartitionStep.h"

#include "VoxelizationStep.h"
#include "SGCore/Navigation/NavMesh/NavMesh.h"

void SGCore::Navigation::RegionsPartitionStep::process(NavMesh& navMesh, const NavMeshConfig& config) noexcept
{
    const auto voxelizationStep = navMesh.getStep<VoxelizationStep>();

    // unordered set that contains indices of visited voxels
    std::unordered_set<size_t> visitedVoxels;

    // flood fill
    for(size_t i = 0; i < voxelizationStep->m_voxels.size(); ++i)
    {
        const auto& currentVoxel = voxelizationStep->m_voxels[i];

        if(visitedVoxels.contains(i)) continue;

        floodFill(voxelizationStep, currentVoxel, i, visitedVoxels);
    }
}

void SGCore::Navigation::RegionsPartitionStep::floodFill(const Ref<VoxelizationStep>& voxelizationStep,
                                                         const NavVoxel& voxel,
                                                         size_t voxelIndex,
                                                         std::unordered_set<size_t>& visitedVoxels) noexcept
{

}
