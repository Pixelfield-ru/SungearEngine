//
// Created by ilya on 01.03.24.
//

#include <entt/entt.hpp>
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"

#include "BatchesRenderer.h"

#include "SGCore/Scene/Scene.h"
#include "Batch.h"

void SGCore::BatchesRenderer::update(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = getScene();
    if(!lockedScene) return;
    
    auto batchesView = lockedScene->getECSRegistry()->view<Batch>();
    auto camerasView = lockedScene->getECSRegistry()->view<Camera3D, RenderingBase, Transform>();
    
    camerasView.each([&batchesView](Camera3D::reg_t& camera3D, RenderingBase::reg_t& renderingBase, Transform::reg_t& transform) {
        CoreMain::getRenderer()->prepareUniformBuffers(renderingBase, transform);
        
        batchesView.each([](Batch& batch) {
            batch.renderAll();
        });
    });
}
