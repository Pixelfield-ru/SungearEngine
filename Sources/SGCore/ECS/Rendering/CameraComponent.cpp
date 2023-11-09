//
// Created by stuka on 31.10.2023.
//
#include "CameraComponent.h"

#include "SGCore/Main/CoreMain.h"

Core::ECS::CameraComponent::CameraComponent()
{
    m_billboard = std::shared_ptr<ImportedScene::IMesh>(Core::Main::CoreMain::getRenderer().createMesh());

    m_billboard->m_indices.push_back(0);
    m_billboard->m_indices.push_back(2);
    m_billboard->m_indices.push_back(1);

    m_billboard->m_indices.push_back(0);
    m_billboard->m_indices.push_back(3);
    m_billboard->m_indices.push_back(2);

    m_billboard->prepare();

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
                                    0
                    )
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

    m_postProcessQuadPassMarkedShader = std::make_shared<Graphics::MarkedShader>();
    m_postProcessQuadPassMarkedShader->m_shader = std::shared_ptr<Graphics::IShader>(
            Core::Main::CoreMain::getRenderer().createShader(Graphics::getShaderPath(
                    Graphics::StandardShaderType::SG_POSTPROCESSING_SHADER)
            )
    );

    m_postProcessQuadPassMarkedShader->addFrameBufferBlockDeclaration("sgpp_defaultFB", 1, 0, 1, 0);

    m_postProcessQuadPassMarkedShader->calculateBlocksOffsets();

    m_postProcessQuadPassMarkedShader->m_shader->bind();
    m_postProcessQuadPassMarkedShader->m_shader->useInteger("sgpp_defaultFB.colorAttachmentsCount", 1);
    m_postProcessQuadPassMarkedShader->m_shader->useInteger("sgpp_defaultFB.depthAttachmentsCount", 1);
}

void Core::ECS::CameraComponent::addPostProcessLayer(const std::string& ppLayerName,
                                                     const std::shared_ptr<ECS::Layer>& layer)
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

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    Core::Main::Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    ppLayer.m_name = ppLayerName;
    ppLayer.m_frameBuffer = std::shared_ptr<Graphics::IFrameBuffer>(Core::Main::CoreMain::getRenderer().createFrameBuffer())
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
                            0
            )
            ->unbind();

    m_postProcessQuadPassMarkedShader->addFrameBufferBlockDeclaration(ppLayerName, 1, 0, 1, 0);

    m_postProcessQuadPassMarkedShader->calculateBlocksOffsets();

    m_postProcessQuadPassMarkedShader->m_shader->bind();
    m_postProcessQuadPassMarkedShader->m_shader->useInteger(ppLayerName + ".colorAttachmentsCount", 1);
    m_postProcessQuadPassMarkedShader->m_shader->useInteger(ppLayerName + ".depthAttachmentsCount", 1);
}

void Core::ECS::CameraComponent::bindPostProcessLayers() const noexcept
{
    m_defaultLayersFrameBuffer->bindAttachments(
            m_postProcessQuadPassMarkedShader->getFrameBuffersAttachmentsBlocks()["sgpp_defaultFB"]
    );

    for(const auto& ppLayer : m_postProcessLayers)
    {
        ppLayer.second.m_frameBuffer->bindAttachments(
                m_postProcessQuadPassMarkedShader
                        ->getFrameBuffersAttachmentsBlocks()[ppLayer.second.m_name]
        );
    }
}

std::shared_ptr<Core::Graphics::IFrameBuffer>
Core::ECS::CameraComponent::getPostProcessLayerFrameBuffer(const std::shared_ptr<ECS::Layer>& layer) noexcept
{
    const auto& foundPPLayer = m_postProcessLayers.find(layer);

    return foundPPLayer != m_postProcessLayers.cend() ? foundPPLayer->second.m_frameBuffer : nullptr;
}

