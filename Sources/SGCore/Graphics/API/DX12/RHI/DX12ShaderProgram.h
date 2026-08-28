//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>
#include <memory>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IShaderProgram.h"
#include "DX12Context.h"

namespace SGCore
{
    class DX12Device;

    /// One reflected resource with the HLSL registers it must be given.
    ///
    /// The register assignment can not be left to spirv-cross: the root signature has to name the
    /// same registers, and "register number = binding number" does not hold — an array of samplers
    /// consumes as many consecutive registers as it has elements, so a second sampler one binding
    /// later would land inside it ("SRV binding ranges overlap", the first DX12 engine run).
    struct DX12RegisterBinding
    {
        std::uint32_t m_set { };
        std::uint32_t m_binding { };
        std::uint32_t m_count = 1;
        ShaderDescriptorType m_type = ShaderDescriptorType::UNKNOWN;
        /// Register in the class of the resource (b for constant buffers, t for read-only, u for
        /// read-write), valid when m_hasView.
        std::uint32_t m_viewRegister { };
        /// s register; a combined image sampler needs both a t and an s.
        std::uint32_t m_samplerRegister { };
        bool m_hasView { };
        bool m_hasSampler { };
    };

    /**
     * The DXIL of every stage plus the root signature derived from reflection — the DX12 twin of
     * VulkanShaderProgram, where the root signature plays the role of the pipeline layout and a
     * descriptor table the role of a descriptor set layout.
     *
     * The binding model is the one spirv-cross produces when it translates SPIR-V to HLSL: descriptor
     * set N becomes register space N, and a binding becomes the register of its own class (b for
     * constant buffers, t for read-only resources, u for read-write ones, s for samplers). One CBV/
     * SRV/UAV table and one sampler table per space, so a bindDescriptorSet(N, ...) maps onto at most
     * two SetGraphicsRootDescriptorTable calls.
     *
     * The bytecode comes from DX12ShaderCompiler: the same SPIR-V the Vulkan backend consumes, run
     * through spirv-cross and d3dcompiler (task 3.4).
     */
    class SGCORE_EXPORT DX12ShaderProgram final : public IShaderProgram
    {
    public:
        /// One resource of a descriptor table, at a known offset from the table start.
        struct TableEntry
        {
            std::uint32_t m_binding { };
            ShaderDescriptorType m_type = ShaderDescriptorType::UNKNOWN;
            std::uint32_t m_count = 1;
            std::uint32_t m_offsetInTable { };
            /// The register this entry was given (see DX12RegisterBinding).
            std::uint32_t m_baseRegister { };
        };

        /// Assigns HLSL registers to every reflected resource: per space, a running counter per
        /// register class, each resource consuming as many registers as it has array elements.
        /// The same assignment feeds the root signature and the HLSL translation.
        [[nodiscard]] static std::vector<DX12RegisterBinding> assignRegisters(const ShaderReflection& reflection) noexcept;

        /// One root parameter: the descriptors of one set, of one heap kind.
        struct RootTable
        {
            std::uint32_t m_set { };
            bool m_isSampler { };
            std::uint32_t m_rootParameterIndex { };
            std::uint32_t m_descriptorsCount { };
            std::vector<TableEntry> m_entries;
        };

        DX12ShaderProgram(DX12Device& device, const ShaderProgramDesc& desc) noexcept;
        ~DX12ShaderProgram() override = default;

        [[nodiscard]] bool isValid() const noexcept override { return m_valid; }

        [[nodiscard]] ID3D12RootSignature* getRootSignature() const noexcept { return m_rootSignature.Get(); }
        [[nodiscard]] const std::vector<RootTable>& getRootTables() const noexcept { return m_rootTables; }
        /// The table of a set, or nullptr when the program declares nothing there.
        [[nodiscard]] const RootTable* findRootTable(std::uint32_t set, bool sampler) const noexcept;
        [[nodiscard]] const ShaderReflection::DescriptorBinding* findBinding(std::uint32_t set, std::uint32_t binding) const noexcept;

        /// Compiled bytecode of one stage; empty when the program carries no such stage.
        [[nodiscard]] D3D12_SHADER_BYTECODE getStageBytecode(SGSLESubShaderType type) const noexcept;
        [[nodiscard]] bool hasStage(SGSLESubShaderType type) const noexcept;

    private:
        struct StageBlob
        {
            SGSLESubShaderType m_type = SGSLESubShaderType::SST_NONE;
            /// Compiled shader bytecode (DXBC at shader model 5.1; DXIL when the path moves to DXC).
            std::vector<std::uint8_t> m_bytecode;
        };

        bool buildRootSignature(const std::vector<DX12RegisterBinding>& registers) noexcept;

        /// Held by value: shader assets outlive the renderer, so a reference to the device would
        /// dangle in static destruction (the lesson VulkanShaderProgram paid for).
        std::shared_ptr<DX12Context> m_context;
        std::vector<StageBlob> m_stages;
        DX12Ptr<ID3D12RootSignature> m_rootSignature;
        std::vector<RootTable> m_rootTables;
        bool m_valid { };
    };
}

#endif
