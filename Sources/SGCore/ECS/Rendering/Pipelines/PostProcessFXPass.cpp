//
// Created by stuka on 26.11.2023.
//

#include "PostProcessFXPass.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"

SGCore::PostProcessFXPass::PostProcessFXPass()
{
    m_postProcessQuadRenderInfo.m_enableFacesCulling = false;

    m_postProcessQuad = Ref<IMeshData>(CoreMain::getRenderer().createMeshData());

    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(2);
    m_postProcessQuad->m_indices.push_back(1);

    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(3);
    m_postProcessQuad->m_indices.push_back(2);

    m_postProcessQuad->prepare();
}

void SGCore::PostProcessFXPass::render(const Ref<Scene>& scene, const SGCore::Ref<SGCore::IRenderPipeline>& renderPipeline)
{
    CoreMain::getRenderer().setDepthTestingEnabled(false);

    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsToRenderIn, camerasLayer, cameraEntity)
            auto cameraComponent = cameraEntity.getComponent<Camera>();

            if(!cameraComponent) continue;

            cameraComponent->bindPostProcessLayers();

            depthPass(cameraComponent);
            FXPass(cameraComponent);
            GBufferCombiningPass(cameraComponent);
            finalFrameFXPass(cameraComponent);

            // ====================================================================
            // first - discard not visible fragments in every frame buffer of layer and apply PP FX for each PP layer

            cameraComponent->m_defaultPostProcessFXShader->bind();
            cameraComponent->m_defaultPostProcessFXShader
                    ->useShaderMarkup(cameraComponent->m_postProcessShadersMarkup);

            // ====================================================================
            // first depth test for pixels in default FB

            cameraComponent->m_defaultPostProcessFXShader->useInteger("currentFBIndex", 0);

            cameraComponent->m_defaultLayersFrameBuffer->bind();

            {
                // YOU DO NOT NEED TO CHANGE DEPTH PASS IN 99% OF CASES
                // first pass - depth pass ------------------------------------------

                cameraComponent->m_defaultLayersFrameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT0);

                cameraComponent->m_defaultPostProcessFXShader->useInteger("isDepthTestPass", true);

                CoreMain::getRenderer().renderMeshData(
                        m_postProcessQuad,
                        m_postProcessQuadRenderInfo
                );

                // todo: make subpasses
                // second pass - FX pass --------------------------------------------

                /*cameraComponent->m_defaultLayersFrameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT1);

                cameraComponent->m_defaultPostProcessShader->useInteger("isDepthTestPass", false);

                CoreMain::getRenderer().renderMeshData(
                        cameraComponent->m_postProcessQuad,
                        cameraComponent->m_postProcessQuadRenderInfo
                );*/
            }

            cameraComponent->m_defaultLayersFrameBuffer->unbind();

            // ---------------------------------
            // ====================================================================

            // and then depth test for PP Layers

            for(const auto& ppLayer: cameraComponent->getPostProcessLayers())
            {
                auto& layerShader = ppLayer.second.m_FXShader;

                layerShader->bind();
                layerShader->useShaderMarkup(cameraComponent->m_postProcessShadersMarkup);

                layerShader->useInteger("currentFBIndex", ppLayer.second.m_index);

                ppLayer.second.m_frameBuffer->bind();

                {
                    // YOU DO NOT NEED TO CHANGE DEPTH PASS IN 99% OF CASES
                    // first pass - depth pass ------------------------------------------

                    ppLayer.second.m_frameBuffer->bindAttachmentToDraw(SGG_COLOR_ATTACHMENT0);

                    layerShader->useInteger("isDepthTestPass", true);

                    CoreMain::getRenderer().renderMeshData(
                            m_postProcessQuad,
                            m_postProcessQuadRenderInfo
                    );

                    // second pass - FX pass --------------------------------------------

                    layerShader->useInteger("isDepthTestPass", false);

                    for(const auto& ppFXSubPass : ppLayer.second.m_subPasses)
                    {
                        layerShader->useInteger("currentSubPass_Idx", ppFXSubPass.m_index);

                        if(ppFXSubPass.m_prepareFunction)
                        {
                            ppFXSubPass.m_prepareFunction(layerShader);
                        }

                        ppLayer.second.m_frameBuffer->bindAttachmentToDraw(ppFXSubPass.m_attachmentRenderTo);

                        CoreMain::getRenderer().renderMeshData(
                                m_postProcessQuad,
                                m_postProcessQuadRenderInfo
                        );
                    }
                }

                ppLayer.second.m_frameBuffer->unbind();
            }

            // --------------------------------------------------------------------

            // --------------------------------------------------------------------

            // render post-process quad -------------------------------------------

            if(cameraComponent->m_useFinalFrameBuffer)
            {
                cameraComponent->m_finalFrameFXFrameBuffer->bind()->clear();
            }

            cameraComponent->m_finalPostProcessFXShader->bind();
            cameraComponent->m_finalPostProcessFXShader
                    ->useShaderMarkup(cameraComponent->m_postProcessShadersMarkup);


            cameraComponent->m_finalPostProcessFXShader->useInteger("allFB[0].colorAttachmentToRenderIdx",
                                                                         cameraComponent->m_attachmentToUseInFinalOverlay -
                                                                         SGG_COLOR_ATTACHMENT0);

            std::uint16_t currentPPLayerIdx = 1;
            for(const auto& ppLayer: cameraComponent->getPostProcessLayers())
            {
                cameraComponent->m_finalPostProcessFXShader->useInteger(
                        "allFB[" + std::to_string(currentPPLayerIdx) + "].colorAttachmentToRenderIdx",
                        ppLayer.second.m_attachmentToUseInFinalOverlay - SGG_COLOR_ATTACHMENT0);

                ++currentPPLayerIdx;
            }

            std::uint8_t gBufferAttachmentsOffset = cameraComponent->m_postProcessShadersMarkup.getGBufferAttachmentsOffset();
            for(std::uint8_t blockIdx = 0; blockIdx < cameraComponent->m_postProcessShadersMarkup.m_gBufferAttachmentsCount; ++blockIdx)
            {
                auto resAttachment = (SGFrameBufferAttachmentType) ((std::uint8_t) SGG_COLOR_ATTACHMENT1 + blockIdx);
                cameraComponent->m_gBuffer->bindAttachment(resAttachment, gBufferAttachmentsOffset + blockIdx);
            }

            CoreMain::getRenderer().renderMeshData(
                    m_postProcessQuad,
                    m_postProcessQuadRenderInfo
            );

            // -------------------------------------
    SG_END_ITERATE_CACHED_ENTITIES

    CoreMain::getRenderer().setDepthTestingEnabled(true);
}

