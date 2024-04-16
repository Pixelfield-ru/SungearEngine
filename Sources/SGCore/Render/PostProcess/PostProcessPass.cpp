//
// Created by stuka on 26.11.2023.
//

#include "PostProcessPass.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/IRenderPipeline.h"

SGCore::PostProcessPass::PostProcessPass()
{
    m_postProcessQuadRenderInfo.m_enableFacesCulling = false;

    m_postProcessQuad = Ref<IMeshData>(CoreMain::getRenderer()->createMeshData());

    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(2);
    m_postProcessQuad->m_indices.push_back(1);

    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(3);
    m_postProcessQuad->m_indices.push_back(2);

    m_postProcessQuad->prepare();
}

void SGCore::PostProcessPass::render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline)
{
    CoreMain::getRenderer()->setDepthTestingEnabled(false);

    auto receiversView = scene->getECSRegistry()->view<LayeredFrameReceiver>();

    receiversView.each([this](LayeredFrameReceiver& camera) {
        depthPass(camera);
        FXPass(camera);
        layersCombiningPass(camera);
        finalFrameFXPass(camera);
    });

    CoreMain::getRenderer()->setDepthTestingEnabled(true);
}

// DONE
void SGCore::PostProcessPass::depthPass(LayeredFrameReceiver& camera) const noexcept
{
    auto depthPassShader = camera.m_shader->getSubPassShader("PostProcessLayerDepthPass");

    if(!depthPassShader) return;
    
    depthPassShader->bind();

    std::uint8_t layerIdx = 0;
    
    // binding depth uniforms =================
    for(const auto& ppLayer : camera.getPostProcessLayers())
    {
        depthPassShader->useTextureBlock("layersDepthAttachments[" + std::to_string(layerIdx) + "]", layerIdx);
        ppLayer->m_frameBuffer->bindAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0, layerIdx);
        ++layerIdx;
    }
    
    depthPassShader->useInteger("layersDepthAttachmentsCount", camera.getPostProcessLayers().size());
    
    // =========================================
    
    layerIdx = 0;
    
    for(const auto& ppLayer : camera.getPostProcessLayers())
    {
        depthPassShader->useInteger("currentFBIndex", layerIdx);

        ppLayer->m_frameBuffer->bind();

        for(const auto& attachmentType : ppLayer->m_attachmentsToDepthTest)
        {
            // bind the attachment to which the fragment will be received and into which this fragment will be overwritten
            ppLayer->m_frameBuffer->bindAttachmentToDrawIn(attachmentType);

            CoreMain::getRenderer()->renderMeshData(
                    m_postProcessQuad,
                    m_postProcessQuadRenderInfo
            );
        }
        
        ppLayer->m_frameBuffer->unbind();

        ++layerIdx;
    }
}

// DONE
void SGCore::PostProcessPass::FXPass(SGCore::LayeredFrameReceiver& camera) const noexcept
{
    for(const auto& ppLayer: camera.getPostProcessLayers())
    {
        auto layerShader = ppLayer->m_FXSubPassShader;
        
        if(!layerShader) continue;
        
        layerShader->bind();

        layerShader->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);

        layerShader->useInteger("currentFBIndex", ppLayer->m_index);

        ppLayer->m_frameBuffer->bind();

        for (const auto& ppFXSubPass : ppLayer->m_subPasses)
        {
            layerShader->useInteger("currentSubPass_Idx", ppFXSubPass.m_index);

            if (ppFXSubPass.m_prepareFunction)
            {
                ppFXSubPass.m_prepareFunction(layerShader);
            }
            
            ppLayer->m_frameBuffer->bindAttachmentToDrawIn(ppFXSubPass.m_attachmentRenderTo);

            CoreMain::getRenderer()->renderMeshData(
                    m_postProcessQuad,
                    m_postProcessQuadRenderInfo
            );
        }

        ppLayer->m_frameBuffer->unbind();
    }
}

// DONE
void SGCore::PostProcessPass::layersCombiningPass(LayeredFrameReceiver& camera) const noexcept
{
    auto ppLayerCombiningShader = camera.m_shader->getSubPassShader("PostProcessAttachmentsCombiningPass");

    if(!ppLayerCombiningShader) return;
    
    camera.m_attachmentsForCombining.clear();

    ppLayerCombiningShader->bind();
    camera.m_ppLayersCombinedBuffer->bind();

    // collecting all attachments (from pp layers) to render in
    for(const auto& ppLayer : camera.getPostProcessLayers())
    {
        for(const auto& attachmentsPair : ppLayer->m_attachmentsForCombining)
        {
            camera.m_attachmentsForCombining.insert(attachmentsPair.first);
        }
    }

    // combining all attachments
    // sequentially render each color attachment of the pp layer (if it is specified for the attachmentToRenderIn) to the target layer
    for(const auto& attachmentToRenderIn : camera.m_attachmentsForCombining)
    {
        camera.m_ppLayersCombinedBuffer->bindAttachmentToDrawIn(attachmentToRenderIn);

        std::uint8_t attachmentIdx = 0;

        for(const auto& ppLayer : camera.getPostProcessLayers())
        {
            const auto& foundAttachmentIter = ppLayer->m_attachmentsForCombining.find(attachmentToRenderIn);

            // binding all found layer for target layer (attachmentToRenderIn)
            if(foundAttachmentIter != ppLayer->m_attachmentsForCombining.cend())
            {
                auto foundAttachment = foundAttachmentIter->second;

                ppLayerCombiningShader->useInteger("attachmentsToCopyInCurrentTargetAttachment[" + std::to_string(attachmentIdx) + "]",
                                                              attachmentIdx
                );
                // binding source attachment to texture block
                ppLayer->m_frameBuffer->bindAttachment(foundAttachment, attachmentIdx);

                ++attachmentIdx;
            }
        }

        ppLayerCombiningShader->useInteger("attachmentsToCopyInCurrentTargetAttachmentCount", attachmentIdx);

        CoreMain::getRenderer()->renderMeshData(
                m_postProcessQuad,
                m_postProcessQuadRenderInfo
        );
    }

    camera.m_ppLayersCombinedBuffer->unbind();
}

// DONE
void SGCore::PostProcessPass::finalFrameFXPass(LayeredFrameReceiver& camera) const
{
    auto ppFinalFxShader = camera.m_shader->getSubPassShader("PostProcessFinalFXPass");

    if(!ppFinalFxShader) return;
    
    ppFinalFxShader->bind();

    // binding all attachments from camera.m_ppLayersCombinedBuffer. these attachments are the assembled layer attachments
    std::uint8_t attachmentIdx = 0;
    for(const auto& attachmentType : camera.m_attachmentsForCombining)
    {
        camera.m_ppLayersCombinedBuffer->bindAttachment(attachmentType, attachmentIdx);
        ppFinalFxShader->useInteger("combinedAttachments[" + std::to_string(attachmentIdx) + "]", attachmentIdx);

        ++attachmentIdx;
    }

    ppFinalFxShader->useInteger("combinedAttachmentsCount", attachmentIdx);

    CoreMain::getRenderer()->renderMeshData(
            m_postProcessQuad,
            m_postProcessQuadRenderInfo
    );
    
    // clearing buffers
    for(const auto& ppLayer : camera.getPostProcessLayers())
    {
        ppLayer->m_frameBuffer->bind();
        ppLayer->m_frameBuffer->clear();
        ppLayer->m_frameBuffer->unbind();
    }
}
