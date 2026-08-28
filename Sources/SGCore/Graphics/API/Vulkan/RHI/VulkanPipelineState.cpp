//
// Created by 8bitniksis on 17.08.2026.
//

#include "VulkanPipelineState.h"

#include <algorithm>

#include "SGCore/Graphics/API/Vulkan/VulkanTypesCaster.h"
#include "SGCore/Logger/Logger.h"
#include "VulkanDevice.h"
#include "VulkanShaderProgram.h"

SGCore::VulkanPipelineState::VulkanPipelineState(VulkanDevice& device, const PipelineStateDesc& desc) noexcept
{
    m_context = device.getContext().shared_from_this();
    m_desc = desc;
    m_debugName = desc.m_debugName;

    // Vulkan clip space has +y down while offscreen passes keep the GL memory layout (no viewport
    // flip), so GL counter-clockwise becomes Vulkan clockwise. Swapchain passes flip the viewport
    // and the command list inverts this dynamically.
    m_frontFace = desc.m_meshRenderState.m_facesCullingPolygonsOrder == SGPolygonsOrder::SGG_CCW
                  ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;

    m_context->registerResource(this, [this] { releaseGPU(); });
}

SGCore::VulkanPipelineState::~VulkanPipelineState()
{
    if(m_context) m_context->unregisterResource(this);
    releaseGPU();
}

void SGCore::VulkanPipelineState::releaseGPU() noexcept
{
    if(m_context && m_context->m_device != VK_NULL_HANDLE)
    {
        for(const auto& variant : m_variants)
        {
            if(variant.m_pipeline != VK_NULL_HANDLE) vkDestroyPipeline(m_context->m_device, variant.m_pipeline, nullptr);
        }
    }
    m_variants.clear();
}

SGCore::VulkanShaderProgram* SGCore::VulkanPipelineState::getProgram() const noexcept
{
    return static_cast<VulkanShaderProgram*>(m_desc.m_program.get());
}

VkPipelineLayout SGCore::VulkanPipelineState::getLayout() const noexcept
{
    const auto* program = getProgram();
    return program ? program->getPipelineLayout() : VK_NULL_HANDLE;
}

VkPipeline SGCore::VulkanPipelineState::getOrCreate(const VulkanPassFormats& formats) noexcept
{
    for(const auto& variant : m_variants)
    {
        if(variant.m_formats == formats) return variant.m_pipeline;
    }
    const VkPipeline pipeline = build(formats);
    m_variants.push_back({ formats, pipeline });
    return pipeline;
}

