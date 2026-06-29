//
// Created by stuka on 06.12.2025.
//

#pragma once

#include <vector>
#include <DetourNavMesh.h>
#include <Recast.h>

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

        [[nodiscard]] std::vector<glm::vec3> findPath(const glm::vec3& start,
                                                      const glm::vec3& end,
                                                      const glm::vec3& polyPickHalfExt) const noexcept;

        const dtNavMesh* getNativeNavMesh() const noexcept;

        void clear() noexcept;

    private:
        // recast data
        rcHeightfield* m_heightfield {};
        rcCompactHeightfield* m_compactHeightfield {};
        rcContourSet* m_contourSet {};
        rcPolyMesh* m_polyMesh {};
        rcPolyMeshDetail* m_detailMesh {};

        // detour data
        std::uint8_t* m_navData {};
        std::int32_t m_navDataSize {};
        dtNavMesh* m_navMesh {};
        dtNavMeshQuery* m_navQuery {};

        NavBuildContext m_context {};
    };
}
