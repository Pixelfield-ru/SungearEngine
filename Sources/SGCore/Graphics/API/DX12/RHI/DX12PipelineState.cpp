//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12PipelineState.h"

#if defined(_WIN32)

#include <algorithm>
#include <string>

#include "SGCore/Graphics/API/DX12/DX12TypesCaster.h"
#include "SGCore/Logger/Logger.h"
#include "DX12Device.h"
#include "DX12ShaderProgram.h"

SGCore::DX12PipelineState::DX12PipelineState(DX12Device& device, const PipelineStateDesc& desc) noexcept
{
    m_context = device.getContextRef();
    m_desc = desc;
    m_debugName = desc.m_debugName;
    m_topology = DX12TypesCaster::sggDrawModeToDX(desc.m_meshRenderState.m_drawMode, desc.m_meshRenderState.m_patchVerticesCount);
    m_stencilRef = static_cast<std::uint32_t>(desc.m_renderState.m_stencilFuncRef);
}

SGCore::DX12ShaderProgram* SGCore::DX12PipelineState::getProgram() const noexcept
{
    return static_cast<DX12ShaderProgram*>(m_desc.m_program.get());
}

ID3D12RootSignature* SGCore::DX12PipelineState::getRootSignature() const noexcept
{
    const auto* program = getProgram();
    return program ? program->getRootSignature() : nullptr;
}

ID3D12PipelineState* SGCore::DX12PipelineState::getOrCreate(const DX12PassFormats& formats) noexcept
{
    for(const auto& variant : m_variants)
    {
        if(variant.m_formats == formats) return variant.m_pipeline.Get();
    }
    auto pipeline = build(formats);
    m_variants.push_back({ formats, pipeline });
    return pipeline.Get();
}

