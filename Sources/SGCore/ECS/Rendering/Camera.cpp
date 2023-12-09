//
// Created by stuka on 31.10.2023.
//
#include "Camera.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"
#include "SGCore/Utils/ShadersPaths.h"

SGCore::Camera::Camera()
{
    auto& shadersPaths = *SGSingleton::getSharedPtrInstance<ShadersPaths>();

    m_depthPassShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    shadersPaths["PostProcessing"]["DepthPassShader"]
            )
    );

    m_ppLayersCombiningShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    shadersPaths["PostProcessing"]["AttachmentsCombiningShader"]
            )
    );

    m_finalPostProcessFXShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    shadersPaths["PostProcessing"]["FinalOverlayShader"]
            )
    );

    // ==================================================================

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    m_ppLayersCombinedBuffer =
            Ref<IFrameBuffer>(CoreMain::getRenderer().createFrameBuffer())
                    ->create()
                    ->setSize(primaryMonitorWidth, primaryMonitorHeight)
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, // FOR COMBINED COLOR
                                    SGGColorFormat::SGG_RGB,
                                    SGGColorInternalFormat::SGG_RGB8,
                                    0,
                                    0
                    )
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
                                    SGGColorFormat::SGG_RGB,
                                    SGGColorInternalFormat::SGG_RGB8,
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
            Ref<IFrameBuffer>(CoreMain::getRenderer().createFrameBuffer())
                    ->create()
                    ->setSize(primaryMonitorWidth, primaryMonitorHeight)
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                                    SGGColorFormat::SGG_RGB,
                                    SGGColorInternalFormat::SGG_RGB8,
                                    0,
                                    0)
                    ->unbind();

    // ---------------------------------------

    // adding default pp layer
    addPostProcessLayer("defaultPPLayer", m_technicalLayer);
    m_currentPPFrameBufferToBind = getDefaultPostProcessLayer().m_frameBuffer;
}

SGCore::PostProcessLayer& SGCore::Camera::addPostProcessLayer(const std::string& ppLayerName,
                                                              const Ref<Layer>& layer,
                                                              const std::uint16_t& fbWidth,
                                                              const std::uint16_t& fbHeight)
{
    auto& shadersPaths = *SGSingleton::getSharedPtrInstance<ShadersPaths>();

    const auto& foundPPLayer = m_postProcessLayers.find(layer);

    if(foundPPLayer != m_postProcessLayers.cend())
    {
        SGCF_ERROR("Error: can not add a new post-process layer to the camera. This layer already exists.", SG_LOG_CURRENT_SESSION_FILE);
        return foundPPLayer->second;
    }

    for(auto& ppLayer : m_postProcessLayers)
    {
        if(ppLayer.second.m_name == ppLayerName)
        {
            SGCF_ERROR("Error: can not add a new post-process layer to the camera. Layer with name '" + ppLayerName + "' already exists.", SG_LOG_CURRENT_SESSION_FILE);

            return ppLayer.second;
        }
    }

    auto& newPPLayer = m_postProcessLayers[layer];
    // without - 1 because 0 is always default FB
    newPPLayer.m_index = m_postProcessLayers.size();

    newPPLayer.m_name = ppLayerName;
    newPPLayer.m_frameBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer().createFrameBuffer())
            ->create()
            ->setSize(fbWidth, fbHeight)
            ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                            SGGColorFormat::SGG_DEPTH_COMPONENT,
                            SGGColorInternalFormat::SGG_DEPTH_COMPONENT16,
                            0,
                            0)
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0, // for DEPTH TEST
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
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
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
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

    newPPLayer.m_FXShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    shadersPaths["PostProcessing"]["DefaultLayerShader"]
            )
    );

    // ----------------------------------

    std::string layerNameInShaders = SG_PP_LAYER_FB_NAME(newPPLayer.m_index);

    newPPLayer.m_nameInShader = layerNameInShaders;

    // ----------------------------------

    m_postProcessShadersMarkup.addFrameBufferBlockDeclaration(layerNameInShaders, 1, 0, 5, 0);
    m_postProcessShadersMarkup.calculateBlocksOffsets();

    // ----------------------------------
    // updating all frame buffers in all shaders

    std::uint16_t ppFBCount = m_postProcessLayers.size() + 1;

    m_finalPostProcessFXShader->bind();
    m_finalPostProcessFXShader->updateFrameBufferAttachmentsCount(newPPLayer.m_frameBuffer, newPPLayer.m_nameInShader);


    for(const auto& ppLayer : m_postProcessLayers)
    {
        ppLayer.second.m_FXShader->bind();
        ppLayer.second.m_FXShader->updateFrameBufferAttachmentsCount(newPPLayer.m_frameBuffer, newPPLayer.m_nameInShader);

        // ------------------------------

        newPPLayer.m_FXShader->bind();
        newPPLayer.m_FXShader->updateFrameBufferAttachmentsCount(ppLayer.second.m_frameBuffer, ppLayer.second.m_nameInShader);
    }

    return newPPLayer;
}

