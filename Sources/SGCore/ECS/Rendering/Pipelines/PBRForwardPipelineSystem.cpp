//
// Created by stuka on 04.07.2023.
//

#include "PBRForwardPipelineSystem.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/CameraComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Rendering/SkyboxComponent.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/MeshedEntitiesCollectorSystem.h"
#include "SGCore/ECS/Transformations/TransformationsSystem.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsSystem.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterSystem.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterComponent.h"

Core::ECS::PBRForwardPipelineSystem::PBRForwardPipelineSystem()
{
    m_geometryPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_geometryPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(SG_GLSL4_PBR_SHADER_PATH)
    );

    m_geometryPassMarkedShader->addBlockDeclaration(SGTextureType::SGTP_DIFFUSE,
                                                    1, 0);
    m_geometryPassMarkedShader->addBlockDeclaration(SGTextureType::SGTP_DIFFUSE_ROUGHNESS,
                                                    1, 1);
    m_geometryPassMarkedShader->addBlockDeclaration(SGTextureType::SGTP_NORMALS,
                                                    1, 2);
    m_geometryPassMarkedShader->addBlockDeclaration(SGTextureType::SGTP_BASE_COLOR,
                                                    1, 3);
    m_geometryPassMarkedShader->addBlockDeclaration(SGTextureType::SGTP_SHADOW_MAP,
                                                    5, 4);

    m_shadowsPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_shadowsPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(SG_GLSL4_SHADOWS_GENERATOR_SHADER_PATH)
    );

    m_shadowsPassMarkedShader->addBlockDeclaration(SGTextureType::SGTP_DIFFUSE,
                                                    1, 0);
}

