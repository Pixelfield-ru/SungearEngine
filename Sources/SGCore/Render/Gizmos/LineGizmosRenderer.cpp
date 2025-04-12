//
// Created by stuka on 02.02.2024.
//

#include "SGCore/Render/DebugDraw.h"
#include "LineGizmosRenderer.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/TransformBase.h"
#include "SGCore/Render/MeshBuilder.h"
#include "LineGizmo.h"
#include "SGCore/Render/RenderPipelinesManager.h"

void SGCore::LineGizmosRenderer::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto debugDraw = RenderPipelinesManager::getCurrentRenderPipeline()->getRenderPass<DebugDraw>();

    if(!debugDraw) return;

    auto lineGizmosView = lockedScene->getECSRegistry()->view<LineGizmo>();
    
    lineGizmosView.each([&debugDraw](LineGizmo::reg_t& gizmo) {
        if(gizmo.m_start != gizmo.m_lastStart || gizmo.m_end != gizmo.m_lastEnd)
        {
            MeshBuilder::buildLine(gizmo.m_base.m_meshBase, gizmo.m_start, gizmo.m_end);

            gizmo.m_lastStart = gizmo.m_start;
            gizmo.m_lastEnd = gizmo.m_end;
        }
        
        debugDraw->drawLine(gizmo.m_start, gizmo.m_end, gizmo.m_base.m_color);
    });
}
