//
// Created by stuka on 07.07.2023.
//

#include "VkRenderer.h"

#include "SGCore/Main/CoreSettings.h"

#include <GLFW/glfw3.h>

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Rendering/CameraComponent.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterComponent.h"

void Core::Graphics::VkRenderer::init() noexcept
{
    /*m_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    m_applicationInfo.pApplicationName = "Sungear Engine Core";
    m_applicationInfo.applicationVersion = VK_MAKE_VERSION(SG_CORE_MAJOR_VERSION, SG_CORE_MINOR_VERSION, SG_CORE_PATCH_VERSION);
    m_applicationInfo.pEngineName = "Sungear Engine";
    m_applicationInfo.engineVersion = VK_MAKE_VERSION(SG_CORE_MAJOR_VERSION, SG_CORE_MINOR_VERSION, SG_CORE_PATCH_VERSION);
    m_applicationInfo.apiVersion = VK_API_VERSION_1_0;

    m_instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    m_instanceCreateInfo.pApplicationInfo = &m_applicationInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    m_instanceCreateInfo.enabledExtensionCount = glfwExtensionCount;
    m_instanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;

    m_instanceCreateInfo.enabledLayerCount = 0;*/

    /*if(vkCreateInstance(&m_instanceCreateInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
    {
        SGCF_ERROR("Can not initialize Vulkan renderer.", SG_LOG_CURRENT_SESSION_FILE);
    }*/
}

bool Core::Graphics::VkRenderer::confirmSupport() noexcept
{
    return false;
}

void Core::Graphics::VkRenderer::renderFrame(const glm::ivec2& windowSize)
{

}

void Core::Graphics::VkRenderer::renderMesh(const std::shared_ptr<ECS::TransformComponent>& transformComponent,
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

Core::Graphics::VkShader* Core::Graphics::VkRenderer::createShader(const std::string& path)
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

Core::Graphics::VkCubemapTexture* Core::Graphics::VkRenderer::createCubemapTexture()
{
    return new VkCubemapTexture;
}

Core::Graphics::VkUniformBuffer* Core::Graphics::VkRenderer::createUniformBuffer()
{
    return new VkUniformBuffer;
}

Core::Graphics::VkFrameBuffer* Core::Graphics::VkRenderer::createFrameBuffer()
{
    return new VkFrameBuffer;
}

Core::Graphics::VkMesh* Core::Graphics::VkRenderer::createMesh()
{
    return new VkMesh;
}

// TODO: impl
Core::Memory::Assets::IMaterial *Core::Graphics::VkRenderer::createPBRMaterial()
{
    return nullptr;
}

const std::shared_ptr<Core::Graphics::VkRenderer>& Core::Graphics::VkRenderer::getInstance() noexcept
{
    static std::shared_ptr<VkRenderer> s_instancePointer(new VkRenderer);
    s_instancePointer->m_apiType = SG_API_TYPE_VULKAN;

    return s_instancePointer;
}
