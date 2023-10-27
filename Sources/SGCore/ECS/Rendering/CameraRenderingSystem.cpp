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
#include "SGCore/ECS/Transformations/TransformationsSystem.h"

void Core::ECS::CameraRenderingSystem::update(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    auto transformationsSystem = Patterns::Singleton::getInstance<TransformationsSystem>();

    auto thisSystemCachedEntities = ECSWorld::getSystemCachedEntities<CameraRenderingSystem>();
    auto meshedCachedEntities = ECSWorld::getSystemCachedEntities<MeshedEntitiesCollectorSystem>();
    auto primitivesCachedEntities = ECSWorld::getSystemCachedEntities<PrimitivesUpdaterSystem>();

    if(!thisSystemCachedEntities || (!meshedCachedEntities && !primitivesCachedEntities)) return;

    size_t curFunc = 0;

    for (const auto& cachedEntities : thisSystemCachedEntities->m_cachedEntities)
    {
        if(!cachedEntities.second) continue;

        std::shared_ptr<CameraComponent> cameraComponent = cachedEntities.second->getComponent<CameraComponent>();
        std::shared_ptr<TransformComponent> cameraTransformComponent = cachedEntities.second->getComponent<TransformComponent>();

        if (!cameraComponent || !cameraTransformComponent) continue;

        Core::Main::CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);

        for(const auto& meshedEntity: meshedCachedEntities->m_cachedEntities)
        {
            if(!meshedEntity.second) continue;

            std::shared_ptr<TransformComponent> transformComponent = meshedEntity.second->getComponent<TransformComponent>();

            if (!transformComponent) continue;

            auto meshComponents =
                    meshedEntity.second->getComponents<MeshComponent>();

            for (const auto& meshComponent : meshComponents)
            {
                transformationsSystem->addFunctionToFixedUpdateQuery(
                        meshComponent->getUUID(),
                        uniformsUpdaterLambda,
                        meshComponent->m_mesh, transformComponent
                );

                Core::Main::CoreMain::getRenderer().renderMesh(
                        transformComponent,
                        meshComponent
                );
            }
        }

        for(const auto& primitiveEntity : primitivesCachedEntities->m_cachedEntities)
        {
            if(!primitiveEntity.second) continue;

            std::shared_ptr<TransformComponent> transformComponent = primitiveEntity.second->getComponent<TransformComponent>();

            if (!transformComponent) continue;

            auto primitiveComponents =
                    primitiveEntity.second->getComponents<IPrimitiveComponent>();

            for (const auto& primitiveComponent : primitiveComponents)
            {
                transformationsSystem->addFunctionToFixedUpdateQuery(
                        primitiveComponent->getUUID(),
                        uniformsUpdaterLambda,
                        primitiveComponent->m_mesh, transformComponent
                );

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

void Core::ECS::CameraRenderingSystem::updateMeshUniforms(const std::shared_ptr<ImportedScene::IMesh>& mesh,
                                                          const std::shared_ptr<TransformComponent>& transformComponent) const noexcept
{
    for(const auto& shader : mesh->m_material->getShaders())
    {
        shader.second->bind();

        shader.second->useMatrix("objectModelMatrix",
                                 transformComponent->m_modelMatrix
        );
        shader.second->useVectorf("objectPosition",
                                  transformComponent->m_position
        );
        shader.second->useVectorf("objectRotation",
                                  transformComponent->m_rotation
        );
        shader.second->useVectorf("objectScale",
                                  transformComponent->m_scale
        );

        // TODO: MOVE MATERIAL UPDATE TO OTHER SYSTEM
        shader.second->useVectorf("materialDiffuseCol",
                                  mesh->m_material->m_diffuseColor
        );
        shader.second->useVectorf("materialSpecularCol",
                                  mesh->m_material->m_specularColor
        );
        shader.second->useVectorf("materialAmbientCol",
                                  mesh->m_material->m_ambientColor
        );
        shader.second->useVectorf("materialEmissionCol",
                                  mesh->m_material->m_emissionColor
        );
        shader.second->useVectorf("materialTransparentCol",
                                  mesh->m_material->m_transparentColor
        );
        shader.second->useFloat("materialShininess",
                                mesh->m_material->m_shininess
        );
        shader.second->useFloat("materialMetallicFactor",
                                mesh->m_material->m_metallicFactor
        );
        shader.second->useFloat("materialRoughnessFactor",
                                mesh->m_material->m_roughnessFactor
        );
    }
}
