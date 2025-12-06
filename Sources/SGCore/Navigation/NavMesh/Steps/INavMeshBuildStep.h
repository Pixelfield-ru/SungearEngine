//
// Created by stuka on 06.12.2025.
//

#pragma once

#include "SGCore/Navigation/NavMesh/NavMeshConfig.h"

namespace SGCore::Navigation
{
    struct NavMesh;

    struct INavMeshBuildStep
    {
        std::uint8_t m_stepPriority = 0;

        virtual ~INavMeshBuildStep() = default;

        virtual void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept = 0;
    };
}
