//
// Created by stuka on 31.10.2023.
//
#include "Camera.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"
#include "SGCore/Utils/ShadersPaths.h"

SGCore::Camera::Camera()
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

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    m_defaultLayersFrameBuffer =
            Ref<IFrameBuffer>(CoreMain::getRenderer().createFrameBuffer())
                    ->create()
                    ->setSize(primaryMonitorWidth, primaryMonitorHeight)
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
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1, // for FX apply
                                    SGGColorFormat::SGG_RGB,
                                    SGGColorInternalFormat::SGG_RGB8,
                                    0,
                                    0
                    )
                    ->unbind();

    // m_defaultLayersFrameBuffer->m_bgColor.a = 0.0;

    m_finalFrameBuffer =
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

    m_postProcessShadersMarkup.addFrameBufferBlockDeclaration("allFB[0]", 1, 0, 2, 0);
    m_postProcessShadersMarkup.calculateBlocksOffsets();

    // ----------------------------------------

    m_finalPostProcessOverlayShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    ShadersPaths::getMainInstance()["PostProcessing"]["FinalOverlayShader"]
            )
    );

    m_finalPostProcessOverlayShader->bind();
    m_finalPostProcessOverlayShader->updateFrameBufferAttachmentsCount(m_defaultLayersFrameBuffer, "allFB[0]");
    m_finalPostProcessOverlayShader->useInteger("FBCount", m_postProcessLayers.size() + 1);

    // -----------------------------------------

    m_defaultPostProcessShader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    ShadersPaths::getMainInstance()["PostProcessing"]["DefaultLayerShader"]
            )
    );

    m_defaultPostProcessShader->bind();
    m_defaultPostProcessShader->updateFrameBufferAttachmentsCount(m_defaultLayersFrameBuffer, "allFB[0]");
    m_defaultPostProcessShader->useInteger("FBCount", m_postProcessLayers.size() + 1);
}

void SGCore::Camera::addPostProcessLayer(const std::string& ppLayerName,
                                            const Ref<Layer>& layer,
                                            const std::uint16_t& fbWidth,
                                            const std::uint16_t& fbHeight)
{
    if(m_postProcessLayers.find(layer) != m_postProcessLayers.end())
    {
        SGCF_ERROR("Error: can not add a new post-process layer to the camera. This layer already exists.", SG_LOG_CURRENT_SESSION_FILE);
        return;
    }

    for(const auto& ppLayer : m_postProcessLayers)
    {
        if(ppLayer.second.m_name == ppLayerName)
        {
            SGCF_ERROR("Error: can not add a new post-process layer to the camera. Layer with name '" + ppLayerName + "' already exists.", SG_LOG_CURRENT_SESSION_FILE);

            return;
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
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT1, // for FX apply
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )
            ->unbind();

    newPPLayer.m_shader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    ShadersPaths::getMainInstance()["PostProcessing"]["DefaultLayerShader"]
            )
    );

    // ----------------------------------

    std::string layerNameInShaders = "allFB[" + std::to_string(newPPLayer.m_index) + "]";

    newPPLayer.m_nameInShader = layerNameInShaders;

    // ----------------------------------

    m_postProcessShadersMarkup.addFrameBufferBlockDeclaration(layerNameInShaders, 1, 0, 2, 0);
    m_postProcessShadersMarkup.calculateBlocksOffsets();

    // ----------------------------------
    // updating all frame buffers in all shaders

    std::uint16_t ppFBCount = m_postProcessLayers.size() + 1;

    m_finalPostProcessOverlayShader->bind();
    m_finalPostProcessOverlayShader->updateFrameBufferAttachmentsCount(newPPLayer.m_frameBuffer, newPPLayer.m_nameInShader);
    m_finalPostProcessOverlayShader->useInteger("FBCount", ppFBCount);

    m_defaultPostProcessShader->bind();
    m_defaultPostProcessShader->updateFrameBufferAttachmentsCount(newPPLayer.m_frameBuffer, newPPLayer.m_nameInShader);
    m_defaultPostProcessShader->useInteger("FBCount", ppFBCount);

    newPPLayer.m_shader->bind();
    newPPLayer.m_shader->updateFrameBufferAttachmentsCount(m_defaultLayersFrameBuffer, "allFB[0]");
    newPPLayer.m_shader->useInteger("FBCount", ppFBCount);

    for(const auto& ppLayer : m_postProcessLayers)
    {
        ppLayer.second.m_shader->bind();
        ppLayer.second.m_shader->updateFrameBufferAttachmentsCount(newPPLayer.m_frameBuffer, newPPLayer.m_nameInShader);

        // ------------------------------

        newPPLayer.m_shader->bind();
        newPPLayer.m_shader->updateFrameBufferAttachmentsCount(ppLayer.second.m_frameBuffer, ppLayer.second.m_nameInShader);
    }
}

void SGCore::Camera::addPostProcessLayer(const std::string& ppLayerName,
                                            const Ref<Layer>& layer)
{
    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    addPostProcessLayer(ppLayerName, layer, primaryMonitorWidth, primaryMonitorHeight);
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
    shader->updateFrameBufferAttachmentsCount(m_defaultLayersFrameBuffer, "allFB[0]");
    shader->useInteger("FBCount", m_postProcessLayers.size() + 1);

    for(const auto& ppLayer : m_postProcessLayers)
    {
        shader->updateFrameBufferAttachmentsCount(ppLayer.second.m_frameBuffer, ppLayer.second.m_nameInShader);
    }

    m_postProcessLayers[layer].m_shader = shader;
}

void SGCore::Camera::bindPostProcessLayers() noexcept
{
    m_defaultLayersFrameBuffer->bindAttachments(
            m_postProcessShadersMarkup.m_frameBuffersAttachmentsBlocks["allFB[0]"]
    );

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
        m_currentPPFrameBufferToBind = m_defaultLayersFrameBuffer;
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
    m_defaultLayersFrameBuffer->bind()->clear();
    m_finalFrameBuffer->bind()->clear();

    for(const auto& ppLayer : m_postProcessLayers)
    {
        ppLayer.second.m_frameBuffer->bind()->clear();
    }
}


