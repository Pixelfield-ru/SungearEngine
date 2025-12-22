//
// Created by stuka on 07.12.2025.
//

#include "VoxelizationStep.h"

#include <iostream>

#include "InputFilteringStep.h"
#include "SGCore/Navigation/NavMesh/NavMesh.h"

void SGCore::Navigation::VoxelizationStep::process(NavMesh& navMesh, const NavMeshConfig& config) noexcept
{
    const auto inputFilteringStep = navMesh.getStep<InputFilteringStep>();

    const auto& sceneAABB = inputFilteringStep->m_sceneAABB;

    m_voxelGridOrigin = sceneAABB.m_min;

    m_voxelGridWidth = std::int32_t((sceneAABB.m_max.x - sceneAABB.m_min.x) / config.m_cellSize) + 1;
    m_voxelGridHeight = std::int32_t((sceneAABB.m_max.y - sceneAABB.m_min.y) / config.m_cellHeight) + 1;
    m_voxelGridDepth = std::int32_t((sceneAABB.m_max.z - sceneAABB.m_min.z) / config.m_cellSize) + 1;

    m_voxels.clear();
    m_voxelsMap.clear();

    // m_voxels.resize(m_voxelGridWidth * m_voxelGridHeight * m_voxelGridDepth);
    m_voxels.reserve(m_voxelGridWidth * m_voxelGridHeight);
    m_voxelsMap.reserve(m_voxelGridWidth * m_voxelGridHeight);

    const float maxSlopeCos = cos(glm::radians(config.m_agentMaxSlope));
    const glm::vec3 up = MathUtils::up3;

    for(const auto& tri : navMesh.m_inputSceneTriangles)
    {
        const auto minXZ = tri.minXZ();
        const auto maxXZ = tri.maxXZ();

        const auto minX = worldToVoxelX(minXZ.x, config.m_cellSize);
        const auto maxX = worldToVoxelX(maxXZ.x, config.m_cellSize);
        const auto minZ = worldToVoxelZ(minXZ.y, config.m_cellSize);
        const auto maxZ = worldToVoxelZ(maxXZ.y, config.m_cellSize);

        const float minY = glm::min(glm::min(tri.m_vertices[0].y,
                                       tri.m_vertices[1].y),
                                       tri.m_vertices[2].y);

        const float maxY = glm::max(glm::max(tri.m_vertices[0].y,
                                       tri.m_vertices[1].y),
                                       tri.m_vertices[2].y);

        const std::int32_t minYv = worldToVoxelY(minY, config.m_cellHeight);
        const std::int32_t maxYv = worldToVoxelY(maxY, config.m_cellHeight);

        const float triangleHeight = maxY - minY;

        // marking solid voxels

        for(auto x = minX; x <= maxX; ++x)
        {
            for(auto z = minZ; z <= maxZ; ++z)
            {
                for(auto y = minYv; y <= maxYv; ++y)
                {
                    if(!isVoxelInTriangle(x, y, z, tri, config)) continue;

                    if(!(x >= 0 && x < m_voxelGridWidth && y >= 0 && y < m_voxelGridHeight && z >= 0 && z < m_voxelGridDepth)) continue;

                    const auto voxelPos = glm::i32vec3 { x, y, z };

                    if(m_voxelsMap.contains(voxelPos)) continue;

                    const float slopeCos = glm::abs(glm::dot(tri.m_normal, up));

                    NavVoxel v;
                    v.m_isWalkable = true;
                    // v.m_isWalkable = true;
                    v.m_position = voxelPos;

                    /*if(slopeCos >= cos(glm::radians(45.0f)))
                    {
                        v.m_isWalkable = false;
                    }*/

                    // marking voxel as non-walkable because of too steep slope (if too steep) or true if normal
                    // todo: how to check steep slopes??
                    /*if(slopeCos >= maxSlopeCos && triangleHeight > config.m_agentMaxClimb)
                    {
                        v.m_isWalkable = false;
                    }*/

                    m_voxels.push_back(v);

                    m_voxelsMap[voxelPos] = m_voxels.size() - 1;
                }
            }
        }
    }

    // =========================== finding walkable voxels

    const std::int32_t agentHeightCells = std::int32_t(ceil(config.m_agentHeight / config.m_cellHeight));

    /*for(std::int32_t z = 0; z < m_voxelGridDepth; z++)
    {
        for(std::int32_t x = 0; x < m_voxelGridWidth; x++)
        {
            for(std::int32_t y = 0; y < m_voxelGridHeight; y++)
            {
                if(!getVoxel(x, y, z).m_isSolid) continue;

                // is there is enough space above the voxel
                bool hasEnoughSpace = true;
                for(std::int32_t dy = 1; dy <= agentHeightCells; dy++)
                {
                    if(y + dy >= m_voxelGridHeight || getVoxel(x, y + dy, z).m_isSolid)
                    {
                        hasEnoughSpace = false;
                        break;
                    }
                }

                if(hasEnoughSpace)
                {
                    getVoxel(x, y, z).m_isWalkable = true;
                }
            }
        }
    }*/

    std::cout << std::format(
        "SGCore::Navigation::VoxelizationStep: created grid with size {}x{}x{}, grid byte size: {}, mapping byte size (approx): {}",
        m_voxelGridWidth,
        m_voxelGridHeight,
        m_voxelGridDepth,
        m_voxels.size() * sizeof(NavVoxel),
        m_voxelsMap.size() * sizeof(NavVoxel)) << std::endl;
}