VkPipeline SGCore::VulkanPipelineState::build(const VulkanPassFormats& formats) noexcept
{
    auto& context = *m_context;
    const auto* program = getProgram();
    if(!program || !program->isValid())
    {
        SG_LOG_E("VulkanPipelineState '{}': no valid shader program.", m_debugName);
        return VK_NULL_HANDLE;
    }

    std::vector<VkPipelineShaderStageCreateInfo> stages;
    for(const auto& stage : program->getStages())
    {
        VkPipelineShaderStageCreateInfo stageInfo { };
        stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stageInfo.stage = VulkanShaderProgram::stageToVk(stage.m_type);
        stageInfo.module = stage.m_module;
        stageInfo.pName = "main";
        stages.push_back(stageInfo);
    }

    // ---- vertex input
    std::vector<VkVertexInputBindingDescription> bindings;
    for(const auto& slot : m_desc.m_vertexInput.m_slots)
    {
        VkVertexInputBindingDescription binding { };
        binding.binding = slot.m_slot;
        binding.stride = slot.m_stride;
        binding.inputRate = slot.m_perInstance ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
        bindings.push_back(binding);
    }

    // The layout comes from the mesh, which carries every attribute the engine knows about, while a
    // given program reads only some of them (the outline and shadow shaders take positions and bones,
    // the post-process ones nothing at all). Declaring an attribute no stage consumes is legal but
    // reported by validation for every pipeline, so the mesh layout is intersected with what the
    // program actually declares. The program is part of the PSO cache key, so this stays per-variant.
    const auto consumesLocation = [program](std::uint32_t location) {
        const auto& inputs = program->getReflection().m_vertexInputs;
        if(inputs.empty()) return true; // no reflection data: keep the layout as the mesh gave it
        return std::any_of(inputs.begin(), inputs.end(), [location](const auto& input) {
            // one input can span several locations (a mat4 covers four)
            return location >= input.m_location && location < input.m_location + input.m_locationsCount;
        });
    };

    std::vector<VkVertexInputAttributeDescription> attributes;
    for(const auto& attribute : m_desc.m_vertexInput.m_attributes)
    {
        if(!consumesLocation(attribute.m_location)) continue;

        VkVertexInputAttributeDescription description { };
        description.location = attribute.m_location;
        description.binding = attribute.m_bufferSlot;
        description.format = VulkanTypesCaster::vertexAttributeFormat(attribute.m_dataType, attribute.m_componentsCount, attribute.m_normalized);
        description.offset = attribute.m_offset;
        attributes.push_back(description);
    }
    VkPipelineVertexInputStateCreateInfo vertexInput { };
    vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInput.vertexBindingDescriptionCount = static_cast<std::uint32_t>(bindings.size());
    vertexInput.pVertexBindingDescriptions = bindings.data();
    vertexInput.vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attributes.size());
    vertexInput.pVertexAttributeDescriptions = attributes.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly { };
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VulkanTypesCaster::sggDrawModeToVk(m_desc.m_meshRenderState.m_drawMode);

    VkPipelineTessellationStateCreateInfo tessellation { };
    tessellation.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellation.patchControlPoints = static_cast<std::uint32_t>(m_desc.m_meshRenderState.m_patchVerticesCount);

    // ---- viewport: dynamic; depth range [-1, 1] when the extension is on
    VkPipelineViewportDepthClipControlCreateInfoEXT depthClipControl { };
    depthClipControl.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_DEPTH_CLIP_CONTROL_CREATE_INFO_EXT;
    depthClipControl.negativeOneToOne = VK_TRUE;

    VkPipelineViewportStateCreateInfo viewport { };
    viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport.viewportCount = 1;
    viewport.scissorCount = 1;
    if(context.m_depthClipControl) viewport.pNext = &depthClipControl;

    // ---- rasterization
    VkPipelineRasterizationStateCreateInfo rasterization { };
    rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization.polygonMode = VK_POLYGON_MODE_FILL;
    rasterization.cullMode = m_desc.m_meshRenderState.m_useFacesCulling
                             ? VulkanTypesCaster::sggFaceTypeToVk(m_desc.m_meshRenderState.m_facesCullingFaceType) : VK_CULL_MODE_NONE;
    rasterization.frontFace = m_frontFace; // dynamic, but the static value must still be valid
    rasterization.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisample { };
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.rasterizationSamples = formats.m_samples;

    // ---- depth / stencil
    const auto& renderState = m_desc.m_renderState;
    VkPipelineDepthStencilStateCreateInfo depthStencil { };
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    const bool hasDepth = formats.m_depthFormat != VK_FORMAT_UNDEFINED;
    depthStencil.depthTestEnable = hasDepth && renderState.m_useDepthTest ? VK_TRUE : VK_FALSE;
    depthStencil.depthWriteEnable = hasDepth && renderState.m_depthMask ? VK_TRUE : VK_FALSE;
    depthStencil.depthCompareOp = VulkanTypesCaster::sggCompareToVk(renderState.m_depthFunc);
    depthStencil.stencilTestEnable = formats.m_stencilFormat != VK_FORMAT_UNDEFINED && renderState.m_useStencilTest ? VK_TRUE : VK_FALSE;
    VkStencilOpState stencilOp { };
    stencilOp.failOp = VulkanTypesCaster::sggStencilOpToVk(renderState.m_stencilFailOp);
    stencilOp.depthFailOp = VulkanTypesCaster::sggStencilOpToVk(renderState.m_stencilZFailOp);
    stencilOp.passOp = VulkanTypesCaster::sggStencilOpToVk(renderState.m_stencilZPassOp);
    stencilOp.compareOp = VulkanTypesCaster::sggCompareToVk(renderState.m_stencilFunc);
    stencilOp.compareMask = renderState.m_stencilFuncMask;
    stencilOp.writeMask = renderState.m_stencilMask;
    stencilOp.reference = static_cast<std::uint32_t>(renderState.m_stencilFuncRef);
    depthStencil.front = stencilOp;
    depthStencil.back = stencilOp;

    // ---- blending: BlendingState::m_forAttachment == -1 applies to every attachment
    const auto& blending = m_desc.m_blendingState;
    std::vector<VkPipelineColorBlendAttachmentState> blendAttachments(formats.m_colorFormats.size());
    for(std::size_t i = 0; i < blendAttachments.size(); ++i)
    {
        auto& attachment = blendAttachments[i];
        const bool enabled = blending.m_useBlending && (blending.m_forAttachment < 0 || static_cast<std::size_t>(blending.m_forAttachment) == i);
        attachment.blendEnable = enabled ? VK_TRUE : VK_FALSE;
        attachment.srcColorBlendFactor = VulkanTypesCaster::sggBlendFactorToVk(blending.m_sFactor);
        attachment.dstColorBlendFactor = VulkanTypesCaster::sggBlendFactorToVk(blending.m_dFactor);
        attachment.colorBlendOp = VulkanTypesCaster::sggBlendEquationToVk(blending.m_blendingEquation);
        attachment.srcAlphaBlendFactor = attachment.srcColorBlendFactor;
        attachment.dstAlphaBlendFactor = attachment.dstColorBlendFactor;
        attachment.alphaBlendOp = attachment.colorBlendOp;
        attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    }
    VkPipelineColorBlendStateCreateInfo colorBlend { };
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.attachmentCount = static_cast<std::uint32_t>(blendAttachments.size());
    colorBlend.pAttachments = blendAttachments.data();

    // ---- dynamic state
    const VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_FRONT_FACE };
    VkPipelineDynamicStateCreateInfo dynamic { };
    dynamic.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic.dynamicStateCount = 3;
    dynamic.pDynamicStates = dynamicStates;

    // ---- dynamic rendering formats
    VkPipelineRenderingCreateInfo rendering { };
    rendering.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    rendering.colorAttachmentCount = static_cast<std::uint32_t>(formats.m_colorFormats.size());
    rendering.pColorAttachmentFormats = formats.m_colorFormats.data();
    rendering.depthAttachmentFormat = formats.m_depthFormat;
    rendering.stencilAttachmentFormat = formats.m_stencilFormat;

    VkGraphicsPipelineCreateInfo pipelineInfo { };
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = &rendering;
    pipelineInfo.stageCount = static_cast<std::uint32_t>(stages.size());
    pipelineInfo.pStages = stages.data();
    pipelineInfo.pVertexInputState = &vertexInput;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pTessellationState = inputAssembly.topology == VK_PRIMITIVE_TOPOLOGY_PATCH_LIST ? &tessellation : nullptr;
    pipelineInfo.pViewportState = &viewport;
    pipelineInfo.pRasterizationState = &rasterization;
    pipelineInfo.pMultisampleState = &multisample;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlend;
    pipelineInfo.pDynamicState = &dynamic;
    pipelineInfo.layout = program->getPipelineLayout();
    pipelineInfo.renderPass = VK_NULL_HANDLE;

    VkPipeline pipeline = VK_NULL_HANDLE;
    if(!SG_VK_CHECK(vkCreateGraphicsPipelines(context.m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline)))
    {
        SG_LOG_E("VulkanPipelineState '{}': pipeline creation failed.", m_debugName);
        return VK_NULL_HANDLE;
    }
    context.setObjectName(reinterpret_cast<std::uint64_t>(pipeline), VK_OBJECT_TYPE_PIPELINE, m_debugName);
    return pipeline;
}
