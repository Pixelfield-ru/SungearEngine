//
// Created by stuka on 31.10.2023.
//
#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "LayeredFrameReceiver.h"

#include "SGCore/Utils/DefaultShadersPaths.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/ShadersPaths.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Scene/Layer.h"
#include "RenderPipelinesManager.h"

SGCore::LayeredFrameReceiver::LayeredFrameReceiver()
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
    
    m_defaultLayer = addLayer("___DEFAULT_LAYER___");
    
    m_shader = MakeRef<IShader>();
    m_shader->addSubPassShadersAndCompile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
            DefaultShadersPaths::getPaths()["LayeredPP"]["ReceiverShader"].getCurrentRealization()));

    // ==================================================================

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);
    
    m_layersCombinedBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer());
    m_layersCombinedBuffer->setSize(primaryMonitorWidth, primaryMonitorHeight);
    m_layersCombinedBuffer->create();
    m_layersCombinedBuffer->bind();
    m_layersCombinedBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB8,
            0,
            0
    );
    
    m_layersCombinedBuffer->unbind();

    // m_defaultLayersFrameBuffer->m_bgColor.a = 0.0;

    m_finalFrameFXFrameBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer());
    m_finalFrameFXFrameBuffer->create();
    m_finalFrameFXFrameBuffer->bind();
    m_finalFrameFXFrameBuffer->setSize(primaryMonitorWidth, primaryMonitorHeight);
    m_finalFrameFXFrameBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                                             SGGColorFormat::SGG_RGB,
                                             SGGColorInternalFormat::SGG_RGB8,
                                             0,
                                             0);
    m_finalFrameFXFrameBuffer->unbind();
    
    auto finalFrameFXShader = MakeRef<IShader>();
    finalFrameFXShader->addSubPassShadersAndCompile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
            DefaultShadersPaths::getPaths()["LayeredPP"]["FinalFrameFXShader"].getCurrentRealization()));
    
    m_finalFrameFXShader = finalFrameFXShader->getSubPassShader("SGLPPFinalFXPass");
    
    m_finalFrameFXShader->addTextureBinding("SGLPP_CombinedAttachments[0]", m_layersCombinedBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
}

SGCore::Ref<SGCore::PostProcessLayer> SGCore::LayeredFrameReceiver::addOrGetLayer(const std::string& name,
                                                                                  const std::uint16_t& fbWidth,
                                                                                  const std::uint16_t& fbHeight)
{
    auto foundPPLayer = getLayer(name);

    if(foundPPLayer)
    {
        LOG_E(SGCORE_TAG,
              "Error: can not add a new post-process layer to the camera. This layer already exists.\n{}",
              SGCore::Utils::sourceLocationToString(std::source_location::current()));
        return foundPPLayer;
    }

    m_layers.emplace_back(MakeRef<PostProcessLayer>());
    
    auto& newPPLayer = *m_layers.rbegin();
    // without - 1 because 0 is always default FB
    newPPLayer->m_index = m_layers.empty() ? 0 : getLayersMaximumIndex() + 1;
    
    newPPLayer->m_name = name;
    newPPLayer->m_frameBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer());
    newPPLayer->m_frameBuffer->create();
    newPPLayer->m_frameBuffer->bind();
    newPPLayer->m_frameBuffer->setSize(fbWidth, fbHeight);
    newPPLayer->m_frameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
            SGGColorFormat::SGG_DEPTH_COMPONENT,
            SGGColorInternalFormat::SGG_DEPTH_COMPONENT32,
            0,
            0);
    newPPLayer->m_frameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, // NOT DEPT-TESTED ATTACHMENT
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB8,
            0,
            0
    );
    newPPLayer->m_frameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1, // DEPTH-TESTED ATTACHMENT
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB8,
            0,
            0
    );
    
    newPPLayer->m_frameBuffer->unbind();

    auto fxShader = MakeRef<IShader>();
    fxShader->addSubPassShadersAndCompile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
            DefaultShadersPaths::getPaths()["LayeredPP"]["LayerFXShader"].getCurrentRealization()));
    
    newPPLayer->m_FXSubPassShader = fxShader->getSubPassShader("SGLPPLayerFXPass");

    return newPPLayer;
}

SGCore::Ref<SGCore::PostProcessLayer> SGCore::LayeredFrameReceiver::addLayer(const std::string& name)
{
    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    return addOrGetLayer(name, primaryMonitorWidth, primaryMonitorHeight);
}

