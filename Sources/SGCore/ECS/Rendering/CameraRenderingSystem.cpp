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

    const auto& meshedCachedEntities = Patterns::Singleton::getInstance<MeshedEntitiesCollectorSystem>()->getCachedEntities();
    const auto& primitivesCachedEntities = Patterns::Singleton::getInstance<PrimitivesUpdaterSystem>()->getCachedEntities();

    if(meshedCachedEntities.empty() && primitivesCachedEntities.empty()) return;

    for(const auto& camerasLayer : m_cachedEntities)
    {
        for(const auto& cachedEntity: camerasLayer.second)
        {
            if(!cachedEntity.second) continue;

            std::shared_ptr<CameraComponent> cameraComponent = cachedEntity.second->getComponent<CameraComponent>();
            std::shared_ptr<TransformComponent> cameraTransformComponent = cachedEntity.second->getComponent<TransformComponent>();

            if(!cameraComponent || !cameraTransformComponent) continue;

            Core::Main::CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);

            for(const auto& meshesLayer : meshedCachedEntities)
            {
                // if layer is layer of transparent objects than sort

                // TODO: MAKE DEPTH MASK FOR RENDERER TO RENDER TRANSPARENT OBJECTS
                // todo: make normal render for transparent objects
                if(!meshesLayer.first->m_isOpaque)
                {
                    meshesLayer.first->m_entities.sort([&cameraTransformComponent](const std::shared_ptr<Entity>& e0, const std::shared_ptr<Entity>& e1)
                                                       {
                                                           auto t0 = e0->getComponent<TransformComponent>();
                                                           auto t1 = e1->getComponent<TransformComponent>();

                                                           float d0 = glm::distance(cameraTransformComponent->m_position, t0->m_position);
                                                           float d1 = glm::distance(cameraTransformComponent->m_position, t1->m_position);

                                                           return d0 > d1;
                                                       });
                }

                // -------------------------------------------------

                for(auto& renderPass : cameraComponent->m_renderPasses)
                {
                    Graphics::RenderOutput& currentRenderOutput = renderPass.m_defaultRenderOutput;

                    const auto& foundPostProcessLayer = renderPass.m_postProcessLayers.find(meshesLayer.first);
                    if(foundPostProcessLayer != renderPass.m_postProcessLayers.cend())
                    {
                        currentRenderOutput = foundPostProcessLayer->second;
                    }

                    currentRenderOutput.m_frameBuffer->bind()->clear();

                    for(const auto& meshesEntity: meshesLayer.second)
                    {
                        if(!meshesEntity.second) continue;

                        std::shared_ptr<TransformComponent> transformComponent = meshesEntity.second->getComponent<TransformComponent>();

                        if(!transformComponent) continue;

                        auto meshComponents =
                                meshesEntity.second->getComponents<MeshComponent>();

                        for(const auto& meshComponent: meshComponents)
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

                    currentRenderOutput.m_frameBuffer->unbind();

                    // now render RenderOutput
                    Core::Main::CoreMain::getRenderer().renderRenderOutput(
                            currentRenderOutput
                    );
                }
            }

            for(const auto& primitivesLayer : primitivesCachedEntities)
            {
                for(const auto& primitiveEntity: primitivesLayer.second)
                {
                    if(!primitiveEntity.second) continue;

                    std::shared_ptr<TransformComponent> transformComponent = primitiveEntity.second->getComponent<TransformComponent>();

                    if(!transformComponent) continue;

                    auto primitiveComponents =
                            primitiveEntity.second->getComponents<IPrimitiveComponent>();

                    for(const auto& primitiveComponent: primitiveComponents)
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
        }
    }

    double t1 = glfwGetTime();

    // last:
    // 5,30880
    // new:
    // 3,348900

    // std::cout << "ms for camera render system: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::CameraRenderingSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity)
{
    cacheEntityComponents<CameraComponent, TransformComponent>(entity);
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
