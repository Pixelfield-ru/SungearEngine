//
// Created by stuka on 29.06.2026.
//

#include "NavMeshDebugDraw.h"

#include "NavMesh.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Scene/Scene.h"

void SGCore::Navigation::NavMeshDebugDraw::update(double dt, double fixedDt)
{
    const auto scene = getScene();
    if(!scene) return;

    const auto renderPipeline = RenderPipelinesManager::instance().getCurrentRenderPipeline();
    const auto debugDraw = renderPipeline->getRenderPass<DebugDraw>();

    const auto registry = scene->getECSRegistry();

    const auto navMeshView = registry->view<NavMesh>();

    navMeshView.each([&](const auto& navMesh) {
        const dtNavMesh* nativeNavMesh = navMesh.getNativeNavMesh();

        if(!nativeNavMesh) return;

        for(int i = 0; i < nativeNavMesh->getMaxTiles(); ++i)
        {
            const dtMeshTile* tile = nativeNavMesh->getTile(i);
            if(!tile || !tile->header) continue;

            const dtPoly* polys = tile->polys;
            for(int j = 0; j < tile->header->polyCount; ++j)
            {
                const auto* poly = &polys[j];

                for(int k = 0; k < poly->vertCount; ++k)
                {
                    const int curIndex = k;
                    const int nextIndex = (k + 1) % poly->vertCount;  // Замыкание

                    const unsigned short v0 = poly->verts[curIndex];
                    const float* pos0 = &tile->verts[v0 * 3];

                    const unsigned short v1 = poly->verts[nextIndex];
                    const float* pos1 = &tile->verts[v1 * 3];

                    debugDraw->drawLine(
                        { pos0[0], pos0[1], pos0[2] },
                        { pos1[0], pos1[1], pos1[2] },
                        { 0.1f, 0.3f, 1.0f, 1.0f }
                    );
                }
            }
        }
    });
}
