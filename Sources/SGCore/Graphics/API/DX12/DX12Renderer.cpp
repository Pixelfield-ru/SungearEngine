//
// Created by 8bitniksis on 18.08.2026.
//

#include "DX12Renderer.h"

#if defined(_WIN32)

#include <cstdlib>
#include <cstring>
#include <set>
#include <string>

#include "SGCore/Graphics/API/AttachmentReadback.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/IGPUObjectsStorage.h"
#include "SGCore/Graphics/RHI/LiveDevice.h"
#include "SGCore/Graphics/RHI/RHILegacyDraw.h"
#include "SGCore/Graphics/RHI/ScreenBlit.h"
#include "SGCore/Graphics/RHI/SharedUniformBuffers.h"
#include "SGCore/Graphics/RHI/TextureUnits.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Main/Window.h"

namespace
{
    /// Nothing to recreate on DX12: GPU objects do not die with the window (no context), same as on
    /// Vulkan.
    struct DX12ObjectsStorage final : SGCore::IGPUObjectsStorage
    {
        void recreateAll() noexcept override { }
        void clear() noexcept override { }
    };

    DX12ObjectsStorage s_storage;
}

SGCore::IGPUObjectsStorage& SGCore::DX12Renderer::storage() noexcept
{
    return s_storage;
}

const SGCore::IGPUObjectsStorage& SGCore::DX12Renderer::storage() const noexcept
{
    return s_storage;
}

const std::shared_ptr<SGCore::DX12Renderer>& SGCore::DX12Renderer::getInstance() noexcept
{
    static std::shared_ptr<DX12Renderer> s_instancePointer(new DX12Renderer);
    s_instancePointer->m_apiType = SG_API_TYPE_DX12;

    return s_instancePointer;
}

SGCore::DX12Renderer::~DX12Renderer()
{
    DX12Renderer::shutdown();
}

bool SGCore::DX12Renderer::confirmSupport() noexcept
{
#ifdef SUNGEAR_DEBUG
    bool enableDebugLayer = true;
#else
    bool enableDebugLayer = false;
#endif
    // SG_DX_VALIDATION=1/0 overrides the build default, the counterpart of SG_VK_VALIDATION
    if(const char* forced = std::getenv("SG_DX_VALIDATION"); forced && forced[0] != 0)
    {
        enableDebugLayer = *forced != '0';
        SG_LOG_I("DX12: debug layer {} by SG_DX_VALIDATION.", enableDebugLayer ? "forced on" : "forced off");
    }

    if(!m_context->create(enableDebugLayer))
    {
        m_context->destroy();
        return false;
    }

    m_device = std::make_unique<DX12Device>(m_context, CoreMain::getWindow().getNativeHandle());
    if(!m_device->isReady())
    {
        m_device.reset();
        m_context->destroy();
        return false;
    }
    s_liveDevice = m_device.get();
    LiveDevice::set(m_device.get());

    return true;
}

void SGCore::DX12Renderer::init() noexcept
{
    // every backend runs confirmSupport() from its own init(), as GL4Renderer and VkRenderer do
    if(!confirmSupport())
    {
        SG_LOG_C("DX12 renderer failed to initialize. Closing the window.");
        CoreMain::getWindow().setShouldClose(true);
        return;
    }

    printInfo();

    IRenderer::init();
}

void SGCore::DX12Renderer::printInfo() noexcept
{
    SG_LOG_I("-----------------------------------");
    SG_LOG_I("DirectX 12 info:");
    SG_LOG_I("Adapter: {}", m_context->getAdapterName());
    SG_LOG_I("Feature level: 12.0");
    SG_LOG_I("Debug layer: {}", m_context->m_debugLayerEnabled ? "on" : "off");
    if(m_device)
    {
        SG_LOG_I("Swapchain: {}x{}", m_device->getSwapchain().getWidth(), m_device->getSwapchain().getHeight());
    }
    SG_LOG_I("-----------------------------------");
}

void SGCore::DX12Renderer::checkForErrors(const std::source_location& /*location*/) noexcept
{
    // the debug layer has no callback: its queue is drained instead
    m_context->drainDebugMessages();
}

void SGCore::DX12Renderer::prepareFrame(const glm::ivec2& /*windowSize*/)
{
    if(!m_device) return;
    m_device->getSwapchain().beginFrame();
    // the per-draw uniform slices of the frames still in flight must stay untouched
    m_device->rotateUniformArena();
}

