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
#include "SGCore/Graphics/API/ITexture2D.h"

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
    auto depthPassShader = camera.m_shader->getSubPassShader("SGLPPLayerDepthPass");

    if(!depthPassShader) return;
    
    depthPassShader->bind();
    
    bindLayersIndices(camera, depthPassShader);

    std::uint8_t layerIdx = 0;
    
    // binding depth uniforms =================
    for(const auto& ppLayer : camera.getLayers())
    {
        depthPassShader->useTextureBlock("SGLPP_LayersDepthAttachments[" + std::to_string(layerIdx) + "]", layerIdx);
        if(ppLayer->m_frameBuffer->getAttachments().contains(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0))
        {
            ppLayer->m_frameBuffer->bindAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0, layerIdx);
            ++layerIdx;
        }
    }
    
    depthPassShader->useInteger("SGLPP_LayersDepthAttachmentsCount", layerIdx);
    
    // =========================================
    
    layerIdx = 0;
    
    for(const auto& ppLayer : camera.getLayers())
    {
        depthPassShader->useInteger("SGLPP_CurrentLayerSeqIndex", layerIdx);
        depthPassShader->useInteger("SGLPP_CurrentLayerIndex", ppLayer->m_index);

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
    std::uint16_t layerIdx = 0;
    
    for(const auto& ppLayer: camera.getLayers())
    {
        if(ppLayer->m_subPasses.empty())
        {
            ++layerIdx;
            continue;
        }
        
        auto layerShader = ppLayer->m_FXSubPassShader;
        
        if(!layerShader)
        {
            ++layerIdx;
            continue;
        }
        
        layerShader->bind();
        
        layerShader->useInteger("SGLPP_CurrentLayerSeqIndex", layerIdx);
        
        layerShader->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);

        // std::cout << "layer name: " << ppLayer->m_name << ", index: " << ppLayer->m_index << std::endl;
        
        layerShader->bindTextureBindings(0);
        
        bindLayersIndices(camera, layerShader);
        
        layerShader->useInteger("SGLPP_CurrentLayerIndex", ppLayer->m_index);
        layerShader->useInteger("SGLPP_LayersCount", camera.getLayers().size());

        ppLayer->m_frameBuffer->bind();
        
        std::uint16_t currentSubPassIdx = 0;
        
        for (const auto& ppFXSubPass : ppLayer->m_subPasses)
        {
            layerShader->useInteger("SGLPP_CurrentSubPassIndex", currentSubPassIdx);

            if (ppFXSubPass.m_prepareFunction)
            {
                ppFXSubPass.m_prepareFunction(layerShader);
            }
            
            ppLayer->m_frameBuffer->bindAttachmentToDrawIn(ppFXSubPass.m_attachmentRenderTo);

            CoreMain::getRenderer()->renderMeshData(
                    m_postProcessQuad,
                    m_postProcessQuadRenderInfo
            );
            
            ++currentSubPassIdx;
        }

        ppLayer->m_frameBuffer->unbind();
        
        ++layerIdx;
    }
}

// DONE
void SGCore::PostProcessPass::layersCombiningPass(LayeredFrameReceiver& camera) const noexcept
{
    auto ppLayerCombiningShader = camera.m_shader->getSubPassShader("SGLPPAttachmentsCombiningPass");

    if(!ppLayerCombiningShader) return;
    
    camera.m_attachmentsForCombining.clear();

    ppLayerCombiningShader->bind();
    camera.m_layersCombinedBuffer->bind();

    // collecting all attachments (from pp layers) to render in
    for(const auto& ppLayer : camera.getLayers())
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
        camera.m_layersCombinedBuffer->bindAttachmentToDrawIn(attachmentToRenderIn);

        std::uint8_t attachmentIdx = 0;

        for(const auto& ppLayer : camera.getLayers())
        {
            const auto& foundAttachmentIter = ppLayer->m_attachmentsForCombining.find(attachmentToRenderIn);

            // binding all found layer for target layer (attachmentToRenderIn)
            if(foundAttachmentIter != ppLayer->m_attachmentsForCombining.cend())
            {
                auto foundAttachment = foundAttachmentIter->second;

                ppLayerCombiningShader->useInteger("SGLPP_AttachmentsToCopyInCurrentTargetAttachment[" + std::to_string(attachmentIdx) + "]",
                                                              attachmentIdx
                );
                // binding source attachment to texture block
                ppLayer->m_frameBuffer->bindAttachment(foundAttachment, attachmentIdx);

                ++attachmentIdx;
            }
        }

        ppLayerCombiningShader->useInteger("SGLPP_AttachmentsToCopyInCurrentTargetAttachmentCount", attachmentIdx);

        CoreMain::getRenderer()->renderMeshData(
                m_postProcessQuad,
                m_postProcessQuadRenderInfo
        );
    }

    camera.m_layersCombinedBuffer->unbind();
}

// DONE
void SGCore::PostProcessPass::finalFrameFXPass(LayeredFrameReceiver& camera) const
{
    auto ppFinalFXShader = camera.m_finalFrameFXShader;

    if(!ppFinalFXShader) return;
    
    ppFinalFXShader->bind();

    // binding all attachments from camera.m_ppLayersCombinedBuffer. these attachments are the assembled layer attachments
    ppFinalFXShader->bindTextureBindings(0);

    CoreMain::getRenderer()->renderMeshData(
            m_postProcessQuad,
            m_postProcessQuadRenderInfo
    );
    
    // clearing buffers
    for(const auto& ppLayer : camera.getLayers())
    {
        ppLayer->m_frameBuffer->bind();
        ppLayer->m_frameBuffer->clear();
        ppLayer->m_frameBuffer->unbind();
    }
}

void SGCore::PostProcessPass::bindLayersIndices(SGCore::LayeredFrameReceiver& camera,
                                                const SGCore::Ref<SGCore::ISubPassShader>& subPassShader) const noexcept
{
    std::uint8_t layerIdx = 0;
    
    for(const auto& ppLayer : camera.getLayers())
    {
        subPassShader->useInteger("SGLPP_LayersIndices[" + std::to_string(layerIdx) + "]", ppLayer->m_index);
        ++layerIdx;
    }
    subPassShader->useInteger("SGLPP_LayersCount", layerIdx);
}
