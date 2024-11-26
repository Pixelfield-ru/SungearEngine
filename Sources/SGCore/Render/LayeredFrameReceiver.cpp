//
// Created by stuka on 31.10.2023.
//
#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "LayeredFrameReceiver.h"

#include "SGCore/Utils/ShadersUniversalPaths.h"
#include "SGCore/Main/CoreMain.h"
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
    m_shader->compile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
            ShadersUniversalPaths::getDefaultPaths()["LayeredPP/ReceiverShader"]->getCurrentRealization()));

    // ==================================================================

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    m_layersFrameBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer());
    m_layersFrameBuffer->setSize(primaryMonitorWidth, primaryMonitorHeight);
    m_layersFrameBuffer->create();
    m_layersFrameBuffer->bind();
    m_layersFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
            SGGColorFormat::SGG_DEPTH_COMPONENT,
            SGGColorInternalFormat::SGG_DEPTH_COMPONENT32,
            0,
            0
    );
    m_layersFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, // CONTAINS LAYERS VOLUMES
            SGGColorFormat::SGG_RGBA,
            SGGColorInternalFormat::SGG_RGBA8,
            0,
            0
    );
    m_layersFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1, // CONTAINS COLORS FROM LAYERS
            SGGColorFormat::SGG_RGBA,
            SGGColorInternalFormat::SGG_RGBA8,
            0,
            0
    );
    m_layersFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT10, // CONTAINS COLOR1 BUT WITH EFFECTS
            SGGColorFormat::SGG_RGBA,
            SGGColorInternalFormat::SGG_RGBA8,
            0,
            0
    );

    m_layersFrameBuffer->unbind();
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

    auto fxShader = MakeRef<IShader>();
    fxShader->compile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
            ShadersUniversalPaths::getDefaultPaths()["LayeredPP/LayerFXShader"]->getCurrentRealization()));
    
    newPPLayer->m_FXSubPassShader = fxShader->getSubPassShader("SGPPLayerFXPass");

    // adding one sub pass
    PostProcessFXSubPass subPass;
    newPPLayer->m_subPasses.push_back(subPass);

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
    // clearing all attachments
    m_layersFrameBuffer->bind();
    m_layersFrameBuffer->bindAttachmentsToDrawIn(std::vector { SGG_COLOR_ATTACHMENT0, SGG_COLOR_ATTACHMENT1, SGG_COLOR_ATTACHMENT10 });
    m_layersFrameBuffer->clear();
    m_layersFrameBuffer->unbind();
}
