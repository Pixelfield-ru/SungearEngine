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
#include "MeshedEntitiesCollectorSystem.h"

void Core::ECS::CameraRenderingSystem::update(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    auto thisSystemCachedEntities = ECSWorld::getSystemCachedEntities<CameraRenderingSystem>();
    auto meshedCachedEntities = ECSWorld::getSystemCachedEntities<MeshedEntitiesCollectorSystem>();

    if(thisSystemCachedEntities == nullptr || meshedCachedEntities == nullptr) return;

    for (const auto& cachedEntities : thisSystemCachedEntities->m_cachedEntities)
    {
        if(cachedEntities.second == nullptr) continue;

        std::shared_ptr<CameraComponent> cameraComponent = cachedEntities.second->getComponent<CameraComponent>();
        std::shared_ptr<TransformComponent> cameraTransformComponent = cachedEntities.second->getComponent<TransformComponent>();

        if (!cameraComponent || !cameraTransformComponent) continue;

        Core::Main::CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);

        for(const auto& meshedEntity: meshedCachedEntities->m_cachedEntities)
        {
            if(meshedEntity.second == nullptr) continue;

            std::shared_ptr<TransformComponent> transformComponent = meshedEntity.second->getComponent<TransformComponent>();

            if (!transformComponent) continue;

            auto meshComponents =
                    meshedEntity.second->getComponents<MeshComponent>();
            auto primitiveComponents =
                    meshedEntity.second->getComponents<IPrimitiveComponent>();

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

    // last:
    // 5,30880
    // new:
    // 3,348900

    // std::cout << "ms for camera render system: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::CameraRenderingSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) const
{
    ECSWorld::cacheComponents<CameraRenderingSystem, CameraComponent, TransformComponent>(entity);
}
