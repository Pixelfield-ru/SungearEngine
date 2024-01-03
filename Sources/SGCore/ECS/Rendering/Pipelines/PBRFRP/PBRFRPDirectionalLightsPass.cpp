//
// Created by stuka on 26.11.2023.
//

#include "PBRFRPDirectionalLightsPass.h"
#include "SGCore/ECS/ECSUtils.h"
#include "PBRFRPGeometryPass.h"
#include "SGCore/ECS/Scene.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/Main/CoreMain.h"
#include "PBRFRPDirectionalLight.h"
#include "SGCore/Graphics/API/IShader.h"

void SGCore::PBRFRPDirectionalLightsPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    m_renderTimer.startFrame();

    m_renderTimerCallback->setUpdateFunction([&, renderPipeline]()
                                             {
                                                 auto geometryPass = renderPipeline->getRenderPass<PBRFRPGeometryPass>();

                                                 if (geometryPass || !geometryPass->m_active)
                                                 {
                                                     // ------ updating all lights in geom shader -------------

                                                     size_t directionalLightsCount = 0;

                                                     SG_BEGIN_ITERATE_CACHED_ENTITIES(
                                                             *m_componentsToRenderIn,
                                                             dirLightsLayer, directionalLightEntity)

                                                             auto directionalLightComponents =
                                                                     directionalLightEntity.getComponents<PBRFRPDirectionalLight>();

                                                             auto directionalLightTransform =
                                                                     directionalLightEntity.getComponent<Transform>();

                                                             if (!directionalLightTransform) continue;

                                                             // TODO: OPTIMIZE
                                                             for (const auto& directionalLightComponent: directionalLightComponents)
                                                             {
                                                                 // TODO: MOVE
                                                                 /*geometryPass->m_shader->bind();

                                                                 std::string directionalLightString =
                                                                         "directionalLights[" +
                                                                         std::to_string(directionalLightsCount) + "]";
                                                                 std::string lightPartString =
                                                                         directionalLightString + ".lightPart";
                                                                 std::string renderingPartString =
                                                                         lightPartString + ".renderingPart";

                                                                 geometryPass->m_shader->useVectorf(
                                                                         lightPartString + ".color",
                                                                         directionalLightComponent->m_color
                                                                 );

                                                                 geometryPass->m_shader->useFloat(
                                                                         lightPartString + ".intensity",
                                                                         directionalLightComponent->m_intensity
                                                                 );

                                                                 geometryPass->m_shader->useInteger(
                                                                         lightPartString + ".shadowSamplesCount",
                                                                         directionalLightComponent->m_samplesCount
                                                                 );


                                                                 geometryPass->m_shader->useVectorf(
                                                                         renderingPartString + ".position",
                                                                         directionalLightTransform->m_position
                                                                 );

                                                                 geometryPass->m_shader->useMatrix(
                                                                         renderingPartString + ".spaceMatrix",
                                                                         directionalLightComponent->m_spaceMatrix
                                                                 );*/

                                                                 directionalLightComponent->m_shadowMap->bind()->clear();

                                                                 CoreMain::getRenderer().prepareUniformBuffers(
                                                                         directionalLightComponent, nullptr
                                                                 );

                                                                 SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRender,
                                                                                                  meshesLayer,
                                                                                                  meshesEntity)
                                                                         Ref<Transform> transformComponent = meshesEntity.getComponent<Transform>();

                                                                         if (!transformComponent) continue;

                                                                         auto meshComponents =
                                                                                 meshesEntity.getComponents<Mesh>();

                                                                         for (const auto& meshComponent: meshComponents)
                                                                         {
                                                                             auto meshSubPassShader = meshComponent->m_meshData->m_material->getShader()->getSubPassShader("PBRFRPDirectionalLightsPass");

                                                                             if(!meshSubPassShader) continue;

                                                                             meshSubPassShader->bind();
                                                                             meshSubPassShader->useMatrix("objectModelMatrix",
                                                                                                 transformComponent->m_modelMatrix
                                                                             );

                                                                             meshSubPassShader->useUniformBuffer(
                                                                                     CoreMain::getRenderer().m_viewMatricesBuffer
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

                                                     // TODO: MOVE
                                                     /*geometryPass->m_shader->bind();
                                                     geometryPass->m_shader->useInteger("DIRECTIONAL_LIGHTS_COUNT",
                                                                                        directionalLightsCount
                                                     );*/
                                                 }
                                             }
    );
}
