//
// Created by stuka on 31.10.2023.
//
#include <spdlog/spdlog.h>

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
    // addRequiredShaderPath("PostProcessingShader");
    
    m_defaultLayer = addPostProcessLayer("___DEFAULT_LAYER___");
    
    m_shader = MakeRef<IShader>();
    m_shader->addSubPassShadersAndCompile(AssetManager::loadAsset<TextFileAsset>(
            DefaultShadersPaths::getPaths()["LayeredPP"]["ReceiverShader"].getCurrentRealization()));
    
    /*m_depthPassShader = Ref<ISubPassShader>(
            CoreMain::getRenderer().createShader(
                    shadersPaths["PostProcessing"]["DepthPassShader"]
            )
    )->addToGlobalStorage();

    m_ppLayersCombiningShader = Ref<ISubPassShader>(
            CoreMain::getRenderer().createShader(
                    shadersPaths["PostProcessing"]["AttachmentsCombiningShader"]
            )
    )->addToGlobalStorage();

    m_finalPostProcessFXShader = Ref<ISubPassShader>(
            CoreMain::getRenderer().createShader(
                    shadersPaths["PostProcessing"]["FinalOverlayShader"]
            )
    )->addToGlobalStorage();*/

    // ==================================================================

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    m_ppLayersCombinedBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer());
    m_ppLayersCombinedBuffer->setSize(primaryMonitorWidth, primaryMonitorHeight);
    m_ppLayersCombinedBuffer->create();
    m_ppLayersCombinedBuffer->bind();
    /*m_ppLayersCombinedBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
            SGGColorFormat::SGG_DEPTH_COMPONENT,
            SGGColorInternalFormat::SGG_DEPTH_COMPONENT16,
            0,
            0);*/
    m_ppLayersCombinedBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB8,
            0,
            0
    );
    /*m_ppLayersCombinedBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1, // DEPTH-TESTED ATTACHMENT
            SGGColorFormat::SGG_RGB,
            SGGColorInternalFormat::SGG_RGB8,
            0,
            0
    );*/
    /*m_ppLayersCombinedBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                            SGGColorFormat::SGG_DEPTH_COMPONENT,
                                            SGGColorInternalFormat::SGG_DEPTH_COMPONENT16,
                                            0,
                                            0);
    m_ppLayersCombinedBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, // for DEPTH PASS
                                            SGGColorFormat::SGG_RGBA,
                                            SGGColorInternalFormat::SGG_RGBA8,
                                            0,
                                            0
    );*/
    // GBUFFER ATTACHMENTS
    // NOT STANDARD
    /*m_ppLayersCombinedBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1,
                                            SGGColorFormat::SGG_RGB,
                                            SGGColorInternalFormat::SGG_RGB8,
                                            0,
                                            0
    );
    m_ppLayersCombinedBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2,
                    SGGColorFormat::SGG_RGB,
                    SGGColorInternalFormat::SGG_RGB8,
                    0,
                    0
    );
    m_ppLayersCombinedBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3,
                    SGGColorFormat::SGG_RGBA,
                    SGGColorInternalFormat::SGG_RGBA8,
                    0,
                    0
    );
    m_ppLayersCombinedBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT4,
                    SGGColorFormat::SGG_RGB,
                    SGGColorInternalFormat::SGG_RGB8,
                    0,
                    0
    );
    m_ppLayersCombinedBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT5,
                    SGGColorFormat::SGG_RGB,
                    SGGColorInternalFormat::SGG_RGB8,
                    0,
                    0
    );*/
    m_ppLayersCombinedBuffer->unbind();

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

    // ---------------------------------------

    // preparing uniforms in some shaders ======================
    /*auto depthPassShader = m_shader->getSubPassShader("PostProcessLayerDepthPass");

    depthPassShader->bind();

    for(std::uint8_t i = 0; i < 32; ++i)
    {
        depthPassShader->useInteger("allFrameBuffersDepthAttachments[" + std::to_string(i) + "]", i);
    }*/
}

