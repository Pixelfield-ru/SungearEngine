//
// Created by stuka on 15.10.2023.
//

#include "DataTransferToShadersSystem.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightComponent.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsSystem.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterSystem.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterComponent.h"
#include "MeshComponent.h"
#include "SGCore/Utils/Timer.h"

void Core::ECS::DataTransferToShadersSystem::FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene,
                                                                    const std::shared_ptr<Core::ECS::Entity>& entity)
{
    double t0 = glfwGetTime();

    // THIS SYSTEM USES VARIOUS CACHED DATA (FOR EXAMPLE, DATA FROM ShadowsCasterSystem or DirectionalLightsSystem SYSTEMS)

    static const size_t directionalLightsSystemHashCode = typeid(DirectionalLightsSystem).hash_code();
    static const size_t shadowsCasterSystemHashCode = typeid(ShadowsCasterSystem).hash_code();

    static const size_t directionalLightComponentHashCode = typeid(DirectionalLightComponent).hash_code();
    static const size_t shadowsCasterComponentHashCode = typeid(ShadowsCasterComponent).hash_code();
    static const size_t transformComponentHashCode = typeid(TransformComponent).hash_code();

    auto meshComponents = entity->getComponents<MeshComponent>();

    if(meshComponents.empty()) return;

    size_t counter = 0;

    // DIRECTIONAL LIGHTS ---------------------------------------------------

    // TODO: BOTTLE NECK
    // WITH THIS: 0.676700 MS AVERAGE FOR FPSRelativeFixedUpdate SYSTEMS
    // WITHOUT THIS: 0.645000 MS AVERAGE FOR FPSRelativeFixedUpdate SYSTEMS
    auto directionalLightsSystemComponents = ECSWorld::getSystemCachedComponents(directionalLightsSystemHashCode);

    //std::cout << "size: " << std::to_string(ECSWorld::m_cachedComponents.size()) << std::endl;

    for(auto& entities : directionalLightsSystemComponents.m_entitiesComponents)
    {
        const auto& directionalLightComponents = entities.second.m_components.find(directionalLightComponentHashCode);
        const auto& transformComponent = entities.second.m_components.find(transformComponentHashCode);

        if(directionalLightComponents == entities.second.m_components.cend()) continue;
        if(transformComponent == entities.second.m_components.cend()) continue;

        auto* castedTransformComponent = dynamic_cast<TransformComponent*>(transformComponent->second.begin()->get());

        for(const auto& directionalLight: directionalLightComponents->second)
        {
            auto* castedDirectionalLight = dynamic_cast<DirectionalLightComponent*>(directionalLight.get());

            for(const auto& meshComponent: meshComponents)
            {
                const auto& materialShader = meshComponent->m_mesh->m_material->getCurrentShader();
                if (!materialShader) continue;

                materialShader->bind();

                materialShader->useVectorf(
                        "directionalLights[" +
                        std::to_string(counter) + "].color",
                        castedDirectionalLight->m_color
                );

                materialShader->useFloat(
                        "directionalLights[" +
                        std::to_string(counter) + "].intensity",
                        castedDirectionalLight->m_intensity
                );

                materialShader->useVectorf(
                        "directionalLights[" +
                        std::to_string(counter) + "].position",
                        castedTransformComponent->m_position
                );
            }

            counter++;
        }
    }

    // ----------------------------------------------
    // SHADOW CASTERS -------------------------------

    counter = 0;

    auto shadowCastersSystemComponents = ECSWorld::getSystemCachedComponents(shadowsCasterSystemHashCode);

    for(auto& entities : shadowCastersSystemComponents.m_entitiesComponents)
    {
        const auto& shadowsCasterComponents = entities.second.m_components.find(shadowsCasterComponentHashCode);
        const auto& transformComponent = entities.second.m_components.find(transformComponentHashCode);

        if(shadowsCasterComponents == entities.second.m_components.cend()) continue;
        if(transformComponent == entities.second.m_components.cend()) continue;

        auto* castedTransformComponent = dynamic_cast<TransformComponent*>(transformComponent->second.begin()->get());

        for(const auto& shadowsCaster: shadowsCasterComponents->second)
        {
            auto* castedShadowsCaster = dynamic_cast<ShadowsCasterComponent*>(shadowsCaster.get());

            for(const auto& meshComponent: meshComponents)
            {
                const auto& materialShader = meshComponent->m_mesh->m_material->getCurrentShader();
                if (!materialShader) continue;

                std::uint8_t shadowMapsBlockOffset = meshComponent->
                        m_mesh->
                        m_material->getBlocks()[SGMaterialTextureType::SGTP_SHADOW_MAP].m_texturesUnitOffset;

                std::uint8_t finalTextureBlock = shadowMapsBlockOffset + counter;

                castedShadowsCaster->m_frameBuffer->bindAttachment(
                        SG_FRAMEBUFFER_DEPTH_ATTACHMENT_NAME,
                        finalTextureBlock
                );

                materialShader->bind();

                materialShader->useTexture(
                        "shadowsCastersShadowMaps[" + std::to_string(counter) + "]",
                        finalTextureBlock);

                // todo: maybe make uniform buffer for shadows casters
                materialShader->useMatrix(
                        "shadowsCasters[" + std::to_string(counter) + "].shadowsCasterSpace",
                        castedShadowsCaster->m_projectionMatrix * castedShadowsCaster->m_viewMatrix);

                materialShader->useVectorf(
                        "shadowsCasters[" + std::to_string(counter) + "].position",
                        castedTransformComponent->m_position);
            }

            counter++;
        }
    }

    double t1 = glfwGetTime();

    // 0.020200 ms average

    //std::cout << "ms: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}
