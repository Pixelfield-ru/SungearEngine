//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12Device.h"

#if defined(_WIN32)

#include <cstring>

#include "SGCore/Logger/Logger.h"
#include "DX12CommandList.h"
#include "DX12DescriptorSet.h"
#include "DX12GPUBuffer.h"
#include "DX12PipelineState.h"
#include "DX12ShaderProgram.h"

SGCore::DX12Device::DX12Device(const std::shared_ptr<DX12Context>& context, GLFWwindow* window) noexcept : m_context(context)
{
    m_properties.m_apiType = SG_API_TYPE_DX12;
    m_properties.m_originBottomLeft = false;
    // The engine keeps GL projections ([-1; 1] clip space z). D3D12 clips to [0; 1] and has no
    // counterpart of VK_EXT_depth_clip_control, so the conversion is done in the shader instead
    // (spirv-cross fixup_clipspace, see DX12ShaderCompiler) — from the engine's side the GL
    // convention still holds.
    m_properties.m_depthZeroToOne = false;
    m_properties.m_ndcYFlipRequired = true;
    m_properties.m_framesInFlight = DX12Swapchain::frames_in_flight;
    // root constants; their mapping arrives with the HLSL translation (task 3.4)
    m_properties.m_pushConstantsMaxSize = 128;
    // D3D12_REQ_BUFFER_RESOURCE_TEXEL_COUNT_2_TO_EXP: a buffer SRV addresses at most 2^27 texels
    m_properties.m_maxTexelBufferElements = 1u << 27;
    m_properties.m_supportsExplicitBarriers = true;
    m_properties.m_supportsMultithreadedRecording = true;
    m_properties.m_supportsBindless = false;

    if(!m_context || !m_context->isReady()) return;

    if(!SG_DX_CHECK(m_context->m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)))) return;
    m_context->setObjectName(m_fence.Get(), "submission_fence");

    m_fenceEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    if(!m_fenceEvent)
    {
        SG_LOG_E("DX12Device: could not create the fence event.");
        return;
    }

    // descriptor tables of the draws in flight are copied here; the ring wraps around, so a region
    // is reused only frames after the submission that read it finished
    if(!m_viewRing.create(m_context->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 4096, true)) return;
    // 2048 is the hardware limit for a shader-visible sampler heap
    if(!m_samplerRing.create(m_context->m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 2048, true)) return;

    m_swapchain = std::make_unique<DX12Swapchain>(*this, window);
    if(!m_swapchain->isValid())
    {
        SG_LOG_E("DX12Device: swapchain creation failed.");
        return;
    }

    m_ready = true;
}

SGCore::DX12Device::~DX12Device()
{
    waitIdle();
    // command lists go before the swapchain: they are what could still reference its back buffers
    m_freeContexts.clear();
    m_pipelineCache.clear();
    for(auto& region : m_uniformArena) region.m_chunks.clear();
    m_dummyTexture.reset();
    m_swapchain.reset();
    m_viewRing.destroy();
    m_samplerRing.destroy();
    if(m_fenceEvent)
    {
        CloseHandle(m_fenceEvent);
        m_fenceEvent = nullptr;
    }
}

// ---- IDevice ------------------------------------------------------------------------------------

SGCore::Ref<SGCore::IGPUBuffer> SGCore::DX12Device::createBuffer(const GPUBufferDesc& desc) noexcept
{
    auto buffer = MakeRef<DX12GPUBuffer>(*this, desc);
    return buffer->isValid() ? buffer : nullptr;
}

SGCore::Ref<SGCore::IShaderProgram> SGCore::DX12Device::createShaderProgram(const ShaderProgramDesc& desc) noexcept
{
    return MakeRef<DX12ShaderProgram>(*this, desc);
}

SGCore::Ref<SGCore::IDescriptorSet> SGCore::DX12Device::createDescriptorSet() noexcept
{
    return MakeRef<DX12DescriptorSet>(*this);
}

SGCore::Ref<SGCore::IPipelineState> SGCore::DX12Device::getOrCreatePipeline(const PipelineStateDesc& desc) noexcept
{
    auto& bucket = m_pipelineCache[desc.hash()];
    for(const auto& existing : bucket)
    {
        if(existing->getDesc().equalsForCache(desc)) return existing;
    }
    auto pipeline = MakeRef<DX12PipelineState>(*this, desc);
    bucket.push_back(pipeline);
    return pipeline;
}

SGCore::Ref<SGCore::ICommandList> SGCore::DX12Device::createCommandList() noexcept
{
    return MakeRef<DX12CommandList>(*this);
}

void SGCore::DX12Device::submit(const Ref<ICommandList>& commandList) noexcept
{
    auto* list = static_cast<DX12CommandList*>(commandList.get());
    if(!list) return;
    if(list->isRecording()) list->end();
    if(!list->hasRecordedWork()) return;

    auto submission = list->takeSubmission();
    if(submission.m_contexts.empty()) return;

    submission.m_keepAlive.push_back(commandList);
    submitRaw(std::move(submission));
}

