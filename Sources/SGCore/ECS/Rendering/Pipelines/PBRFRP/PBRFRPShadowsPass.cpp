//
// Created by stuka on 25.11.2023.
//

#include "PBRFRPShadowsPass.h"
#include "PBRFRPGeometryPass.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCaster.h"
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/Mesh.h"

SGCore::PBRFRPShadowsPass::PBRFRPShadowsPass() noexcept
{
    m_renderTimer.m_useFixedUpdateCatchUp = false;
    m_renderTimer.m_targetFrameRate = 24;

    m_renderTimer.addCallback(m_renderTimerCallback);
}

void SGCore::PBRFRPShadowsPass::render(const Ref<IRenderPipeline>& renderPipeline)
{
    m_renderTimer.startFrame();

    m_renderTimerCallback->setUpdateFunction([&, renderPipeline]()
                                             {
                                                 Ref<PBRFRPGeometryPass> geometryPass = renderPipeline->getRenderPass<PBRFRPGeometryPass>();

                                                 m_shader->bind();
                                                 m_shader->useShaderMarkup(m_shaderMarkup);
                                                 m_shader->useUniformBuffer(
                                                         CoreMain::getRenderer().m_viewMatricesBuffer
                                                 );

                                                 size_t shadowsCastersCount = 0;

                                                 SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRenderIn,
                                                                                  shadowsCastersLayer,
                                                                                  shadowsCasterEntity)
                                                         // todo: make process all ShadowsCasterComponent (cachedEntities.second->getComponents)
                                                         Ref<ShadowsCaster> shadowsCasterComponent = shadowsCasterEntity.getComponent<ShadowsCaster>();
                                                         if(!shadowsCasterComponent) continue;
                                                         Ref<Transform> shadowsCasterTransform = shadowsCasterEntity.getComponent<Transform>();
                                                         if(!shadowsCasterTransform) continue;

                                                         if(geometryPass || !geometryPass->m_active)
                                                         {
                                                             std::string currentShadowCasterStr = "shadowsCasters[";
                                                             currentShadowCasterStr += std::to_string(
                                                                     shadowsCastersCount
                                                             );
                                                             currentShadowCasterStr += "]";

                                                             // updating shadows casters uniforms of geometry shader
                                                             geometryPass->m_shader->bind();

                                                             geometryPass->m_shader->useMatrix(
                                                                     currentShadowCasterStr + ".shadowsCasterSpace",
                                                                     shadowsCasterComponent->m_spaceMatrix
                                                             );

                                                             geometryPass->m_shader->useVectorf(
                                                                     currentShadowCasterStr + ".position",
                                                                     shadowsCasterTransform->m_position
                                                             );
                                                         }

                                                         // binding frame buffer of shadow caster and rendering in
                                                         shadowsCasterComponent->m_frameBuffer->bind()->clear();

                                                         m_shader->bind();
                                                         CoreMain::getRenderer().prepareUniformBuffers(
                                                                 shadowsCasterComponent, nullptr
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

                                                         shadowsCasterComponent->m_frameBuffer->unbind();

                                                         shadowsCastersCount++;
                                                 SG_END_ITERATE_CACHED_ENTITIES

                                                 if(geometryPass || !geometryPass->m_active)
                                                 {
                                                     geometryPass->m_shader->bind();
                                                     geometryPass->m_shader->useInteger(
                                                             sgStandardTextureTypeToString(
                                                                     SGTextureType::SGTP_SHADOW_MAP
                                                             ) + "_COUNT",
                                                             shadowsCastersCount
                                                     );
                                                 }
                                             }
    );
}
