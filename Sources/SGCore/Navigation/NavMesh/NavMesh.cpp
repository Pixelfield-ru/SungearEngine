//
// Created by stuka on 06.12.2025.
//

#include "NavMesh.h"

#include <recastnavigation/Recast.h>
#include <recastnavigation/DetourNavMeshBuilder.h>
#include <recastnavigation/DetourNavMesh.h>
#include <recastnavigation/DetourNavMeshQuery.h>

#include "SGCore/Logger/Logger.h"
#include "SGCore/Math/AABB.h"

SGCore::Navigation::NavMesh::~NavMesh() noexcept
{
    clear();
}

void SGCore::Navigation::NavMesh::build(const std::vector<Primitives::Triangle<>>& geometry) noexcept
{
    if(geometry.empty())
    {
        LOG_W(SGCORE_TAG, "Cannot build navigation mesh: no geometry was provided.");
        return;
    }

    clear();

    // step 1. init

    const auto* vertices = reinterpret_cast<const float*>(geometry.data());
    const size_t verticesCount = geometry.size() * 3;

    // making triangles indices
    std::vector<int> trianglesIndices(verticesCount, 0);
    for(size_t i = 0; i < trianglesIndices.size(); ++i)
    {
        trianglesIndices[i] = i;
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
        trianglesIndices.size() / 3,
        triAreas.data());

    const bool isRasterized = rcRasterizeTriangles(
        &m_context,
        vertices,
        verticesCount,
        trianglesIndices.data(),
        triAreas.data(),
        trianglesIndices.size() / 3,
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

    if(!rcBuildContours(&m_context, *m_compactHeightfield, recastConfig.maxSimplificationError, recastConfig.maxEdgeLen, *m_contourSet))
    {
        m_context.log(RC_LOG_ERROR, "Could not create contours.");
        return;
    }

    m_polyMesh = rcAllocPolyMesh();
    if (!m_polyMesh)
    {
        m_context.log(RC_LOG_ERROR, "Cannot allocate poly mesh.");
        return;
    }
    if (!rcBuildPolyMesh(&m_context, *m_contourSet, recastConfig.maxVertsPerPoly, *m_polyMesh))
    {
        m_context.log(RC_LOG_ERROR, "Cannot triangulate contours.");
        return;
    }

    m_detailMesh = rcAllocPolyMeshDetail();
    if (!m_detailMesh)
    {
        m_context.log(RC_LOG_ERROR, "Cannot allocate detail mesh.");
        return;
    }
    if (!rcBuildPolyMeshDetail(
            &m_context,
            *m_polyMesh,
            *m_compactHeightfield,
            recastConfig.detailSampleDist,
            recastConfig.detailSampleMaxError,
            *m_detailMesh))
    {
        m_context.log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
        return;
    }

    LOG_I(SGCORE_TAG, "Building navmesh... Vertices count: {}, polys count: {}, NVP: {}",
          m_polyMesh->nverts,
          m_polyMesh->npolys,
          m_polyMesh->nvp);

    dtNavMeshCreateParams params {};
    params.verts = m_polyMesh->verts;
    params.vertCount = m_polyMesh->nverts;
    params.polys = m_polyMesh->polys;
    params.polyAreas = m_polyMesh->areas;
    params.polyFlags = m_polyMesh->flags;
    params.polyCount = m_polyMesh->npolys;
    params.nvp = m_polyMesh->nvp;
    params.detailMeshes = m_detailMesh->meshes;
    params.detailVerts = m_detailMesh->verts;
    params.detailVertsCount = m_detailMesh->nverts;
    params.detailTris = m_detailMesh->tris;
    params.detailTriCount = m_detailMesh->ntris;
    /*params.offMeshConVerts = inputGeometry->offmeshConnVerts.data();
    params.offMeshConRad = inputGeometry->offmeshConnRadius.data();
    params.offMeshConDir = inputGeometry->offmeshConnBidirectional.data();
    params.offMeshConAreas = inputGeometry->offmeshConnArea.data();
    params.offMeshConFlags = inputGeometry->offmeshConnFlags.data();
    params.offMeshConUserID = inputGeometry->offmeshConnId.data();
    params.offMeshConCount = static_cast<int>(inputGeometry->offmeshConnArea.size());*/
    /*params.walkableHeight = recastConfig.walkableHeight;
    params.walkableRadius = recastConfig.walkableRadius;
    params.walkableClimb = recastConfig.walkableClimb;*/
    params.walkableHeight = m_config.m_agentHeight;
    params.walkableRadius = m_config.m_agentRadius;
    params.walkableClimb = m_config.m_agentMaxClimb;
    rcVcopy(params.bmin, m_polyMesh->bmin);
    rcVcopy(params.bmax, m_polyMesh->bmax);
    params.cs = recastConfig.cs;
    params.ch = recastConfig.ch;
    params.buildBvTree = true;

    auto polyFlags = new unsigned short[m_polyMesh->npolys];
    auto polyAreas = new unsigned char[m_polyMesh->npolys];

    // todo: make flags & areas
    for (int i = 0; i < params.polyCount; ++i)
    {
        polyAreas[i] = RC_WALKABLE_AREA;
        polyFlags[i] = 1;
    }

    params.polyAreas = polyAreas;
    params.polyFlags = polyFlags;

    if (!dtCreateNavMeshData(&params, &m_navData, &m_navDataSize))
    {
        m_context.log(RC_LOG_ERROR, "Cannot build Detour navmesh.");
        return;
    }

    m_navMesh = dtAllocNavMesh();
    if (!m_navMesh)
    {
        m_context.log(RC_LOG_ERROR, "Cannot create Detour navmesh.");
        return;
    }

    auto status = m_navMesh->init(m_navData, m_navDataSize, DT_TILE_FREE_DATA);
    if (dtStatusFailed(status))
    {
        dtFree(m_navData);
        m_navData = {};

        m_context.log(RC_LOG_ERROR, "Cannot init Detour navmesh.");
        return;
    }

    m_navQuery = dtAllocNavMeshQuery();
    if(!m_navQuery)
    {
        m_context.log(RC_LOG_ERROR, "Cannot allocate navmesh query.");
        return;
    }

    status = m_navQuery->init(m_navMesh, 2048);
    if (dtStatusFailed(status))
    {
        m_context.log(RC_LOG_ERROR, "Cannot init Detour navmesh query.");
        return;
    }

    if(m_navMesh->getMaxTiles() <= 0)
    {
        m_context.log(RC_LOG_ERROR, "Cannot build navmesh.");
        return;
    }

    LOG_I(SGCORE_TAG, "Navmesh was built successfully.");
}

std::vector<glm::vec3> SGCore::Navigation::NavMesh::findPath(const glm::vec3& start,
                                                             const glm::vec3& end,
                                                             const glm::vec3& polyPickHalfExt) const noexcept
{
    if(!m_navQuery)
    {
        LOG_E(SGCORE_TAG, "NavMesh was not built.");
        return {};
    }

    dtPolyRef startRef, endRef;

    dtQueryFilter filter {};
    filter.setIncludeFlags(0xFFFF);  // Все полигоны
    filter.setExcludeFlags(0);

    if(dtStatusFailed(m_navQuery->findNearestPoly(&start[0], &polyPickHalfExt[0], &filter, &startRef, nullptr)))
    {
        LOG_E(SGCORE_TAG, "Cannot find nearest poly to start point.");
        return {};
    }

    if(dtStatusFailed(m_navQuery->findNearestPoly(&end[0], &polyPickHalfExt[0], &filter, &endRef, nullptr)))
    {
        LOG_E(SGCORE_TAG, "Cannot find nearest poly to end point.");
        return {};
    }

    if(!startRef || !endRef)
    {
        if(!startRef)
        {
            LOG_E(SGCORE_TAG, "Cannot find start polygon.");
        }

        if(!endRef)
        {
            LOG_E(SGCORE_TAG, "Cannot find end polygon.");
        }

        return {};
    }

    constexpr std::int32_t maxPolygons = 256;
    dtPolyRef polys[maxPolygons];
    std::int32_t polyCount = 0;

    if(dtStatusFailed(m_navQuery->findPath(startRef, endRef, &start[0], &end[0], &filter, polys, &polyCount,
                                           maxPolygons)))
    {
        LOG_E(SGCORE_TAG, "Cannot find path between polys.");
        return {};
    }

    if(polyCount == 0)
    {
        LOG_E(SGCORE_TAG, "Cannot find path between polys. No polys found.");
        return {};
    }

    float straightPath[maxPolygons * 3];
    unsigned char straightPathFlags[maxPolygons] {};
    dtPolyRef straightPathPolys[maxPolygons];
    int straightPathCount;

    if(dtStatusFailed(m_navQuery->findStraightPath(&start[0], &end[0], polys, polyCount,
                                                   straightPath, nullptr, nullptr,
                                                   &straightPathCount, maxPolygons)))
    {
        LOG_E(SGCORE_TAG, "Cannot find straight path between polys.");
        return {};
    }

    std::vector<glm::vec3> points;

    for(int i = 0; i < straightPathCount; ++i)
    {
        points.emplace_back(straightPath[i * 3], straightPath[i * 3 + 1], straightPath[i * 3 + 2]);
    }

    return points;
}

const dtNavMesh* SGCore::Navigation::NavMesh::getNativeNavMesh() const noexcept
{
    return m_navMesh;
}

void SGCore::Navigation::NavMesh::clear() noexcept
{
    if(m_heightfield) rcFreeHeightField(m_heightfield);
    m_heightfield = {};

    if(m_compactHeightfield) rcFreeCompactHeightfield(m_compactHeightfield);
    m_compactHeightfield = {};

    if(m_contourSet) rcFreeContourSet(m_contourSet);
    m_contourSet = {};

    if(m_polyMesh) rcFreePolyMesh(m_polyMesh);
    m_polyMesh = {};

    if(m_detailMesh) rcFreePolyMeshDetail(m_detailMesh);
    m_detailMesh = {};

    if(m_navMesh) dtFreeNavMesh(m_navMesh);
    m_navMesh = {};

    if(m_navQuery) dtFreeNavMeshQuery(m_navQuery);
    m_navQuery = {};

    /*if(m_navData) dtFree(m_navData);
    m_navData = {};*/
}
