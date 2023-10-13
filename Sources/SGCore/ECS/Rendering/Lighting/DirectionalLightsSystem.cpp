//
// Created by stuka on 01.08.2023.
//

#include "DirectionalLightsSystem.h"
#include "DirectionalLightComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"

void Core::ECS::DirectionalLightsSystem::FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene)
{
    size_t totalDirectionalLights = 0;

    for(const auto& directionalLightEntity: scene->m_entities)
    {
        std::list<std::shared_ptr<DirectionalLightComponent>> directionalLightComponents =
                directionalLightEntity->getComponents<DirectionalLightComponent>();

        std::shared_ptr<TransformComponent> transformComponent =
                directionalLightEntity->getComponent<TransformComponent>();

        if(!transformComponent) continue;

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
}