void SGCore::LayeredFrameReceiver::removeLayer(const std::string& name) noexcept
{
    std::erase_if(m_layers, [&name](const Ref<PostProcessLayer>& layer) {
        return layer->m_name == name;
    });
}

void SGCore::LayeredFrameReceiver::setLayerShader(const std::string& name,
                                                  const Ref<ISubPassShader>& shader) noexcept
{
    auto foundLayer = getLayer(name);
    
    if(!foundLayer)
    {
        LOG_E(SGCORE_TAG,
              "Error: can not add a new post-process layer to the camera. This layer does not exist as post-processing layer.\n{}",
              Utils::sourceLocationToString(std::source_location::current()));
        return;
    }
    
    foundLayer->m_FXSubPassShader = shader;
}

SGCore::Ref<SGCore::PostProcessLayer> SGCore::LayeredFrameReceiver::getLayer(const std::string& name) noexcept
{
    auto it = std::find_if(m_layers.begin(), m_layers.end(), [&](const Ref<PostProcessLayer>& layer) {
        return layer->m_name == name;
    });
    
    return it != m_layers.end() ? *it : nullptr;
}

SGCore::Ref<SGCore::PostProcessLayer> SGCore::LayeredFrameReceiver::getDefaultLayer() noexcept
{
    return m_defaultLayer;
}

std::uint16_t SGCore::LayeredFrameReceiver::getLayersMaximumIndex() const noexcept
{
    std::uint16_t maxIdx = 0;
    
    for(const auto& layer : m_layers)
    {
        if(layer->m_index > maxIdx)
        {
            maxIdx = layer->m_index;
        }
    }
    
    return maxIdx;
}

void SGCore::LayeredFrameReceiver::clearPostProcessFrameBuffers() const noexcept
{
    for(const auto& ppLayer : m_layers)
    {
        // ppLayer.second.m_frameBuffer->bind()->clear();
        ppLayer->m_frameBuffer->bind();
        ppLayer->m_frameBuffer->bindAttachmentsToDrawIn(
                std::vector<SGFrameBufferAttachmentType> { SGG_COLOR_ATTACHMENT0, SGG_COLOR_ATTACHMENT1,
                                                           SGG_COLOR_ATTACHMENT2, SGG_COLOR_ATTACHMENT3,
                                                           SGG_COLOR_ATTACHMENT4, SGG_COLOR_ATTACHMENT5,
                                                           SGG_COLOR_ATTACHMENT6, SGG_COLOR_ATTACHMENT7 });
        ppLayer->m_frameBuffer->clear();
    }
    
    m_layersCombinedBuffer->bind();
    m_layersCombinedBuffer->bindAttachmentsToDrawIn(m_attachmentsForCombining);
    m_layersCombinedBuffer->clear();
    
    m_finalFrameFXFrameBuffer->bind();
    m_finalFrameFXFrameBuffer->clear();
    m_finalFrameFXFrameBuffer->unbind();
}

void SGCore::LayeredFrameReceiver::attachmentDepthPass
(const SGCore::Ref<SGCore::PostProcessLayer>& layer, SGFrameBufferAttachmentType attachmentType) noexcept
{
    auto depthPassShader = m_shader->getSubPassShader("SGLPPLayerDepthPass");
    
    if(!depthPassShader) return;
    
    depthPassShader->bind();
    
    depthPassShader->useInteger("SGLPP_CurrentLayerIndex", layer->m_index);
    
    std::uint8_t layerIdx = 0;
    
    // binding depth uniforms =================
    for(const auto& ppLayer : m_layers)
    {
        if(ppLayer->m_frameBuffer->getAttachments().contains(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0))
        {
            depthPassShader->useTextureBlock("SGLPP_LayersDepthAttachments[" + std::to_string(layerIdx) + "]", layerIdx);
            ppLayer->m_frameBuffer->bindAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0, layerIdx);
            ++layerIdx;
        }
    }
    
    depthPassShader->useInteger("SGLPP_LayersDepthAttachmentsCount", layerIdx);
    
    // =========================================
    
    // layer->m_frameBuffer->bind();
    
    // bind the attachment to which the fragment will be received and into which this fragment will be overwritten
    layer->m_frameBuffer->bindAttachmentToDrawIn(attachmentType);
    
    CoreMain::getRenderer()->renderMeshData(
            m_postProcessQuad,
            m_postProcessQuadRenderInfo
    );
    
    // layer->m_frameBuffer->unbind();
}
