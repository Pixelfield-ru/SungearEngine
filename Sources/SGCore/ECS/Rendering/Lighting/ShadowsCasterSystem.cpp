//
// Created by stuka on 27.07.2023.
//

#include "ShadowsCasterSystem.h"
#include "ShadowsCasterComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Rendering/SkyboxComponent.h"
#include "SGCore/ECS/ECSWorld.h"

void Core::ECS::ShadowsCasterSystem::FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene,
                                            const std::shared_ptr<Core::ECS::Entity>& entity)
{
    std::shared_ptr<ShadowsCasterComponent> shadowsCasterComponent = entity->getComponent<ShadowsCasterComponent>();

    if(!shadowsCasterComponent) return;

    // --------------
    ECSWorld::cacheComponents<ShadowsCasterSystem, ShadowsCasterComponent, TransformComponent>(entity);
    // --------------

    shadowsCasterComponent->m_frameBuffer->bind()->clear();

    Core::Main::CoreMain::getRenderer().prepareUniformBuffers(shadowsCasterComponent, nullptr);

    for(auto& sceneEntity : scene->m_entities)
    {
        auto transformComponent = sceneEntity->getComponent<TransformComponent>();
        auto meshComponent = sceneEntity->getComponent<MeshComponent>();

        if(!transformComponent || !meshComponent || sceneEntity->getComponent<SkyboxComponent>()) continue;

        //auto lastFacesCullingType = meshComponent->m_facesCullingFaceType;
        //auto lastFacesCullingPolygonsOrder = meshComponent->m_facesCullingPolygonsOrder;
        meshComponent->m_mesh->m_material->setCurrentShader(SGMAT_SHADOW_GEN_SHADER_NAME);
        //meshComponent->m_facesCullingFaceType = SGFaceType::SGG_FRONT_FACE;
        //meshComponent->m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;

        Core::Main::CoreMain::getRenderer().renderMesh(transformComponent, meshComponent);

        meshComponent->m_mesh->m_material->setCurrentShader(SGMAT_STANDARD_SHADER_NAME);
        //meshComponent->m_facesCullingFaceType = lastFacesCullingType;
        //meshComponent->m_facesCullingPolygonsOrder = lastFacesCullingPolygonsOrder;
    }

    shadowsCasterComponent->m_frameBuffer->bind()->unbind();
}

