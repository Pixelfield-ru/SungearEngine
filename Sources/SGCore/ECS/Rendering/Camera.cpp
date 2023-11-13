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

    m_colorPostProcessQuadPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_colorPostProcessQuadPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(Graphics::getShaderPath(
                    Graphics::StandardShaderType::SG_COLOR_POSTPROCESSING_SHADER)
            )
    );

    m_colorPostProcessQuadPassMarkedShader->addFrameBufferBlockDeclaration("sgpp_defaultFB", 1, 0, 1, 0);

    m_colorPostProcessQuadPassMarkedShader->calculateBlocksOffsets();

    m_colorPostProcessQuadPassMarkedShader->m_shader->bind();
    m_colorPostProcessQuadPassMarkedShader->m_shader->useInteger("sgpp_defaultFB.colorAttachmentsCount", 1);
    m_colorPostProcessQuadPassMarkedShader->m_shader->useInteger("sgpp_defaultFB.depthAttachmentsCount", 1);

    // -------------------------------------

    m_depthPostProcessQuadPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_depthPostProcessQuadPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(Graphics::getShaderPath(
                    Graphics::StandardShaderType::SG_DEPTH_POSTPROCESSING_SHADER)
            )
    );

    m_depthPostProcessQuadPassMarkedShader->addFrameBufferBlockDeclaration("allFB[0]", 1, 0, 1, 0);

    m_depthPostProcessQuadPassMarkedShader->calculateBlocksOffsets();

    m_depthPostProcessQuadPassMarkedShader->m_shader->bind();
    //m_depthPostProcessQuadPassMarkedShader->m_shader->useInteger("allFB[0].index", 0);
    m_depthPostProcessQuadPassMarkedShader->m_shader->useInteger("allFB[0].colorAttachmentsCount", 1);
    m_depthPostProcessQuadPassMarkedShader->m_shader->useInteger("allFB[0].depthAttachmentsCount", 1);
    m_depthPostProcessQuadPassMarkedShader->m_shader->useInteger("FBCount", m_postProcessLayers.size() + 1);
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

    auto& ppLayer = m_postProcessLayers[layer];
    // without - 1 because 0 is always default FB
    ppLayer.m_index = m_postProcessLayers.size();

    ppLayer.m_name = ppLayerName;
    ppLayer.m_frameBuffer = std::shared_ptr<Graphics::IFrameBuffer>(Core::Main::CoreMain::getRenderer().createFrameBuffer())
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

    m_colorPostProcessQuadPassMarkedShader->addFrameBufferBlockDeclaration(ppLayerName, 1, 0, 1, 0);

    m_colorPostProcessQuadPassMarkedShader->calculateBlocksOffsets();

    m_colorPostProcessQuadPassMarkedShader->m_shader->bind();
    m_colorPostProcessQuadPassMarkedShader->m_shader->useInteger(ppLayerName + ".colorAttachmentsCount", 1);
    m_colorPostProcessQuadPassMarkedShader->m_shader->useInteger(ppLayerName + ".depthAttachmentsCount", 1);

    // ----------------------------------

    std::string depthPassNewLayerName = "allFB[" + std::to_string(ppLayer.m_index) + "]";

    m_depthPostProcessQuadPassMarkedShader->addFrameBufferBlockDeclaration(depthPassNewLayerName, 1, 0, 1, 0);

    m_depthPostProcessQuadPassMarkedShader->calculateBlocksOffsets();

    m_depthPostProcessQuadPassMarkedShader->m_shader->bind();
    //m_depthPostProcessQuadPassMarkedShader->m_shader->useInteger(depthPassNewLayerName + ".index", newLayerIndex);
    m_depthPostProcessQuadPassMarkedShader->m_shader->useInteger(depthPassNewLayerName + ".colorAttachmentsCount", 1);
    m_depthPostProcessQuadPassMarkedShader->m_shader->useInteger(depthPassNewLayerName + ".depthAttachmentsCount", 1);
    m_depthPostProcessQuadPassMarkedShader->m_shader->useInteger("FBCount", m_postProcessLayers.size() + 1);
}

void Core::ECS::Camera::addPostProcessLayer(const std::string& ppLayerName,
                                            const std::shared_ptr<ECS::Layer>& layer)
{
    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Core::Main::Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    addPostProcessLayer(ppLayerName, layer, primaryMonitorWidth, primaryMonitorHeight);
}

void Core::ECS::Camera::bindPostProcessLayers() const noexcept
{
    m_defaultLayersFrameBuffer->bindAttachments(
            m_colorPostProcessQuadPassMarkedShader->getFrameBuffersAttachmentsBlocks()["sgpp_defaultFB"]
    );

    for(const auto& ppLayer : m_postProcessLayers)
    {
        ppLayer.second.m_frameBuffer->bindAttachments(
                m_colorPostProcessQuadPassMarkedShader
                        ->getFrameBuffersAttachmentsBlocks()[ppLayer.second.m_name]
        );
    }
}

std::shared_ptr<Core::Graphics::IFrameBuffer>
Core::ECS::Camera::getPostProcessLayerFrameBuffer(const std::shared_ptr<ECS::Layer>& layer) noexcept
{
    const auto& foundPPLayer = m_postProcessLayers.find(layer);

    return foundPPLayer != m_postProcessLayers.cend() ? foundPPLayer->second.m_frameBuffer : nullptr;
}

