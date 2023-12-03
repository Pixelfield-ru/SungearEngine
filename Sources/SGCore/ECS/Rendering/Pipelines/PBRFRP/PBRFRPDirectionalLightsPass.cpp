//
// Created by stuka on 26.11.2023.
//

#include "PBRFRPDirectionalLightsPass.h"
#include "SGCore/ECS/ECSUtils.h"
#include "PBRFRPGeometryPass.h"
#include "SGCore/ECS/Scene.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightsCollector.h"
#include "SGCore/Main/CoreMain.h"

void SGCore::PBRFRPDirectionalLightsPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    m_renderTimer.startFrame();

    m_renderTimerCallback->setUpdateFunction([&, renderPipeline]()
                                             {
                                                 auto geometryPass = renderPipeline->getRenderPass<PBRFRPGeometryPass>();

                                                 if(geometryPass || !geometryPass->m_active)
                                                 {
                                                     std::uint8_t currentShadowsCaster = 0;

                                                     const auto& shadowsMapsTexturesBlock =
                                                             geometryPass->m_shaderMarkup.m_texturesBlocks[SGTextureType::SGTP_SHADOW_MAP];

                                                     // ------ updating all lights in geom shader -------------

                                                     size_t directionalLightsCount = 0;

                                                     SG_BEGIN_ITERATE_CACHED_ENTITIES(
                                                             *m_componentsToRenderIn,
                                                             dirLightsLayer, directionalLightEntity)

                                                             auto directionalLightComponents =
                                                                     directionalLightEntity.getComponents<DirectionalLight>();

                                                             auto directionalLightTransform =
                                                                     directionalLightEntity.getComponent<Transform>();

                                                             if(!directionalLightTransform) continue;

                                                             for(const auto& directionalLightComponent: directionalLightComponents)
                                                             {
                                                                 geometryPass->m_shader->bind();

                                                                 std::string directionalLightString =
                                                                         "directionalLights[" +
                                                                         std::to_string(directionalLightsCount) + "]";
                                                                 std::string lightPartString = directionalLightString + ".lightPart";
                                                                 std::string renderingPartString = lightPartString + ".renderingPart";

                                                                 geometryPass->m_shader->useVectorf(
                                                                         lightPartString + ".color",
                                                                         directionalLightComponent->m_color
                                                                 );

                                                                 geometryPass->m_shader->useFloat(
                                                                         lightPartString + ".intensity",
                                                                         directionalLightComponent->m_intensity
                                                                 );

                                                                 geometryPass->m_shader->useFloat(
                                                                         lightPartString + ".shadowSamplesCount",
                                                                         directionalLightComponent->m_samplesCount
                                                                 );


                                                                 // todo: take into account the type of transformation and the direction of rotation
                                                                 geometryPass->m_shader->useVectorf(
                                                                         renderingPartString + ".position",
                                                                         directionalLightTransform->m_position
                                                                 );

                                                                 geometryPass->m_shader->useMatrix(
                                                                         renderingPartString + ".spaceMatrix",
                                                                         directionalLightComponent->m_spaceMatrix
                                                                 );

                                                                 geometryPass->m_shader->useInteger(
                                                                         directionalLightString + ".shadowMap",
                                                                         shadowsMapsTexturesBlock.m_offset +
                                                                         currentShadowsCaster
                                                                 );

                                                                 directionalLightComponent->m_shadowMap->bindAttachment(
                                                                         SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                                                         shadowsMapsTexturesBlock.m_offset +
                                                                         currentShadowsCaster
                                                                 );

                                                                 m_shader->bind();
                                                                 m_shader->useShaderMarkup(m_shaderMarkup);
                                                                 directionalLightComponent->m_shadowMap->bind()->clear();

                                                                 CoreMain::getRenderer().prepareUniformBuffers(
                                                                         directionalLightComponent, nullptr
                                                                 );
                                                                 m_shader->useUniformBuffer(
                                                                         CoreMain::getRenderer().m_viewMatricesBuffer
                                                                 );

                                                                 SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRender,
                                                                                                  meshesLayer, meshesEntity)
                                                                         Ref<Transform> transformComponent = meshesEntity.getComponent<Transform>();

                                                                         if(!transformComponent) continue;

                                                                         auto meshComponents =
                                                                                 meshesEntity.getComponents<Mesh>();

                                                                         for(const auto& meshComponent: meshComponents)
                                                                         {
                                                                             meshComponent->m_meshData->m_material->bind(
                                                                                     m_shader, m_shaderMarkup
                                                                             );
                                                                             m_shader->useMatrix("objectModelMatrix",
                                                                                                 transformComponent->m_modelMatrix
                                                                             );

                                                                             CoreMain::getRenderer().renderMeshData(
                                                                                     meshComponent->m_meshData,
                                                                                     meshComponent->m_meshDataRenderInfo
                                                                             );
                                                                         }
                                                                 SG_END_ITERATE_CACHED_ENTITIES

                                                                 directionalLightComponent->m_shadowMap->unbind();

                                                                 ++directionalLightsCount;
                                                             }
                                                     SG_END_ITERATE_CACHED_ENTITIES

                                                     geometryPass->m_shader->bind();
                                                     // todo: make name as define
                                                     geometryPass->m_shader->useInteger("DIRECTIONAL_LIGHTS_COUNT",
                                                                                        directionalLightsCount
                                                     );
                                                 }
                                             }
    );
}
