//
// Created by stuka on 02.02.2024.
//

#include <entt/entt.hpp>

#include "SGCore/Render/DebugDraw.h"
#include "BoxGizmosRenderer.h"
#include "SGCore/Scene/Scene.h"
#include "BoxGizmo.h"
#include "SGCore/Render/MeshBuilder.h"
#include "SGCore/Render/RenderPipelinesManager.h"

void SGCore::BoxGizmosRenderer::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto lockedScene = getScene();

    if(!lockedScene) return;

    auto debugDraw = RenderPipelinesManager::getCurrentRenderPipeline()->getRenderPass<DebugDraw>();

    if(!debugDraw) return;

    auto boxGizmosView = lockedScene->getECSRegistry()->view<BoxGizmo>();
    
    boxGizmosView.each([&debugDraw](BoxGizmo::reg_t& gizmo) {
        if(gizmo.m_size != gizmo.m_lastSize)
        {
            MeshBuilder::buildBox3D(gizmo.m_base.m_meshBase, gizmo.m_size);

            gizmo.m_lastSize = gizmo.m_size;
        }
        
        debugDraw->drawAABB(-gizmo.m_size / 2, gizmo.m_size / 2, gizmo.m_base.m_color);
    });
}
