//
// Created by stuka on 08.12.2025.
//

#pragma once

#include <limits>
#include <vector>

#include "INavMeshBuildStep.h"

namespace SGCore::Navigation
{
    struct HeightfieldCell
    {
        float m_floorHeight = -std::numeric_limits<float>::max();
        float m_ceilingHeight = std::numeric_limits<float>::max();
        bool m_isWalkable {};
    };

    struct HeightfieldBuildStep : INavMeshBuildStep
    {
        std::int32_t m_heightfieldWidth {};
        std::int32_t m_heightfieldDepth {};

        std::vector<HeightfieldCell> m_cells;

        void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept override;

        HeightfieldCell& getCell(std::int32_t x, std::int32_t y) noexcept;
    };
}
