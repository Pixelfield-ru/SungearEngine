//
// Created by stuka on 31.10.2023.
//
#include "Camera.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"

Core::ECS::Camera::Camera()
{
    m_postProcessQuadRenderInfo.m_enableFacesCulling = false;

    m_postProcessQuad = std::shared_ptr<ImportedScene::IMeshData>(Core::Main::CoreMain::getRenderer().createMeshData());

    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(2);
    m_postProcessQuad->m_indices.push_back(1);

    m_postProcessQuad->m_indices.push_back(0);
    m_postProcessQuad->m_indices.push_back(3);
    m_postProcessQuad->m_indices.push_back(2);

    m_postProcessQuad->prepare();

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Core::Main::Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    m_defaultLayersFrameBuffer =
            std::shared_ptr<Graphics::IFrameBuffer>(Core::Main::CoreMain::getRenderer().createFrameBuffer())
                    ->create()
                    ->setSize(primaryMonitorWidth, primaryMonitorHeight)
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                                    SGGColorFormat::SGG_RGBA,
                                    SGGColorInternalFormat::SGG_RGBA16,
                                    0,
                                    0
                    )
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                    SGGColorFormat::SGG_DEPTH_COMPONENT,
                                    SGGColorInternalFormat::SGG_DEPTH_COMPONENT16,
                                    0,
                                    0)
                    ->unbind();

    // m_defaultLayersFrameBuffer->m_bgColor.a = 0.0;

    m_finalFrameBuffer =
            std::shared_ptr<Graphics::IFrameBuffer>(Core::Main::CoreMain::getRenderer().createFrameBuffer())
                    ->create()
                    ->setSize(primaryMonitorWidth, primaryMonitorHeight)
                    ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                                    SGGColorFormat::SGG_RGBA,
                                    SGGColorInternalFormat::SGG_RGBA16,
                                    0,
                                    0)
                    ->unbind();

    // ---------------------------------------

    m_postProcessShadersMarkup.addFrameBufferBlockDeclaration("allFB[0]", 1, 0, 1, 0);
    m_postProcessShadersMarkup.calculateBlocksOffsets();

    // ----------------------------------------

    m_finalPostProcessOverlayShader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(Graphics::getShaderPath(
                    Graphics::StandardShaderType::SG_PP_FINAL_OVERLAY_SHADER)
            )
    );

    m_finalPostProcessOverlayShader->bind();
    m_finalPostProcessOverlayShader->updateFrameBufferAttachmentsCount(m_defaultLayersFrameBuffer, "allFB[0]");
    m_finalPostProcessOverlayShader->useInteger("FBCount", m_postProcessLayers.size() + 1);

    // -----------------------------------------

    m_defaultPostProcessShader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(Graphics::getShaderPath(
                    Graphics::StandardShaderType::SG_PP_LAYER_DEPTH_TEST_SHADER)
            )
    );

    m_defaultPostProcessShader->bind();
    m_defaultPostProcessShader->updateFrameBufferAttachmentsCount(m_defaultLayersFrameBuffer, "allFB[0]");
    m_defaultPostProcessShader->useInteger("FBCount", m_postProcessLayers.size() + 1);
}

void Core::ECS::Camera::addPostProcessLayer(const std::string& ppLayerName,
                                            const std::shared_ptr<ECS::Layer>& layer,
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
    newPPLayer.m_frameBuffer = std::shared_ptr<Graphics::IFrameBuffer>(Core::Main::CoreMain::getRenderer().createFrameBuffer())
            ->create()
            ->setSize(fbWidth, fbHeight)
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0,
                            SGGColorFormat::SGG_RGBA,
                            SGGColorInternalFormat::SGG_RGBA16,
                            0,
                            0
            )
            ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                            SGGColorFormat::SGG_DEPTH_COMPONENT,
                            SGGColorInternalFormat::SGG_DEPTH_COMPONENT16,
                            0,
                            0)
            ->unbind();

    // newPPLayer.m_frameBuffer->m_bgColor.a = 0.0;

    newPPLayer.m_postProcessLayerShader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(Graphics::getShaderPath(
                    Graphics::StandardShaderType::SG_PP_LAYER_DEPTH_TEST_SHADER)
            )
    );

    // ----------------------------------

    std::string layerNameInShaders = "allFB[" + std::to_string(newPPLayer.m_index) + "]";

    newPPLayer.m_nameInShader = layerNameInShaders;

    // ----------------------------------

    m_postProcessShadersMarkup.addFrameBufferBlockDeclaration(layerNameInShaders, 1, 0, 1, 0);
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

    newPPLayer.m_postProcessLayerShader->bind();
    newPPLayer.m_postProcessLayerShader->updateFrameBufferAttachmentsCount(m_defaultLayersFrameBuffer, "allFB[0]");
    newPPLayer.m_postProcessLayerShader->useInteger("FBCount", ppFBCount);

    for(const auto& ppLayer : m_postProcessLayers)
    {
        ppLayer.second.m_postProcessLayerShader->bind();
        ppLayer.second.m_postProcessLayerShader->updateFrameBufferAttachmentsCount(newPPLayer.m_frameBuffer, newPPLayer.m_nameInShader);

        // ------------------------------

        newPPLayer.m_postProcessLayerShader->bind();
        newPPLayer.m_postProcessLayerShader->updateFrameBufferAttachmentsCount(ppLayer.second.m_frameBuffer, ppLayer.second.m_nameInShader);
    }
}

void Core::ECS::Camera::addPostProcessLayer(const std::string& ppLayerName,
                                            const std::shared_ptr<ECS::Layer>& layer)
{
    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Core::Main::Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    addPostProcessLayer(ppLayerName, layer, primaryMonitorWidth, primaryMonitorHeight);
}

void Core::ECS::Camera::setPostProcessLayerShader(const std::shared_ptr<Layer>& layer,
                                                  const std::shared_ptr<Graphics::IShader>& shader) noexcept
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

    m_postProcessLayers[layer].m_postProcessLayerShader = shader;
}

void Core::ECS::Camera::bindPostProcessLayers() noexcept
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

std::shared_ptr<Core::Graphics::IFrameBuffer>
Core::ECS::Camera::getPostProcessLayerFrameBuffer(const std::shared_ptr<ECS::Layer>& layer) noexcept
{
    const auto& foundPPLayer = m_postProcessLayers.find(layer);

    return foundPPLayer != m_postProcessLayers.cend() ? foundPPLayer->second.m_frameBuffer : nullptr;
}


