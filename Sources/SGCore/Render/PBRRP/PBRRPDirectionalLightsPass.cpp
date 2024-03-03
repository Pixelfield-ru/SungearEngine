//
// Created by stuka on 26.11.2023.
//

#include <glm/gtc/type_ptr.hpp>

#include "PBRRPDirectionalLightsPass.h"
#include "PBRRPGeometryPass.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Lighting/DirectionalLight.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Render/Mesh.h"

void SGCore::PBRRPDirectionalLightsPass::create(const SGCore::Ref<SGCore::IRenderPipeline>& parentRenderPipeline)
{
    m_renderTimer.setTargetFrameRate(24);
    
    m_renderTimer.addCallback(m_renderTimerCallback);
}

void SGCore::PBRRPDirectionalLightsPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    m_renderTimer.startFrame();

    m_renderTimerCallback->setUpdateFunction([&](const double& dt, const double& fixedDt) {
        // todo: shadows
        
        // std::cout << "endupd : " << std::endl;
    });

    /*m_renderTimerCallback->setUpdateFunction([&, renderPipeline]()
                                             {
                                                 auto geometryPass = renderPipeline->getRenderPass<PBRRPGeometryPass>();

                                                 if (geometryPass || !geometryPass->m_active)
                                                 {
                                                     // ------ updating all lights in geom shader -------------

                                                     size_t directionalLightsCount = 0;

                                                     SG_BEGIN_ITERATE_CACHED_ENTITIES(
                                                             *m_componentsToRenderIn,
                                                             dirLightsLayer, directionalLightEntity)

                                                             auto directionalLightComponents =
                                                                     directionalLightEntity.getComponents<PBRRPDirectionalLight>();

                                                             auto directionalLightTransform =
                                                                     directionalLightEntity.getComponent<TransformBase>();

                                                             if (!directionalLightTransform) continue;

                                                             // TODO: OPTIMIZE
                                                             for (const auto& directionalLightComponent: directionalLightComponents)
                                                             {
                                                                 // TODO: MOVE
                                                                 *//*geometryPass->m_shader->bind();

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
                                                                         directionalLightComponent->m_projectionSpaceMatrix
                                                                 );*//*

                                                                 directionalLightComponent->m_shadowMap->bind()->clear();

                                                                 CoreMain::getRenderer()->prepareUniformBuffers(
                                                                         directionalLightComponent, nullptr
                                                                 );

                                                                 SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRender,
                                                                                                  meshesLayer,
                                                                                                  meshesEntity)
                                                                         Ref<TransformBase> transformComponent = meshesEntity.getComponent<TransformBase>();

                                                                         if (!transformComponent) continue;

                                                                         auto meshComponents =
                                                                                 meshesEntity.getComponents<Mesh>();

                                                                         for (const auto& meshComponent: meshComponents)
                                                                         {
                                                                             auto meshSubPassShader = meshComponent->m_meshData->m_material->getShader()->getSubPassShader("PBRRPDirectionalLightsPass");

                                                                             if(!meshSubPassShader) continue;

                                                                             meshSubPassShader->bind();
                                                                             meshSubPassShader->useMatrix("objectModelMatrix",
                                                                                                 transformComponent->m_modelMatrix
                                                                             );

                                                                             meshSubPassShader->useUniformBuffer(
                                                                                     CoreMain::getRenderer()->m_viewMatricesBuffer
                                                                             );

                                                                             CoreMain::getRenderer()->renderMeshData(
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
                                                     *//*geometryPass->m_shader->bind();
                                                     geometryPass->m_shader->useInteger("DIRECTIONAL_LIGHTS_COUNT",
                                                                                        directionalLightsCount
                                                     );*//*
                                                 }
                                             }
    );*/
}