std::int32_t SGCore::Navigation::VoxelizationStep::worldToVoxelX(float wx, float cellSize) const noexcept
{
    return std::int32_t((wx - m_voxelGridOrigin.x) / cellSize);
}

std::int32_t SGCore::Navigation::VoxelizationStep::worldToVoxelY(float wy, float cellHeight) const noexcept
{
    return std::int32_t((wy - m_voxelGridOrigin.y) / cellHeight);
}

std::int32_t SGCore::Navigation::VoxelizationStep::worldToVoxelZ(float wz, float cellSize) const noexcept
{
    return std::int32_t((wz - m_voxelGridOrigin.z) / cellSize);
}

glm::vec3 SGCore::Navigation::VoxelizationStep::voxelToWorld(const glm::i32vec3& voxelPos, float cellSize, float cellHeight) const noexcept
{
    return glm::vec3(voxelToWorldX(voxelPos.x, cellSize), voxelToWorldY(voxelPos.y, cellHeight), voxelToWorldZ(voxelPos.z, cellSize));
}

float SGCore::Navigation::VoxelizationStep::voxelToWorldX(std::int32_t vx, float cellSize) const noexcept
{
    return m_voxelGridOrigin.x + (vx + 0.5f) * cellSize;
}

float SGCore::Navigation::VoxelizationStep::voxelToWorldY(std::int32_t vy, float cellHeight) const noexcept
{
    return m_voxelGridOrigin.y + (vy + 0.5f) * cellHeight;
}

float SGCore::Navigation::VoxelizationStep::voxelToWorldZ(std::int32_t vz, float cellSize) const noexcept
{
    return m_voxelGridOrigin.z + (vz + 0.5f) * cellSize;
}

SGCore::Navigation::NavVoxel& SGCore::Navigation::VoxelizationStep::getVoxel(std::int32_t x,
                                                                             std::int32_t y,
                                                                             std::int32_t z) noexcept
{
    return m_voxels[(z * m_voxelGridHeight + y) * m_voxelGridWidth + x];
}

bool SGCore::Navigation::VoxelizationStep::isVoxelInTriangle(std::int32_t vx, std::int32_t vy, std::int32_t vz,
                                                             const MathPrimitivesUtils::Triangle<>& tri,
                                                             const NavMeshConfig& config) const
{
    const glm::vec3 voxelCenter {
        m_voxelGridOrigin.x + (vx + 0.5f) * config.m_cellSize,
        m_voxelGridOrigin.y + (vy + 0.5f) * config.m_cellHeight,
        m_voxelGridOrigin.z + (vz + 0.5f) * config.m_cellSize
    };

    // distance to triangle
    const glm::vec3 toVoxel = voxelCenter - tri.m_vertices[0];
    const float dist = glm::dot(tri.m_normal, toVoxel);

    // voxel is too far from triangle
    if(glm::abs(dist) > config.m_cellHeight * 0.866f) return false;

    const glm::vec3 proj = voxelCenter - tri.m_normal * dist;

    const glm::vec3 v0 = tri.m_vertices[2] - tri.m_vertices[0];
    const glm::vec3 v1 = tri.m_vertices[1] - tri.m_vertices[0];
    const glm::vec3 v2 = proj - tri.m_vertices[0];

    const float dot00 = glm::dot(v0, v0);
    const float dot01 = glm::dot(v0, v1);
    const float dot02 = glm::dot(v0, v2);
    const float dot11 = glm::dot(v1, v1);
    const float dot12 = glm::dot(v1, v2);

    const float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    const float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    const float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}
