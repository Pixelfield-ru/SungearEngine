//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>
#include <string>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Utils/SGSL/SGSLESubShaderType.h"
#include "DX12ShaderProgram.h"

namespace SGCore
{
    /**
     * SPIR-V -> HLSL -> shader bytecode, the DX12 half of the single shader path (task 3.4).
     *
     * SPIR-V comes from the same glslang pass the Vulkan backend uses, spirv-cross translates it to
     * HLSL, and DXC compiles that to DXIL at shader model 6.0.
     *
     * DXC rather than d3dcompiler: FXC (SM 5.x) refuses the loops of the engine's corpus — "unable to
     * unroll loop, loop does not appear to terminate in a timely manner" in the volumetric fog and
     * the terrain shaders — because it insists on unrolling what spirv-cross emits without a [loop]
     * attribute. And DXC needs no package of its own: dxcapi.h and dxcompiler.lib come with the
     * Windows SDK. Only dxcompiler.dll has to sit next to the executable — the DXC of the SDK signs
     * the DXIL internally, so dxil.dll is not needed (measured: unsigned DXIL is rejected with
     * E_INVALIDARG, the one produced here is accepted).
     */
    struct SGCORE_EXPORT DX12ShaderCompiler
    {
        struct Result
        {
            std::vector<std::uint8_t> m_bytecode;
            /// The generated HLSL: the only way to read a d3dcompiler error, which points at a line
            /// of code nobody wrote by hand.
            std::string m_hlsl;
            std::string m_log;
            bool m_success { };
        };

        /// p registers pins the HLSL register of every resource, so that the generated code and the
        /// root signature agree (see DX12RegisterBinding).
        [[nodiscard]] static Result compile(const std::vector<std::uint32_t>& spirv, SGSLESubShaderType stage,
                                            const std::string& debugName,
                                            const std::vector<DX12RegisterBinding>& registers) noexcept;

        /// Shader model 6.0 target of a stage ("vs_6_0", ...); nullptr for a stage D3D has none for.
        [[nodiscard]] static const char* stageTarget(SGSLESubShaderType stage) noexcept;
    };
}

#endif
