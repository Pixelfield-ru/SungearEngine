//
// Created by stuka on 27.07.2023.
//

#include "ShadowsCasterSystem.h"
#include "ShadowsCasterComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "MeshComponent.h"
#include "SkyboxComponent.h"

void Core::ECS::ShadowsCasterSystem::update(const std::shared_ptr<Scene>& scene)
{
    size_t totalShadowCasters = 0;

    for(const auto& shadowsCasterEntity: scene->m_entities)
    {
        std::list<std::shared_ptr<ShadowsCasterComponent>> shadowsCasterComponents =
                shadowsCasterEntity->getComponents<ShadowsCasterComponent>();
        std::shared_ptr<TransformComponent> shadowCasterTransform =
                shadowsCasterEntity->getComponent<TransformComponent>();

        if(!shadowCasterTransform) continue;

        for(const auto& shadowsCasterComponent : shadowsCasterComponents)
        {
            for(const auto& entity: scene->m_entities)
            {
                auto meshComponents = entity->getComponents<MeshComponent>();

                for(auto& meshComponent: meshComponents)
                {
                    std::uint8_t shadowMapsBlockOffset = meshComponent->
                            m_mesh->
                            m_material->getBlocks()[SGMaterialTextureType::SGTP_SHADOW_MAP].m_texturesUnitOffset;

                    std::uint8_t finalTextureBlock = shadowMapsBlockOffset + totalShadowCasters;

                    shadowsCasterComponent->m_frameBuffer->bindAttachment(
                            SG_FRAMEBUFFER_DEPTH_ATTACHMENT_NAME,
                            finalTextureBlock
                            );

                    meshComponent->m_mesh->m_material->getShader()->bind();

                    meshComponent->m_mesh->m_material->getShader()->useTexture(
                            "shadowsCastersShadowMaps[" + std::to_string(totalShadowCasters) + "]",
                            finalTextureBlock);

                    // todo: maybe make uniform buffer for shadows casters
                    meshComponent->m_mesh->m_material->getShader()->useMatrix(
                            "shadowsCasters[" + std::to_string(totalShadowCasters) + "].shadowsCasterSpace",
                            shadowsCasterComponent->m_projectionMatrix * shadowsCasterComponent->m_viewMatrix);

                    meshComponent->m_mesh->m_material->getShader()->useVectorf(
                            "shadowsCasters[" + std::to_string(totalShadowCasters) + "].position",
                            shadowCasterTransform->m_position);
                }
            }

            totalShadowCasters++;
        }
    }
}

void Core::ECS::ShadowsCasterSystem::update(const std::shared_ptr<Scene>& scene,
                                            const std::shared_ptr<Core::ECS::Entity>& entity)
{
    // todo: make for all shadows casters
    std::shared_ptr<ShadowsCasterComponent> shadowsCasterComponent = entity->getComponent<ShadowsCasterComponent>();

    if(!shadowsCasterComponent) return;

    shadowsCasterComponent->m_frameBuffer->bind()->clear()->unbind();

    for(auto& sceneEntity : scene->m_entities)
    {
        auto transformComponent = sceneEntity->getComponent<TransformComponent>();
        auto meshComponent = sceneEntity->getComponent<MeshComponent>();

        if(!transformComponent || !meshComponent || sceneEntity->getComponent<SkyboxComponent>()) continue;

        // TODO: make rendering
        Core::Main::CoreMain::getRenderer().renderMesh(shadowsCasterComponent, transformComponent, meshComponent);
    }
}

void Core::ECS::ShadowsCasterSystem::deltaUpdate(const std::shared_ptr<Scene>& scene,
                                                 const std::shared_ptr<Core::ECS::Entity>& entity,
                                                 const double& deltaTime)
{

}
