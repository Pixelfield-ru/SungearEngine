//
// Created by stuka on 06.12.2025.
//

#include "NavMesh.h"

#include <recastnavigation/Recast.h>

#include "SGCore/Logger/Logger.h"
#include "SGCore/Math/AABB.h"

SGCore::Navigation::NavMesh::~NavMesh() noexcept
{
    rcFreeHeightField(m_heightfield);
    m_heightfield = {};

    rcFreeCompactHeightfield(m_compactHeightfield);
    m_compactHeightfield = {};

    rcFreeContourSet(m_contourSet);
    m_contourSet = {};
}

void SGCore::Navigation::NavMesh::build(const std::vector<Primitives::Triangle<>>& geometry) noexcept
{
    if(geometry.empty())
    {
        LOG_W(SGCORE_TAG, "Cannot build navigation mesh: no geometry was provided.");
        return;
    }

    // step 1. init

    const auto* vertices = reinterpret_cast<const float*>(geometry.data());
    const size_t verticesCount = geometry.size() * 3;

    // making triangles indices
    std::vector<int> trianglesIndices(verticesCount);
    for(size_t i = 0; i < verticesCount; ++i)
    {
        trianglesIndices[i] = i / 3;
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

    // step 2. rasterize geometry

    m_context.resetTimers();
    m_context.startTimer(RC_TIMER_TOTAL);

    m_heightfield = rcAllocHeightfield();

    if(!m_heightfield)
    {
        m_context.log(RC_LOG_ERROR, "Failed to allocate memory for heightfield.");
        return;
    }

    const bool isHeightfieldCreated = rcCreateHeightfield(
        &m_context,
        *m_heightfield,
        recastConfig.width,
        recastConfig.height,
        recastConfig.bmin,
        recastConfig.bmax,
        recastConfig.cs,
        recastConfig.ch);

    if(!isHeightfieldCreated)
    {
        m_context.log(RC_LOG_ERROR, "Failed to create heightfield.");
        return;
    }

    std::vector<std::uint8_t> triAreas(geometry.size(), 0);

    rcMarkWalkableTriangles(
        &m_context,
        recastConfig.walkableSlopeAngle,
        vertices,
        verticesCount,
        trianglesIndices.data(),
        trianglesIndices.size(),
        triAreas.data());

    const bool isRasterized = rcRasterizeTriangles(
        &m_context,
        vertices,
        verticesCount,
        trianglesIndices.data(),
        triAreas.data(),
        trianglesIndices.size(),
        *m_heightfield,
        recastConfig.walkableClimb);

    if(!isRasterized)
    {
        m_context.log(RC_LOG_ERROR, "Failed to rasterize geometry.");
        return;
    }

    // step 3. filtering walkable cells

    if (m_config.m_filterLowHangingObstacles)
    {
        rcFilterLowHangingWalkableObstacles(&m_context, recastConfig.walkableClimb, *m_heightfield);
    }
    if (m_config.m_filterLedgeSpans)
    {
        rcFilterLedgeSpans(&m_context, recastConfig.walkableHeight, recastConfig.walkableClimb, *m_heightfield);
    }
    if (m_config.m_filterLowHeightSpans)
    {
        rcFilterWalkableLowHeightSpans(&m_context, recastConfig.walkableHeight, *m_heightfield);
    }

    // step 4. creating compact heightfield & connections between neighbor walkable cells

    m_compactHeightfield = rcAllocCompactHeightfield();
    if(!m_compactHeightfield)
    {
        m_context.log(RC_LOG_ERROR, "Failed to allocate memory for compact heightfield.");
        return;
    }

    const bool isCompactHeightfieldBuilt = rcBuildCompactHeightfield(
            &m_context,
            recastConfig.walkableHeight,
            recastConfig.walkableClimb,
            *m_heightfield,
            *m_compactHeightfield);

    if(!isCompactHeightfieldBuilt)
    {
        m_context.log(RC_LOG_ERROR, "Failed to build compact heightfield.");
        return;
    }

    if (!rcErodeWalkableArea(&m_context, recastConfig.walkableRadius, *m_compactHeightfield))
    {
        m_context.log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
        return;
    }

    // todo:
    /*for (ConvexVolume& vol : inputGeometry->convexVolumes)
    {
        rcMarkConvexPolyArea(
            buildContext,
            vol.verts,
            vol.nverts,
            vol.hmin,
            vol.hmax,
            (unsigned char)vol.area,
            *compactHeightfield);
    }*/

    if (m_config.m_partitionType == PartitionType::PT_WATERSHED)
    {
        // Prepare for region partitioning, by calculating distance field along the walkable surface.
        if (!rcBuildDistanceField(&m_context, *m_compactHeightfield))
        {
            m_context.log(RC_LOG_ERROR, "Could not build distance field.");
            return;
        }

        // Partition the walkable surface into contiguous regions.
        if(!rcBuildRegions(&m_context, *m_compactHeightfield, 0, recastConfig.minRegionArea,
                           recastConfig.mergeRegionArea))
        {
            m_context.log(RC_LOG_ERROR, "Could not build watershed regions.");
            return;
        }
    }
    else if (m_config.m_partitionType == PartitionType::PT_MONOTONE)
    {
        // Partition the walkable surface into contiguous regions.
        // Monotone partitioning does not need distancefield.
        if(!rcBuildRegionsMonotone(&m_context, *m_compactHeightfield, 0, recastConfig.minRegionArea,
                                   recastConfig.mergeRegionArea))
        {
            m_context.log(RC_LOG_ERROR, "Could not build monotone regions.");
            return;
        }
    }
    else if(m_config.m_partitionType == PartitionType::PT_LAYERS)
    {
        // Partition the walkable surface into contiguous regions.
        // Layer partitioning does not need distancefield.
        if (!rcBuildLayerRegions(&m_context, *m_compactHeightfield, 0, recastConfig.minRegionArea))
        {
            m_context.log(RC_LOG_ERROR, "Could not build layer regions.");
            return;
        }
    }

    // step 5. trace and simplify region contours

    m_contourSet = rcAllocContourSet();
    if(!m_contourSet)
    {
        m_context.log(RC_LOG_ERROR, "Could not allocate contour set.");
        return;
    }

    if (!rcBuildContours(&m_context, *m_compactHeightfield, recastConfig.maxSimplificationError, recastConfig.maxEdgeLen, *m_contourSet))
    {
        m_context.log(RC_LOG_ERROR, "Could not create contours.");
        return;
    }

    // todo: finish
}
