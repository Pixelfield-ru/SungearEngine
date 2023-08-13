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

    for(auto& sceneEntity : scene->m_entities)
    {
        std::shared_ptr<TransformComponent> transformComponent = sceneEntity->getComponent<TransformComponent>();
        std::shared_ptr<MeshComponent> meshComponent = sceneEntity->getComponent<MeshComponent>();
        std::shared_ptr<SkyboxComponent> skyboxComponent = sceneEntity->getComponent<SkyboxComponent>();

        if(!transformComponent || !meshComponent) continue;

        if(!skyboxComponent)
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
}

void Core::ECS::CameraRenderingSystem::deltaUpdate
(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity, const double& deltaTime)
{

}
