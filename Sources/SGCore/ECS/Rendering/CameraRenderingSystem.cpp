//
// Created by stuka on 04.07.2023.
//

#include "CameraRenderingSystem.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "SkyboxComponent.h"
#include "SGCore/ECS/ECSWorld.h"

void Core::ECS::CameraRenderingSystem::FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    auto systemCachedComponents = ECSWorld::getSystemCachedEntities<CameraRenderingSystem>();

    if(systemCachedComponents == nullptr) return;

    for (const auto& cachedEntities : systemCachedComponents->m_cachedEntities)
    {
        if(cachedEntities.second == nullptr) return;

        std::shared_ptr<CameraComponent> cameraComponent = cachedEntities.second->getComponent<CameraComponent>();
        std::shared_ptr<TransformComponent> cameraTransformComponent = cachedEntities.second->getComponent<TransformComponent>();

        if (!cameraComponent || !cameraTransformComponent) continue;

        Core::Main::CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);

        for (auto& sceneEntity: scene->m_entities)
        {
            std::shared_ptr<TransformComponent> transformComponent = sceneEntity->getComponent<TransformComponent>();

            if (!transformComponent) continue;

            std::list<std::shared_ptr<MeshComponent>> meshComponents = sceneEntity->getComponents<MeshComponent>();
            std::shared_ptr<SkyboxComponent> skyboxComponent = sceneEntity->getComponent<SkyboxComponent>();
            std::list<std::shared_ptr<IPrimitiveComponent>> primitiveComponents = sceneEntity->getComponents<IPrimitiveComponent>();

            for (const auto& meshComponent: meshComponents)
            {
                Core::Main::CoreMain::getRenderer().renderMesh(
                        transformComponent,
                        meshComponent
                );
            }

            for (const auto& primitiveComponent: primitiveComponents)
            {
                Core::Main::CoreMain::getRenderer().renderPrimitive(
                        transformComponent,
                        primitiveComponent
                );
            }
        }
    }

    double t1 = glfwGetTime();

    // 0.0016
    // 0,249000
    // 0,253200
    // 0,341100
    // 0,674100

    // std::cout << "ms for directional lights system: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::CameraRenderingSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) const
{
    ECSWorld::cacheComponents<CameraRenderingSystem, CameraComponent, TransformComponent>(entity);
}
