//
// Created by stuka on 08.12.2025.
//

#include "FilterErosionStep.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/gtx/norm.hpp>

#include "VoxelizationStep.h"
#include "SGCore/Navigation/NavMesh/NavMesh.h"

void SGCore::Navigation::FilterErosionStep::process(NavMesh& navMesh, const NavMeshConfig& config) noexcept
{
    // todo: OPTIMIZE!! VERY SLOW STEP!!

    const auto voxelizationStep = navMesh.getStep<VoxelizationStep>();

    // filtering too narrow spaces and too low =========================================

    const std::int32_t agentHeightInVoxels = std::ceil(config.m_agentHeight / config.m_cellHeight);
    const std::int32_t agentRadiusInVoxels = std::ceil(config.m_agentRadius / config.m_cellSize);
    // todo: maybe provides bad result
    const std::int32_t agentMaxClimbInVoxels = std::floor(config.m_agentMaxClimb / config.m_cellHeight);

    size_t filteredVoxelsCount = 0;

    for(auto& voxel : voxelizationStep->m_voxels)
    {
        if(!voxel.m_isWalkable) continue;

        // filtering too low spaces
        for(std::int32_t y = agentMaxClimbInVoxels; y <= agentHeightInVoxels; ++y)
        // for(std::int32_t y = 1; y <= agentHeightInVoxels; ++y)
        {
            for(std::int32_t x = -agentRadiusInVoxels; x <= agentRadiusInVoxels; ++x)
            {
                for(std::int32_t z = -agentRadiusInVoxels; z <= agentRadiusInVoxels; ++z)
                {
                    if(glm::length2(glm::vec2 { (float) x, (float) z} * config.m_cellSize) > config.m_agentRadius * config.m_agentRadius - config.m_cellSize * config.m_cellSize)
                    {
                        continue;
                    }

                    const auto it = voxelizationStep->m_voxelsMap.find(voxel.m_position + glm::i32vec3 { x, y, z });
                    if(it == voxelizationStep->m_voxelsMap.end()) continue;

                    voxel.m_isWalkable = false;

                    ++filteredVoxelsCount;
                }
            }
        }
    }

    std::cout << std::format("SGCore::Navigation::FilterErosionStep: filtered {} pathless voxels", filteredVoxelsCount) << std::endl;
}
