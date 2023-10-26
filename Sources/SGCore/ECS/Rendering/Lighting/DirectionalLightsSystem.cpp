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

void Core::ECS::DirectionalLightsSystem::update(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    auto thisSystemCachedEntities = ECSWorld::getSystemCachedEntities<DirectionalLightsSystem>();
    auto meshedCachedEntities = ECSWorld::getSystemCachedEntities<MeshedEntitiesCollectorSystem>();

    if(!thisSystemCachedEntities || !meshedCachedEntities) return;

    size_t totalDirectionalLights = 0;

    for (const auto& directionalLightEntity : thisSystemCachedEntities->m_cachedEntities)
    {
        if(!directionalLightEntity.second) continue;

        auto directionalLightComponents =
                directionalLightEntity.second->getComponents<DirectionalLightComponent>();

        auto directionalLightTransform =
                directionalLightEntity.second->getComponent<TransformComponent>();

        if(!directionalLightTransform || directionalLightComponents.empty()) continue;

        for(const auto& directionalLightComponent: directionalLightComponents)
        {
            if(directionalLightComponent->m_color != directionalLightComponent->m_lastColor ||
            directionalLightComponent->m_intensity != directionalLightComponent->m_lastIntensity ||
            directionalLightTransform->m_positionChanged)
            {
                for(const auto& meshedEntity: meshedCachedEntities->m_cachedEntities)
                {
                    if(!meshedEntity.second) continue;

                    auto meshComponents =
                            meshedEntity.second->getComponents<MeshComponent>();

                    for(auto& meshComponent: meshComponents)
                    {
                        const auto& materialShader = meshComponent->m_mesh->m_material->getCurrentShader();

                        if(!materialShader) continue;

                        std::string directionalLightString = "directionalLights[" + std::to_string(totalDirectionalLights) + "]";

                        materialShader->bind();

                        materialShader->useVectorf(
                                directionalLightString + ".color",
                                directionalLightComponent->m_color
                        );

                        materialShader->useFloat(
                                directionalLightString + ".intensity",
                                directionalLightComponent->m_intensity
                        );

                        // todo: take into account the type of transformation and the direction of rotation
                        materialShader->useVectorf(
                                directionalLightString + ".position",
                                directionalLightTransform->m_position
                        );
                    }
                }

                directionalLightComponent->m_lastColor = directionalLightComponent->m_color;
                directionalLightComponent->m_lastIntensity = directionalLightComponent->m_intensity;
            }

            totalDirectionalLights++;
        }
    }

    double t1 = glfwGetTime();

    // 0.002500 ms average

    // std::cout << "ms for directional lights system: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::DirectionalLightsSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) const
{
    ECSWorld::cacheComponents<DirectionalLightsSystem, DirectionalLightComponent, TransformComponent>(entity);
}