//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>
#include <memory>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IPipelineState.h"
#include "DX12Context.h"

namespace SGCore
{
    class DX12Device;
    class DX12ShaderProgram;

    /// Attachment formats of the pass a pipeline is used in. Not part of PipelineStateDesc (the GL
    /// backend never needed them), so one ID3D12PipelineState is kept per set of formats met, built
    /// lazily at bind time — the same arrangement as VulkanPipelineState.
    struct DX12PassFormats
    {
        std::vector<DXGI_FORMAT> m_colorFormats;
        DXGI_FORMAT m_depthFormat = DXGI_FORMAT_UNKNOWN;
        std::uint32_t m_samples = 1;
        /// An offscreen pass renders with a flipped viewport, which inverts the winding: the pipeline
        /// has to be built with the opposite front face, and front face is not dynamic state on
        /// D3D12 — so it belongs to the variant key.
        bool m_viewportFlipped { };

        bool operator==(const DX12PassFormats&) const noexcept = default;
    };

    class SGCORE_EXPORT DX12PipelineState final : public IPipelineState
    {
    public:
        DX12PipelineState(DX12Device& device, const PipelineStateDesc& desc) noexcept;
        ~DX12PipelineState() override = default;

        /// The pipeline object for the given pass formats (built on first use). nullptr on failure.
        [[nodiscard]] ID3D12PipelineState* getOrCreate(const DX12PassFormats& formats) noexcept;

        [[nodiscard]] DX12ShaderProgram* getProgram() const noexcept;
        [[nodiscard]] ID3D12RootSignature* getRootSignature() const noexcept;
        [[nodiscard]] D3D12_PRIMITIVE_TOPOLOGY getTopology() const noexcept { return m_topology; }
        /// The stencil reference is command list state on D3D12, not pipeline state.
        [[nodiscard]] std::uint32_t getStencilRef() const noexcept { return m_stencilRef; }
        [[nodiscard]] bool usesIndices() const noexcept { return m_desc.m_meshRenderState.m_useIndices; }

    private:
        struct Variant
        {
            DX12PassFormats m_formats;
            DX12Ptr<ID3D12PipelineState> m_pipeline;
        };

        [[nodiscard]] DX12Ptr<ID3D12PipelineState> build(const DX12PassFormats& formats) noexcept;

        std::shared_ptr<DX12Context> m_context;
        std::vector<Variant> m_variants;
        D3D12_PRIMITIVE_TOPOLOGY m_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        std::uint32_t m_stencilRef { };
    };
}

#endif
