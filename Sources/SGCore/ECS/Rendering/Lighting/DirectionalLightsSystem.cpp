//
// Created by stuka on 01.08.2023.
//

#include "DirectionalLightsSystem.h"
#include "DirectionalLightComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/ECSWorld.h"
#include "GLFW/glfw3.h"

void Core::ECS::DirectionalLightsSystem::FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    auto systemCachedComponents = ECSWorld::getSystemCachedEntities<DirectionalLightsSystem>();

    if(systemCachedComponents == nullptr) return;

    size_t totalDirectionalLights = 0;

    for (const auto& cachedEntities : systemCachedComponents->m_cachedEntities)
    {
        if(cachedEntities.second == nullptr) continue;

        std::list<std::shared_ptr<DirectionalLightComponent>> directionalLightComponents =
                cachedEntities.second->getComponents<DirectionalLightComponent>();

        std::shared_ptr<TransformComponent> transformComponent =
                cachedEntities.second->getComponent<TransformComponent>();

        if(!transformComponent || directionalLightComponents.empty()) continue;

        for(const auto& directionalLightComponent : directionalLightComponents)
        {
            for(const auto& entity: scene->m_entities)
            {
                auto meshComponents = entity->getComponents<MeshComponent>();

                for(auto& meshComponent: meshComponents)
                {
                    const auto& materialShader = meshComponent->m_mesh->m_material->getCurrentShader();

                    if (!materialShader) continue;

                    materialShader->bind();

                    materialShader->useVectorf(
                            "directionalLights[" +
                            std::to_string(totalDirectionalLights) + "].color",
                            directionalLightComponent->m_color);

                    materialShader->useFloat(
                            "directionalLights[" +
                            std::to_string(totalDirectionalLights) + "].intensity",
                            directionalLightComponent->m_intensity);

                    // todo: take into account the type of transformation and the direction of rotation
                    materialShader->useVectorf(
                            "directionalLights[" +
                            std::to_string(totalDirectionalLights) + "].position",
                            transformComponent->m_position);
                }
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