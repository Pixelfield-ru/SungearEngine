//
// Created by stuka on 15.10.2023.
//

#include "DataTransferToShadersSystem.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightComponent.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsSystem.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterSystem.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterComponent.h"
#include "SGCore/ECS/Rendering/CameraRenderingSystem.h"
#include "SGCore/ECS/Rendering/CameraComponent.h"
#include "MeshComponent.h"
#include "SGCore/Utils/Timer.h"

void Core::ECS::DataTransferToShadersSystem::FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene,
                                                                    const std::shared_ptr<Core::ECS::Entity>& entity)
{
    double t0 = glfwGetTime();

    // THIS SYSTEM USES VARIOUS CACHED DATA (FOR EXAMPLE, DATA FROM ShadowsCasterSystem or DirectionalLightsSystem SYSTEMS)

    static const size_t directionalLightsSystemHashCode = typeid(DirectionalLightsSystem).hash_code();
    static const size_t shadowsCasterSystemHashCode = typeid(ShadowsCasterSystem).hash_code();
    static const size_t cameraRenderingSystemHashCode = typeid(CameraRenderingSystem).hash_code();

    static const size_t directionalLightComponentHashCode = typeid(DirectionalLightComponent).hash_code();
    static const size_t shadowsCasterComponentHashCode = typeid(ShadowsCasterComponent).hash_code();
    static const size_t cameraComponentHashCode = typeid(CameraComponent).hash_code();
    static const size_t transformComponentHashCode = typeid(TransformComponent).hash_code();

    auto meshComponents = entity->getComponents<MeshComponent>();

    if(meshComponents.empty()) return;

    size_t counter = 0;

    // DIRECTIONAL LIGHTS ---------------------------------------------------
    /*const auto& directionalLightsSystemComponents = ECSWorld::getSystemCachedComponents(directionalLightsSystemHashCode);

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

                std::string dirLightStr = "directionalLights[" + std::to_string(counter) + "]";

                materialShader->useVectorf(dirLightStr + ".color",
                        castedDirectionalLight->m_color
                );

                materialShader->useFloat(dirLightStr + ".intensity",
                        castedDirectionalLight->m_intensity
                );

                materialShader->useVectorf(dirLightStr + ".position",
                        castedTransformComponent->m_position
                );
            }

            counter++;
        }
    }

    // ----------------------------------------------
    // SHADOW CASTERS -------------------------------

    counter = 0;

    const auto& shadowCastersSystemComponents = ECSWorld::getSystemCachedComponents(shadowsCasterSystemHashCode);

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

                std::string counterString = std::to_string(counter);

                materialShader->useTexture(
                        "shadowsCastersShadowMaps[" + counterString + "]",
                        finalTextureBlock);

                // todo: maybe make uniform buffer for shadows casters
                materialShader->useMatrix(
                        "shadowsCasters[" + counterString + "].shadowsCasterSpace",
                        castedShadowsCaster->m_projectionMatrix * castedShadowsCaster->m_viewMatrix);

                materialShader->useVectorf(
                        "shadowsCasters[" + counterString + "].position",
                        castedTransformComponent->m_position);
            }

            counter++;
        }
    }*/

    std::shared_ptr<TransformComponent> entityTransform = entity->getComponent<TransformComponent>();
    if(!entityTransform) return; //!!!!!!!!!!!!!!!!!!!!!!!!!!!

    const auto& cameraRenderingSystemComponents = ECSWorld::getSystemCachedComponents(cameraRenderingSystemHashCode);

    for(auto& entities : cameraRenderingSystemComponents.m_entitiesComponents)
    {
        const auto& cameraComponent = entities.second.m_components.find(cameraComponentHashCode);
        const auto& transformComponent = entities.second.m_components.find(transformComponentHashCode);

        if(cameraComponent == entities.second.m_components.cend()) continue;
        if(transformComponent == entities.second.m_components.cend()) continue;

        auto castedTransformComponent = std::dynamic_pointer_cast<TransformComponent>(*transformComponent->second.begin());
        auto castedCameraComponent = std::dynamic_pointer_cast<CameraComponent>(*cameraComponent->second.begin());

        Core::Main::CoreMain::getRenderer().prepareUniformBuffers(castedCameraComponent, castedTransformComponent);

        std::list<std::shared_ptr<MeshComponent>> entityMeshComponents = entity->getComponents<MeshComponent>();
        std::list<std::shared_ptr<IPrimitiveComponent>> primitiveComponents = entity->getComponents<IPrimitiveComponent>();

        for(const auto& meshComponent : meshComponents)
        {
            Core::Main::CoreMain::getRenderer().renderMesh(
                    entityTransform,
                    meshComponent
            );
        }

        for(const auto& primitiveComponent : primitiveComponents)
        {
            Core::Main::CoreMain::getRenderer().renderPrimitive(
                    entityTransform,
                    primitiveComponent
            );
        }
    }

    /*std::shared_ptr<CameraComponent> cameraComponent = entity->getComponent<CameraComponent>();
    std::shared_ptr<TransformComponent> cameraTransformComponent = entity->getComponent<TransformComponent>();

    if(!cameraComponent || !cameraTransformComponent) return;

    Core::Main::CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);

    for(auto& sceneEntity : scene->m_entities)
    {
        std::shared_ptr<TransformComponent> transformComponent = sceneEntity->getComponent<TransformComponent>();

        if(!transformComponent) continue;

        std::list<std::shared_ptr<MeshComponent>> meshComponents = sceneEntity->getComponents<MeshComponent>();
        std::shared_ptr<SkyboxComponent> skyboxComponent = sceneEntity->getComponent<SkyboxComponent>();
        std::list<std::shared_ptr<IPrimitiveComponent>> primitiveComponents = sceneEntity->getComponents<IPrimitiveComponent>();

        for(const auto& meshComponent : meshComponents)
        {
            Core::Main::CoreMain::getRenderer().renderMesh(
                    transformComponent,
                    meshComponent
            );
        }

        for(const auto& primitiveComponent : primitiveComponents)
        {
            Core::Main::CoreMain::getRenderer().renderPrimitive(
                    transformComponent,
                    primitiveComponent
            );
        }
    }*/

    double t1 = glfwGetTime();

    // 0.020200 ms average

    //std::cout << "ms: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}
