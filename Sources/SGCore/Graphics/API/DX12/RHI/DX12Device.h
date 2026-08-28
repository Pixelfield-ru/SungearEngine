//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IDevice.h"
#include "DX12Context.h"
#include "DX12DescriptorHeap.h"
#include "DX12Swapchain.h"
#include "DX12Texture.h"

struct GLFWwindow;

namespace SGCore
{
    class DX12GPUBuffer;
    class DX12PipelineState;

    /// A command allocator with the command list recorded into it. D3D12 needs both, and both are
    /// recycled together when the submission that used them has finished.
    struct DX12CommandContext
    {
        DX12Ptr<ID3D12CommandAllocator> m_allocator;
        DX12Ptr<ID3D12GraphicsCommandList> m_list;

        [[nodiscard]] bool isValid() const noexcept { return m_list != nullptr; }
    };

    /// One ExecuteCommandLists worth of work plus everything that must stay alive until its fence
    /// value is reached. The DX12 twin of VulkanSubmission.
    struct DX12Submission
    {
        std::vector<DX12CommandContext> m_contexts;
        std::vector<Ref<IGPUObject>> m_keepAlive;
    };

    /**
     * IDevice on D3D12. One direct queue and one monotonic fence: a submission id is the fence value
     * it signals, so waiting for a submission is a fence wait, and retiring means recycling
     * everything up to the completed value. Shaped after VulkanDevice so the two explicit backends
     * stay comparable line by line.
     */
    class SGCORE_EXPORT DX12Device final : public IDevice
    {
    public:
        DX12Device(const std::shared_ptr<DX12Context>& context, GLFWwindow* window) noexcept;
        ~DX12Device() override;

        [[nodiscard]] bool isReady() const noexcept { return m_ready; }

        // ---- IDevice
        [[nodiscard]] const DeviceProperties& getProperties() const noexcept override { return m_properties; }
        [[nodiscard]] Ref<IGPUBuffer> createBuffer(const GPUBufferDesc& desc) noexcept override;
        [[nodiscard]] Ref<IShaderProgram> createShaderProgram(const ShaderProgramDesc& desc) noexcept override;
        [[nodiscard]] Ref<IDescriptorSet> createDescriptorSet() noexcept override;
        [[nodiscard]] Ref<IPipelineState> getOrCreatePipeline(const PipelineStateDesc& desc) noexcept override;
        [[nodiscard]] Ref<ICommandList> createCommandList() noexcept override;
        void submit(const Ref<ICommandList>& commandList) noexcept override;
        [[nodiscard]] ISwapchain& getSwapchain() noexcept override { return *m_swapchain; }
        void destroyDeferred(Ref<IGPUObject> object) noexcept override;
        void waitIdle() noexcept override;

        // ---- backend services
        [[nodiscard]] DX12Context& getContext() noexcept { return *m_context; }
        [[nodiscard]] const std::shared_ptr<DX12Context>& getContextRef() const noexcept { return m_context; }
        [[nodiscard]] DX12Swapchain& getDX12Swapchain() noexcept { return *m_swapchain; }
        [[nodiscard]] ID3D12CommandQueue* getQueue() const noexcept { return m_context->m_directQueue.Get(); }

        /// A command list in the recording state, with its allocator (recycled from finished work).
        [[nodiscard]] DX12CommandContext acquireCommandContext() noexcept;
        /// Executes closed command lists and signals the fence; the returned id is that fence value.
        std::uint64_t submitRaw(DX12Submission&& submission) noexcept;
        /// Blocks until the submission with the given id has finished; 0 = no-op.
        void waitForSubmission(std::uint64_t id) noexcept;
        /// Records into a one-shot command list, submits and waits. For uploads / readbacks / setup.
        void immediateSubmit(const std::function<void(ID3D12GraphicsCommandList*)>& record) noexcept;

        /// Host-visible (UPLOAD heap) buffer of the given size.
        [[nodiscard]] Ref<DX12GPUBuffer> createStagingBuffer(std::uint64_t size, const char* debugName) noexcept;
        /// A READBACK-heap resource: the only heap a GPU->CPU copy can target. Not a DX12GPUBuffer —
        /// the RHI has no readback access mode, and only readbacks need this.
        [[nodiscard]] DX12Ptr<ID3D12Resource> createReadbackResource(std::uint64_t size, const char* debugName) noexcept;

        /// The shader-visible heaps the command lists write their descriptor tables into. D3D12
        /// keeps samplers in a heap of their own, so one set can need a range in each.
        [[nodiscard]] DX12DescriptorHeap& getViewRing() noexcept { return m_viewRing; }
        [[nodiscard]] DX12DescriptorHeap& getSamplerRing() noexcept { return m_samplerRing; }

        [[nodiscard]] UniformSlice allocateTransientUniforms(std::uint64_t size) noexcept override;
        [[nodiscard]] Ref<IGPUObject> getDummyBackendTexture() noexcept override;

        /// Opens a new frame's arena region. Regions rotate, so a region is refilled only after the
        /// frames that recorded into it have long since finished.
        void rotateUniformArena() noexcept;

        /// A 1x1 white texture. Every descriptor a shader declares has to be written before a draw,
        /// and the engine passes leave samplers unbound whenever a material has no texture of that
        /// slot (on GL such a sampler simply reads unit 0). This stands in, as it does on Vulkan.
        [[nodiscard]] const Ref<DX12Texture>& getDummyTexture() noexcept;

        /// Recycles the command contexts and released objects of finished submissions.
        void retire(bool waitAll) noexcept;

        [[nodiscard]] std::uint64_t getSubmissionCounter() const noexcept { return m_submissionCounter; }

    private:
        struct PendingSubmission
        {
            std::uint64_t m_id { };
            DX12Submission m_payload;
        };

        std::shared_ptr<DX12Context> m_context;
        DeviceProperties m_properties;
        bool m_ready { };

        std::unique_ptr<DX12Swapchain> m_swapchain;

        DX12Ptr<ID3D12Fence> m_fence;
        HANDLE m_fenceEvent { };
        std::uint64_t m_submissionCounter { };
        std::deque<PendingSubmission> m_pending;
        std::vector<DX12CommandContext> m_freeContexts;

        DX12DescriptorHeap m_viewRing;
        DX12DescriptorHeap m_samplerRing;

        std::vector<Ref<IGPUObject>> m_deferredDestroy;
        std::unordered_map<std::size_t, std::vector<Ref<DX12PipelineState>>> m_pipelineCache;

        struct UniformArenaChunk
        {
            Ref<DX12GPUBuffer> m_buffer;
            void* m_mapped { };
            std::uint64_t m_size { };
        };

        /// Bump allocator over a list of chunks; a chunk is added only when a frame needs more room
        /// than the region already has, and is then kept for every later frame.
        struct UniformArenaRegion
        {
            std::vector<UniformArenaChunk> m_chunks;
            std::size_t m_currentChunk { };
            std::uint64_t m_usedInChunk { };
        };

        static constexpr std::size_t uniform_arena_regions = 3;

        UniformArenaRegion m_uniformArena[uniform_arena_regions];
        std::size_t m_currentUniformRegion { };

        Ref<DX12Texture> m_dummyTexture;
    };
}

#endif
