//
// Created by stuka on 07.07.2023.
//

#include "VkRenderer.h"

void Core::Graphics::VkRenderer::init() noexcept
{

}

bool Core::Graphics::VkRenderer::confirmSupport() noexcept
{
    return false;
}

void Core::Graphics::VkRenderer::renderFrame(const glm::ivec2& windowSize)
{

}

void Core::Graphics::VkRenderer::renderMesh(const std::shared_ptr<ECS::CameraComponent>& cameraComponent,
                                            const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                                            const std::shared_ptr<ECS::MeshComponent>& meshComponent)
{

}

void Core::Graphics::VkRenderer::printInfo() noexcept
{

}

void Core::Graphics::VkRenderer::checkForErrors(const std::source_location& location) noexcept
{

}

Core::Graphics::VkShader* Core::Graphics::VkRenderer::createShader()
{
    return new VkShader;
}

Core::Graphics::VkVertexArray* Core::Graphics::VkRenderer::createVertexArray()
{
    return new VkVertexArray;
}

Core::Graphics::VkVertexBuffer* Core::Graphics::VkRenderer::createVertexBuffer()
{
    return new VkVertexBuffer;
}

Core::Graphics::VkVertexBufferLayout* Core::Graphics::VkRenderer::createVertexBufferLayout()
{
    return new VkVertexBufferLayout;
}

Core::Graphics::VkIndexBuffer* Core::Graphics::VkRenderer::createIndexBuffer()
{
    return new VkIndexBuffer;
}

Core::Graphics::VkTexture2D* Core::Graphics::VkRenderer::createTexture2D()
{
    return new VkTexture2D;
}

Core::Graphics::VkUniformBuffer* Core::Graphics::VkRenderer::createUniformBuffer()
{
    return new VkUniformBuffer;
}

Core::Graphics::VkMesh* Core::Graphics::VkRenderer::createMesh()
{
    return new VkMesh;
}

const std::shared_ptr<Core::Graphics::VkRenderer>& Core::Graphics::VkRenderer::getInstance() noexcept
{
    static std::shared_ptr<VkRenderer> s_instancePointer(new VkRenderer);
    s_instancePointer->m_apiType = APIType::VULKAN;

    return s_instancePointer;
}
