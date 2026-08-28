//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <sgcore_export.h>

#include "SGSLESubShaderType.h"

namespace SGCore
{
    /**
     * Rewrites translated SGSL sub-shaders (GLSL, OpenGL flavour) into Vulkan-compatible GLSL
     * that glslang accepts with a Vulkan target:
     *
     *  - loose non-opaque uniforms (`uniform vec4 u_color;`) are collected into a generated
     *    std140 block per stage (`SGLegacyUniforms_vertex`, `SGLegacyUniforms_fragment`, ...) —
     *    block members without an instance name stay visible under their old names, so the rest
     *    of the shader code is untouched. Blocks are per stage because a stage only knows the
     *    struct types its own code declares; a uniform declared in several stages is a member of
     *    each of their blocks and consumers write it by name into every block that has it;
     *  - samplers / images / texel buffers and uniform blocks get `layout(set = S, binding = B)`;
     *    existing explicit bindings are kept;
     *  - `gl_FragColor` becomes a declared `out` variable, `gl_VertexID` / `gl_InstanceID`
     *    become their Vulkan counterparts.
     *
     * The whole program (all stages) is processed together so that a resource gets the same
     * binding in every stage and the legacy block is identical everywhere.
     *
     * Preprocessor conditionals are respected: a uniform declared under `#if X` becomes a block
     * member guarded by the same condition. Defines that shaders rely on must therefore be
     * prepended to each stage's code before calling vulkanize(), exactly as the GL backend
     * prepends them before compilation.
     */
    struct SGCORE_EXPORT SGSLEVulkanizer
    {
        /// Dialect of the produced GLSL. The transformation is the same; only what the target
        /// compiler accepts differs.
        enum class Target
        {
            /// glslang with Vulkan rules: `layout(set = S, binding = B)`, gl_VertexIndex / gl_InstanceIndex.
            VULKAN,
            /// OpenGL 4.6 core GLSL: `layout(binding = B)` only (no `set`), gl_VertexID / gl_InstanceID kept.
            OPENGL
        };

        struct Config
        {
            Target m_target = Target::VULKAN;
            std::uint32_t m_descriptorSet = 0;
            std::uint32_t m_firstBinding = 0;
            /// Prefix of the per-stage legacy blocks: `<prefix>_vertex`, `<prefix>_fragment`, ...
            std::string m_legacyBlockName = "SGLegacyUniforms";
            std::string m_fragColorName = "sgFragColor";
        };

        enum class ResourceKind
        {
            LEGACY_UNIFORM_BLOCK,
            UNIFORM_BLOCK,
            STORAGE_BLOCK,
            SAMPLER,
            IMAGE
        };

        struct Binding
        {
            std::string m_name;
            ResourceKind m_kind { };
            std::uint32_t m_set { };
            std::uint32_t m_binding { };
            /// Array size for arrays of samplers (`sampler2D s[3]`), 1 otherwise.
            std::uint32_t m_count = 1;
        };

        struct Stage
        {
            SGSLESubShaderType m_type = SGSLESubShaderType::SST_NONE;
            /// Input: translated GLSL (defines already prepended). Output: Vulkan GLSL.
            std::string m_code;
        };

        struct Report
        {
            std::uint32_t m_looseUniformsMoved { };
            std::uint32_t m_uniformBlocksBound { };
            std::uint32_t m_storageBlocksBound { };
            std::uint32_t m_opaqueUniformsBound { };
            std::uint32_t m_fragColorReplaced { };
            std::uint32_t m_builtinsReplaced { };
            std::uint32_t m_initializersDropped { };

            /// A loose uniform that had an initializer (`uniform float x = 6.0;`): block members can not
            /// carry one, so the value is reported here for the consumer to write as the default.
            struct DefaultValue
            {
                std::string m_blockName;
                std::string m_memberName;
                /// Initializer expression text as written, e.g. "6.0" or "vec4(0.0, 0.0, 0.0, 1.0)".
                std::string m_expression;
            };

            std::vector<Binding> m_bindings;
            std::vector<DefaultValue> m_defaults;
            std::vector<std::string> m_warnings;
        };

        [[nodiscard]] static Report vulkanize(std::vector<Stage>& stages, const Config& config) noexcept;

        [[nodiscard]] static bool isOpaqueType(std::string_view typeName) noexcept;
    };
}
