//
// Created by stuka on 06.12.2025.
//

#pragma once

#include <Recast.h>
#include <vector>

#include "NavBuildContext.h"
#include "NavMeshConfig.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Math/Primitives.h"

namespace SGCore::Navigation
{
    struct SGCORE_EXPORT NavMesh : ECS::Component<NavMesh, const NavMesh>
    {
        NavMeshConfig m_config{};

        ~NavMesh() noexcept override;

        void build(const std::vector<Primitives::Triangle<>>& geometry) noexcept;

    private:
        rcHeightfield* m_heightfield {};
        rcCompactHeightfield* m_compactHeightfield {};
        rcContourSet* m_contourSet {};

        NavBuildContext m_context {};
    };
}
