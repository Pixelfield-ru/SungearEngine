//
// Created by stuka on 18.09.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_pipeline_state.h"

#include "pixelfield/rhi/vulkan/vulkan_context.h"
#include "pixelfield/rhi/vulkan/vulkan_descriptor_set.h"
#include "pixelfield/rhi/vulkan/vulkan_errors.h"
#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_shader.h"
#include "pixelfield/rhi/vulkan/vulkan_pass.h"
#include "pixelfield/rhi/vulkan/vulkan_types.h"

pf::rhi::VulkanPipelineState::VulkanPipelineState(VulkanPipelineState&& other) noexcept :
    context_(std::exchange(other.context_, nullptr)),
    logical_device_(std::exchange(other.logical_device_, nullptr)),
    pass_(std::exchange(other.pass_, nullptr)),
    pipeline_layout_(std::exchange(other.pipeline_layout_, VK_NULL_HANDLE)),
    graphics_pipeline_(std::exchange(other.graphics_pipeline_, VK_NULL_HANDLE))
{

}

pf::rhi::VulkanPipelineState::~VulkanPipelineState() noexcept
{
    destroy();
}

pf::rhi::VulkanPipelineState& pf::rhi::VulkanPipelineState::operator=(VulkanPipelineState&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    destroy();

    context_ = std::exchange(other.context_, nullptr);
    logical_device_ = std::exchange(other.logical_device_, nullptr);
    pass_ = std::exchange(other.pass_, nullptr);
    pipeline_layout_ = std::exchange(other.pipeline_layout_, VK_NULL_HANDLE);
    graphics_pipeline_ = std::exchange(other.graphics_pipeline_, VK_NULL_HANDLE);

    return *this;
}

void pf::rhi::VulkanPipelineState::destroy() noexcept
{
    if(graphics_pipeline_)
    {
        vkDestroyPipeline(logical_device_->handle_, graphics_pipeline_, context_->allocator_.get());
        graphics_pipeline_ = VK_NULL_HANDLE;
    }

    if(pipeline_layout_)
    {
        vkDestroyPipelineLayout(logical_device_->handle_, pipeline_layout_, context_->allocator_.get());
        pipeline_layout_ = VK_NULL_HANDLE;
    }
}

