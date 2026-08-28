//
// Created by stuka on 07.07.2023.
//

#include "VkRenderer.h"

#include <cstdlib>
#include <cstring>

#include "RHI/VulkanCommandList.h"
#include "RHI/VulkanDevice.h"
#include "RHI/VulkanGPUBuffer.h"
#include "SGCore/Graphics/RHI/LiveDevice.h"
#include "SGCore/Graphics/RHI/RHILegacyDraw.h"
#include "SGCore/Graphics/RHI/SharedUniformBuffers.h"
#include "SGCore/Graphics/RHI/TextureUnits.h"
#include "SGCore/Graphics/API/AttachmentReadback.h"
#include <algorithm>

#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/IGPUObjectsStorage.h"
#include "SGCore/Graphics/RHI/ScreenBlit.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Main/CoreSettings.h"

namespace
{
    /// Nothing to recreate on Vulkan: GPU objects do not die with the window (no context).
    struct VulkanObjectsStorage final : SGCore::IGPUObjectsStorage
    {
        void recreateAll() noexcept override { }
        void clear() noexcept override { }
    };

    VulkanObjectsStorage s_storage;
}

SGCore::VkRenderer::~VkRenderer()
{
    shutdown();
}

void SGCore::VkRenderer::shutdown() noexcept
{
    // legacy facades stop reaching the device from here on (assets are destroyed later, in static
    // destruction, when this object may already be gone)
    s_liveDevice = nullptr;
    LiveDevice::set(nullptr);
    m_currentLegacyShader = nullptr;
    TextureUnits::clear();
    SharedUniformBuffers::clear();

    if(m_device) m_device->waitIdle();
    m_screenBlit.reset();
    if(m_dummyTexelBufferView != VK_NULL_HANDLE)
    {
        vkDestroyBufferView(m_context->m_device, m_dummyTexelBufferView, nullptr);
        m_dummyTexelBufferView = VK_NULL_HANDLE;
    }
    m_dummyTexelBuffer.reset();
    m_dummyTexture.reset();
    m_frameBufferCommandList.reset();
    m_device.reset();
    m_context->destroy();
}

void SGCore::VkRenderer::init() noexcept
{
    if(!confirmSupport())
    {
        SG_LOG_C("Vulkan renderer failed to initialize. Closing the window.");
        CoreMain::getWindow().setShouldClose(true);
        return;
    }

    m_frameBufferCommandList = m_device->createCommandList();
    m_frameBufferCommandList->setDebugName("legacy_framebuffer_list");

    printInfo();

    IRenderer::init();
}

bool SGCore::VkRenderer::confirmSupport() noexcept
{
    if(m_device && m_device->isReady()) return true;

#ifdef SUNGEAR_DEBUG
    bool enableValidation = true;
#else
    bool enableValidation = false;
#endif
    // SG_VK_VALIDATION=1/0 overrides the build default, so a Release build can be inspected with the
    // layers on (and a Debug run can skip them when they get in the way)
    if(const char* forced = std::getenv("SG_VK_VALIDATION"); forced && forced[0] != 0)
    {
        enableValidation = *forced != '0';
        SG_LOG_I("Vulkan: validation layers {} by SG_VK_VALIDATION.", enableValidation ? "forced on" : "forced off");
    }

    if(!m_context->createInstance(enableValidation)) return false;
    if(!m_context->createSurface(CoreMain::getWindow().getNativeHandle())) return false;
    if(!m_context->pickPhysicalDevice()) return false;
    if(!m_context->createDevice()) return false;
    if(!m_context->createAllocator()) return false;

    m_device = std::make_unique<VulkanDevice>(*m_context, CoreMain::getWindow().getNativeHandle());
    if(!m_device->isReady())
    {
        m_device.reset();
        return false;
    }
    s_liveDevice = m_device.get();
    LiveDevice::set(m_device.get());
    return true;
}

void SGCore::VkRenderer::prepareFrame(const glm::ivec2& /*windowSize*/)
{
    if(!m_device) return;
    m_device->getSwapchain().beginFrame();
    // the per-draw uniform slices of the frames still in flight must stay untouched
    m_device->rotateUniformArena();
}

void SGCore::VkRenderer::printInfo() noexcept
{
    SG_LOG_I("-----------------------------------");
    SG_LOG_I("Vulkan info:");
    SG_LOG_I("Device: {}", m_context->getDeviceName());
    SG_LOG_I("Validation layers: {}", m_context->m_validationEnabled ? "on" : "off");
    SG_LOG_I("Depth clip control (GL depth range): {}", m_context->m_depthClipControl ? "yes" : "no");
    if(m_device)
    {
        SG_LOG_I("Swapchain: {}x{}", m_device->getSwapchain().getWidth(), m_device->getSwapchain().getHeight());
    }
    SG_LOG_I("-----------------------------------");
}

void SGCore::VkRenderer::checkForErrors(const std::source_location& /*location*/) noexcept
{
    // Vulkan reports through the validation layer callback (VulkanContext); nothing to poll
}

SGCore::VkShader* SGCore::VkRenderer::createShader()
{
    return new VkShader;
}

