//
// Created by stuka on 02.02.2024.
//

#include <entt/entt.hpp>

#include "BoxGizmosUpdater.h"
#include "SGCore/Scene/Scene.h"
#include "BoxGizmo.h"
#include "SGCore/Render/MeshBuilder.h"

void SGCore::BoxGizmosUpdater::fixedUpdate()
{
    auto lockedScene = m_scene.lock();
    if(!lockedScene) return;
    
    auto boxGizmosView = lockedScene->getECSRegistry().view<BoxGizmo>();

    boxGizmosView.each([](BoxGizmo& gizmo) {
        if(gizmo.m_size != gizmo.m_lastSize)
        {
            MeshBuilder::buildBox3D(gizmo.m_base.m_meshBase, gizmo.m_size);

            gizmo.m_lastSize = gizmo.m_size;
        }
    });
}
