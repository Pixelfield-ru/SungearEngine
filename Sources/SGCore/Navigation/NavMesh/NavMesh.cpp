//
// Created by stuka on 06.12.2025.
//

#include "NavMesh.h"

#include <recastnavigation/Recast.h>

#include "SGCore/Logger/Logger.h"
#include "SGCore/Math/AABB.h"

void SGCore::Navigation::NavMesh::build(const std::vector<Primitives::Triangle<>>& geometry) noexcept
{
    if(geometry.empty())
    {
        LOG_W(SGCORE_TAG, "Cannot build navigation mesh: no geometry was provided.");
        return;
    }

    AABB<> geometryAABB;
    geometryAABB.calculate(geometry);

    rcConfig recastConfig {};
    memset(&recastConfig, 0, sizeof(rcConfig));

    recastConfig.cs = m_config.m_cellSize;
    recastConfig.ch = m_config.m_cellHeight;
    recastConfig.walkableSlopeAngle = m_config.m_agentMaxSlope;
    recastConfig.walkableHeight = static_cast<int>(std::ceilf(m_config.m_agentHeight / recastConfig.ch));
    recastConfig.walkableClimb = static_cast<int>(std::floorf(m_config.m_agentMaxClimb / recastConfig.ch));
    recastConfig.walkableRadius = static_cast<int>(std::ceilf(m_config.m_agentRadius / recastConfig.cs));
    recastConfig.maxEdgeLen = static_cast<int>(m_config.m_edgeMaxLength / recastConfig.cs);
    recastConfig.maxSimplificationError = m_config.m_maxSimplificationError;
    recastConfig.minRegionArea = rcSqr(m_config.m_minRegionSize);
    recastConfig.mergeRegionArea = rcSqr(m_config.m_mergeRegionSize);
    recastConfig.maxVertsPerPoly = m_config.m_maxVerticesPerPoly;
    recastConfig.detailSampleDist = m_config.m_detailSampleDistance < 0.9f ? 0 : recastConfig.cs * m_config.m_detailSampleDistance;
    recastConfig.detailSampleMaxError = recastConfig.ch * m_config.m_detailSampleMaxError;

    rcVcopy(recastConfig.bmin, &geometryAABB.m_min[0]);
    rcVcopy(recastConfig.bmax, &geometryAABB.m_max[0]);

    rcCalcGridSize(recastConfig.bmin, recastConfig.bmax, recastConfig.cs, &recastConfig.width, &recastConfig.height);


}
