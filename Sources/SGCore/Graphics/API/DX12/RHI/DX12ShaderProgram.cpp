//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12ShaderProgram.h"

#if defined(_WIN32)

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>

#include "SGCore/Graphics/SPIRV/SPIRVCompiler.h"
#include "SGCore/Logger/Logger.h"
#include "DX12Device.h"
#include "DX12ShaderCompiler.h"

namespace
{
    /// Which heap a descriptor of this kind lives in on D3D12.
    bool isSamplerDescriptor(SGCore::ShaderDescriptorType type) noexcept
    {
        return type == SGCore::ShaderDescriptorType::SAMPLER ||
               type == SGCore::ShaderDescriptorType::COMBINED_IMAGE_SAMPLER;
    }

    /// Which register class spirv-cross puts this kind of resource in.
    D3D12_DESCRIPTOR_RANGE_TYPE viewRangeType(SGCore::ShaderDescriptorType type) noexcept
    {
        switch(type)
        {
            case SGCore::ShaderDescriptorType::UNIFORM_BUFFER: return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            case SGCore::ShaderDescriptorType::STORAGE_IMAGE:
            case SGCore::ShaderDescriptorType::STORAGE_TEXEL_BUFFER: return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
            // storage buffers of the engine are read-only (bone matrices), so they stay SRVs
            default: return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        }
    }
}

SGCore::DX12ShaderProgram::DX12ShaderProgram(DX12Device& device, const ShaderProgramDesc& desc) noexcept
{
    m_debugName = desc.m_debugName;
    m_context = device.getContextRef();

    // stages lacking SPIR-V are compiled from their (vulkanized) GLSL, exactly as on Vulkan: the
    // reflection the root signature is built from comes out of SPIR-V either way
    std::vector<SPIRVCompiler::StageSource> toCompile;
    for(const auto& stage : desc.m_stages)
    {
        if(stage.m_spirv.empty()) toCompile.push_back({ stage.m_type, stage.m_code });
    }
    SPIRVCompiler::Result compiled;
    if(!toCompile.empty())
    {
        SPIRVCompiler::Options options;
        options.m_programName = desc.m_debugName;
        compiled = SPIRVCompiler::compile(toCompile, options);
        if(!compiled.m_success)
        {
            m_log = compiled.m_log;
            SG_LOG_E("DX12ShaderProgram '{}': SPIR-V compilation failed:\n{}", m_debugName, m_log);
            return;
        }
    }

    std::vector<SPIRVCompiler::StageResult> compiledStages;
    std::size_t compiledIndex = 0;
    for(const auto& stage : desc.m_stages)
    {
        SPIRVCompiler::StageResult result;
        result.m_type = stage.m_type;
        result.m_success = true;
        result.m_spirv = stage.m_spirv.empty() ? compiled.m_stages[compiledIndex++].m_spirv : stage.m_spirv;
        compiledStages.push_back(std::move(result));
    }

    std::string reflectLog;
    if(!SPIRVCompiler::reflect(compiledStages, m_reflection, reflectLog))
    {
        m_log = reflectLog;
        SG_LOG_E("DX12ShaderProgram '{}': SPIR-V reflection failed:\n{}", m_debugName, reflectLog);
        return;
    }

    // SPIR-V -> HLSL -> bytecode. The same SPIR-V the Vulkan backend consumes, so the two explicit
    // backends really do share one shader path. The register assignment is computed once, from the
    // merged reflection, and handed to both the translation and the root signature.
    const auto registers = assignRegisters(m_reflection);

    for(const auto& stage : compiledStages)
    {
        auto translated = DX12ShaderCompiler::compile(stage.m_spirv, stage.m_type, m_debugName, registers);
        if(!translated.m_success)
        {
            m_log = translated.m_log;
            SG_LOG_E("DX12ShaderProgram '{}': stage {} could not be translated to shader bytecode:\n{}\n"
                     "--- generated HLSL ---\n{}", m_debugName, static_cast<int>(stage.m_type), m_log, translated.m_hlsl);
            m_stages.clear();
            return;
        }
        // SG_DX_DUMP_HLSL=1: the HLSL spirv-cross produced is the only place where the D3D-side
        // layout of a block is visible — dumping it is what turns "the numbers differ" into a diff
        if(const char* dump = std::getenv("SG_DX_DUMP_HLSL"); dump && std::string_view(dump) == "1")
        {
            std::filesystem::path out = "DX12HLSLOutputDebug";
            std::error_code ec;
            std::filesystem::create_directories(out, ec);
            std::string name = m_debugName;
            std::replace_if(name.begin(), name.end(), [](char c) { return c == '/' || c == '\\' || c == ':'; }, '_');
            std::ofstream file(out / (name + "_" + std::to_string(static_cast<int>(stage.m_type)) + ".hlsl"));
            file << translated.m_hlsl;
        }

        m_stages.push_back({ stage.m_type, std::move(translated.m_bytecode) });
    }

    if(!buildRootSignature(registers)) return;

    m_valid = true;
}

