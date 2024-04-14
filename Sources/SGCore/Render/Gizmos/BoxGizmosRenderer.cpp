//
// Created by stuka on 02.02.2024.
//

#include <entt/entt.hpp>

#include "SGCore/Render/DebugDraw.h"
#include "BoxGizmosRenderer.h"
#include "SGCore/Scene/Scene.h"
#include "BoxGizmo.h"
#include "SGCore/Render/MeshBuilder.h"

void SGCore::BoxGizmosRenderer::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto lockedScene = m_scene.lock();
    if(!lockedScene) return;
    
    auto boxGizmosView = lockedScene->getECSRegistry()->view<BoxGizmo>();

    auto debugDraw = lockedScene->getSystem<DebugDraw>();
    
    boxGizmosView.each([&debugDraw](BoxGizmo& gizmo) {
        if(gizmo.m_size != gizmo.m_lastSize)
        {
            MeshBuilder::buildBox3D(gizmo.m_base.m_meshBase, gizmo.m_size);

            gizmo.m_lastSize = gizmo.m_size;
        }
        
        debugDraw->drawAABB(-gizmo.m_size / 2, gizmo.m_size / 2, gizmo.m_base.m_color);
    });
}
