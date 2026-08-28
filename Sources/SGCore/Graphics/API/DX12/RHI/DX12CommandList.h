//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <array>
#include <cstdint>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/ICommandList.h"
#include "DX12Device.h"
#include "DX12PipelineState.h"
#include "DX12DescriptorSet.h"
#include "DX12Texture.h"

namespace SGCore
{
    class DX12DescriptorSet;
    class DX12GPUBuffer;

    /**
     * Records into a direct command list; uploads go into a second one the device submits first, so
     * uploadData() works inside a render pass exactly as it does on GL and Vulkan. The pipeline,
     * descriptor tables, viewport and scissor are resolved lazily at the first draw (flushState()),
     * because the pipeline variant depends on the attachment formats of the active pass.
     *
     * A render pass here is OMSetRenderTargets plus explicit clears rather than the
     * ID3D12GraphicsCommandList4 render pass API: the engine needs no tiling hints, and this keeps
     * clearColorAttachment() legal at any point of the pass.
     */
    class SGCORE_EXPORT DX12CommandList final : public ICommandList
    {
    public:
        static constexpr std::uint32_t max_descriptor_sets = 4;

        explicit DX12CommandList(DX12Device& device) noexcept;
        ~DX12CommandList() override;

        // ---- ICommandList
        void begin() noexcept override;
        void end() noexcept override;
        void beginRenderPass(const RenderPassBeginDesc& desc) noexcept override;
        void endRenderPass() noexcept override;
        void bindPipeline(const Ref<IPipelineState>& pipeline) noexcept override;
        void bindDescriptorSet(std::uint32_t setIndex, const Ref<IDescriptorSet>& set) noexcept override;
        void pushConstants(const void* data, std::uint32_t size, std::uint32_t offset = 0) noexcept override;
        void bindVertexBuffer(std::uint32_t slot, const Ref<IGPUBuffer>& buffer, std::uint64_t offset = 0) noexcept override;
        void bindIndexBuffer(const Ref<IGPUBuffer>& buffer, SGIndexType indexType, std::uint64_t offset = 0) noexcept override;
        void setViewport(const Viewport& viewport) noexcept override;
        void setScissor(const Scissor& scissor) noexcept override;
        void draw(std::uint32_t vertexCount, std::uint32_t instanceCount = 1,
                  std::uint32_t firstVertex = 0, std::uint32_t firstInstance = 0) noexcept override;
        void drawIndexed(std::uint32_t indexCount, std::uint32_t instanceCount = 1,
                         std::uint32_t firstIndex = 0, std::int32_t vertexOffset = 0,
                         std::uint32_t firstInstance = 0) noexcept override;
        void clearColorAttachment(std::uint32_t colorIndex, const glm::vec4& color) noexcept override;
        void clearDepthStencil(float depth = 1.0f, bool clearStencil = false, std::uint32_t stencil = 0) noexcept override;
        void transition(const Ref<IGPUObject>& resource, GPUResourceState newState) noexcept override;
        void uploadData(const Ref<IGPUBuffer>& dst, const void* data, std::uint64_t size, std::uint64_t offset = 0) noexcept override;

        // ---- backend
        /// Moves the recorded work out for DX12Device::submit(). Empty when nothing was recorded.
        [[nodiscard]] DX12Submission takeSubmission() noexcept;
        [[nodiscard]] bool isRecording() const noexcept { return m_recording; }
        [[nodiscard]] bool hasRecordedWork() const noexcept { return m_ended && !m_submitted; }
        [[nodiscard]] bool touchedSwapchain() const noexcept { return m_touchedSwapchain; }
        [[nodiscard]] ID3D12GraphicsCommandList* getCommandList() const noexcept { return m_context.m_list.Get(); }

    private:
        struct SetSlot
        {
            Ref<DX12DescriptorSet> m_set;
            std::uint64_t m_materializedVersion = ~0ull;
            bool m_materialized { };
            ID3D12RootSignature* m_materializedRootSignature { };
        };

        void resetState() noexcept;
        /// Submits work recorded but not yet handed to the device (see begin()).
        void flushRecordedWork() noexcept;
        /// Binds the pipeline / tables / viewport / scissor that changed since the last draw.
        bool flushState() noexcept;
        bool materializeSet(SetSlot& slot, std::uint32_t setIndex) noexcept;
        /// The backend texture behind a descriptor entry (bound directly or through the facade).
        [[nodiscard]] Ref<DX12Texture> resolveTexture(const DX12DescriptorSet::Entry& entry) const noexcept;
        /// True when the texture is a colour or depth attachment of the pass being recorded.
        [[nodiscard]] bool isPassAttachment(const Ref<DX12Texture>& texture) const noexcept;
        void applyViewportScissor() noexcept;

        DX12Device& m_device;
        DX12CommandContext m_context;
        DX12CommandContext m_uploadContext;
        bool m_recording { };
        bool m_ended { };
        bool m_submitted = true;
        DX12Submission m_submission;

        // active render pass
        bool m_inPass { };
        bool m_targetIsSwapchain { };
        bool m_touchedSwapchain { };
        DX12PassFormats m_passFormats;
        std::uint32_t m_passWidth { };
        std::uint32_t m_passHeight { };
        std::vector<Ref<DX12Texture>> m_passColorTextures;
        std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_passColorViews;
        Ref<DX12Texture> m_passDepthTexture;

        // deferred state
        Ref<DX12PipelineState> m_pipeline;
        ID3D12PipelineState* m_boundPipeline { };
        ID3D12RootSignature* m_boundRootSignature { };
        std::array<SetSlot, max_descriptor_sets> m_sets;
        Viewport m_viewport { };
        bool m_viewportExplicit { };
        Scissor m_scissor { };
        bool m_scissorExplicit { };
        bool m_viewportDirty = true;
    };
}

#endif
