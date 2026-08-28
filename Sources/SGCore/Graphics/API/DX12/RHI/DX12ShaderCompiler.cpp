//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12ShaderCompiler.h"

#if defined(_WIN32)

#include <dxcapi.h>
#include <spirv_hlsl.hpp>

#include "SGCore/Graphics/API/DX12/DX12Common.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Utils/Utils.h"

namespace
{
    /// The execution model spirv-cross expects when a resource binding is pinned for a stage.
    spv::ExecutionModel stageExecutionModel(SGCore::SGSLESubShaderType stage) noexcept
    {
        switch(stage)
        {
            case SGCore::SGSLESubShaderType::SST_VERTEX: return spv::ExecutionModelVertex;
            case SGCore::SGSLESubShaderType::SST_FRAGMENT: return spv::ExecutionModelFragment;
            case SGCore::SGSLESubShaderType::SST_GEOMETRY: return spv::ExecutionModelGeometry;
            case SGCore::SGSLESubShaderType::SST_TESS_CONTROL: return spv::ExecutionModelTessellationControl;
            case SGCore::SGSLESubShaderType::SST_TESS_EVALUATION: return spv::ExecutionModelTessellationEvaluation;
            default: return spv::ExecutionModelGLCompute;
        }
    }
}

const char* SGCore::DX12ShaderCompiler::stageTarget(SGSLESubShaderType stage) noexcept
{
    switch(stage)
    {
        case SGSLESubShaderType::SST_VERTEX: return "vs_6_0";
        case SGSLESubShaderType::SST_FRAGMENT: return "ps_6_0";
        case SGSLESubShaderType::SST_GEOMETRY: return "gs_6_0";
        case SGSLESubShaderType::SST_TESS_CONTROL: return "hs_6_0";
        case SGSLESubShaderType::SST_TESS_EVALUATION: return "ds_6_0";
        case SGSLESubShaderType::SST_COMPUTE: return "cs_6_0";
        default: return nullptr;
    }
}

SGCore::DX12ShaderCompiler::Result SGCore::DX12ShaderCompiler::compile(const std::vector<std::uint32_t>& spirv,
                                                                       SGSLESubShaderType stage,
                                                                       const std::string& debugName,
                                                                       const std::vector<DX12RegisterBinding>& registers) noexcept
{
    Result result;

    const char* target = stageTarget(stage);
    if(!target)
    {
        result.m_log = "no D3D shader model target for this stage";
        return result;
    }
    if(spirv.empty())
    {
        result.m_log = "no SPIR-V to translate";
        return result;
    }

    try
    {
        spirv_cross::CompilerHLSL compiler(spirv);

        spirv_cross::CompilerHLSL::Options hlslOptions;
        // 6.0: register spaces need >= 5.1, and shader model 6 is what DXC compiles to DXIL
        hlslOptions.shader_model = 60;
        compiler.set_hlsl_options(hlslOptions);

        spirv_cross::CompilerGLSL::Options commonOptions = compiler.get_common_options();
        // y is flipped by the viewport (window passes only), not in the shader, so that offscreen
        // targets keep the GL memory layout — the same choice the Vulkan backend made
        commonOptions.vertex.flip_vert_y = false;
        // The engine's projections are GL-style: clip space z runs [-w; w]. D3D clips to [0; w] and
        // has no counterpart of VK_EXT_depth_clip_control (which is how the Vulkan backend keeps the
        // GL range), so the conversion is done here — this rewrites gl_Position.z into (z + w) * 0.5
        // in the emitted HLSL. Without it every depth test works on the wrong half of the range.
        commonOptions.vertex.fixup_clipspace = true;
        compiler.set_common_options(commonOptions);

        // Pin every register: left to itself spirv-cross numbers resources its own way, and the root
        // signature would then describe different registers than the code uses. Arrays are what makes
        // "register = binding" wrong (an array of 8 samplers occupies 8 registers).
        const spv::ExecutionModel executionModel = stageExecutionModel(stage);
        for(const auto& assigned : registers)
        {
            spirv_cross::HLSLResourceBinding hlslBinding;
            hlslBinding.stage = executionModel;
            hlslBinding.desc_set = assigned.m_set;
            hlslBinding.binding = assigned.m_binding;
            hlslBinding.cbv.register_space = assigned.m_set;
            hlslBinding.cbv.register_binding = assigned.m_viewRegister;
            hlslBinding.srv.register_space = assigned.m_set;
            hlslBinding.srv.register_binding = assigned.m_viewRegister;
            hlslBinding.uav.register_space = assigned.m_set;
            hlslBinding.uav.register_binding = assigned.m_viewRegister;
            // a combined image sampler is a texture plus a sampler: the two take their registers from
            // different members of the same entry
            hlslBinding.sampler.register_space = assigned.m_set;
            hlslBinding.sampler.register_binding = assigned.m_samplerRegister;
            compiler.add_hlsl_resource_binding(hlslBinding);
        }

        result.m_hlsl = compiler.compile();
    }
    catch(const std::exception& exception)
    {
        result.m_log = std::string("spirv-cross failed: ") + exception.what();
        return result;
    }

    DX12Ptr<IDxcCompiler3> compiler;
    if(!SG_DX_CHECK(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler))))
    {
        result.m_log = "DxcCreateInstance failed: is dxcompiler.dll next to the executable?";
        return result;
    }

    std::wstring wideName;
    Utils::fromUTF8(debugName, wideName);
    std::wstring wideTarget;
    Utils::fromUTF8(target, wideTarget);

    // argv[0] is the source name shown in diagnostics, not a flag
    std::vector<const wchar_t*> arguments = { wideName.c_str(), L"-T", wideTarget.c_str(), L"-E", L"main" };
#ifdef SUNGEAR_DEBUG
    // -Qembed_debug keeps the debug info in the object instead of a separate PDB blob
    arguments.insert(arguments.end(), { L"-Zi", L"-Qembed_debug", L"-Od" });
#endif
    // never -Vd: unsigned DXIL is rejected by the runtime with E_INVALIDARG (measured). The DXC of
    // the Windows SDK signs internally, so no dxil.dll has to ship next to it.

    const DxcBuffer source { result.m_hlsl.data(), result.m_hlsl.size(), DXC_CP_UTF8 };

    DX12Ptr<IDxcResult> compiled;
    if(!SG_DX_CHECK(compiler->Compile(&source, arguments.data(), static_cast<UINT32>(arguments.size()),
                                      nullptr, IID_PPV_ARGS(&compiled))))
    {
        result.m_log = "IDxcCompiler3::Compile failed";
        return result;
    }

    // warnings land here too, so the text is taken independently of the status
    DX12Ptr<IDxcBlobUtf8> errors;
    if(SUCCEEDED(compiled->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr)) && errors && errors->GetStringLength() > 0)
    {
        result.m_log.assign(errors->GetStringPointer(), errors->GetStringLength());
    }

    // a compilation that failed still returns S_OK from Compile(): the real verdict is the status
    HRESULT status = E_FAIL;
    compiled->GetStatus(&status);
    if(FAILED(status))
    {
        if(result.m_log.empty()) result.m_log = dx12ResultToString(status);
        return result;
    }

    DX12Ptr<IDxcBlob> object;
    if(!SG_DX_CHECK(compiled->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&object), nullptr)) || !object)
    {
        result.m_log = "DXC produced no object blob";
        return result;
    }

    const auto* bytes = static_cast<const std::uint8_t*>(object->GetBufferPointer());
    result.m_bytecode.assign(bytes, bytes + object->GetBufferSize());
    result.m_success = true;
    return result;
}

#endif
