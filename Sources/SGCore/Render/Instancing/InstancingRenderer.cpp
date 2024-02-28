//
// Created by ilya on 28.02.24.
//

#include <SGCore/Render/Camera3D.h>
#include <SGCore/Render/RenderingBase.h>
#include "InstancingRenderer.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "Instancing.h"

void SGCore::InstancingRenderer::update(const double& dt)
{
    auto lockedScene = m_scene.lock();
    
    if(!lockedScene) return;
    
    auto& registry = lockedScene->getECSRegistry();
    
    auto instancingEntitiesView = registry.view<Instancing>();
    auto camerasView = registry.view<Camera3D, RenderingBase, Transform>();
    
    instancingEntitiesView.each([&registry, &camerasView](Instancing& instancing) {
        for(const auto& e : instancing.m_entitiesToRender)
        {
            Mesh* entityMesh = registry.try_get<Mesh>(e);
            Transform* entityTransform = registry.try_get<Transform>(e);
            
            if(entityMesh && entityTransform)
            {
                instancing.drawMesh(*entityMesh, *entityTransform);
            }
        }
        
        camerasView.each([&instancing](Camera3D& camera3D, RenderingBase& renderingBase, Transform& cameraTransform) {
            CoreMain::getRenderer()->prepareUniformBuffers(renderingBase, cameraTransform);
            
            instancing.drawAll();
        });
    });
}
