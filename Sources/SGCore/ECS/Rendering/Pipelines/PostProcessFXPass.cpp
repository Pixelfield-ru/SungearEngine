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
            layersCombiningPass(cameraComponent);
            finalFrameFXPass(cameraComponent);

    SG_END_ITERATE_CACHED_ENTITIES

    CoreMain::getRenderer().setDepthTestingEnabled(true);
}

// DONE
void SGCore::PostProcessFXPass::depthPass(const SGCore::Ref<SGCore::Camera>& camera) const noexcept
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

        for(const auto& attachmentType : ppLayer.m_attachmentsToDepthTest)
        {
            ppLayer.m_frameBuffer->bindAttachmentToDraw(attachmentType);

            CoreMain::getRenderer().renderMeshData(
                    m_postProcessQuad,
                    m_postProcessQuadRenderInfo
            );
        }

        ++layerIdx;
    }
}

// DONE
void SGCore::PostProcessFXPass::FXPass(const SGCore::Ref<SGCore::Camera>& camera) const noexcept
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
void SGCore::PostProcessFXPass::layersCombiningPass(const Ref<Camera>& camera) const noexcept
{
    camera->m_attachmentsForCombining.clear();

    camera->m_ppLayersCombiningShader->bind();
    camera->m_ppLayersCombinedBuffer->bind();

    // collecting all attachment to render in
    for(const auto& ppLayerPair : camera->getPostProcessLayers())
    {
        const auto& ppLayer = ppLayerPair.second;

        for(const auto& attachmentsPair : ppLayer.m_attachmentsForCombining)
        {
            camera->m_attachmentsForCombining.insert(attachmentsPair.first);
        }
    }

    // combining all attachments
    for(const auto& attachmentToRenderIn : camera->m_attachmentsForCombining)
    {
        camera->m_ppLayersCombinedBuffer->bindAttachmentToDraw(attachmentToRenderIn);

        std::uint8_t attachmentIdx = 0;

        for(const auto& ppLayerPair : camera->getPostProcessLayers())
        {
            const auto& ppLayer = ppLayerPair.second;

            const auto& foundAttachmentIter = ppLayer.m_attachmentsForCombining.find(attachmentToRenderIn);

            if(foundAttachmentIter != ppLayer.m_attachmentsForCombining.cend())
            {
                auto foundAttachment = foundAttachmentIter->second;

                camera->m_ppLayersCombiningShader->useInteger("layersAttachmentN[" + std::to_string(attachmentIdx) + "]",
                                                              attachmentIdx
                );
                ppLayer.m_frameBuffer->bindAttachment(foundAttachment, attachmentIdx);

                ++attachmentIdx;
            }
        }

        camera->m_ppLayersCombiningShader->useInteger("layersAttachmentNCount", attachmentIdx);

        CoreMain::getRenderer().renderMeshData(
                m_postProcessQuad,
                m_postProcessQuadRenderInfo
        );
    }

    camera->m_ppLayersCombinedBuffer->unbind();
}

// DONE
void SGCore::PostProcessFXPass::finalFrameFXPass(const SGCore::Ref<SGCore::Camera>& camera) const
{
    camera->m_finalPostProcessFXShader->bind();

    std::uint8_t attachmentIdx = 0;
    for(const auto& attachmentType : camera->m_attachmentsForCombining)
    {
        camera->m_ppLayersCombinedBuffer->bindAttachment(attachmentType, attachmentIdx);
        camera->m_finalPostProcessFXShader->useInteger("combinedBuffer[" + std::to_string(attachmentIdx) + "]", attachmentIdx);

        ++attachmentIdx;
    }

    camera->m_finalPostProcessFXShader->useInteger("combinedBufferAttachmentsCount", attachmentIdx);

    CoreMain::getRenderer().renderMeshData(
            m_postProcessQuad,
            m_postProcessQuadRenderInfo
    );
}