SGCore::IDevice* SGCore::DX12Renderer::getDevice() noexcept
{
    return m_device.get();
}

SGCore::ICommandList* SGCore::DX12Renderer::getFrameBufferCommandList() noexcept
{
    return getFrameBufferCommandListRef().get();
}

const SGCore::Ref<SGCore::ICommandList>& SGCore::DX12Renderer::getFrameBufferCommandListRef() noexcept
{
    if(!m_frameBufferCommandList && m_device)
    {
        m_frameBufferCommandList = m_device->createCommandList();
        m_frameBufferCommandList->setDebugName("legacy_framebuffer_list");
    }
    return m_frameBufferCommandList;
}

SGCore::DX12Device* SGCore::DX12Renderer::getLiveDevice() noexcept
{
    return s_liveDevice;
}

bool SGCore::DX12Renderer::readScreenPixels(AttachmentReadback& out) const noexcept
{
    if(!m_device) return false;

    auto texture = m_device->getDX12Swapchain().getCurrentTexture();
    if(!texture)
    {
        SG_LOG_W("DX12Renderer::readScreenPixels: no back buffer is acquired for this frame.");
        return false;
    }

    m_device->waitIdle();

    // a GPU->CPU texture copy goes through a row-pitch-aligned footprint, not a tight buffer
    const D3D12_RESOURCE_DESC resourceDesc = texture->getResource()->GetDesc();
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint { };
    UINT rowsCount = 0;
    UINT64 rowSizeInBytes = 0;
    UINT64 totalBytes = 0;
    m_context->m_device->GetCopyableFootprints(&resourceDesc, 0, 1, 0, &footprint, &rowsCount, &rowSizeInBytes, &totalBytes);

    auto readback = m_device->createReadbackResource(totalBytes, "screen_readback");
    if(!readback) return false;

    m_device->immediateSubmit([&](ID3D12GraphicsCommandList* commandList) {
        const D3D12_RESOURCE_STATES restore = texture->getState();
        texture->recordTransition(commandList, D3D12_RESOURCE_STATE_COPY_SOURCE);

        D3D12_TEXTURE_COPY_LOCATION destination { };
        destination.pResource = readback.Get();
        destination.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        destination.PlacedFootprint = footprint;

        D3D12_TEXTURE_COPY_LOCATION source { };
        source.pResource = texture->getResource();
        source.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        source.SubresourceIndex = 0;

        commandList->CopyTextureRegion(&destination, 0, 0, 0, &source, nullptr);
        texture->recordTransition(commandList, restore);
    });

    void* mapped = nullptr;
    const D3D12_RANGE readRange { 0, static_cast<SIZE_T>(totalBytes) };
    if(!SG_DX_CHECK(readback->Map(0, &readRange, &mapped)) || !mapped) return false;

    const std::uint32_t width = texture->getWidth();
    const std::uint32_t height = texture->getHeight();
    out.m_width = static_cast<std::int32_t>(width);
    out.m_height = static_cast<std::int32_t>(height);
    out.m_format = SGGColorFormat::SGG_RGBA;
    out.m_dataType = SGGDataType::SGG_UNSIGNED_BYTE;
    out.m_channelsCount = 4;
    out.m_data.resize(static_cast<std::size_t>(width) * height * 4);

    // rows are handed out bottom-up, the way glReadPixels does it, so consumers need no per-backend
    // flip (the same rule the Vulkan backend follows)
    const auto* source = static_cast<const std::uint8_t*>(mapped);
    const std::size_t destinationRowSize = static_cast<std::size_t>(width) * 4;
    for(std::uint32_t row = 0; row < height; ++row)
    {
        const std::uint8_t* sourceRow = source + static_cast<std::size_t>(height - 1 - row) * footprint.Footprint.RowPitch;
        std::memcpy(out.m_data.data() + static_cast<std::size_t>(row) * destinationRowSize, sourceRow, destinationRowSize);
    }

    const D3D12_RANGE writtenRange { 0, 0 };
    readback->Unmap(0, &writtenRange);
    return true;
}