void SGCore::DX12Device::destroyDeferred(Ref<IGPUObject> object) noexcept
{
    if(!object) return;
    // rides along with whatever gets submitted next, or is released on waitIdle()
    m_deferredDestroy.push_back(std::move(object));
}

void SGCore::DX12Device::waitIdle() noexcept
{
    if(!m_fence) return;

    // an empty submit is not needed: the counter already names the last signalled value
    if(m_submissionCounter > 0) waitForSubmission(m_submissionCounter);
    retire(true);
    m_deferredDestroy.clear();
}

// ---- backend services ---------------------------------------------------------------------------

SGCore::DX12CommandContext SGCore::DX12Device::acquireCommandContext() noexcept
{
    if(!m_freeContexts.empty())
    {
        DX12CommandContext context = std::move(m_freeContexts.back());
        m_freeContexts.pop_back();
        if(SG_DX_CHECK(context.m_allocator->Reset()) && SG_DX_CHECK(context.m_list->Reset(context.m_allocator.Get(), nullptr)))
        {
            return context;
        }
        return { };
    }

    DX12CommandContext context;
    if(!SG_DX_CHECK(m_context->m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&context.m_allocator))))
    {
        return { };
    }
    // a freshly created command list is already recording
    if(!SG_DX_CHECK(m_context->m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, context.m_allocator.Get(),
                                                           nullptr, IID_PPV_ARGS(&context.m_list))))
    {
        return { };
    }
    return context;
}

std::uint64_t SGCore::DX12Device::submitRaw(DX12Submission&& submission) noexcept
{
    std::vector<ID3D12CommandList*> lists;
    lists.reserve(submission.m_contexts.size());
    for(const auto& context : submission.m_contexts)
    {
        if(context.m_list) lists.push_back(context.m_list.Get());
    }

    PendingSubmission pending;
    pending.m_id = ++m_submissionCounter;
    pending.m_payload = std::move(submission);
    // objects queued for deferred destruction ride along with this submission
    for(auto& object : m_deferredDestroy) pending.m_payload.m_keepAlive.push_back(std::move(object));
    m_deferredDestroy.clear();

    if(!lists.empty())
    {
        m_context->m_directQueue->ExecuteCommandLists(static_cast<UINT>(lists.size()), lists.data());
    }
    SG_DX_CHECK(m_context->m_directQueue->Signal(m_fence.Get(), pending.m_id));

    m_pending.push_back(std::move(pending));
    // the debug layer has no callback: whatever the submission produced is drained here
    m_context->drainDebugMessages();
    return m_submissionCounter;
}

void SGCore::DX12Device::waitForSubmission(std::uint64_t id) noexcept
{
    if(id == 0 || !m_fence) return;
    if(m_fence->GetCompletedValue() < id)
    {
        if(SG_DX_CHECK(m_fence->SetEventOnCompletion(id, m_fenceEvent)))
        {
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }
    }
    retire(false);
}

void SGCore::DX12Device::retire(bool waitAll) noexcept
{
    if(!m_fence) return;

    while(!m_pending.empty())
    {
        auto& pending = m_pending.front();
        if(m_fence->GetCompletedValue() < pending.m_id)
        {
            if(!waitAll) break;
            if(SG_DX_CHECK(m_fence->SetEventOnCompletion(pending.m_id, m_fenceEvent)))
            {
                WaitForSingleObject(m_fenceEvent, INFINITE);
            }
        }

        for(auto& context : pending.m_payload.m_contexts)
        {
            if(!context.isValid()) continue;

            // A closed command list keeps a reference to every resource it recorded, and those
            // references outlive the fence: resetting it here is what drops them. Without this the
            // swapchain back buffers stay referenced, and both ResizeBuffers and the destruction of
            // the swapchain report it — the crash the first DX12 frame ended with.
            if(SG_DX_CHECK(context.m_allocator->Reset()) && SG_DX_CHECK(context.m_list->Reset(context.m_allocator.Get(), nullptr)))
            {
                SG_DX_CHECK(context.m_list->Close());
            }
            m_freeContexts.push_back(std::move(context));
        }
        // keepAlive references drop here
        m_pending.pop_front();
    }
}

void SGCore::DX12Device::immediateSubmit(const std::function<void(ID3D12GraphicsCommandList*)>& record) noexcept
{
    auto context = acquireCommandContext();
    if(!context.isValid()) return;

    record(context.m_list.Get());
    if(!SG_DX_CHECK(context.m_list->Close())) return;

    DX12Submission submission;
    submission.m_contexts.push_back(std::move(context));
    const auto id = submitRaw(std::move(submission));
    waitForSubmission(id);
}

SGCore::Ref<SGCore::DX12GPUBuffer> SGCore::DX12Device::createStagingBuffer(std::uint64_t size, const char* debugName) noexcept
{
    GPUBufferDesc desc;
    desc.m_size = size;
    desc.m_usage = GPUBufferUsage::SGG_TRANSFER_SRC | GPUBufferUsage::SGG_TRANSFER_DST;
    desc.m_access = GPUMemoryAccess::SGG_HOST_VISIBLE;
    desc.m_debugName = debugName ? debugName : "staging";
    auto buffer = MakeRef<DX12GPUBuffer>(*this, desc);
    return buffer->isValid() ? buffer : nullptr;
}

