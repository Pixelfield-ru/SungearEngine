//
// Created by stuka on 04.07.2023.
//

#include "PBRForwardRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Rendering/Skybox.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "SGCore/ECS/Transformations/TransformationsUpdater.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsCollector.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCastersCollector.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCaster.h"
#include "SGCore/ECS/Rendering/SkyboxesCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/LinesGizmosCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/ComplexGizmosCollector.h"
#include "SGCore/ECS/Rendering/Gizmos/LineGizmo.h"
#include "SGCore/ECS/Rendering/Gizmos/IComplexGizmo.h"

Core::ECS::PBRForwardRenderPipeline::PBRForwardRenderPipeline()
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

void Core::ECS::PBRForwardRenderPipeline::update(const std::shared_ptr<Scene>& scene)
{
    double t0 = glfwGetTime();

    auto transformationsSystem = Patterns::Singleton::getInstance<TransformationsUpdater>();

    const auto& meshedCachedEntities = Patterns::Singleton::getInstance<MeshesCollector>()->getCachedEntities();
    const auto& primitivesCachedEntities = Patterns::Singleton::getInstance<GizmosMeshesRebuilder>()->getCachedEntities();

    const auto& directionalLightsCachedEntities = Patterns::Singleton::getInstance<DirectionalLightsCollector>()->getCachedEntities();
    const auto& shadowsCastersCachedEntities = Patterns::Singleton::getInstance<ShadowsCastersCollector>()->getCachedEntities();

    const auto& skyboxesCachedEntities = Patterns::Singleton::getInstance<SkyboxesCollector>()->getCachedEntities();

    const auto& linesCachedEntities = Patterns::Singleton::getInstance<LinesGizmosCollector>()->getCachedEntities();
    const auto& complexPrimitivesCachedEntities =
            Patterns::Singleton::getInstance<ComplexGizmosCollector>()->getCachedEntities();

    if(meshedCachedEntities.empty() && primitivesCachedEntities.empty() && skyboxesCachedEntities.empty()) return;

    // first render skybox
    m_skyboxPassMarkedShader->bind();

    for (const auto& camerasLayer: m_cachedEntities)
    {
        for (const auto& cachedEntity: camerasLayer.second)
        {
            if (!cachedEntity.second) continue;

            std::shared_ptr<Camera> cameraComponent = cachedEntity.second->getComponent<Camera>();
            std::shared_ptr<Transform> cameraTransformComponent = cachedEntity.second->getComponent<Transform>();

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

                    std::shared_ptr<Transform> transformComponent = skyboxEntity.second->getComponent<Transform>();

                    if (!transformComponent) continue;

                    auto meshComponents =
                            skyboxEntity.second->getComponents<Mesh>();

                    for (const auto& meshComponent: meshComponents)
                    {
                        meshComponent->m_meshData->m_material->bind(m_skyboxPassMarkedShader);
                        updateUniforms(m_skyboxPassMarkedShader->m_shader,
                                       meshComponent->m_meshData->m_material,
                                       transformComponent);

                        Core::Main::CoreMain::getRenderer().renderMeshData(
                                meshComponent->m_meshData,
                                meshComponent->m_meshDataRenderInfo
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
                    directionalLightEntity.second->getComponents<DirectionalLight>();

            auto directionalLightTransform =
                    directionalLightEntity.second->getComponent<Transform>();

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
            std::shared_ptr<ShadowsCaster> shadowsCasterComponent = cachedEntity.second->getComponent<ShadowsCaster>();
            std::shared_ptr<Transform> shadowsCasterTransform = cachedEntity.second->getComponent<Transform>();

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

                    std::shared_ptr<Transform> transformComponent = meshesEntity.second->getComponent<Transform>();

                    if (!transformComponent) continue;

                    auto meshComponents =
                            meshesEntity.second->getComponents<Mesh>();

                    for (const auto& meshComponent: meshComponents)
                    {
                        meshComponent->m_meshData->m_material->bind(m_shadowsPassMarkedShader);
                        updateUniforms(m_shadowsPassMarkedShader->m_shader,
                                       meshComponent->m_meshData->m_material,
                                       transformComponent);

                        Core::Main::CoreMain::getRenderer().renderMeshData(
                                meshComponent->m_meshData,
                                meshComponent->m_meshDataRenderInfo
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

            std::shared_ptr<Transform> cameraTransformComponent = cachedEntity.second->getComponent<Transform>();
            if(!cameraTransformComponent) continue;
            std::shared_ptr<Camera> cameraComponent = cachedEntity.second->getComponent<Camera>();
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

                    std::shared_ptr<Transform> transformComponent = meshesEntity.second->getComponent<Transform>();

                    if (!transformComponent) continue;

                    auto meshComponents =
                            meshesEntity.second->getComponents<Mesh>();

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
                                std::shared_ptr<ShadowsCaster> shadowsCasterComponent = shadowsCasterEntity.second->getComponent<ShadowsCaster>();

                                if(!shadowsCasterComponent) continue;

                                shadowsCasterComponent->m_frameBuffer->bindAttachment(
                                            SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                            shadowsMapsTexturesBlock.m_offset + currentShadowsCaster
                                        );

                                currentShadowsCaster++;
                            }
                        }

                        meshComponent->m_meshData->m_material->bind(m_geometryPassMarkedShader);
                        updateUniforms(m_geometryPassMarkedShader->m_shader,
                                       meshComponent->m_meshData->m_material,
                                       transformComponent);

                        Core::Main::CoreMain::getRenderer().renderMeshData(
                                meshComponent->m_meshData,
                                meshComponent->m_meshDataRenderInfo
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

                    std::shared_ptr<Transform> complexPrimitiveTransform = cachedPrimitive.second->getComponent<Transform>();
                    if(!complexPrimitiveTransform) continue;
                    std::list<std::shared_ptr<IComplexGizmo>> complexPrimitiveComponents =
                            cachedPrimitive.second->getComponents<IComplexGizmo>();

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

                        Core::Main::CoreMain::getRenderer().renderMeshData(
                                complexPrimitiveComponent->m_meshData,
                                complexPrimitiveComponent->m_meshDataRenderInfo
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

            Core::Main::CoreMain::getRenderer().renderMeshData(
                    cameraComponent->m_postProcessQuad,
                    cameraComponent->m_postProcessQuadRenderInfo
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

                Core::Main::CoreMain::getRenderer().renderMeshData(
                        cameraComponent->m_postProcessQuad,
                        cameraComponent->m_postProcessQuadRenderInfo
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

            Core::Main::CoreMain::getRenderer().renderMeshData(
                    cameraComponent->m_postProcessQuad,
                    cameraComponent->m_postProcessQuadRenderInfo
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

void Core::ECS::PBRForwardRenderPipeline::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity)
{
    cacheEntityComponents<Camera, Transform>(entity);
}

void Core::ECS::PBRForwardRenderPipeline::updateUniforms(const std::shared_ptr<Graphics::IShader>& shader,
                                                         const std::shared_ptr<Memory::Assets::IMaterial>& material,
                                                         const std::shared_ptr<Transform>& transformComponent) const noexcept
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
