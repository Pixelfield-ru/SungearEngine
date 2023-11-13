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
#include "SGCore/ECS/Rendering/SkyboxesCollectorSystem.h"
#include "SGCore/ECS/Rendering/Primitives/LinesCollectorSystem.h"
#include "SGCore/ECS/Rendering/Primitives/ComplexPrimitivesCollectorSystem.h"
#include "SGCore/ECS/Rendering/Primitives/LineComponent.h"
#include "SGCore/ECS/Rendering/Primitives/ComplexPrimitiveComponent.h"

Core::ECS::PBRForwardPipelineSystem::PBRForwardPipelineSystem()
{
    m_geometryPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_geometryPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(
                    Graphics::getShaderPath(Graphics::StandardShaderType::SG_PBR_SHADER))
    );

    m_geometryPassMarkedShader->addTexturesBlockDeclaration(
            SGTextureType::SGTP_DIFFUSE,
            sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE),
            1
    );
    m_geometryPassMarkedShader->addTexturesBlockDeclaration(
            SGTextureType::SGTP_DIFFUSE_ROUGHNESS,
            sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE_ROUGHNESS),
            1
    );
    m_geometryPassMarkedShader->addTexturesBlockDeclaration(
            SGTextureType::SGTP_NORMALS,
            sgStandardTextureTypeToString(SGTextureType::SGTP_NORMALS),
            1
    );
    m_geometryPassMarkedShader->addTexturesBlockDeclaration(
            SGTextureType::SGTP_BASE_COLOR,
            sgStandardTextureTypeToString(SGTextureType::SGTP_BASE_COLOR),
            1
    );
    m_geometryPassMarkedShader->addTexturesBlockDeclaration(
            SGTextureType::SGTP_SHADOW_MAP,
            sgStandardTextureTypeToString(SGTextureType::SGTP_SHADOW_MAP),
            5,
            false
    );

    m_geometryPassMarkedShader->calculateBlocksOffsets();

    // ---------------------

    m_shadowsPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_shadowsPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(
                    Graphics::getShaderPath(Graphics::StandardShaderType::SG_SHADOWS_GENERATOR_SHADER))
    );

    m_shadowsPassMarkedShader->addTexturesBlockDeclaration(
            SGTextureType::SGTP_DIFFUSE,
            sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE),
            1
    );

    m_shadowsPassMarkedShader->calculateBlocksOffsets();

    // ---------------------

    m_skyboxPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_skyboxPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(Graphics::getShaderPath(
                    Graphics::StandardShaderType::SG_SKYBOX_SHADER)
            )
    );

    m_skyboxPassMarkedShader->addTexturesBlockDeclaration(
            SGTextureType::SGTP_SKYBOX,
            sgStandardTextureTypeToString(SGTextureType::SGTP_SKYBOX),
            1
    );

    m_skyboxPassMarkedShader->calculateBlocksOffsets();

    // ---------------------

    m_linesPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_linesPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(
                    Graphics::getShaderPath(Graphics::StandardShaderType::SG_LINES_SHADER))
    );

    // ---------------------

    m_complexPrimitivesPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_complexPrimitivesPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(
                    Graphics::getShaderPath(Graphics::StandardShaderType::SG_COMPLEX_PRIMITIVES_SHADER))
    );
}

