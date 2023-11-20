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

SGCore::PBRForwardRenderPipeline::PBRForwardRenderPipeline()
{
    m_geometryPassShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    ShadersPaths::getMainInstance()["PBR"]["DefaultShader"]
            )
    );

    m_geometryPassShaderMarkup.addTexturesBlockDeclaration(
            SGTextureType::SGTP_DIFFUSE,
            sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE),
            1
    );
    m_geometryPassShaderMarkup.addTexturesBlockDeclaration(
            SGTextureType::SGTP_DIFFUSE_ROUGHNESS,
            sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE_ROUGHNESS),
            1
    );
    m_geometryPassShaderMarkup.addTexturesBlockDeclaration(
            SGTextureType::SGTP_NORMALS,
            sgStandardTextureTypeToString(SGTextureType::SGTP_NORMALS),
            1
    );
    m_geometryPassShaderMarkup.addTexturesBlockDeclaration(
            SGTextureType::SGTP_BASE_COLOR,
            sgStandardTextureTypeToString(SGTextureType::SGTP_BASE_COLOR),
            1
    );
    m_geometryPassShaderMarkup.addTexturesBlockDeclaration(
            SGTextureType::SGTP_SHADOW_MAP,
            sgStandardTextureTypeToString(SGTextureType::SGTP_SHADOW_MAP),
            5,
            false
    );

    m_geometryPassShaderMarkup.calculateBlocksOffsets();

    // ---------------------

    m_shadowsPassShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    ShadersPaths::getMainInstance()["ShadowsGeneration"]["DefaultShader"]
            )
    );

    m_shadowsPassShaderMarkup.addTexturesBlockDeclaration(
            SGTextureType::SGTP_DIFFUSE,
            sgStandardTextureTypeToString(SGTextureType::SGTP_DIFFUSE),
            1
    );

    m_shadowsPassShaderMarkup.calculateBlocksOffsets();

    // ---------------------

    m_skyboxPassShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    ShadersPaths::getMainInstance()["Skybox"]["DefaultShader"]
            )
    );

    m_skyboxPassShaderMarkup.addTexturesBlockDeclaration(
            SGTextureType::SGTP_SKYBOX,
            sgStandardTextureTypeToString(SGTextureType::SGTP_SKYBOX),
            1
    );

    m_skyboxPassShaderMarkup.calculateBlocksOffsets();

    // ---------------------

    m_linesGizmosPassShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    ShadersPaths::getMainInstance()["Gizmos"]["LinesShader"]
            )
    );

    // ---------------------

    m_complexGizmosPassShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    ShadersPaths::getMainInstance()["Gizmos"]["ComplexGizmosShader"]
            )
    );
}

