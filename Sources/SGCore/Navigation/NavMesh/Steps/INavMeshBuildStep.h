//
// Created by stuka on 06.12.2025.
//

#pragma once

#include <sgcore_export.h>

#include "SGCore/Navigation/NavMesh/NavMeshConfig.h"

namespace SGCore::Navigation
{
    struct NavMesh;

    struct SGCORE_EXPORT INavMeshBuildStep
    {
        std::uint8_t m_stepPriority = 0;

        virtual ~INavMeshBuildStep() = default;

        virtual void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept = 0;
    };
}