void Core::ECS::PBRForwardPipelineSystem::update(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    auto transformationsSystem = Patterns::Singleton::getInstance<TransformationsSystem>();

    const auto& meshedCachedEntities = Patterns::Singleton::getInstance<MeshedEntitiesCollectorSystem>()->getCachedEntities();
    const auto& primitivesCachedEntities = Patterns::Singleton::getInstance<PrimitivesUpdaterSystem>()->getCachedEntities();

    const auto& directionalLightsCachedEntities = Patterns::Singleton::getInstance<DirectionalLightsSystem>()->getCachedEntities();
    const auto& shadowsCastersCachedEntities = Patterns::Singleton::getInstance<ShadowsCasterSystem>()->getCachedEntities();

    const auto& skyboxesCachedEntities = Patterns::Singleton::getInstance<SkyboxesCollectorSystem>()->getCachedEntities();

    const auto& linesCachedEntities = Patterns::Singleton::getInstance<LinesCollectorSystem>()->getCachedEntities();
    const auto& complexPrimitivesCachedEntities =
            Patterns::Singleton::getInstance<ComplexPrimitivesCollectorSystem>()->getCachedEntities();

    if(meshedCachedEntities.empty() && primitivesCachedEntities.empty() && skyboxesCachedEntities.empty()) return;

    // first render skybox
    m_skyboxPassMarkedShader->bind();

    for (const auto& camerasLayer: m_cachedEntities)
    {
        for (const auto& cachedEntity: camerasLayer.second)
        {
            if (!cachedEntity.second) continue;

            std::shared_ptr<CameraComponent> cameraComponent = cachedEntity.second->getComponent<CameraComponent>();
            std::shared_ptr<TransformComponent> cameraTransformComponent = cachedEntity.second->getComponent<TransformComponent>();

            if (!cameraComponent || !cameraTransformComponent) continue;

            Core::Main::CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);
            m_skyboxPassMarkedShader->m_shader->useUniformBuffer(Core::Main::CoreMain::getRenderer().m_viewMatricesBuffer);

            auto currentLayerFrameBuffer = cameraComponent->m_defaultLayersFrameBuffer;
            currentLayerFrameBuffer->bind()->clear();

            for (const auto& skyboxesLayer: skyboxesCachedEntities)
            {
                const auto& foundPPLayer = cameraComponent->getPostProcessLayerFrameBuffer(skyboxesLayer.first);
                // if pp layer exists
                if(foundPPLayer)
                {
                    currentLayerFrameBuffer = foundPPLayer;
                    currentLayerFrameBuffer->bind()->clear();
                }
                else
                {
                    currentLayerFrameBuffer->bind();
                }

                for (const auto& skyboxEntity : skyboxesLayer.second)
                {
                    if (!skyboxEntity.second) continue;

                    std::shared_ptr<TransformComponent> transformComponent = skyboxEntity.second->getComponent<TransformComponent>();

                    if (!transformComponent) continue;

                    auto meshComponents =
                            skyboxEntity.second->getComponents<MeshComponent>();

                    for (const auto& meshComponent: meshComponents)
                    {
                        meshComponent->m_mesh->m_material->bind(m_skyboxPassMarkedShader);
                        updateUniforms(m_skyboxPassMarkedShader->m_shader,
                                       meshComponent->m_mesh->m_material,
                                       transformComponent);

                        Core::Main::CoreMain::getRenderer().renderMeshComponent(
                                transformComponent,
                                meshComponent
                        );
                    }
                }

                currentLayerFrameBuffer->unbind();
            }
        }
    }

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

            std::string currentShadowCasterStr = "shadowsCasters[";
            currentShadowCasterStr += std::to_string(shadowsCastersCount);
            currentShadowCasterStr += "]";

            // updating shadows casters uniforms of geometry shader
            m_geometryPassMarkedShader->m_shader->bind();

            m_geometryPassMarkedShader->m_shader->useMatrix(
                    currentShadowCasterStr + ".shadowsCasterSpace",
                    shadowsCasterComponent->m_spaceMatrix
            );

            m_geometryPassMarkedShader->m_shader->useVectorf(
                    currentShadowCasterStr + ".position",
                    shadowsCasterTransform->m_position
            );

            // binding frame buffer of shadow caster and rendering in
            shadowsCasterComponent->m_frameBuffer->bind()->clear();

            m_shadowsPassMarkedShader->bind();

            Core::Main::CoreMain::getRenderer().prepareUniformBuffers(shadowsCasterComponent, nullptr);
            m_shadowsPassMarkedShader->m_shader->useUniformBuffer(Core::Main::CoreMain::getRenderer().m_viewMatricesBuffer);

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

                        Core::Main::CoreMain::getRenderer().renderMeshComponent(
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

    m_geometryPassMarkedShader->m_shader->bind();
    m_geometryPassMarkedShader->m_shader->useInteger(
            sgStandardTextureTypeToString(SGTextureType::SGTP_SHADOW_MAP) + "_COUNT",
            shadowsCastersCount
    );

    // ---------------------------------------------------

    // ----- render meshes and primitives (geometry + light pass pbr) ------

    for (const auto& camerasLayer: m_cachedEntities)
    {
        for (const auto& cachedEntity: camerasLayer.second)
        {
            if (!cachedEntity.second) continue;

            std::shared_ptr<TransformComponent> cameraTransformComponent = cachedEntity.second->getComponent<TransformComponent>();
            if(!cameraTransformComponent) continue;
            std::shared_ptr<CameraComponent> cameraComponent = cachedEntity.second->getComponent<CameraComponent>();
            if (!cameraComponent) continue;

            Core::Main::CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);

            m_geometryPassMarkedShader->m_shader->bind();
            m_geometryPassMarkedShader->m_shader->useUniformBuffer(Core::Main::CoreMain::getRenderer().m_viewMatricesBuffer);

            std::shared_ptr<Graphics::IFrameBuffer> currentLayerFrameBuffer = cameraComponent->m_defaultLayersFrameBuffer;

            for (const auto& meshesLayer: meshedCachedEntities)
            {
                const auto& foundPPLayer = cameraComponent->getPostProcessLayerFrameBuffer(meshesLayer.first);
                // if pp layer exists
                if(foundPPLayer)
                {
                    currentLayerFrameBuffer = foundPPLayer;
                    currentLayerFrameBuffer->bind()->clear();
                }
                else
                {
                    currentLayerFrameBuffer->bind();
                }
                // todo: make less bindings

                for (const auto& meshesEntity: meshesLayer.second)
                {
                    if (!meshesEntity.second) continue;

                    std::shared_ptr<TransformComponent> transformComponent = meshesEntity.second->getComponent<TransformComponent>();

                    if (!transformComponent) continue;

                    auto meshComponents =
                            meshesEntity.second->getComponents<MeshComponent>();

                    for (const auto& meshComponent: meshComponents)
                    {
                        const auto& shadowsMapsTexturesBlock = m_geometryPassMarkedShader->getTexturesBlocks()[SGTextureType::SGTP_SHADOW_MAP];

                        std::uint8_t currentShadowsCaster = 0;
                        for(const auto& shadowsCastersLayer : shadowsCastersCachedEntities)
                        {
                            for(const auto& shadowsCasterEntity: shadowsCastersLayer.second)
                            {
                                if(!shadowsCasterEntity.second) continue;

                                // todo: make process all ShadowsCasterComponent (cachedEntities.second->getComponents)
                                std::shared_ptr<ShadowsCasterComponent> shadowsCasterComponent = shadowsCasterEntity.second->getComponent<ShadowsCasterComponent>();

                                if(!shadowsCasterComponent) continue;

                                shadowsCasterComponent->m_frameBuffer->bindAttachment(
                                            SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                            shadowsMapsTexturesBlock.m_offset + currentShadowsCaster
                                        );

                                currentShadowsCaster++;
                            }
                        }

                        meshComponent->m_mesh->m_material->bind(m_geometryPassMarkedShader);
                        updateUniforms(m_geometryPassMarkedShader->m_shader,
                                       meshComponent->m_mesh->m_material,
                                       transformComponent);

                        Core::Main::CoreMain::getRenderer().renderMeshComponent(
                                transformComponent,
                                meshComponent
                        );
                    }
                }

                currentLayerFrameBuffer->unbind();
            }

            // complex primitives pass ----------------
            m_complexPrimitivesPassMarkedShader->m_shader->bind();
            m_complexPrimitivesPassMarkedShader->m_shader->useUniformBuffer(Core::Main::CoreMain::getRenderer().m_viewMatricesBuffer);

            currentLayerFrameBuffer = cameraComponent->m_defaultLayersFrameBuffer;

            for(const auto& complexPrimitivesLayer : complexPrimitivesCachedEntities)
            {
                const auto& foundPPLayer = cameraComponent->getPostProcessLayerFrameBuffer(complexPrimitivesLayer.first);
                // if pp layer exists
                if(foundPPLayer)
                {
                    currentLayerFrameBuffer = foundPPLayer;
                    currentLayerFrameBuffer->bind()->clear();
                }
                else
                {
                    currentLayerFrameBuffer->bind();
                }

                for(const auto& cachedPrimitive : complexPrimitivesLayer.second)
                {
                    if(!cachedPrimitive.second) continue;

                    std::shared_ptr<TransformComponent> complexPrimitiveTransform = cachedPrimitive.second->getComponent<TransformComponent>();
                    if(!complexPrimitiveTransform) continue;
                    std::list<std::shared_ptr<ComplexPrimitiveComponent>> complexPrimitiveComponents =
                            cachedPrimitive.second->getComponents<ComplexPrimitiveComponent>();

                    for(const auto& complexPrimitiveComponent : complexPrimitiveComponents)
                    {
                        m_complexPrimitivesPassMarkedShader->m_shader->useMatrix(
                                "u_primitiveModelMatrix", complexPrimitiveTransform->m_modelMatrix
                        );

                        m_complexPrimitivesPassMarkedShader->m_shader->useVectorf(
                                "u_offset", complexPrimitiveComponent->m_offset
                        );

                        m_complexPrimitivesPassMarkedShader->m_shader->useVectorf(
                                "u_color", complexPrimitiveComponent->m_color
                        );

                        Core::Main::CoreMain::getRenderer().renderPrimitiveComponent(
                                complexPrimitiveTransform,
                                complexPrimitiveComponent
                        );
                    }
                }

                currentLayerFrameBuffer->unbind();
            }

            // ------------------------------

            Core::Main::CoreMain::getRenderer().setDepthTestingEnabled(false);

            // first - discard not visible fragments in every frame buffer of layer

            cameraComponent->bindPostProcessLayers();

            cameraComponent->m_depthPostProcessQuadPassMarkedShader->bind();

            // first depth test for pixels in default FB

            cameraComponent->m_defaultLayersFrameBuffer->bind();

            cameraComponent->m_depthPostProcessQuadPassMarkedShader
                    ->m_shader
                    ->useInteger("currentFBIndex", 0);

            Core::Main::CoreMain::getRenderer().renderMesh(
                    cameraComponent->m_billboard
            );

            cameraComponent->m_defaultLayersFrameBuffer->unbind();

            // ---------------------------------

            // and then depth test for PP Layers

            for(const auto& ppLayer : cameraComponent->getPostProcessLayers())
            {
                ppLayer.second.m_frameBuffer->bind();

                cameraComponent->m_depthPostProcessQuadPassMarkedShader
                        ->m_shader
                        ->useInteger("currentFBIndex", ppLayer.second.m_index);

                Core::Main::CoreMain::getRenderer().renderMesh(
                        cameraComponent->m_billboard
                );

                ppLayer.second.m_frameBuffer->unbind();
            }

            Core::Main::CoreMain::getRenderer().setDepthTestingEnabled(true);

            // ----------------------------------

            // --------------------------------------------------------------------

            // render post-process quad ------------

            cameraComponent->bindPostProcessLayers();

            if(cameraComponent->m_useFinalFrameBuffer)
            {
                cameraComponent->m_finalFrameBuffer->bind()->clear();
            }

            cameraComponent->m_colorPostProcessQuadPassMarkedShader->bind();

            Core::Main::CoreMain::getRenderer().renderMesh(
                    cameraComponent->m_billboard
            );

            if(cameraComponent->m_useFinalFrameBuffer)
            {
                cameraComponent->m_finalFrameBuffer->unbind();
            }

            // -------------------------------------
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
    // shader->bind();

    shader->useMatrix("objectModelMatrix",
                      transformComponent->m_modelMatrix
    );
    /*shader->useVectorf("objectPosition",
                       transformComponent->m_position
    );
    shader->useVectorf("objectRotation",
                       transformComponent->m_rotation
    );
    shader->useVectorf("objectScale",
                       transformComponent->m_scale
    );*/

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
