//
// Created by stuka on 26.11.2023.
//

#include "PostProcessFXPass.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/ECS/Rendering/Camera.h"

void SGCore::PostProcessFXPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    CoreMain::getRenderer().setDepthTestingEnabled(false);

    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRenderIn, camerasLayer, cameraEntity)
            auto cameraComponent = cameraEntity.getComponent<Camera>();

            if(!cameraComponent) continue;

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
    SG_END_ITERATE_CACHED_ENTITIES

    CoreMain::getRenderer().setDepthTestingEnabled(true);
}
