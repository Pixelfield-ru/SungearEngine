//
// Created by stuka on 06.12.2025.
//

#pragma once

#include <vector>

#include "NavMeshConfig.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Math/Primitives.h"

namespace SGCore::Navigation
{
    struct SGCORE_EXPORT NavMesh : ECS::Component<NavMesh, const NavMesh>
    {
        NavMeshConfig m_config{};

        void build(const std::vector<Primitives::Triangle<>>& geometry) noexcept;
    };
}