void SGCore::PBRForwardRenderPipeline::update(const Ref<Scene>& scene)
{
    auto transformationsSystem = SGSingleton::getSharedPtrInstance<TransformationsUpdater>();

    const auto& meshedCachedEntities = SGSingleton::getSharedPtrInstance<MeshesCollector>()->getCachedEntities();
    const auto& primitivesCachedEntities = SGSingleton::getSharedPtrInstance<GizmosMeshesRebuilder>()->getCachedEntities();

    const auto& directionalLightsCachedEntities = SGSingleton::getSharedPtrInstance<DirectionalLightsCollector>()->getCachedEntities();
    const auto& shadowsCastersCachedEntities = SGSingleton::getSharedPtrInstance<ShadowsCastersCollector>()->getCachedEntities();

    const auto& skyboxesCachedEntities = SGSingleton::getSharedPtrInstance<SkyboxesCollector>()->getCachedEntities();

    const auto& linesCachedEntities = SGSingleton::getSharedPtrInstance<LinesGizmosCollector>()->getCachedEntities();
    const auto& complexPrimitivesCachedEntities =
            SGSingleton::getSharedPtrInstance<ComplexGizmosCollector>()->getCachedEntities();

    if(meshedCachedEntities.empty() && primitivesCachedEntities.empty() && skyboxesCachedEntities.empty()) return;

    // first render skybox
    m_skyboxPassShader->bind();
    m_skyboxPassShader->useShaderMarkup(m_skyboxPassShaderMarkup);

    SG_BEGIN_ITERATE_CACHED_ENTITIES(m_cachedEntities, camerasLayer, cameraEntity)
            Ref<Camera> cameraComponent = cameraEntity->getComponent<Camera>();
            Ref<Transform> cameraTransformComponent = cameraEntity->getComponent<Transform>();

            if(!cameraComponent || !cameraTransformComponent) continue;

            CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);
            m_skyboxPassShader->useUniformBuffer(CoreMain::getRenderer().m_viewMatricesBuffer);

            auto currentLayerFrameBuffer = cameraComponent->m_defaultLayersFrameBuffer;
            Ref<IFrameBuffer> foundFrameBuffer;
            currentLayerFrameBuffer->bind()->clear();

            for(const auto& skyboxesLayer: skyboxesCachedEntities)
            {
                foundFrameBuffer = cameraComponent->getPostProcessLayerFrameBuffer(skyboxesLayer.first);
                // if pp layer exists
                if(foundFrameBuffer)
                {
                    currentLayerFrameBuffer = foundFrameBuffer;
                    currentLayerFrameBuffer->bind()->clear();
                }
                else
                {
                    currentLayerFrameBuffer->bind();
                }

                currentLayerFrameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT0);

                for(const auto& skyboxEntity: skyboxesLayer.second)
                {

                    if(!skyboxEntity.second) continue;

                    Ref<Transform> transformComponent = skyboxEntity.second->getComponent<Transform>();

                    if(!transformComponent) continue;

                    auto meshComponents =
                            skyboxEntity.second->getComponents<Mesh>();

                    for(const auto& meshComponent: meshComponents)
                    {
                        meshComponent->m_meshData->m_material->bind(m_skyboxPassShader, m_skyboxPassShaderMarkup);
                        updateUniforms(m_skyboxPassShader,
                                       meshComponent->m_meshData->m_material,
                                       transformComponent
                        );

                        CoreMain::getRenderer().renderMeshData(
                                meshComponent->m_meshData,
                                meshComponent->m_meshDataRenderInfo
                        );
                    }

                    currentLayerFrameBuffer->unbind();
                }
            }

    SG_END_ITERATE_CACHED_ENTITIES

    // binding geom pass shader
    m_geometryPassShader->bind();
    m_geometryPassShader->useUniformBuffer(CoreMain::getRenderer().m_programDataBuffer);

    // ------ updating all lights in geom shader -------------

    size_t directionalLightsCount = 0;

    SG_BEGIN_ITERATE_CACHED_ENTITIES(directionalLightsCachedEntities, dirLightsLayer, directionalLightEntity)
            auto directionalLightComponents =
                    directionalLightEntity->getComponents<DirectionalLight>();

            auto directionalLightTransform =
                    directionalLightEntity->getComponent<Transform>();

            if(!directionalLightTransform) continue;

            for(const auto& directionalLightComponent: directionalLightComponents)
            {
                std::string directionalLightString =
                        "directionalLights[" + std::to_string(directionalLightsCount) + "]";;

                m_geometryPassShader->useVectorf(
                        directionalLightString + ".color",
                        directionalLightComponent->m_color
                );

                m_geometryPassShader->useFloat(
                        directionalLightString + ".intensity",
                        directionalLightComponent->m_intensity
                );

                // todo: take into account the type of transformation and the direction of rotation
                m_geometryPassShader->useVectorf(
                        directionalLightString + ".position",
                        directionalLightTransform->m_position
                );

                directionalLightsCount++;
            }
    SG_END_ITERATE_CACHED_ENTITIES

    // todo: make name as define
    m_geometryPassShader->useInteger("DIRECTIONAL_LIGHTS_COUNT", directionalLightsCount);

    // -----------------------------------------

    // ----- updating shadows casters in geom shader and render shadows pass -----

    size_t shadowsCastersCount = 0;

    SG_BEGIN_ITERATE_CACHED_ENTITIES(shadowsCastersCachedEntities, shadowsCastersLayer, shadowsCasterEntity)
            // todo: make process all ShadowsCasterComponent (cachedEntities.second->getComponents)
            Ref<ShadowsCaster> shadowsCasterComponent = shadowsCasterEntity->getComponent<ShadowsCaster>();
            Ref<Transform> shadowsCasterTransform = shadowsCasterEntity->getComponent<Transform>();

            if(!shadowsCasterTransform || !shadowsCasterComponent) continue;

            std::string currentShadowCasterStr = "shadowsCasters[";
            currentShadowCasterStr += std::to_string(shadowsCastersCount);
            currentShadowCasterStr += "]";

            // updating shadows casters uniforms of geometry shader
            m_geometryPassShader->bind();

            m_geometryPassShader->useMatrix(
                    currentShadowCasterStr + ".shadowsCasterSpace",
                    shadowsCasterComponent->m_spaceMatrix
            );

            m_geometryPassShader->useVectorf(
                    currentShadowCasterStr + ".position",
                    shadowsCasterTransform->m_position
            );

            // binding frame buffer of shadow caster and rendering in
            shadowsCasterComponent->m_frameBuffer->bind()->clear();

            m_shadowsPassShader->bind();
            m_shadowsPassShader->useShaderMarkup(m_shadowsPassShaderMarkup);

            CoreMain::getRenderer().prepareUniformBuffers(shadowsCasterComponent, nullptr);
            m_shadowsPassShader->useUniformBuffer(CoreMain::getRenderer().m_viewMatricesBuffer);

            SG_BEGIN_ITERATE_CACHED_ENTITIES(meshedCachedEntities, meshesLayer, meshesEntity)
                    Ref<Transform> transformComponent = meshesEntity->getComponent<Transform>();

                    if(!transformComponent) continue;

                    auto meshComponents =
                            meshesEntity->getComponents<Mesh>();

                    for(const auto& meshComponent: meshComponents)
                    {
                        meshComponent->m_meshData->m_material->bind(m_shadowsPassShader, m_shadowsPassShaderMarkup);
                        updateUniforms(m_shadowsPassShader,
                                       meshComponent->m_meshData->m_material,
                                       transformComponent
                        );

                        CoreMain::getRenderer().renderMeshData(
                                meshComponent->m_meshData,
                                meshComponent->m_meshDataRenderInfo
                        );
                    }
            SG_END_ITERATE_CACHED_ENTITIES

            shadowsCasterComponent->m_frameBuffer->unbind();

            shadowsCastersCount++;
    SG_END_ITERATE_CACHED_ENTITIES

    m_geometryPassShader->bind();
    m_geometryPassShader->useInteger(
            sgStandardTextureTypeToString(SGTextureType::SGTP_SHADOW_MAP) + "_COUNT",
            shadowsCastersCount
    );

    // ---------------------------------------------------

    // ----- render meshes and primitives (geometry + light pass pbr) ------

    SG_BEGIN_ITERATE_CACHED_ENTITIES(m_cachedEntities, camerasLayer, cameraEntity)
            Ref<Transform> cameraTransformComponent = cameraEntity->getComponent<Transform>();
            if(!cameraTransformComponent) continue;
            Ref<Camera> cameraComponent = cameraEntity->getComponent<Camera>();
            if(!cameraComponent) continue;

            CoreMain::getRenderer().prepareUniformBuffers(cameraComponent, cameraTransformComponent);

            m_geometryPassShader->bind();
            m_geometryPassShader->useShaderMarkup(m_geometryPassShaderMarkup);
            m_geometryPassShader->useUniformBuffer(CoreMain::getRenderer().m_viewMatricesBuffer);

            Ref<IFrameBuffer> currentLayerFrameBuffer = cameraComponent->m_defaultLayersFrameBuffer;
            Ref<IFrameBuffer> foundFrameBuffer;
            // todo: make less bindings

            for(const auto& meshesLayer: meshedCachedEntities)
            {
                foundFrameBuffer = cameraComponent->getPostProcessLayerFrameBuffer(meshesLayer.first);
                // if pp layer exists
                if(foundFrameBuffer)
                {
                    currentLayerFrameBuffer = foundFrameBuffer;
                    currentLayerFrameBuffer->bind()->clear();
                }
                else
                {
                    currentLayerFrameBuffer->bind();
                }

                currentLayerFrameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT0);

                for(const auto& meshesEntity: meshesLayer.second)
                {
                    if(!meshesEntity.second) continue;

                    Ref<Transform> transformComponent = meshesEntity.second->getComponent<Transform>();

                    if(!transformComponent) continue;

                    auto meshComponents =
                            meshesEntity.second->getComponents<Mesh>();

                    for(const auto& meshComponent: meshComponents)
                    {
                        const auto& shadowsMapsTexturesBlock = m_geometryPassShaderMarkup.m_texturesBlocks[SGTextureType::SGTP_SHADOW_MAP];

                        std::uint8_t currentShadowsCaster = 0;

                        SG_BEGIN_ITERATE_CACHED_ENTITIES(shadowsCastersCachedEntities, shadowsCastersLayer,
                                                         shadowsCasterEntity)
                                // todo: make process all ShadowsCasterComponent (cachedEntities.second->getComponents)
                                Ref<ShadowsCaster> shadowsCasterComponent = shadowsCasterEntity->getComponent<ShadowsCaster>();

                                if(!shadowsCasterComponent) continue;

                                shadowsCasterComponent->m_frameBuffer->bindAttachment(
                                        SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                        shadowsMapsTexturesBlock.m_offset + currentShadowsCaster
                                );

                                currentShadowsCaster++;
                        SG_END_ITERATE_CACHED_ENTITIES

                        meshComponent->m_meshData->m_material->bind(m_geometryPassShader, m_geometryPassShaderMarkup);
                        updateUniforms(m_geometryPassShader,
                                       meshComponent->m_meshData->m_material,
                                       transformComponent
                        );

                        CoreMain::getRenderer().renderMeshData(
                                meshComponent->m_meshData,
                                meshComponent->m_meshDataRenderInfo
                        );
                    }
                }
            }

            currentLayerFrameBuffer->unbind();

            // complex primitives pass ----------------
            m_complexGizmosPassShader->bind();
            m_complexGizmosPassShader->useUniformBuffer(CoreMain::getRenderer().m_viewMatricesBuffer);

            currentLayerFrameBuffer = cameraComponent->m_defaultLayersFrameBuffer;

            for(const auto& cachedPrimitivesLayer: complexPrimitivesCachedEntities)
            {
                foundFrameBuffer = cameraComponent->getPostProcessLayerFrameBuffer(cachedPrimitivesLayer.first);
                // if pp layer exists
                if(foundFrameBuffer)
                {
                    currentLayerFrameBuffer = foundFrameBuffer;
                    currentLayerFrameBuffer->bind()->clear();
                }
                else
                {
                    currentLayerFrameBuffer->bind();
                }

                currentLayerFrameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT0);

                for(const auto& cachedPrimitive: cachedPrimitivesLayer.second)
                {
                    if(!cachedPrimitive.second) continue;

                    Ref<Transform> complexPrimitiveTransform = cachedPrimitive.second->getComponent<Transform>();
                    if(!complexPrimitiveTransform) continue;
                    std::list<Ref<IComplexGizmo>> complexPrimitiveComponents =
                            cachedPrimitive.second->getComponents<IComplexGizmo>();

                    for(const auto& complexPrimitiveComponent: complexPrimitiveComponents)
                    {
                        m_complexGizmosPassShader->useMatrix(
                                "u_primitiveModelMatrix", complexPrimitiveTransform->m_modelMatrix
                        );

                        m_complexGizmosPassShader->useVectorf(
                                "u_offset", complexPrimitiveComponent->m_offset
                        );

                        m_complexGizmosPassShader->useVectorf(
                                "u_color", complexPrimitiveComponent->m_color
                        );

                        CoreMain::getRenderer().renderMeshData(
                                complexPrimitiveComponent->m_meshData,
                                complexPrimitiveComponent->m_meshDataRenderInfo
                        );
                    }
                }

                currentLayerFrameBuffer->unbind();
            }

            // todo: make lines gizmos render

            // ------------------------------

            CoreMain::getRenderer().setDepthTestingEnabled(false);

            // first - discard not visible fragments in every frame buffer of layer and apply PP FX for each PP layer

            cameraComponent->bindPostProcessLayers();

            cameraComponent->m_defaultPostProcessShader->bind();
            cameraComponent->m_defaultPostProcessShader
                    ->useShaderMarkup(cameraComponent->m_postProcessShadersMarkup);

            // first depth test for pixels in default FB

            cameraComponent->m_defaultPostProcessShader->useInteger("currentFBIndex", 0);

            cameraComponent->m_defaultLayersFrameBuffer->bind();

            // first pass - depth pass --------------------------------------------

            {
                cameraComponent->m_defaultLayersFrameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT0);

                cameraComponent->m_defaultPostProcessShader->useInteger("depthTestPass", true);

                CoreMain::getRenderer().renderMeshData(
                        cameraComponent->m_postProcessQuad,
                        cameraComponent->m_postProcessQuadRenderInfo
                );

                // second pass - FX pass --------------------------------------------

                cameraComponent->m_defaultLayersFrameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT1);

                cameraComponent->m_defaultPostProcessShader->useInteger("depthTestPass", false);

                CoreMain::getRenderer().renderMeshData(
                        cameraComponent->m_postProcessQuad,
                        cameraComponent->m_postProcessQuadRenderInfo
                );
            }

            cameraComponent->m_defaultLayersFrameBuffer->unbind();

            // ---------------------------------

            // and then depth test for PP Layers

            for(const auto& ppLayer: cameraComponent->getPostProcessLayers())
            {
                ppLayer.second.m_shader->bind();
                ppLayer.second.m_shader
                        ->useShaderMarkup(cameraComponent->m_postProcessShadersMarkup);

                ppLayer.second.m_shader
                        ->useInteger("currentFBIndex", ppLayer.second.m_index);

                ppLayer.second.m_frameBuffer->bind();

                {
                    ppLayer.second.m_frameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT0);

                    // if pass == 1 then depth pass
                    ppLayer.second.m_shader->useInteger("depthTestPass", true);

                    CoreMain::getRenderer().renderMeshData(
                            cameraComponent->m_postProcessQuad,
                            cameraComponent->m_postProcessQuadRenderInfo
                    );

                    ppLayer.second.m_frameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT1);

                    ppLayer.second.m_shader->useInteger("depthTestPass", false);

                    CoreMain::getRenderer().renderMeshData(
                            cameraComponent->m_postProcessQuad,
                            cameraComponent->m_postProcessQuadRenderInfo
                    );
                }

                ppLayer.second.m_frameBuffer->unbind();
            }

            CoreMain::getRenderer().setDepthTestingEnabled(true);

            // --------------------------------------------------------------------

            // --------------------------------------------------------------------

            // render post-process quad -------------------------------------------

            if(cameraComponent->m_useFinalFrameBuffer)
            {
                cameraComponent->m_finalFrameBuffer->bind()->clear();
            }

            cameraComponent->m_finalPostProcessOverlayShader->bind();
            cameraComponent->m_finalPostProcessOverlayShader
                    ->useShaderMarkup(cameraComponent->m_postProcessShadersMarkup);

            CoreMain::getRenderer().renderMeshData(
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
}

void SGCore::PBRForwardRenderPipeline::cacheEntity(const Ref<Entity>& entity)
{
    cacheEntityComponents<Camera, Transform>(entity);
}

void SGCore::PBRForwardRenderPipeline::updateUniforms(const Ref<IShader>& shader,
                                                         const Ref<IMaterial>& material,
                                                         const Ref<Transform>& transformComponent) const noexcept
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
