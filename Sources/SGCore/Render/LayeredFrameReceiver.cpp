//
// Created by stuka on 31.10.2023.
//
#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "LayeredFrameReceiver.h"

#include "SGCore/Utils/Paths.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Scene/Layer.h"
#include "RenderPipelinesManager.h"
#include "SGCore/Graphics/API/ITexture2D.h"

SGCore::LayeredFrameReceiver::LayeredFrameReceiver()
{
    m_quadMeshRenderState.m_useFacesCulling = false;
    
    m_postProcessQuad = Ref<IMeshData>(CoreMain::getRenderer()->createMeshData());
    
    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(2);
    m_postProcessQuad->m_indices.push_back(1);
    
    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(3);
    m_postProcessQuad->m_indices.push_back(2);
    
    m_postProcessQuad->prepare();
    
    m_defaultLayer = addLayer("___DEFAULT_LAYER___");

    // ==================================================================

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    LOG_I(SGCORE_TAG, "Creating framebuffers for LayeredFrameReceiver. Primary monitor size: {}x{}", primaryMonitorWidth, primaryMonitorHeight)

    /*primaryMonitorWidth = 4096;
    primaryMonitorHeight = 4096;*/

    m_layersFrameBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer());
    m_layersFrameBuffer->setSize(primaryMonitorWidth, primaryMonitorHeight);
    m_layersFrameBuffer->create();
    m_layersFrameBuffer->bind();
    m_layersFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
            SGGColorFormat::SGG_DEPTH_COMPONENT,
            SGGColorInternalFormat::SGG_DEPTH_COMPONENT24,
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
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2, // CONTAINS PICKING COLORS
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB16_FLOAT,
            0,
            0
    );
    m_layersFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3, // COLOR FOR STOCHASTIC TRANSPARENCY
            SGGColorFormat::SGG_RGBA,
            SGGColorInternalFormat::SGG_RGBA16_FLOAT,
            0,
            0
    );
    m_layersFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT4, // GBUFFER WORLD POS
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB16_FLOAT,
            0,
            0
    );
    m_layersFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT5, // GBUFFER FRAGMENT NORMAL
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB16_FLOAT,
            0,
            0
    );
    m_layersFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT6, // GBUFFER VERTEX NORMAL
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB16_FLOAT,
            0,
            0
    );

    auto colorAttachment2 = m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2);
    colorAttachment2->m_blendingState.m_sFactor = SGBlendingFactor::SGG_ONE;
    colorAttachment2->m_blendingState.m_dFactor = SGBlendingFactor::SGG_ZERO;
    colorAttachment2->m_clearColor = { 1, 1, 1, 1 };

    auto colorAttachment3 = m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3);
    /*colorAttachment3->m_blendingState.m_sFactor = SGBlendingFactor::SGG_ONE;
    colorAttachment3->m_blendingState.m_dFactor = SGBlendingFactor::SGG_ONE;*/
    colorAttachment3->m_clearColor = { 0, 0, 0, 0 };

    // FOR WBOIT
    /*auto colorAttachment4 = m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT4);
    colorAttachment4->m_blendingState.m_sFactor = SGBlendingFactor::SGG_ZERO;
    colorAttachment4->m_blendingState.m_dFactor = SGBlendingFactor::SGG_ONE_MINUS_SRC_COLOR;
    colorAttachment4->m_clearColor = { 1, 1, 1, 1 };*/

    m_layersFrameBuffer->unbind();

    m_layersFXFrameBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer());
    m_layersFXFrameBuffer->setSize(primaryMonitorWidth, primaryMonitorHeight);
    m_layersFXFrameBuffer->create();
    m_layersFXFrameBuffer->bind();
    /*m_layersFXFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT0,
            SGGColorFormat::SGG_DEPTH_STENCIL,
            SGGColorInternalFormat::SGG_DEPTH24_STENCIL8,
            0,
            0
    );*/
    m_layersFXFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, // IMPROVISED STENCIL BUFFER
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB8,
            0,
            0
    );
    m_layersFXFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1, // CONTAINS OBJECTS THAT MUST BE OUTLINED
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB8,
            0,
            0
    );
    m_layersFXFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7, // ATTACHMENT WITH APPLIED FX
            SGGColorFormat::SGG_RGBA,
            SGGColorInternalFormat::SGG_RGBA16_FLOAT,
            0,
            0
    );

    // m_layersFXFrameBuffer->attachAttachment(m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT0));

    m_layersFXFrameBuffer->unbind();
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

    auto newPPLayer = MakeRef<PostProcessLayer>();
    
    // without - 1 because 0 is always default FB
    newPPLayer->m_index = m_layers.empty() ? 0 : getLayersMaximumIndex() + 1;

    newPPLayer->m_FXSubPassShader = AssetManager::getInstance()->loadAsset<IShader>(*Paths::getDefaultPaths()["Shaders/LayeredPP/LayerFXShader"]);

    // adding one sub pass
    PostProcessFXSubPass subPass;
    newPPLayer->m_subPasses.push_back(subPass);
    
    m_layers.push_back(newPPLayer);

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

SGCore::Ref<SGCore::PostProcessLayer> SGCore::LayeredFrameReceiver::getDefaultLayer() const noexcept
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
    m_layersFrameBuffer->clear();
    m_layersFrameBuffer->unbind();

    m_layersFXFrameBuffer->bind();
    m_layersFXFrameBuffer->clear();
    m_layersFXFrameBuffer->unbind();
}
