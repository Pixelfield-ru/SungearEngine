//
// Created by stuka on 26.11.2024.
//

#include "PostProcessPass.h"

#include "PostProcessPass.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Render/LayeredFrameReceiver.h"

SGCore::PostProcessPass::PostProcessPass()
{
    m_renderState.m_useFacesCulling = false;

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
    m_renderState.m_useDepthTest = false;
    m_renderState.m_useStencilTest = false;
    m_renderState.use();

    auto receiversView = scene->getECSRegistry()->view<LayeredFrameReceiver, Ref<RenderingBase>, Ref<Transform>>();

    receiversView.each([this](LayeredFrameReceiver& receiver, Ref<RenderingBase> renderingBase, Ref<Transform> transform) {
        layersFX(receiver);
    });

    m_renderState.m_useDepthTest = true;
    m_renderState.m_useStencilTest = true;
    m_renderState.use();
}

void SGCore::PostProcessPass::layersFX(LayeredFrameReceiver& receiver) noexcept
{
    receiver.m_layersFXFrameBuffer->bind();

    for(const auto& layer : receiver.getLayers())
    {
        auto layerShader = layer->getFXSubPassShader();
        if(!layerShader) continue;

        layerShader->bind();

        bindCommonUniforms(receiver, layerShader);

        layerShader->useInteger("SGPP_CurrentLayerIndex", layer->getIndex());

        for(size_t i = 0; i < layer->m_subPasses.size(); ++i)
        {
            const auto& fxSubPass = layer->m_subPasses[i];

            layerShader->useInteger("SGPP_CurrentFXSubPassSeqIndex", i);

            receiver.m_layersFXFrameBuffer->bindAttachmentToDrawIn(fxSubPass.m_attachmentRenderTo);
            // receiver.m_layersFrameBuffer->clearAttachment(fxSubPass.m_attachmentRenderTo);

            CoreMain::getRenderer()->renderMeshData(
                    m_postProcessQuad.get(),
                    m_renderState
            );

            // receiver.m_layersFXFrameBuffer->unbindAttachmentToDrawIn();
        }
    }

    receiver.m_layersFXFrameBuffer->unbind();
}

void SGCore::PostProcessPass::bindCommonUniforms(LayeredFrameReceiver& receiver,
                                                 const SGCore::Ref<SGCore::IShader>& subPassShader) const noexcept
{
    for(std::uint8_t i = 0; i < receiver.getLayers().size(); ++i)
    {
        const auto layer = receiver.getLayers()[i];
        subPassShader->useInteger("SGPP_LayersIndices[" + std::to_string(i) + "]", layer->getIndex());
    }

    subPassShader->useInteger("SGPP_LayersCount", receiver.getLayers().size());

    subPassShader->useTextureBlock("SGPP_LayersVolumes", 0);
    subPassShader->useTextureBlock("SGPP_LayersColors", 1);

    receiver.m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0)->bind(0);
    receiver.m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1)->bind(1);
}
