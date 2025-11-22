//
// Created by stuka on 07.07.2023.
//

#include "VkRenderer.h"

#include "SGCore/Main/CoreSettings.h"

void SGCore::VkRenderer::init() noexcept
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

    IRenderer::init();
}

bool SGCore::VkRenderer::confirmSupport() noexcept
{
    return false;
}

void SGCore::VkRenderer::prepareFrame(const glm::ivec2& windowSize)
{

}

void SGCore::VkRenderer::printInfo() noexcept
{

}

void SGCore::VkRenderer::checkForErrors(const std::source_location& location) noexcept
{

}

SGCore::VkShader* SGCore::VkRenderer::createShader()
{
    return new VkShader;
}

SGCore::VkVertexArray* SGCore::VkRenderer::createVertexArray()
{
    return new VkVertexArray;
}

SGCore::VkVertexBuffer* SGCore::VkRenderer::createVertexBuffer()
{
    return new VkVertexBuffer;
}

SGCore::VkIndexBuffer* SGCore::VkRenderer::createIndexBuffer()
{
    return new VkIndexBuffer;
}

SGCore::VkTexture2D* SGCore::VkRenderer::createTexture2D()
{
    return new VkTexture2D;
}

SGCore::VkCubemapTexture* SGCore::VkRenderer::createCubemapTexture()
{
    return new VkCubemapTexture;
}

SGCore::VkUniformBuffer* SGCore::VkRenderer::createUniformBuffer()
{
    return new VkUniformBuffer;
}

SGCore::VkFrameBuffer* SGCore::VkRenderer::createFrameBuffer()
{
    return new VkFrameBuffer;
}

SGCore::VkMeshData* SGCore::VkRenderer::createMeshData() const
{
    return new VkMeshData;
}

void SGCore::VkRenderer::bindScreenFrameBuffer() const noexcept
{
    // todo: impl
}

SGCore::IGPUObjectsStorage& SGCore::VkRenderer::storage() noexcept
{
    return **m_dummyStorage;
}

const SGCore::IGPUObjectsStorage& SGCore::VkRenderer::storage() const noexcept
{
    return **m_dummyStorage;
}

void SGCore::VkRenderer::reload() noexcept
{

}

const std::shared_ptr<SGCore::VkRenderer>& SGCore::VkRenderer::getInstance() noexcept
{
    static std::shared_ptr<VkRenderer> s_instancePointer(new VkRenderer);
    s_instancePointer->m_apiType = SG_API_TYPE_VULKAN;

    return s_instancePointer;
}

void SGCore::VkRenderer::useState(const SGCore::RenderState& newRenderState, bool forceState) noexcept
{

}

void SGCore::VkRenderer::useBlendingState(const SGCore::BlendingState& newBlendingState, bool forceState) noexcept
{

}

void SGCore::VkRenderer::useMeshRenderState(const SGCore::MeshRenderState& newMeshRenderState, bool forceState) noexcept
{

}