SGCore::Ref<SGCore::PostProcessLayer> SGCore::LayeredFrameReceiver::addPostProcessLayer(const std::string& name,
                                                                                        const std::uint16_t& fbWidth,
                                                                                        const std::uint16_t& fbHeight)
{
    auto foundPPLayer = getPostProcessLayer(name);

    if(foundPPLayer)
    {
        spdlog::error("Error: can not add a new post-process layer to the camera. This layer already exists.\n{0}",
                      SGUtils::Utils::sourceLocationToString(std::source_location::current()));
        return foundPPLayer;
    }

    m_postProcessLayers.emplace_back(MakeRef<PostProcessLayer>());
    
    auto& newPPLayer = *m_postProcessLayers.rbegin();
    // without - 1 because 0 is always default FB
    newPPLayer->m_index = m_postProcessLayers.size() - 1;
    
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
            // NON STANDARD!!
            // GBUFFER ATTACHMENTS
            /*->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2,
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3,
                            SGGColorFormat::SGG_RGBA,
                            SGGColorInternalFormat::SGG_RGBA8,
                            0,
                            0
            )
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT4,
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT5,
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT6,
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )*/
    newPPLayer->m_frameBuffer->unbind();

    auto fxShader = MakeRef<IShader>();
    fxShader->addSubPassShadersAndCompile(AssetManager::loadAsset<TextFileAsset>(
            DefaultShadersPaths::getPaths()["LayeredPP"]["LayerFXShader"].getCurrentRealization()));
    
    newPPLayer->m_FXSubPassShader = fxShader->getSubPassShader("PostProcessLayerFXPass");

    // ----------------------------------

    std::string layerNameInShaders = SG_PP_LAYER_FB_NAME(newPPLayer->m_index);

    newPPLayer->m_nameInShader = layerNameInShaders;

    // ----------------------------------
    // updating all frame buffers in all shaders

    std::uint16_t ppFBCount = m_postProcessLayers.size() + 1;

    /*auto postProcessFinalFXShader = m_shader->getSubPassShader("PostProcessFinalFXPass");
    auto depthPassShader = m_shader->getSubPassShader("PostProcessLayerDepthPass");

    postProcessFinalFXShader->bind();
    postProcessFinalFXShader->updateFrameBufferAttachmentsCount(newPPLayer.m_frameBuffer, newPPLayer.m_nameInShader);

    for(const auto& ppLayer : m_postProcessLayers)
    {
        ppLayer.second.m_FXShader->bind();
        ppLayer.second.m_FXShader->updateFrameBufferAttachmentsCount(newPPLayer.m_frameBuffer, newPPLayer.m_nameInShader);

        // ------------------------------

        newPPLayer.m_FXShader->bind();
        newPPLayer.m_FXShader->updateFrameBufferAttachmentsCount(ppLayer.second.m_frameBuffer, ppLayer.second.m_nameInShader);
    }

    // updating uniforms in shader ====================================

    depthPassShader->bind();

    depthPassShader->useInteger("allFrameBuffersDepthAttachmentCount", m_postProcessLayers.size());*/

    return newPPLayer;
}

SGCore::Ref<SGCore::PostProcessLayer> SGCore::LayeredFrameReceiver::addPostProcessLayer(const std::string& name)
{
    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    return addPostProcessLayer(name, primaryMonitorWidth, primaryMonitorHeight);
}

void SGCore::LayeredFrameReceiver::setPostProcessLayerShader(const std::string& name,
                                                             const Ref<ISubPassShader>& shader) noexcept
{
    auto foundLayer = getPostProcessLayer(name);
    
    if(!foundLayer)
    {
        spdlog::error("Error: can not add a new post-process layer to the camera. This layer does not exist as post-processing layer.\n{0}",
                      SGUtils::Utils::sourceLocationToString(std::source_location::current()));
        return;
    }
    
    foundLayer->m_FXSubPassShader = shader;
}

void SGCore::LayeredFrameReceiver::bindPostProcessFrameBuffer
(const Ref<Layer>& layer) noexcept
{
    auto foundPPLayer = getPostProcessLayer(layer->m_name);

    m_currentPPFrameBufferToBind = foundPPLayer->m_frameBuffer;

    m_currentPPFrameBufferToBind->bind();
    m_currentPPFrameBufferToBind->bindAttachmentsToDrawIn(foundPPLayer->m_attachmentsToRenderIn);
}

void SGCore::LayeredFrameReceiver::unbindPostProcessFrameBuffer() const noexcept
{
    if(m_currentPPFrameBufferToBind)
    {
        m_currentPPFrameBufferToBind->unbind();
    }
}

void SGCore::LayeredFrameReceiver::clearPostProcessFrameBuffers() const noexcept
{
    for(const auto& ppLayer : m_postProcessLayers)
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

    m_ppLayersCombinedBuffer->bind();
    m_ppLayersCombinedBuffer->bindAttachmentsToDrawIn(m_attachmentsForCombining);
    m_ppLayersCombinedBuffer->clear();

    m_finalFrameFXFrameBuffer->bind();
    m_finalFrameFXFrameBuffer->clear();
    m_finalFrameFXFrameBuffer->unbind();
}

SGCore::Ref<SGCore::PostProcessLayer> SGCore::LayeredFrameReceiver::getPostProcessLayer(const std::string& name) noexcept
{
    auto it = std::find_if(m_postProcessLayers.begin(), m_postProcessLayers.end(), [&](const Ref<PostProcessLayer>& layer) {
        return layer->m_name == name;
    });
    
    return it != m_postProcessLayers.end() ? *it : nullptr;
}

SGCore::Ref<SGCore::PostProcessLayer> SGCore::LayeredFrameReceiver::getDefaultPostProcessLayer() noexcept
{
    return m_defaultLayer;
    // return getPostProcessLayer("___DEFAULT_LAYER___");
}