void Core::ECS::PBRForwardPipelineSystem::update(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    auto transformationsSystem = Patterns::Singleton::getInstance<TransformationsSystem>();

    const auto& meshedCachedEntities = Patterns::Singleton::getInstance<MeshedEntitiesCollectorSystem>()->getCachedEntities();
    const auto& primitivesCachedEntities = Patterns::Singleton::getInstance<PrimitivesUpdaterSystem>()->getCachedEntities();

    const auto& directionalLightsCachedEntities = Patterns::Singleton::getInstance<DirectionalLightsSystem>()->getCachedEntities();
    const auto& shadowsCastersCachedEntities = Patterns::Singleton::getInstance<ShadowsCasterSystem>()->getCachedEntities();

    if(meshedCachedEntities.empty() && primitivesCachedEntities.empty()) return;

    // binding geom pass shader
    m_geometryPassMarkedShader->bind();
    m_geometryPassMarkedShader->m_shader->useUniformBuffer(Core::Main::CoreMain::getRenderer().m_programDataBuffer);

    // ------ updating all lights in geom shader -------------

    size_t directionalLightsCount = 0;

    for(const auto& directionalLightsLayer : directionalLightsCachedEntities)
    {
        for(const auto& directionalLightEntity : directionalLightsLayer.second)
        {
            if (!directionalLightEntity.second) continue;

            auto directionalLightComponents =
                    directionalLightEntity.second->getComponents<DirectionalLightComponent>();

            auto directionalLightTransform =
                    directionalLightEntity.second->getComponent<TransformComponent>();

            if(!directionalLightTransform) continue;

            for(const auto& directionalLightComponent: directionalLightComponents)
            {
                std::string directionalLightString =
                        "directionalLights[" + std::to_string(directionalLightsCount) + "]";;

                m_geometryPassMarkedShader->m_shader->useVectorf(
                        directionalLightString + ".color",
                        directionalLightComponent->m_color
                );

                m_geometryPassMarkedShader->m_shader->useFloat(
                        directionalLightString + ".intensity",
                        directionalLightComponent->m_intensity
                );

                // todo: take into account the type of transformation and the direction of rotation
                m_geometryPassMarkedShader->m_shader->useVectorf(
                        directionalLightString + ".position",
                        directionalLightTransform->m_position
                );

                directionalLightsCount++;
            }
        }
    }

    // todo: make name as define
    m_geometryPassMarkedShader->m_shader->useInteger("DIRECTIONAL_LIGHTS_COUNT", directionalLightsCount);

    // -----------------------------------------

    // ----- updating shadows casters in geom shader and render shadows pass -----

    size_t shadowsCastersCount = 0;

    for(const auto& shadowsCastersLayer : shadowsCastersCachedEntities)
    {
        for (const auto& cachedEntity : shadowsCastersLayer.second)
        {
            if (!cachedEntity.second) continue;

            // todo: make process all ShadowsCasterComponent (cachedEntities.second->getComponents)
            std::shared_ptr<ShadowsCasterComponent> shadowsCasterComponent = cachedEntity.second->getComponent<ShadowsCasterComponent>();
            std::shared_ptr<TransformComponent> shadowsCasterTransform = cachedEntity.second->getComponent<TransformComponent>();

            if (!shadowsCasterTransform || !shadowsCasterComponent) continue;

            shadowsCasterComponent->m_frameBuffer->bind()->clear();

            std::uint8_t shadowMapsBlockOffset = m_geometryPassMarkedShader->
                    getBlocks()[SGTextureType::SGTP_SHADOW_MAP].m_texturesUnitOffset;

            std::uint8_t finalTextureBlock = shadowMapsBlockOffset + shadowsCastersCount;

            shadowsCasterComponent->m_frameBuffer->bindAttachment(
                    SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT,
                    finalTextureBlock
            );

            std::string totalShadowsCastersStr = std::to_string(shadowsCastersCount);

            m_geometryPassMarkedShader->m_shader->bind();

            m_geometryPassMarkedShader->m_shader->useTexture(
                    "shadowsCastersShadowMaps[" + totalShadowsCastersStr + "]",
                    finalTextureBlock
            );

            // todo: maybe make uniform buffer for shadows casters
            m_geometryPassMarkedShader->m_shader->useMatrix(
                    "shadowsCasters[" + totalShadowsCastersStr + "].shadowsCasterSpace",
                    shadowsCasterComponent->m_spaceMatrix
            );

            m_geometryPassMarkedShader->m_shader->useVectorf(
                    "shadowsCasters[" + totalShadowsCastersStr + "].position",
                    shadowsCasterTransform->m_position
            );

            m_shadowsPassMarkedShader->m_shader->bind();

            for (const auto& meshesLayer: meshedCachedEntities)
            {
                for (const auto& meshesEntity: meshesLayer.second)
                {
                    if (!meshesEntity.second) continue;

                    std::shared_ptr<TransformComponent> transformComponent = meshesEntity.second->getComponent<TransformComponent>();

                    if (!transformComponent) continue;

                    auto meshComponents =
                            meshesEntity.second->getComponents<MeshComponent>();

                    for (const auto& meshComponent: meshComponents)
                    {
                        meshComponent->m_mesh->m_material->bind(m_shadowsPassMarkedShader);
                        updateUniforms(m_shadowsPassMarkedShader->m_shader,
                                       meshComponent->m_mesh->m_material,
                                       transformComponent);

                        Core::Main::CoreMain::getRenderer().renderMesh(
                                transformComponent,
                                meshComponent
                        );
                    }
                }
            }

            shadowsCasterComponent->m_frameBuffer->unbind();

            shadowsCastersCount++;
        }
    }

    // m_geometryPassMarkedShader->bind();
    // todo: make name as define
    m_geometryPassMarkedShader->m_shader->useInteger("SHADOWS_CASTERS_COUNT", shadowsCastersCount);

    // ---------------------------------------------------

    // ----- render meshes and primitives ------

    for (const auto& camerasLayer: m_cachedEntities)
    {
        for (const auto& cachedEntity: camerasLayer.second)
        {
            if (!cachedEntity.second) continue;

            std::shared_ptr<CameraComponent> cameraComponent = cachedEntity.second->getComponent<CameraComponent>();
            std::shared_ptr<TransformComponent> cameraTransformComponent = cachedEntity.second->getComponent<TransformComponent>();

            if (!cameraComponent || !cameraTransformComponent) continue;

            Core::Main::CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);
            m_geometryPassMarkedShader->m_shader->useUniformBuffer(Core::Main::CoreMain::getRenderer().m_viewMatricesBuffer);

            for (const auto& meshesLayer: meshedCachedEntities)
            {
                for (const auto& meshesEntity: meshesLayer.second)
                {
                    if (!meshesEntity.second) continue;

                    std::shared_ptr<TransformComponent> transformComponent = meshesEntity.second->getComponent<TransformComponent>();

                    if (!transformComponent) continue;

                    auto meshComponents =
                            meshesEntity.second->getComponents<MeshComponent>();

                    for (const auto& meshComponent: meshComponents)
                    {
                        meshComponent->m_mesh->m_material->bind(m_geometryPassMarkedShader);
                        updateUniforms(m_geometryPassMarkedShader->m_shader,
                                       meshComponent->m_mesh->m_material,
                                       transformComponent);

                        Core::Main::CoreMain::getRenderer().renderMesh(
                                transformComponent,
                                meshComponent
                        );
                    }
                }
            }
        }
    }

    // --------------------------------

    double t1 = glfwGetTime();

    // last:
    // 5,30880
    // new:
    // 3,348900

    // std::cout << "ms for camera render system: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::PBRForwardPipelineSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity)
{
    cacheEntityComponents<CameraComponent, TransformComponent>(entity);
}

void Core::ECS::PBRForwardPipelineSystem::updateUniforms(const std::shared_ptr<Graphics::IShader>& shader,
                                                         const std::shared_ptr<Memory::Assets::IMaterial>& material,
                                                         const std::shared_ptr<TransformComponent>& transformComponent) const noexcept
{
    shader->bind();

    shader->useMatrix("objectModelMatrix",
                      transformComponent->m_modelMatrix
    );
    shader->useVectorf("objectPosition",
                       transformComponent->m_position
    );
    shader->useVectorf("objectRotation",
                       transformComponent->m_rotation
    );
    shader->useVectorf("objectScale",
                       transformComponent->m_scale
    );

    // TODO: MOVE MATERIAL UPDATE TO OTHER SYSTEM
    shader->useVectorf("materialDiffuseCol",
                       material->m_diffuseColor
    );
    shader->useVectorf("materialSpecularCol",
                       material->m_specularColor
    );
    shader->useVectorf("materialAmbientCol",
                       material->m_ambientColor
    );
    shader->useVectorf("materialEmissionCol",
                       material->m_emissionColor
    );
    shader->useVectorf("materialTransparentCol",
                       material->m_transparentColor
    );
    shader->useFloat("materialShininess",
                     material->m_shininess
    );
    shader->useFloat("materialMetallicFactor",
                     material->m_metallicFactor
    );
    shader->useFloat("materialRoughnessFactor",
                     material->m_roughnessFactor
    );
}
