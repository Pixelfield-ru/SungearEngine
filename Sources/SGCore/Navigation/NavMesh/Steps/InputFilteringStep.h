//
// Created by stuka on 07.12.2025.
//

#pragma once

#include "INavMeshBuildStep.h"

#include <vector>

#include "SGCore/Math/MathPrimitivesUtils.h"

namespace SGCore::Navigation
{
    struct InputFilteringStep : INavMeshBuildStep
    {
        std::vector<MathPrimitivesUtils::Triangle<>> m_walkableTriangles;
        AABB<> m_sceneAABB{};

        void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept;
    };
}
