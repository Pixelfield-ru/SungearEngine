//
// Created by stuka on 08.12.2025.
//

#pragma once

#include <limits>
#include <vector>
#include <glm/vec3.hpp>

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
        std::int32_t m_heightfieldHeight {};
        std::int32_t m_heightfieldDepth {};
        glm::vec3 m_heightfieldOrigin {};

        std::vector<HeightfieldCell> m_cells;

        void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept override;

        HeightfieldCell& getCell(std::int32_t x, std::int32_t y) noexcept;
    };
}