std::vector<SGCore::DX12RegisterBinding> SGCore::DX12ShaderProgram::assignRegisters(const ShaderReflection& reflection) noexcept
{
    // sorted by (set, binding) so the assignment is deterministic and reproducible
    std::vector<const ShaderReflection::DescriptorBinding*> sorted;
    sorted.reserve(reflection.m_bindings.size());
    for(const auto& binding : reflection.m_bindings) sorted.push_back(&binding);
    std::sort(sorted.begin(), sorted.end(), [](const auto* a, const auto* b) {
        return a->m_set != b->m_set ? a->m_set < b->m_set : a->m_binding < b->m_binding;
    });

    /// running register counters of one space
    struct SpaceCounters
    {
        std::uint32_t m_cbv { };
        std::uint32_t m_srv { };
        std::uint32_t m_uav { };
        std::uint32_t m_sampler { };
    };
    std::map<std::uint32_t, SpaceCounters> spaces;

    std::vector<DX12RegisterBinding> assigned;
    assigned.reserve(sorted.size());

    for(const auto* binding : sorted)
    {
        auto& counters = spaces[binding->m_set];
        const std::uint32_t count = binding->m_count == 0 ? 1 : binding->m_count;

        DX12RegisterBinding entry;
        entry.m_set = binding->m_set;
        entry.m_binding = binding->m_binding;
        entry.m_count = count;
        entry.m_type = binding->m_type;

        if(binding->m_type != ShaderDescriptorType::SAMPLER)
        {
            entry.m_hasView = true;
            switch(viewRangeType(binding->m_type))
            {
                case D3D12_DESCRIPTOR_RANGE_TYPE_CBV: entry.m_viewRegister = counters.m_cbv; counters.m_cbv += count; break;
                case D3D12_DESCRIPTOR_RANGE_TYPE_UAV: entry.m_viewRegister = counters.m_uav; counters.m_uav += count; break;
                default: entry.m_viewRegister = counters.m_srv; counters.m_srv += count; break;
            }
        }
        if(isSamplerDescriptor(binding->m_type))
        {
            // a combined image sampler is two HLSL resources: a texture and a sampler
            entry.m_hasSampler = true;
            entry.m_samplerRegister = counters.m_sampler;
            counters.m_sampler += count;
        }

        assigned.push_back(entry);
    }
    return assigned;
}