void SGCore::DX12Renderer::shutdown() noexcept
{
    // legacy facades stop reaching the device from here on (assets are destroyed later, in static
    // destruction, when this object may already be gone)
    s_liveDevice = nullptr;
    LiveDevice::set(nullptr);

    TextureUnits::clear();
    SharedUniformBuffers::clear();

    m_currentLegacyShader = nullptr;

    if(m_device) m_device->waitIdle();
    m_screenBlit.reset();
    m_frameBufferCommandList.reset();
    m_device.reset();
    m_context->destroy();
}

void SGCore::DX12Renderer::reload() noexcept
{
}



// ---- resource factories

SGCore::IShader* SGCore::DX12Renderer::createShader() { return new DX12Shader; }
SGCore::IVertexArray* SGCore::DX12Renderer::createVertexArray() { return new RHIVertexArray; }
SGCore::IVertexBuffer* SGCore::DX12Renderer::createVertexBuffer() { return new RHIVertexBuffer; }
SGCore::IIndexBuffer* SGCore::DX12Renderer::createIndexBuffer() { return new RHIIndexBuffer; }
SGCore::ITexture2D* SGCore::DX12Renderer::createTexture2D() { return new DX12Texture2D; }
SGCore::ICubemapTexture* SGCore::DX12Renderer::createCubemapTexture() { return new DX12CubemapTexture; }
SGCore::IUniformBuffer* SGCore::DX12Renderer::createUniformBuffer() { return new RHIUniformBuffer; }
SGCore::IFrameBuffer* SGCore::DX12Renderer::createFrameBuffer() { return new DX12FrameBuffer; }

// no backend-specific mesh data: IMeshData::prepare() builds the buffers through the factories
SGCore::IMeshData* SGCore::DX12Renderer::createMeshData() const { return new IMeshData; }

// ---- drawing

void SGCore::DX12Renderer::renderMeshData(const IMeshData* meshData, const MeshRenderState& meshRenderState)
{
    if(!meshData || !m_device || !m_currentLegacyShader || !m_frameBufferCommandList) return;

    RHILegacyDraw::drawMesh(*m_device, *m_frameBufferCommandList, *m_currentLegacyShader, m_cachedRenderState,
                            *meshData, meshRenderState);
}

void SGCore::DX12Renderer::renderArray(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                       const int& verticesCount, const int& indicesCount)
{
    if(!m_device || !m_currentLegacyShader || !m_frameBufferCommandList) return;

    RHILegacyDraw::drawArray(*m_device, *m_frameBufferCommandList, *m_currentLegacyShader, m_cachedRenderState,
                             vertexArray, meshRenderState, verticesCount, indicesCount, 1);
}

void SGCore::DX12Renderer::renderArrayInstanced(const Ref<IVertexArray>& vertexArray, const MeshRenderState& meshRenderState,
                                                const int& verticesCount, const int& indicesCount, const int& instancesCount)
{
    if(!m_device || !m_currentLegacyShader || !m_frameBufferCommandList) return;

    RHILegacyDraw::drawArray(*m_device, *m_frameBufferCommandList, *m_currentLegacyShader, m_cachedRenderState,
                             vertexArray, meshRenderState, verticesCount, indicesCount, instancesCount);
}

// ---- fixed-function state: it lives in pipelines here, so it is only remembered for the next PSO

void SGCore::DX12Renderer::useState(const RenderState& newRenderState, bool) noexcept
{
    m_cachedRenderState = newRenderState;
}

void SGCore::DX12Renderer::useBlendingState(const BlendingState& newBlendingState, bool) noexcept
{
    m_cachedRenderState.m_globalBlendingState = newBlendingState;
}

void SGCore::DX12Renderer::useMeshRenderState(const MeshRenderState& newMeshRenderState, bool) noexcept
{
    m_cachedMeshRenderState = newMeshRenderState;
}

void SGCore::DX12Renderer::bindScreenFrameBuffer() const noexcept
{
    // the window is the implicit target of a render pass with RenderPassBeginDesc::m_frameBuffer == nullptr
}

void SGCore::DX12Renderer::setViewport(int, int, int, int) const noexcept
{
    // viewport is per command list here (ICommandList::setViewport)
}

void SGCore::DX12Renderer::renderTextureOnScreen(const ITexture2D* texture, bool flipOutput,
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
            SG_LOG_E("DX12Renderer: RHI ScreenBlit failed to initialize.");
                }
    }
    if(!m_screenBlit) return;

    m_screenBlit->blit(texture, flipOutput, x, y, width, height);
}

#endif
