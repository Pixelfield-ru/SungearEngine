//
// Created by stuka on 02.02.2024.
//

#include "LineGizmosUpdater.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Transformations/TransformBase.h"
#include "SGCore/Render/MeshBuilder.h"
#include "LineGizmo.h"

void SGCore::LineGizmosUpdater::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto lockedScene = m_scene.lock();
    if(!lockedScene) return;
    
    auto lineGizmosView = lockedScene->getECSRegistry().view<LineGizmo>();

    lineGizmosView.each([](LineGizmo& gizmo) {
        if(gizmo.m_start != gizmo.m_lastStart || gizmo.m_end != gizmo.m_lastEnd)
        {
            MeshBuilder::buildLine(gizmo.m_base.m_meshBase, gizmo.m_start, gizmo.m_end);

            gizmo.m_lastStart = gizmo.m_start;
            gizmo.m_lastEnd = gizmo.m_end;
        }
    });
}