void SGCore::DX12Device::rotateUniformArena() noexcept
{
    m_currentUniformRegion = (m_currentUniformRegion + 1) % uniform_arena_regions;
    auto& region = m_uniformArena[m_currentUniformRegion];
    region.m_currentChunk = 0;
    region.m_usedInChunk = 0;
}

SGCore::UniformSlice SGCore::DX12Device::allocateTransientUniforms(std::uint64_t size) noexcept
{
    if(size == 0) return { };

    // a constant buffer view is addressed in 256-byte multiples, so slices start there too
    constexpr std::uint64_t alignment = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;

    auto& region = m_uniformArena[m_currentUniformRegion];

    while(region.m_currentChunk < region.m_chunks.size())
    {
        const auto& chunk = region.m_chunks[region.m_currentChunk];
        const std::uint64_t offset = (region.m_usedInChunk + alignment - 1) / alignment * alignment;
        if(chunk.m_mapped && offset + size <= chunk.m_size)
        {
            region.m_usedInChunk = offset + size;
            return { chunk.m_buffer, offset, static_cast<std::uint8_t*>(chunk.m_mapped) + offset };
        }
        // this chunk is full: continue in the next one, adding it below if there is none
        ++region.m_currentChunk;
        region.m_usedInChunk = 0;
    }

    constexpr std::uint64_t min_chunk_size = 256ull * 1024;

    UniformArenaChunk chunk;
    chunk.m_size = std::max(min_chunk_size, size);

    GPUBufferDesc desc;
    desc.m_size = chunk.m_size;
    desc.m_usage = GPUBufferUsage::SGG_UNIFORM_BUFFER;
    desc.m_access = GPUMemoryAccess::SGG_HOST_VISIBLE;
    desc.m_debugName = "transient_uniforms";

    chunk.m_buffer = MakeRef<DX12GPUBuffer>(*this, desc);
    if(!chunk.m_buffer->isValid()) return { };
    chunk.m_mapped = chunk.m_buffer->getMappedPointer();
    if(!chunk.m_mapped) return { };

    region.m_chunks.push_back(chunk);
    region.m_currentChunk = region.m_chunks.size() - 1;
    region.m_usedInChunk = size;
    return { chunk.m_buffer, 0, chunk.m_mapped };
}

SGCore::Ref<SGCore::IGPUObject> SGCore::DX12Device::getDummyBackendTexture() noexcept
{
    return getDummyTexture();
}

const SGCore::Ref<SGCore::DX12Texture>& SGCore::DX12Device::getDummyTexture() noexcept
{
    if(m_dummyTexture) return m_dummyTexture;

    DX12TextureDesc desc;
    desc.m_width = 1;
    desc.m_height = 1;
    desc.m_format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.m_debugName = "dummy_white";
    m_dummyTexture = DX12Texture::create(m_context, desc);
    if(!m_dummyTexture) return m_dummyTexture;

    const std::uint8_t white[4] = { 255, 255, 255, 255 };
    // the upload heap needs the row pitch aligned even for a single pixel
    auto staging = createStagingBuffer(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT, "dummy_white_upload");
    if(!staging) return m_dummyTexture;
    std::memcpy(staging->getMappedPointer(), white, sizeof(white));

    immediateSubmit([&](ID3D12GraphicsCommandList* commandList) {
        m_dummyTexture->recordTransition(commandList, D3D12_RESOURCE_STATE_COPY_DEST);

        D3D12_TEXTURE_COPY_LOCATION destination { };
        destination.pResource = m_dummyTexture->getResource();
        destination.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        destination.SubresourceIndex = 0;

        D3D12_TEXTURE_COPY_LOCATION source { };
        source.pResource = staging->getResource();
        source.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        source.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        source.PlacedFootprint.Footprint.Width = 1;
        source.PlacedFootprint.Footprint.Height = 1;
        source.PlacedFootprint.Footprint.Depth = 1;
        source.PlacedFootprint.Footprint.RowPitch = D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;

        commandList->CopyTextureRegion(&destination, 0, 0, 0, &source, nullptr);
        m_dummyTexture->recordTransition(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    });

    return m_dummyTexture;
}

SGCore::DX12Ptr<ID3D12Resource> SGCore::DX12Device::createReadbackResource(std::uint64_t size, const char* debugName) noexcept
{
    DX12Ptr<ID3D12Resource> resource;

    D3D12_HEAP_PROPERTIES heapProperties { };
    heapProperties.Type = D3D12_HEAP_TYPE_READBACK;

    D3D12_RESOURCE_DESC resourceDesc { };
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = size == 0 ? 1 : size;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    if(!SG_DX_CHECK(m_context->m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc,
                                                                 D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource))))
    {
        resource.Reset();
        return resource;
    }
    m_context->setObjectName(resource.Get(), debugName ? debugName : "readback");
    return resource;
}

#endif