SGCore::IVertexArray* SGCore::VkRenderer::createVertexArray()
{
    return new RHIVertexArray;
}

SGCore::IVertexBuffer* SGCore::VkRenderer::createVertexBuffer()
{
    return new RHIVertexBuffer;
}

SGCore::IIndexBuffer* SGCore::VkRenderer::createIndexBuffer()
{
    return new RHIIndexBuffer;
}

SGCore::VkTexture2D* SGCore::VkRenderer::createTexture2D()
{
    return new VkTexture2D;
}

SGCore::VkCubemapTexture* SGCore::VkRenderer::createCubemapTexture()
{
    return new VkCubemapTexture;
}

SGCore::IUniformBuffer* SGCore::VkRenderer::createUniformBuffer()
{
    return new RHIUniformBuffer;
}

SGCore::VkFrameBuffer* SGCore::VkRenderer::createFrameBuffer()
{
    return new VkFrameBuffer;
}

SGCore::IMeshData* SGCore::VkRenderer::createMeshData() const
{
    // no backend-specific mesh data: IMeshData::prepare() builds the buffers through the factories
    // (the old VkMeshData stubbed prepare() out, which left every mesh without vertex buffers)
    return new IMeshData;
}

void SGCore::VkRenderer::bindScreenFrameBuffer() const noexcept
{
    // the window is the implicit target of a render pass with RenderPassBeginDesc::m_frameBuffer == nullptr
}

void SGCore::VkRenderer::setViewport(int /*x*/, int /*y*/, int /*width*/, int /*height*/) const noexcept
{
    // viewport is per command list on Vulkan (ICommandList::setViewport)
}

void SGCore::VkRenderer::renderTextureOnScreen(const ITexture2D* texture, bool flipOutput,
                                               int x, int y, int width, int height) noexcept
{
    if(!m_device) return;

    // lazy: the screen shader asset is loadable only after the asset manager is up, later than init()
    if(!m_screenBlitInitTried)
    {
        m_screenBlitInitTried = true;
        m_screenBlit = std::make_unique<ScreenBlit>();
        if(!m_screenBlit->init(*m_device))
        {
            SG_LOG_E("VkRenderer: RHI ScreenBlit failed to initialize.");
            m_screenBlit.reset();
        }
    }
    if(!m_screenBlit) return;

    m_screenBlit->blit(texture, flipOutput, x, y, width, height);
}

bool SGCore::VkRenderer::readScreenPixels(AttachmentReadback& out) const noexcept
{
    if(!m_device) return false;
    auto& swapchain = m_device->getVulkanSwapchain();
    auto texture = swapchain.getCurrentTexture();
    if(!texture)
    {
        SG_LOG_W("VkRenderer::readScreenPixels: no swapchain image is acquired for this frame.");
        return false;
    }

    const auto extent = texture->getExtent();
    const std::uint64_t pixelCount = static_cast<std::uint64_t>(extent.width) * extent.height;
    m_device->waitIdle();

    auto staging = m_device->createStagingBuffer(pixelCount * 4, "screen_readback");
    if(!staging) return false;

    m_device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
        const VkImageLayout restore = texture->getLayout() == VK_IMAGE_LAYOUT_UNDEFINED ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : texture->getLayout();
        texture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        VkBufferImageCopy region { };
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.layerCount = 1;
        region.imageExtent = { extent.width, extent.height, 1 };
        vkCmdCopyImageToBuffer(commandBuffer, texture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, staging->getHandle(), 1, &region);
        texture->recordTransition(commandBuffer, restore);
    });

    out.m_width = static_cast<std::int32_t>(extent.width);
    out.m_height = static_cast<std::int32_t>(extent.height);
    out.m_format = SGGColorFormat::SGG_RGBA;
    out.m_dataType = SGGDataType::SGG_UNSIGNED_BYTE;
    out.m_channelsCount = 4;
    out.m_data.resize(pixelCount * 4);

    // window passes are flipped (row 0 = top of the screen); hand the rows out bottom-up like glReadPixels
    const bool bgra = texture->getFormat() == VK_FORMAT_B8G8R8A8_UNORM || texture->getFormat() == VK_FORMAT_B8G8R8A8_SRGB;
    const auto* src = static_cast<const std::uint8_t*>(staging->getMappedPointer());
    const std::size_t rowSize = static_cast<std::size_t>(extent.width) * 4;
    for(std::uint32_t row = 0; row < extent.height; ++row)
    {
        const std::uint8_t* srcRow = src + static_cast<std::size_t>(extent.height - 1 - row) * rowSize;
        std::uint8_t* dstRow = out.m_data.data() + static_cast<std::size_t>(row) * rowSize;
        if(!bgra)
        {
            std::memcpy(dstRow, srcRow, rowSize);
            continue;
        }
        for(std::uint32_t x = 0; x < extent.width; ++x)
        {
            dstRow[x * 4 + 0] = srcRow[x * 4 + 2];
            dstRow[x * 4 + 1] = srcRow[x * 4 + 1];
            dstRow[x * 4 + 2] = srcRow[x * 4 + 0];
            dstRow[x * 4 + 3] = srcRow[x * 4 + 3];
        }
    }
    return true;
}

