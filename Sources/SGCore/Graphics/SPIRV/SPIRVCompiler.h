//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <sgcore_export.h>

#include "ShaderReflection.h"
#include "SGCore/Utils/SGSL/SGSLESubShaderType.h"

namespace SGCore
{
    /**
     * Compiles Vulkan-flavoured GLSL (output of SGSLEVulkanizer, defines already prepended)
     * into SPIR-V with glslang and reflects the resulting program with SPIRV-Reflect.
     *
     * All stages of a program are compiled and linked together: glslang assigns matching
     * locations to unqualified in/out varyings across stages, and the reflection is the union
     * over stages (a binding used in several stages appears once with a stage mask).
     */
    struct SGCORE_EXPORT SPIRVCompiler
    {
        struct StageSource
        {
            SGSLESubShaderType m_type = SGSLESubShaderType::SST_NONE;
            std::string m_code;
        };

        struct Options
        {
            /// Prepended as `#version <glslVersion>` unless the code already starts with #version.
            std::uint32_t m_glslVersion = 460;
            std::string m_entryPoint = "main";
            bool m_generateDebugInfo = false;
            /// Names used in error messages.
            std::string m_programName = "shader";
        };

        struct StageResult
        {
            SGSLESubShaderType m_type = SGSLESubShaderType::SST_NONE;
            std::vector<std::uint32_t> m_spirv;
            std::string m_log;
            bool m_success { };
        };

        struct Result
        {
            std::vector<StageResult> m_stages;
            ShaderReflection m_reflection;
            /// Compile + link + reflection log (empty on clean success).
            std::string m_log;
            bool m_success { };
        };

        [[nodiscard]] static Result compile(const std::vector<StageSource>& stages, const Options& options) noexcept;

        /// Reflects already compiled SPIR-V modules (one per stage) into a merged ShaderReflection.
        [[nodiscard]] static bool reflect(const std::vector<StageResult>& stages, ShaderReflection& outReflection, std::string& outLog) noexcept;
    };
}
