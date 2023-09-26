//
// Created by stuka on 04.07.2023.
//

#include "CameraRenderingSystem.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "SkyboxComponent.h"

void Core::ECS::CameraRenderingSystem::update
(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity)
{
    std::shared_ptr<CameraComponent> cameraComponent = entity->getComponent<CameraComponent>();
    std::shared_ptr<TransformComponent> cameraTransformComponent = entity->getComponent<TransformComponent>();

    if(!cameraComponent || !cameraTransformComponent) return;

    Core::Main::CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);

    for(auto& sceneEntity : scene->m_entities)
    {
        std::shared_ptr<TransformComponent> transformComponent = sceneEntity->getComponent<TransformComponent>();

        if(!transformComponent) continue;

        std::shared_ptr<MeshComponent> meshComponent = sceneEntity->getComponent<MeshComponent>();
        std::shared_ptr<SkyboxComponent> skyboxComponent = sceneEntity->getComponent<SkyboxComponent>();
        std::list<std::shared_ptr<IPrimitiveComponent>> primitiveComponents = sceneEntity->getComponents<IPrimitiveComponent>();

        if(meshComponent)
        {
            if (!skyboxComponent)
            {
                Core::Main::CoreMain::getRenderer().renderMesh(
                        cameraComponent,
                        cameraTransformComponent,
                        transformComponent,
                        meshComponent
                );
            }
            else
            {
                Core::Main::CoreMain::getRenderer().renderMesh(
                        cameraComponent,
                        skyboxComponent,
                        transformComponent,
                        meshComponent
                );
            }
        }

        for(const auto& primitiveComponent : primitiveComponents)
        {
            Core::Main::CoreMain::getRenderer().renderPrimitive(
                    cameraComponent,
                    transformComponent,
                    primitiveComponent
            );
        }
    }
}

void Core::ECS::CameraRenderingSystem::deltaUpdate
(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity, const double& deltaTime)
{

}
