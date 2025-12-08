//
// Created by stuka on 08.12.2025.
//

#include "HeightfieldBuildStep.h"

#include <iostream>

#include "InputFilteringStep.h"
#include "VoxelizationStep.h"
#include "SGCore/Navigation/NavMesh/NavMesh.h"

void SGCore::Navigation::HeightfieldBuildStep::process(NavMesh& navMesh, const NavMeshConfig& config) noexcept
{
    std::cout << "SGCore::Navigation::HeightfieldBuildStep start" << std::endl;

    const auto inputFilteringStep = navMesh.getStep<InputFilteringStep>();
    const auto voxelizationStep = navMesh.getStep<VoxelizationStep>();

    m_heightfieldWidth = voxelizationStep->m_voxelGridWidth;
    m_heightfieldDepth = voxelizationStep->m_voxelGridDepth;

    m_cells.clear();
    m_cells.resize(m_heightfieldWidth * m_heightfieldDepth);

    for(auto& cell : m_cells)
    {
        cell.m_floorHeight = inputFilteringStep->m_sceneAABB.m_min.y;
        cell.m_ceilingHeight = inputFilteringStep->m_sceneAABB.m_max.y;
    }

    const std::int32_t agentHeightCells = std::int32_t(ceil(config.m_agentHeight / config.m_cellHeight));

    std::cout << "agentHeightCells: " << agentHeightCells << std::endl;

    size_t walkableCellsCount {};

    for(std::int32_t x = 0; x < m_heightfieldWidth; ++x)
    {
        for(std::int32_t z = 0; z < m_heightfieldDepth; ++z)
        {
            // finding highest walkable voxel
            float highestFloor = -std::numeric_limits<float>::max();
            float lowestCeiling = std::numeric_limits<float>::max();
            bool foundWalkable = false;

            for(std::int32_t y = 0; y < voxelizationStep->m_voxelGridHeight; ++y)
            {
                if(!voxelizationStep->getVoxel(x, y, z).m_isWalkable) continue;

                // is voxel has space above
                bool hasSpaceAbove = true;
                float floorHeight = voxelizationStep->voxelToWorldY(y, config.m_cellHeight);

                // checking voxels above current
                for(std::int32_t dy = 1; dy <= agentHeightCells; ++dy)
                {
                    if(y + dy >= voxelizationStep->m_voxelGridHeight || voxelizationStep->getVoxel(x, y + dy, z).m_isSolid)
                    {
                        hasSpaceAbove = false;
                        break;
                    }
                }

                if(!hasSpaceAbove) continue;

                const float ceilingHeight = floorHeight + config.m_agentHeight;

                if(floorHeight > highestFloor)
                {
                    highestFloor = floorHeight;
                    lowestCeiling = ceilingHeight;
                    foundWalkable = true;
                }
            }

            if(foundWalkable)
            {
                HeightfieldCell& cell = getCell(x, z);
                cell.m_floorHeight = highestFloor;
                cell.m_ceilingHeight = lowestCeiling;
                cell.m_isWalkable = true;

                ++walkableCellsCount;
            }
        }
    }

    std::cout << std::format("SGCore::Navigation::HeightfieldBuildStep: found {} walkable cells from {} cells", walkableCellsCount, m_cells.size()) << std::endl;

    const float maxSlopeRad = glm::radians(config.m_agentMaxSlope);
    const float maxHeightDiff = config.m_cellSize * tan(maxSlopeRad);
    size_t filteredCount = 0;

    for(std::int32_t x = 0; x < m_heightfieldWidth; ++x)
    {
        for(std::int32_t z = 0; z < m_heightfieldDepth; ++z)
        {
            auto& currentCell = getCell(x, z);

            if(!currentCell.m_isWalkable) continue;

            float centerHeight = currentCell.m_floorHeight;

            // checking 4 neighbours
            const std::int8_t dx[] { -1, 1, 0, 0 };
            const std::int8_t dz[] { 0, 0, -1, 1 };

            for(std::uint8_t i = 0; i < 4; ++i)
            {
                const std::int32_t nx = x + dx[i];
                const std::int32_t nz = z + dz[i];

                if(!(nx >= 0 && nx < m_heightfieldWidth && nz >= 0 && nz < m_heightfieldDepth)) continue;

                const auto& neighbourCell = getCell(nx, nz);

                if(!neighbourCell.m_isWalkable) continue;

                const float neighborHeight = neighbourCell.m_floorHeight;
                float heightDiff = glm::abs(centerHeight - neighborHeight);

                if(heightDiff > maxHeightDiff)
                {
                    currentCell.m_isWalkable = false;
                    ++filteredCount;
                    break;
                }
            }
        }
    }

    std::cout << std::format("SGCore::Navigation::HeightfieldBuildStep: filtered {} cells with steep slopes from {} cells", filteredCount, m_cells.size()) << std::endl;
}

SGCore::Navigation::HeightfieldCell& SGCore::Navigation::HeightfieldBuildStep::getCell(std::int32_t x, std::int32_t y) noexcept
{
    return m_cells[y * m_heightfieldWidth + x];
}