SGCore::IGPUObjectsStorage& SGCore::VkRenderer::storage() noexcept
{
    return s_storage;
}

const SGCore::IGPUObjectsStorage& SGCore::VkRenderer::storage() const noexcept
{
    return s_storage;
}

void SGCore::VkRenderer::reload() noexcept
{
    // TODO(vulkan): recreate the surface/swapchain after window recreation (task 1.3a)
}

SGCore::IDevice* SGCore::VkRenderer::getDevice() noexcept
{
    return m_device.get();
}

SGCore::VulkanDevice* SGCore::VkRenderer::getLiveDevice() noexcept
{
    return s_liveDevice;
}

const SGCore::Ref<SGCore::VulkanTexture>& SGCore::VkRenderer::getDummyTexture() noexcept
{
    // the image itself belongs to the device: command lists need it too (see VulkanCommandList's
    // substitution for an attachment that is bound as a sampler while the pass draws into it)
    static const Ref<VulkanTexture> none;
    return m_device ? m_device->getDummyTexture() : none;
}

VkBufferView SGCore::VkRenderer::getDummyTexelBufferView() noexcept
{
    if(m_dummyTexelBufferView != VK_NULL_HANDLE || !m_device) return m_dummyTexelBufferView;

    GPUBufferDesc desc;
    desc.m_size = 4 * sizeof(float);
    desc.m_usage = GPUBufferUsage::SGG_STORAGE_BUFFER;
    desc.m_access = GPUMemoryAccess::SGG_HOST_VISIBLE;
    desc.m_debugName = "dummy_texel_buffer";
    m_dummyTexelBuffer = m_device->createBuffer(desc);
    if(!m_dummyTexelBuffer) return m_dummyTexelBufferView;

    const float zeros[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_dummyTexelBuffer->write(zeros, sizeof(zeros));

    VkBufferViewCreateInfo viewInfo { };
    viewInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    viewInfo.buffer = static_cast<VulkanGPUBuffer*>(m_dummyTexelBuffer.get())->getHandle();
    viewInfo.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    viewInfo.offset = 0;
    viewInfo.range = VK_WHOLE_SIZE;
    SG_VK_CHECK(vkCreateBufferView(m_context->m_device, &viewInfo, nullptr, &m_dummyTexelBufferView));

    return m_dummyTexelBufferView;
}

SGCore::ICommandList* SGCore::VkRenderer::getFrameBufferCommandList() noexcept
{
    return m_frameBufferCommandList.get();
}

const SGCore::Ref<SGCore::ICommandList>& SGCore::VkRenderer::getFrameBufferCommandListRef() noexcept
{
    return m_frameBufferCommandList;
}

const std::shared_ptr<SGCore::VkRenderer>& SGCore::VkRenderer::getInstance() noexcept
{
    static std::shared_ptr<VkRenderer> s_instancePointer(new VkRenderer);
    s_instancePointer->m_apiType = SG_API_TYPE_VULKAN;

    return s_instancePointer;
}

void SGCore::VkRenderer::renderArray(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                    const int& verticesCount, const int& indicesCount)
{
    drawLegacyArray(vertexArray, meshRenderState, verticesCount, indicesCount, 1);
}

void SGCore::VkRenderer::renderArrayInstanced(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                              const int& verticesCount, const int& indicesCount, const int& instancesCount)
{
    drawLegacyArray(vertexArray, meshRenderState, verticesCount, indicesCount, instancesCount);
}

void SGCore::VkRenderer::drawLegacyArray(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                         int verticesCount, int indicesCount, int instancesCount) noexcept
{
    if(!m_device || !m_currentLegacyShader || !m_frameBufferCommandList) return;

    RHILegacyDraw::drawArray(*m_device, *m_frameBufferCommandList, *m_currentLegacyShader, m_cachedRenderState,
                             vertexArray, meshRenderState, verticesCount, indicesCount, instancesCount);
}

void SGCore::VkRenderer::useState(const SGCore::RenderState& newRenderState, bool /*forceState*/) noexcept
{
    // state lives in pipelines on Vulkan: remember it for the next draw's PSO
    m_cachedRenderState = newRenderState;
}

void SGCore::VkRenderer::useBlendingState(const SGCore::BlendingState& newBlendingState, bool /*forceState*/) noexcept
{
    m_cachedBlendingState = newBlendingState;
}

void SGCore::VkRenderer::useMeshRenderState(const SGCore::MeshRenderState& newMeshRenderState, bool /*forceState*/) noexcept
{
    m_cachedMeshRenderState = newMeshRenderState;
}

void SGCore::VkRenderer::renderMeshData(const IMeshData* meshData, const MeshRenderState& meshRenderState)
{
    if(!meshData || !m_device || !m_currentLegacyShader || !m_frameBufferCommandList) return;

    RHILegacyDraw::drawMesh(*m_device, *m_frameBufferCommandList, *m_currentLegacyShader, m_cachedRenderState,
                            *meshData, meshRenderState);
}
