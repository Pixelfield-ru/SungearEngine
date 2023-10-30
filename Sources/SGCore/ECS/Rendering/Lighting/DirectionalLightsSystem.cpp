//
// Created by stuka on 01.08.2023.
//

#include "DirectionalLightsSystem.h"
#include "DirectionalLightComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/MeshedEntitiesCollectorSystem.h"
#include "GLFW/glfw3.h"
#include "SGCore/ECS/Transformations/TransformationsSystem.h"

void Core::ECS::DirectionalLightsSystem::fixedUpdate(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    auto transformationsSystem = Patterns::Singleton::getInstance<TransformationsSystem>();

    const auto& meshedCachedEntities = Patterns::Singleton::getInstance<MeshedEntitiesCollectorSystem>()->getCachedEntities();

    if(meshedCachedEntities.empty()) return;

    size_t currentDirectionalLight = 0;

    for(const auto& layer0 : m_cachedEntities)
    {
        for(const auto& directionalLightEntity: layer0.second)
        {
            if(!directionalLightEntity.second) continue;

            auto directionalLightComponents =
                    directionalLightEntity.second->getComponents<DirectionalLightComponent>();

            auto directionalLightTransform =
                    directionalLightEntity.second->getComponent<TransformComponent>();

            if(!directionalLightTransform || directionalLightComponents.empty()) continue;

            for(const auto& directionalLightComponent: directionalLightComponents)
            {
                bool colorChanged = directionalLightComponent->m_color != directionalLightComponent->m_lastColor;
                bool intensityChanged =
                        directionalLightComponent->m_intensity != directionalLightComponent->m_lastIntensity;
                bool posChanged = directionalLightTransform->m_positionChanged;

                if(colorChanged || intensityChanged || posChanged)
                {
                    for(const auto& layer1 : meshedCachedEntities)
                    {
                        for(const auto& meshedEntity: layer1.second)
                        {
                            if(!meshedEntity.second) continue;

                            auto meshComponents =
                                    meshedEntity.second->getComponents<MeshComponent>();

                            for(auto& meshComponent: meshComponents)
                            {
                                const auto& materialShader = meshComponent->m_mesh->m_material->getCurrentShader();

                                if(!materialShader) continue;

                                std::string directionalLightString =
                                        "directionalLights[" + std::to_string(currentDirectionalLight) + "]";

                                materialShader->bind();

                                if(colorChanged)
                                {
                                    materialShader->useVectorf(
                                            directionalLightString + ".color",
                                            directionalLightComponent->m_color
                                    );
                                }

                                if(intensityChanged)
                                {
                                    materialShader->useFloat(
                                            directionalLightString + ".intensity",
                                            directionalLightComponent->m_intensity
                                    );
                                }

                                if(posChanged)
                                {
                                    // todo: take into account the type of transformation and the direction of rotation
                                    materialShader->useVectorf(
                                            directionalLightString + ".position",
                                            directionalLightTransform->m_position
                                    );
                                }
                            }
                        }
                    }

                    directionalLightComponent->m_lastColor = directionalLightComponent->m_color;
                    directionalLightComponent->m_lastIntensity = directionalLightComponent->m_intensity;
                }

                currentDirectionalLight++;
            }
        }
    }

    double t1 = glfwGetTime();

    // 0.002500 ms average

    // std::cout << "ms for directional lights system: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::DirectionalLightsSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity)
{
    cacheEntityComponents<DirectionalLightComponent, TransformComponent>(entity);
}