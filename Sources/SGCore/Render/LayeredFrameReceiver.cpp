//
// Created by stuka on 31.10.2023.
//
#include <spdlog/spdlog.h>

#include "LayeredFrameReceiver.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/ShadersPaths.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Scene/Layer.h"

SGCore::LayeredFrameReceiver::LayeredFrameReceiver()
{
    // addRequiredShaderPath("PostProcessingShader");
    
    m_defaultLayer->m_name = "___DEFAULT_LAYER___";
    addPostProcessLayer(m_defaultLayer);
    
    m_shader = MakeRef<IShader>();

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

    m_ppLayersCombinedBuffer =
            Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer())
                    ->setSize(primaryMonitorWidth, primaryMonitorHeight)
                    ->create()
                    ->bind()
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                    SGGColorFormat::SGG_DEPTH_COMPONENT,
                                    SGGColorInternalFormat::SGG_DEPTH_COMPONENT16,
                                    0,
                                    0)
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, // for DEPTH PASS
                                    SGGColorFormat::SGG_RGBA,
                                    SGGColorInternalFormat::SGG_RGBA8,
                                    0,
                                    0
                    )
                            // GBUFFER ATTACHMENTS
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1,
                                    SGGColorFormat::SGG_RGB,
                                    SGGColorInternalFormat::SGG_RGB8,
                                    0,
                                    0
                    )
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2,
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
                    ->unbind();

    // m_defaultLayersFrameBuffer->m_bgColor.a = 0.0;

    m_finalFrameFXFrameBuffer =
            Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer())
                    ->create()
                    ->bind()
                    ->setSize(primaryMonitorWidth, primaryMonitorHeight)
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                                    SGGColorFormat::SGG_RGB,
                                    SGGColorInternalFormat::SGG_RGB8,
                                    0,
                                    0)
                    ->unbind();

    // ---------------------------------------

    // preparing uniforms in some shaders ======================
    /*auto depthPassShader = m_shader->getSubPassShader("PostProcessLayerDepthPass");

    depthPassShader->bind();

    for(std::uint8_t i = 0; i < 32; ++i)
    {
        depthPassShader->useInteger("allFrameBuffersDepthAttachments[" + std::to_string(i) + "]", i);
    }*/
}

SGCore::PostProcessLayer& SGCore::LayeredFrameReceiver::addPostProcessLayer(const Ref<Layer>& layer,
                                                                            const std::uint16_t& fbWidth,
                                                                            const std::uint16_t& fbHeight)
{
    auto& shadersPaths = *SGUtils::Singleton::getSharedPtrInstance<ShadersPaths>();

    PostProcessLayer* foundPPLayer = tryGetPostProcessLayer(layer->m_name);

    if(foundPPLayer)
    {
        spdlog::error("Error: can not add a new post-process layer to the camera. This layer already exists.\n{0}",
                      SGUtils::Utils::sourceLocationToString(std::source_location::current()));
        return *foundPPLayer;
    }

    m_postProcessLayers.emplace_back();
    
    auto& newPPLayer = *m_postProcessLayers.rbegin();
    // without - 1 because 0 is always default FB
    newPPLayer.m_index = m_postProcessLayers.size() - 1;

    newPPLayer.m_sceneLayer = layer;
    newPPLayer.m_frameBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer())
            ->create()
            ->bind()
            ->setSize(fbWidth, fbHeight)
            ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                            SGGColorFormat::SGG_DEPTH_COMPONENT,
                            SGGColorInternalFormat::SGG_DEPTH_COMPONENT16,
                            0,
                            0)
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, // NOT DEPT-TESTED ATTACHMENT
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1, // DEPTH-TESTED ATTACHMENT
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )
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
            ->unbind();

    newPPLayer.m_FXShader = Ref<ISubPassShader>(
            CoreMain::getRenderer()->createShader(
                    shadersPaths["PostProcessing"]["DefaultLayerShader"]
            )
    );
    
    newPPLayer.m_FXShader->addToGlobalStorage();

    // ----------------------------------

    std::string layerNameInShaders = SG_PP_LAYER_FB_NAME(newPPLayer.m_index);

    newPPLayer.m_nameInShader = layerNameInShaders;

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

SGCore::PostProcessLayer& SGCore::LayeredFrameReceiver::addPostProcessLayer(const Ref<Layer>& layer)
{
    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    return addPostProcessLayer(layer, primaryMonitorWidth, primaryMonitorHeight);
}

void SGCore::LayeredFrameReceiver::setPostProcessLayerShader(const Ref<Layer>& layer,
                                                             const Ref<ISubPassShader>& shader) noexcept
{
    PostProcessLayer* foundLayer = tryGetPostProcessLayer(layer->m_name);
    
    if(!foundLayer)
    {
        spdlog::error("Error: can not add a new post-process layer to the camera. This layer does not exist as post-processing layer.\n{0}",
                      SGUtils::Utils::sourceLocationToString(std::source_location::current()));
        return;
    }
    
    foundLayer->m_FXShader = shader;
}

void SGCore::LayeredFrameReceiver::bindPostProcessFrameBuffer
(const Ref<Layer>& layer) noexcept
{
    PostProcessLayer* foundPPLayer = tryGetPostProcessLayer(layer->m_name);

    m_currentPPFrameBufferToBind = foundPPLayer->m_frameBuffer;

    m_currentPPFrameBufferToBind->bind();
    m_currentPPFrameBufferToBind->bindAttachmentsToDraw(foundPPLayer->m_attachmentsToRenderIn);
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
        ppLayer.m_frameBuffer->bind()->bindAttachmentsToDraw(
                std::vector<SGFrameBufferAttachmentType> { SGG_COLOR_ATTACHMENT0, SGG_COLOR_ATTACHMENT1,
                                                           SGG_COLOR_ATTACHMENT2, SGG_COLOR_ATTACHMENT3,
                                                           SGG_COLOR_ATTACHMENT4, SGG_COLOR_ATTACHMENT5,
                                                           SGG_COLOR_ATTACHMENT6, SGG_COLOR_ATTACHMENT7 })->clear();
    }

    m_ppLayersCombinedBuffer->bind()->bindAttachmentsToDraw(m_attachmentsForCombining)->clear();

    m_finalFrameFXFrameBuffer->bind()->clear()->unbind();
}

SGCore::PostProcessLayer* SGCore::LayeredFrameReceiver::tryGetPostProcessLayer(const std::string& name) noexcept
{
    auto it = std::find_if(m_postProcessLayers.begin(), m_postProcessLayers.end(), [&](const PostProcessLayer& layer) {
        auto lockedSceneLayer = layer.m_sceneLayer.lock();
        if(!lockedSceneLayer) return false;
        
        return lockedSceneLayer->m_name == name;
    });
    
    return it != m_postProcessLayers.end() ? &*it : nullptr;
}

SGCore::PostProcessLayer* SGCore::LayeredFrameReceiver::tryGetDefaultPostProcessLayer() noexcept
{
    return tryGetPostProcessLayer("___DEFAULT_LAYER___");
}