bool SGCore::DX12ShaderProgram::buildRootSignature(const std::vector<DX12RegisterBinding>& registers) noexcept
{
    if(!m_reflection.m_pushConstants.empty())
    {
        // no engine shader uses push constants (they do not exist on GL, and the GL backend leaves
        // them unimplemented too), so the mapping onto root constants is written when one appears
        SG_LOG_W("DX12ShaderProgram '{}': push constants are not mapped to root constants yet.", m_debugName);
    }

    // group by (set, heap kind) — one descriptor table each, with the registers assigned above
    std::map<std::pair<std::uint32_t, bool>, RootTable> tables;
    for(const auto& assigned : registers)
    {
        const bool kinds[2] = { false, true };
        for(const bool sampler : kinds)
        {
            if(sampler && !assigned.m_hasSampler) continue;
            if(!sampler && !assigned.m_hasView) continue;

            auto& table = tables[{ assigned.m_set, sampler }];
            table.m_set = assigned.m_set;
            table.m_isSampler = sampler;
            table.m_entries.push_back({ assigned.m_binding, assigned.m_type, assigned.m_count, table.m_descriptorsCount,
                                        sampler ? assigned.m_samplerRegister : assigned.m_viewRegister });
            table.m_descriptorsCount += assigned.m_count;
        }
    }

    std::vector<D3D12_ROOT_PARAMETER> parameters;
    // the ranges have to outlive the serialization call
    std::vector<std::vector<D3D12_DESCRIPTOR_RANGE>> rangeStorage;
    rangeStorage.reserve(tables.size());

    for(auto& [key, table] : tables)
    {
        std::sort(table.m_entries.begin(), table.m_entries.end(),
                  [](const TableEntry& a, const TableEntry& b) { return a.m_binding < b.m_binding; });

        std::vector<D3D12_DESCRIPTOR_RANGE> ranges;
        for(const auto& entry : table.m_entries)
        {
            D3D12_DESCRIPTOR_RANGE range { };
            range.RangeType = table.m_isSampler ? D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER : viewRangeType(entry.m_type);
            range.NumDescriptors = entry.m_count;
            // the register the HLSL translation was told to use, not the SPIR-V binding number
            range.BaseShaderRegister = entry.m_baseRegister;
            range.RegisterSpace = table.m_set;
            // explicit offsets: the table order is the binding order, whatever the range types are
            range.OffsetInDescriptorsFromTableStart = entry.m_offsetInTable;
            ranges.push_back(range);
        }
        rangeStorage.push_back(std::move(ranges));

        D3D12_ROOT_PARAMETER parameter { };
        parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        parameter.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(rangeStorage.back().size());
        parameter.DescriptorTable.pDescriptorRanges = rangeStorage.back().data();

        table.m_rootParameterIndex = static_cast<std::uint32_t>(parameters.size());
        parameters.push_back(parameter);
        m_rootTables.push_back(table);
    }

    D3D12_ROOT_SIGNATURE_DESC desc { };
    desc.NumParameters = static_cast<UINT>(parameters.size());
    desc.pParameters = parameters.data();
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    DX12Ptr<ID3DBlob> serialized;
    DX12Ptr<ID3DBlob> errors;
    const HRESULT result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &serialized, &errors);
    if(FAILED(result))
    {
        m_log = errors ? std::string(static_cast<const char*>(errors->GetBufferPointer()), errors->GetBufferSize())
                       : dx12ResultToString(result);
        SG_LOG_E("DX12ShaderProgram '{}': root signature serialization failed: {}", m_debugName, m_log);
        return false;
    }

    if(!SG_DX_CHECK(m_context->m_device->CreateRootSignature(0, serialized->GetBufferPointer(), serialized->GetBufferSize(),
                                                             IID_PPV_ARGS(&m_rootSignature))))
    {
        m_log = "CreateRootSignature failed";
        return false;
    }
    m_context->setObjectName(m_rootSignature.Get(), m_debugName + "_root_signature");
    return true;
}

const SGCore::DX12ShaderProgram::RootTable* SGCore::DX12ShaderProgram::findRootTable(std::uint32_t set, bool sampler) const noexcept
{
    for(const auto& table : m_rootTables)
    {
        if(table.m_set == set && table.m_isSampler == sampler) return &table;
    }
    return nullptr;
}

const SGCore::ShaderReflection::DescriptorBinding* SGCore::DX12ShaderProgram::findBinding(std::uint32_t set, std::uint32_t binding) const noexcept
{
    return m_reflection.findBinding(set, binding);
}

D3D12_SHADER_BYTECODE SGCore::DX12ShaderProgram::getStageBytecode(SGSLESubShaderType type) const noexcept
{
    for(const auto& stage : m_stages)
    {
        if(stage.m_type != type || stage.m_bytecode.empty()) continue;
        return { stage.m_bytecode.data(), stage.m_bytecode.size() };
    }
    return { nullptr, 0 };
}

bool SGCore::DX12ShaderProgram::hasStage(SGSLESubShaderType type) const noexcept
{
    return std::any_of(m_stages.begin(), m_stages.end(), [type](const StageBlob& stage) { return stage.m_type == type; });
}

#endif