std::error_code pf::rhi::VulkanPipelineState::create(const PipelineStateCreateInfo& create_info) noexcept
{
    const auto* vulkan_context = static_cast<const VulkanContext*>(create_info.context);
    const auto* vulkan_logical_device = static_cast<const VulkanLogicalDevice*>(create_info.logical_device);
    const auto* vulkan_pass = static_cast<const VulkanPass*>(create_info.pass);
    const auto* vulkan_shader = static_cast<const VulkanShader*>(create_info.shader);

    context_ = vulkan_context;
    logical_device_ = vulkan_logical_device;
    pass_ = vulkan_pass;

    std::vector<VkVertexInputBindingDescription> vk_vertex_bindings;
    std::vector<VkVertexInputAttributeDescription> vk_vertex_attribs;
    for(const auto& binding : create_info.vertex_bindings)
    {
        auto& vk_binding = vk_vertex_bindings.emplace_back();
        vk_binding.binding = binding.binding;
        vk_binding.stride = binding.stride;
        vk_binding.inputRate = toVkInputRate(binding.input_rate);

        for(const auto& attribute : binding.attributes)
        {
            auto& vk_attribute = vk_vertex_attribs.emplace_back();
            vk_attribute.binding = binding.binding;
            vk_attribute.location = attribute.location;
            vk_attribute.offset = attribute.offset;
            vk_attribute.format = toVkFormat(attribute.format);
        }
    }

    VkPipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = vk_vertex_bindings.size();
    vertex_input_info.pVertexBindingDescriptions = vk_vertex_bindings.data(); // Optional
    vertex_input_info.vertexAttributeDescriptionCount = vk_vertex_attribs.size();
    vertex_input_info.pVertexAttributeDescriptions = vk_vertex_attribs.data(); // Optional

    VkPipelineInputAssemblyStateCreateInfo input_assembly{};
    input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology = toVkPrimitiveTopology(create_info.topology);
    input_assembly.primitiveRestartEnable = create_info.enable_primitive_restart;

    VkPipelineViewportStateCreateInfo viewport_state{};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = nullptr;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = nullptr;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = create_info.enable_depth_clamp;
    rasterizer.rasterizerDiscardEnable = create_info.enable_rasterizer_discard;
    rasterizer.polygonMode = toVkPolygonMode(create_info.polygon_mode);
    rasterizer.cullMode = toVkCullMode(create_info.cull_mode);
    rasterizer.frontFace = toVkFrontFace(create_info.front_face);
    rasterizer.depthBiasEnable = create_info.enable_depth_bias;
    rasterizer.depthBiasConstantFactor = create_info.depth_bias_constant_factor; // Optional
    rasterizer.depthBiasClamp = create_info.depth_bias_clamp; // Optional
    rasterizer.depthBiasSlopeFactor = create_info.depth_bias_slope_factor; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = create_info.enable_sample_shading;
    multisampling.rasterizationSamples = toVkSampleCount(create_info.rasterization_sample_count);
    multisampling.minSampleShading = create_info.min_sample_shading; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = create_info.enable_alpha_to_coverage; // Optional
    multisampling.alphaToOneEnable = create_info.enable_alpha_to_one; // Optional

    std::array dynamic_states = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamic_state{};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount = dynamic_states.size();
    dynamic_state.pDynamicStates = dynamic_states.data();

    auto vulkan_blend_states = std::vector<VkPipelineColorBlendAttachmentState>(create_info.attachments_blend_info.size());

    for(size_t i = 0; i < create_info.attachments_blend_info.size(); ++i)
    {
        const auto& blend_info = create_info.attachments_blend_info[i];

        VkPipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.colorWriteMask = toVkColorComponents(blend_info.color_write_mask);
        color_blend_attachment.blendEnable = blend_info.enable_blend;
        color_blend_attachment.srcColorBlendFactor = toVkBlendFactor(blend_info.src_color_blend_factor); // Optional
        color_blend_attachment.dstColorBlendFactor = toVkBlendFactor(blend_info.dst_color_blend_factor); // Optional
        color_blend_attachment.colorBlendOp = toVkBlendOp(blend_info.color_blend_op); // Optional
        color_blend_attachment.srcAlphaBlendFactor = toVkBlendFactor(blend_info.src_alpha_blend_factor); // Optional
        color_blend_attachment.dstAlphaBlendFactor = toVkBlendFactor(blend_info.dst_alpha_blend_factor); // Optional
        color_blend_attachment.alphaBlendOp = toVkBlendOp(blend_info.alpha_blend_op); // Optional

        vulkan_blend_states[i] = color_blend_attachment;
    }

    VkPipelineColorBlendStateCreateInfo color_blending{};
    color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable = create_info.enable_logic_op;
    color_blending.logicOp = toVkLogicOp(create_info.logic_op); // Optional
    color_blending.attachmentCount = vulkan_blend_states.size();
    color_blending.pAttachments = vulkan_blend_states.data();
    color_blending.blendConstants[0] = create_info.blend_constants.x; // Optional
    color_blending.blendConstants[1] = create_info.blend_constants.y; // Optional
    color_blending.blendConstants[2] = create_info.blend_constants.z; // Optional
    color_blending.blendConstants[3] = create_info.blend_constants.w; // Optional

    std::vector<VkDescriptorSetLayout> set_layouts(create_info.descriptor_sets.size());
    for(size_t i = 0; i < create_info.descriptor_sets.size(); ++i)
    {
        set_layouts[i] = static_cast<const VulkanDescriptorSet*>(create_info.descriptor_sets[i])->layout_;
    }

    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = set_layouts.size();
    pipeline_layout_info.pSetLayouts = set_layouts.data();
    pipeline_layout_info.pushConstantRangeCount = 0; // Optional
    pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

    if(vkCreatePipelineLayout(vulkan_logical_device->handle_, &pipeline_layout_info, vulkan_context->allocator_.get(), &pipeline_layout_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreatePipelineLayout;
    }

    std::vector<VkPipelineShaderStageCreateInfo> shader_stages_info(vulkan_shader->stages_.size());
    for(size_t i = 0; i < vulkan_shader->stages_.size(); ++i)
    {
        shader_stages_info[i] = vulkan_shader->stages_[i].pipeline_stage_info;
    }

    // todo: use other pipelines
    VkGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = shader_stages_info.size();
    pipeline_info.pStages = shader_stages_info.data();
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pDepthStencilState = nullptr; // Optional
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.pDynamicState = &dynamic_state; // Optional
    pipeline_info.layout = pipeline_layout_;
    pipeline_info.renderPass = vulkan_pass->handle_;
    pipeline_info.subpass = 0;
    pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipeline_info.basePipelineIndex = -1; // Optional

    if(vkCreateGraphicsPipelines(vulkan_logical_device->handle_, VK_NULL_HANDLE, 1, &pipeline_info, vulkan_context->allocator_.get(), &graphics_pipeline_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreatePipeline;
    }

    return {};
}