// DONE
void SGCore::PostProcessFXPass::depthPass(const SGCore::Ref<SGCore::Camera>& camera)
{
    camera->m_depthPassShader->bind();
    camera->m_depthPassShader
            ->useShaderMarkup(camera->m_postProcessShadersMarkup);

    std::uint8_t layerIdx = 0;
    for(const auto& ppLayerPair : camera->getPostProcessLayers())
    {
        const auto& ppLayer = ppLayerPair.second;

        camera->m_depthPassShader->useInteger("currentFBIndex", layerIdx);

        ppLayer.m_frameBuffer->bind();

        for(std::uint8_t attachmentIdx = 0; attachmentIdx < ppLayer.m_attachmentsToDepthTest.size(); ++attachmentIdx)
        {
            ppLayer.m_frameBuffer->bindAttachmentToDraw(ppLayer.m_attachmentsToDepthTest[attachmentIdx]);

            CoreMain::getRenderer().renderMeshData(
                    m_postProcessQuad,
                    m_postProcessQuadRenderInfo
            );
        }

        ++layerIdx;
    }
}

// DONE
void SGCore::PostProcessFXPass::FXPass(const SGCore::Ref<SGCore::Camera>& camera)
{
    for(const auto& ppLayerPair: camera->getPostProcessLayers())
    {
        const auto& ppLayer = ppLayerPair.second;

        auto& layerShader = ppLayer.m_FXShader;

        layerShader->bind();
        layerShader->useShaderMarkup(camera->m_postProcessShadersMarkup);

        layerShader->useInteger("currentFBIndex", ppLayer.m_index);

        ppLayer.m_frameBuffer->bind();

        for (const auto& ppFXSubPass: ppLayer.m_subPasses)
        {
            layerShader->useInteger("currentSubPass_Idx", ppFXSubPass.m_index);

            if (ppFXSubPass.m_prepareFunction)
            {
                ppFXSubPass.m_prepareFunction(layerShader);
            }

            ppLayer.m_frameBuffer->bindAttachmentToDraw(ppFXSubPass.m_attachmentRenderTo);

            CoreMain::getRenderer().renderMeshData(
                    m_postProcessQuad,
                    m_postProcessQuadRenderInfo
            );
        }


        ppLayer.m_frameBuffer->unbind();
    }
}

// DONE
void SGCore::PostProcessFXPass::GBufferCombiningPass(const SGCore::Ref<SGCore::Camera>& camera)
{
    camera->m_gBufferCombiningShader->bind();
    camera->m_combinedGBuffer->bind();

    for(const auto& attachmentType : camera->m_attachmentsToCombine)
    {
        camera->m_combinedGBuffer->bindAttachmentToDraw(attachmentType);

        std::uint8_t attachmentIdx = 0;

        for(const auto& ppLayerPair : camera->getPostProcessLayers())
        {
            const auto& ppLayer = ppLayerPair.second;

            // todo:
            camera->m_gBufferCombiningShader->useInteger("layersAttachmentN[" + std::to_string(attachmentIdx) + "]", attachmentIdx);
            ppLayer.m_frameBuffer->bindAttachment(attachmentType, attachmentIdx);

            ++attachmentIdx;
        }

        camera->m_gBufferCombiningShader->useInteger("layersAttachmentNCount", attachmentIdx);

        CoreMain::getRenderer().renderMeshData(
                m_postProcessQuad,
                m_postProcessQuadRenderInfo
        );
    }
}

// TODO::
void SGCore::PostProcessFXPass::finalFrameFXPass(const SGCore::Ref<SGCore::Camera>& camera)
{

}