SGCore::PostProcessLayer& SGCore::Camera::addPostProcessLayer(const std::string& ppLayerName,
                                                              const Ref<Layer>& layer)
{
    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    return addPostProcessLayer(ppLayerName, layer, primaryMonitorWidth, primaryMonitorHeight);
}

void SGCore::Camera::setPostProcessLayerShader(const Ref<Layer>& layer,
                                                  const Ref<IShader>& shader) noexcept
{
    if(m_postProcessLayers.find(layer) == m_postProcessLayers.end())
    {
        SGCF_ERROR("Error: can not add a new post-process layer to the camera. This layer does not exist as post-processing layer.", SG_LOG_CURRENT_SESSION_FILE);
        return;
    }

    shader->bind();

    for(const auto& ppLayer : m_postProcessLayers)
    {
        shader->updateFrameBufferAttachmentsCount(ppLayer.second.m_frameBuffer, ppLayer.second.m_nameInShader);
    }

    m_postProcessLayers[layer].m_FXShader = shader;
}

void SGCore::Camera::bindPostProcessLayers() noexcept
{
    for(const auto& ppLayer : m_postProcessLayers)
    {
        ppLayer.second.m_frameBuffer->bindAttachments(
                m_postProcessShadersMarkup.m_frameBuffersAttachmentsBlocks[ppLayer.second.m_nameInShader]
        );
    }
}

SGCore::Ref<SGCore::IFrameBuffer>
SGCore::Camera::getPostProcessLayerFrameBuffer(const Ref<Layer>& layer) noexcept
{
    const auto& foundPPLayer = m_postProcessLayers.find(layer);

    return foundPPLayer != m_postProcessLayers.cend() ? foundPPLayer->second.m_frameBuffer : nullptr;
}

void SGCore::Camera::bindPostProcessFrameBuffer
(const Ref<Layer>& layer, const SGFrameBufferAttachmentType& attachmentToDrawType) noexcept
{
    auto foundFrameBuffer = getPostProcessLayerFrameBuffer(layer);

    if(foundFrameBuffer)
    {
        m_currentPPFrameBufferToBind = foundFrameBuffer;
    }
    else
    {
        // It is assumed that the technical layer always exists
        m_currentPPFrameBufferToBind = getPostProcessLayerFrameBuffer(m_technicalLayer);
    }

    m_currentPPFrameBufferToBind->bind();
    m_currentPPFrameBufferToBind->bindAttachmentToDraw(attachmentToDrawType);
}

void SGCore::Camera::unbindPostProcessFrameBuffer() const noexcept
{
    if(m_currentPPFrameBufferToBind)
    {
        m_currentPPFrameBufferToBind->unbind();
    }
}

void SGCore::Camera::clearPostProcessFrameBuffers() const noexcept
{
    for(const auto& ppLayer : m_postProcessLayers)
    {
        ppLayer.second.m_frameBuffer->bind()->clear();
    }

    m_finalFrameFXFrameBuffer->bind()->clear()->unbind();
}

SGCore::PostProcessLayer& SGCore::Camera::getDefaultPostProcessLayer() noexcept
{
    // It is assumed that the technical layer always exists
    return m_postProcessLayers.find(m_technicalLayer)->second;
}