SGCore::DX12Ptr<ID3D12PipelineState> SGCore::DX12PipelineState::build(const DX12PassFormats& formats) noexcept
{
    DX12Ptr<ID3D12PipelineState> pipeline;

    auto* program = getProgram();
    if(!program || !program->getRootSignature())
    {
        SG_LOG_E("DX12PipelineState '{}': no shader program with a root signature.", m_debugName);
        return pipeline;
    }
    if(!program->isValid())
    {
        // reflection and root signature exist, the DXIL does not — see DX12ShaderProgram (task 3.4)
        SG_LOG_E("DX12PipelineState '{}': the shader program carries no DXIL yet (stage 3, task 3.4).", m_debugName);
        return pipeline;
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc { };
    pipelineDesc.pRootSignature = program->getRootSignature();
    pipelineDesc.VS = program->getStageBytecode(SGSLESubShaderType::SST_VERTEX);
    pipelineDesc.PS = program->getStageBytecode(SGSLESubShaderType::SST_FRAGMENT);
    pipelineDesc.GS = program->getStageBytecode(SGSLESubShaderType::SST_GEOMETRY);
    pipelineDesc.HS = program->getStageBytecode(SGSLESubShaderType::SST_TESS_CONTROL);
    pipelineDesc.DS = program->getStageBytecode(SGSLESubShaderType::SST_TESS_EVALUATION);

    // ---- input layout. HLSL addresses vertex inputs by semantic, and spirv-cross names a SPIR-V
    // location N as TEXCOORDN, so the location of the RHI layout becomes the semantic index. The mesh
    // carries every attribute the engine knows while a program reads only some of them, so the layout
    // is intersected with what the program declares (as on Vulkan, where it silenced validation).
    const auto inputCovering = [program](std::uint32_t location) -> const ShaderReflection::VertexInput* {
        const auto& inputs = program->getReflection().m_vertexInputs;
        for(const auto& input : inputs)
        {
            // one input can span several locations (a mat4 covers four)
            if(location >= input.m_location && location < input.m_location + input.m_locationsCount) return &input;
        }
        return nullptr;
    };
    const bool hasReflectedInputs = !program->getReflection().m_vertexInputs.empty();

    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
    // SemanticName is a borrowed pointer read at CreateGraphicsPipelineState time, so the matrix
    // semantics have to outlive the loop; reserving keeps the strings from moving
    std::vector<std::string> semanticNames;
    semanticNames.reserve(m_desc.m_vertexInput.m_attributes.size());
    for(const auto& attribute : m_desc.m_vertexInput.m_attributes)
    {
        const auto* covering = inputCovering(attribute.m_location);
        // no reflection data at all: keep the layout as the mesh gave it
        if(hasReflectedInputs && !covering) continue;

        bool perInstance = false;
        std::uint32_t stepRate = 0;
        for(const auto& slot : m_desc.m_vertexInput.m_slots)
        {
            if(slot.m_slot != attribute.m_bufferSlot) continue;
            perInstance = slot.m_perInstance;
            stepRate = perInstance ? 1 : 0;
            break;
        }

        // A matrix input is not TEXCOORD<location> per column: spirv-cross gives the whole matrix the
        // semantic of its first location and numbers the columns, so `in mat4` at location 0 becomes
        // TEXCOORD0_0..TEXCOORD0_3 — D3D reads that as name "TEXCOORD0_" with indices 0..3. Naming the
        // columns TEXCOORD0..3 the ordinary way made CreateGraphicsPipelineState refuse the instancing
        // pipeline outright (E_INVALIDARG, "input signature expects TEXCOORD0_/0").
        const bool isMatrixColumn = covering && covering->m_locationsCount > 1;
        semanticNames.push_back(isMatrixColumn ? "TEXCOORD" + std::to_string(covering->m_location) + "_" : "TEXCOORD");

        D3D12_INPUT_ELEMENT_DESC element { };
        element.SemanticName = semanticNames.back().c_str();
        element.SemanticIndex = isMatrixColumn ? attribute.m_location - covering->m_location : attribute.m_location;
        element.Format = DX12TypesCaster::vertexAttributeFormat(attribute.m_dataType, attribute.m_componentsCount, attribute.m_normalized);
        element.InputSlot = attribute.m_bufferSlot;
        element.AlignedByteOffset = attribute.m_offset;
        element.InputSlotClass = perInstance ? D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA : D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        element.InstanceDataStepRate = stepRate;
        inputElements.push_back(element);
    }
    pipelineDesc.InputLayout.pInputElementDescs = inputElements.data();
    pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputElements.size());
    pipelineDesc.PrimitiveTopologyType = DX12TypesCaster::sggDrawModeToTopologyType(m_desc.m_meshRenderState.m_drawMode);

    // ---- rasterizer. D3D clips with +y up like GL but keeps row 0 at the top of the target, so an
    // unflipped pass mirrors the picture and with it the winding: GL counter-clockwise is clockwise
    // for us. A flipped pass (offscreen, see DX12CommandList::applyViewportScissor) mirrors it back.
    const auto& meshState = m_desc.m_meshRenderState;
    pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    pipelineDesc.RasterizerState.CullMode = meshState.m_useFacesCulling
                                            ? DX12TypesCaster::sggFaceTypeToDX(meshState.m_facesCullingFaceType)
                                            : D3D12_CULL_MODE_NONE;
    // D3D decides the winding on the render target, whose y grows downwards, so GL's front face
    // reads inverted here; a flipped viewport mirrors it back. Measured with a gl_FrontFacing probe
    // against GL46 (2026-08-22), not derived — the sign of this is easy to reason wrong.
    const bool glFrontIsCounterClockwise = meshState.m_facesCullingPolygonsOrder == SGPolygonsOrder::SGG_CCW;
    pipelineDesc.RasterizerState.FrontCounterClockwise = (glFrontIsCounterClockwise != formats.m_viewportFlipped) ? TRUE : FALSE;
    pipelineDesc.RasterizerState.DepthClipEnable = TRUE;

    // ---- depth / stencil
    const auto& renderState = m_desc.m_renderState;
    const bool hasDepth = formats.m_depthFormat != DXGI_FORMAT_UNKNOWN;
    pipelineDesc.DepthStencilState.DepthEnable = hasDepth && renderState.m_useDepthTest ? TRUE : FALSE;
    pipelineDesc.DepthStencilState.DepthWriteMask = hasDepth && renderState.m_depthMask ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
    pipelineDesc.DepthStencilState.DepthFunc = DX12TypesCaster::sggCompareToDX(renderState.m_depthFunc);
    pipelineDesc.DepthStencilState.StencilEnable = hasDepth && renderState.m_useStencilTest ? TRUE : FALSE;
    pipelineDesc.DepthStencilState.StencilReadMask = static_cast<UINT8>(renderState.m_stencilFuncMask);
    pipelineDesc.DepthStencilState.StencilWriteMask = static_cast<UINT8>(renderState.m_stencilMask);
    D3D12_DEPTH_STENCILOP_DESC stencilOp { };
    stencilOp.StencilFailOp = DX12TypesCaster::sggStencilOpToDX(renderState.m_stencilFailOp);
    stencilOp.StencilDepthFailOp = DX12TypesCaster::sggStencilOpToDX(renderState.m_stencilZFailOp);
    stencilOp.StencilPassOp = DX12TypesCaster::sggStencilOpToDX(renderState.m_stencilZPassOp);
    stencilOp.StencilFunc = DX12TypesCaster::sggCompareToDX(renderState.m_stencilFunc);
    pipelineDesc.DepthStencilState.FrontFace = stencilOp;
    pipelineDesc.DepthStencilState.BackFace = stencilOp;
    pipelineDesc.DSVFormat = formats.m_depthFormat;

    // ---- blending: BlendingState::m_forAttachment == -1 applies to every attachment
    const auto& blending = m_desc.m_blendingState;
    pipelineDesc.BlendState.IndependentBlendEnable = blending.m_forAttachment >= 0 ? TRUE : FALSE;
    for(std::size_t i = 0; i < 8; ++i)
    {
        auto& target = pipelineDesc.BlendState.RenderTarget[i];
        const bool enabled = blending.m_useBlending &&
                             (blending.m_forAttachment < 0 || static_cast<std::size_t>(blending.m_forAttachment) == i) &&
                             i < formats.m_colorFormats.size();
        target.BlendEnable = enabled ? TRUE : FALSE;
        target.SrcBlend = DX12TypesCaster::sggBlendFactorToDX(blending.m_sFactor);
        target.DestBlend = DX12TypesCaster::sggBlendFactorToDX(blending.m_dFactor);
        target.BlendOp = DX12TypesCaster::sggBlendEquationToDX(blending.m_blendingEquation);
        target.SrcBlendAlpha = target.SrcBlend;
        target.DestBlendAlpha = target.DestBlend;
        target.BlendOpAlpha = target.BlendOp;
        target.LogicOp = D3D12_LOGIC_OP_NOOP;
        target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    }

    pipelineDesc.SampleMask = UINT_MAX;
    pipelineDesc.SampleDesc.Count = formats.m_samples;
    pipelineDesc.NumRenderTargets = static_cast<UINT>(std::min<std::size_t>(formats.m_colorFormats.size(), 8));
    for(UINT i = 0; i < pipelineDesc.NumRenderTargets; ++i)
    {
        pipelineDesc.RTVFormats[i] = formats.m_colorFormats[i];
    }

    if(!SG_DX_CHECK(m_context->m_device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipeline))))
    {
        SG_LOG_E("DX12PipelineState '{}': pipeline creation failed.", m_debugName);
        pipeline.Reset();
        return pipeline;
    }
    m_context->setObjectName(pipeline.Get(), m_debugName);
    return pipeline;
}

#endif
