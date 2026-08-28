//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <sgcore_export.h>

namespace SGCore
{
    enum class ShaderDescriptorType
    {
        UNIFORM_BUFFER,
        STORAGE_BUFFER,
        COMBINED_IMAGE_SAMPLER,
        SAMPLED_IMAGE,
        SAMPLER,
        STORAGE_IMAGE,
        UNIFORM_TEXEL_BUFFER,
        STORAGE_TEXEL_BUFFER,
        INPUT_ATTACHMENT,
        UNKNOWN
    };

    /// Bitmask over SGSLESubShaderType values: bit (1 << type).
    using shader_stages_mask_t = std::uint32_t;

    /**
     * Layout of a shader program as seen by the GPU: descriptor bindings with block members
     * (offsets/sizes as laid out in SPIR-V), vertex inputs and push constants. Produced from
     * SPIR-V by SPIRVReflector, merged across all stages of the program.
     */
    struct SGCORE_EXPORT ShaderReflection
    {
        struct BlockMember
        {
            std::string m_name;
            std::string m_typeName;
            std::uint32_t m_offset { };
            std::uint32_t m_size { };
            /// For an array member: the stride between elements ("name[i]" addressing).
            /// For a plain member: its padded size. Both backends must fill it this way — GL
            /// reports GL_ARRAY_STRIDE, SPIR-V takes the stride from the array traits (its own
            /// padded_size covers the whole array and would walk past the block).
            std::uint32_t m_paddedSize { };
            /// Product of array dimensions, 1 for scalars.
            std::uint32_t m_arrayCount = 1;
        };

        struct DescriptorBinding
        {
            /// Variable name; for blocks without an instance name — the block type name.
            std::string m_name;
            ShaderDescriptorType m_type = ShaderDescriptorType::UNKNOWN;
            std::uint32_t m_set { };
            std::uint32_t m_binding { };
            /// Array size for arrays of samplers, 1 otherwise.
            std::uint32_t m_count = 1;
            /// Block size in bytes for buffer descriptors, 0 otherwise.
            std::uint32_t m_blockSize { };
            std::vector<BlockMember> m_members;
            shader_stages_mask_t m_stages { };
        };

        struct VertexInput
        {
            std::string m_name;
            std::uint32_t m_location { };
            /// SPIR-V/Vulkan format enum value (VkFormat) as reported by the reflector.
            std::uint32_t m_format { };
            /// How many consecutive locations the input occupies. A matrix input is a single
            /// variable at location N but takes one location per column: `in mat4` covers N..N+3.
            std::uint32_t m_locationsCount = 1;
        };

        struct PushConstantRange
        {
            std::string m_name;
            std::uint32_t m_offset { };
            std::uint32_t m_size { };
            std::vector<BlockMember> m_members;
            shader_stages_mask_t m_stages { };
        };

        std::vector<DescriptorBinding> m_bindings;
        std::vector<VertexInput> m_vertexInputs;
        std::vector<PushConstantRange> m_pushConstants;

        [[nodiscard]] const DescriptorBinding* findBinding(std::string_view name) const noexcept;
        [[nodiscard]] const DescriptorBinding* findBinding(std::uint32_t set, std::uint32_t binding) const noexcept;
        [[nodiscard]] const BlockMember* findMember(std::string_view blockName, std::string_view memberName) const noexcept;
    };
}
