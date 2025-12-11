//
// Created by stuka on 08.12.2025.
//

#include "FilterErosionStep.h"

#include "VoxelizationStep.h"
#include "SGCore/Navigation/NavMesh/NavMesh.h"

void SGCore::Navigation::FilterErosionStep::process(NavMesh& navMesh, const NavMeshConfig& config) noexcept
{
    const auto voxelizationStep = navMesh.getStep<VoxelizationStep>();

    const float maxSlopeCos = cos(glm::radians(config.m_agentMaxSlope));
    const glm::vec3 up = MathUtils::up3;

    // ===================================== marking voxels that are in too steep slopes
    for(const auto& tri : navMesh.m_inputSceneTriangles)
    {
        const auto minXZ = tri.minXZ();
        const auto maxXZ = tri.maxXZ();

        const auto minX = voxelizationStep->worldToVoxelX(minXZ.x, config.m_cellSize);
        const auto maxX = voxelizationStep->worldToVoxelX(maxXZ.x, config.m_cellSize);
        const auto minZ = voxelizationStep->worldToVoxelZ(minXZ.y, config.m_cellSize);
        const auto maxZ = voxelizationStep->worldToVoxelZ(maxXZ.y, config.m_cellSize);

        const float minY = glm::min(glm::min(tri.m_vertices[0].y,
                                       tri.m_vertices[1].y),
                                       tri.m_vertices[2].y);

        const float maxY = glm::max(glm::max(tri.m_vertices[0].y,
                                       tri.m_vertices[1].y),
                                       tri.m_vertices[2].y);

        const std::int32_t minYv = voxelizationStep->worldToVoxelY(minY, config.m_cellHeight);
        const std::int32_t maxYv = voxelizationStep->worldToVoxelY(maxY, config.m_cellHeight);

        for(auto x = minX; x <= maxX; x++)
        {
            for(auto z = minZ; z <= maxZ; z++)
            {
                for(auto y = minYv; y <= maxYv; y++)
                {
                    if(!voxelizationStep->isVoxelInTriangle(x, y, z, tri, config)) continue;

                    if(!(x >= 0 && x < voxelizationStep->m_voxelGridWidth && y >= 0 && y < voxelizationStep->m_voxelGridHeight && z >= 0 && z < voxelizationStep->m_voxelGridDepth)) continue;

                    const auto it = voxelizationStep->m_voxelsMap.find({ x, y, z});

                    if(it == voxelizationStep->m_voxelsMap.end()) continue;

                    const float slopeCos = glm::abs(glm::dot(tri.m_normal, up));

                    if(slopeCos < maxSlopeCos)
                    {
                        // marking voxel as non-walkable because of too steep slope
                        voxelizationStep->m_voxels[it->second].m_isWalkable = false;
                    }
                }
            }
        }
    }

    // filtering too narrow spaces and too low =========================================

    const std::int32_t agentSizeInVoxels = std::ceil(config.m_agentHeight / config.m_cellHeight);

    for(auto& voxel : voxelizationStep->m_voxels)
    {
        if(!voxel.m_isWalkable) continue;

        // filtering too low spaces
        for(std::int32_t y = 1; y <= agentSizeInVoxels; ++y)
        {
            if(voxelizationStep->m_voxelsMap.contains({ voxel.m_position.x, voxel.m_position.y + y, voxel.m_position.z }))
            {
                voxel.m_isWalkable = false;
            }
        }
    }
}
