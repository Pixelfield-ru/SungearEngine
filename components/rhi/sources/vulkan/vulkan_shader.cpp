//
// Created by stuka on 16.09.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_shader.h"

#include "pixelfield/rhi/vulkan/vulkan_context.h"
#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_types.h"

pf::rhi::VulkanShader::VulkanShader(VulkanShader&& other) noexcept :
    context_(std::exchange(other.context_, nullptr)),
    logical_device_(std::exchange(other.logical_device_, nullptr)),
    stages_(std::move(other.stages_))
{
}

pf::rhi::VulkanShader::~VulkanShader() noexcept
{
    destroy();
}

bool pf::rhi::VulkanShader::valid() const noexcept
{
    return !stages_.empty();
}

pf::rhi::VulkanShader& pf::rhi::VulkanShader::operator=(VulkanShader&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    destroy();

    context_ = std::exchange(other.context_, nullptr);
    logical_device_ = std::exchange(other.logical_device_, nullptr);
    stages_ = std::move(other.stages_);

    return *this;
}

void pf::rhi::VulkanShader::destroy() noexcept
{
    for(const auto& stage : stages_)
    {
        vkDestroyShaderModule(logical_device_->handle_, stage.module, context_->allocator_.get());
    }
    stages_.clear();
}

std::error_code pf::rhi::VulkanShader::create(const ShaderCreateInfo& create_info) noexcept
{
    const auto* vulkan_context = static_cast<const VulkanContext*>(create_info.context);
    const auto* vulkan_logical_device = static_cast<const VulkanLogicalDevice*>(create_info.logical_device);

    context_ = vulkan_context;
    logical_device_ = vulkan_logical_device;

    stages_.reserve(create_info.stages_create_info.size());

    for(const auto& stage : create_info.stages_create_info)
    {
        auto& vulkan_stage = stages_.emplace_back();
        vulkan_stage.stage_info = stage.stage;

        VkShaderModuleCreateInfo vulkan_module_create_info {};
        vulkan_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vulkan_module_create_info.codeSize = stage.code_buffer_size;
        vulkan_module_create_info.pCode = static_cast<const uint32_t*>(stage.code_buffer);

        if(vkCreateShaderModule(logical_device_->handle_, &vulkan_module_create_info, context_->allocator_.get(), &vulkan_stage.module) != VK_SUCCESS)
        {
            return VulkanError::kCannotCreateShaderModule;
        }

        auto& pipeline_stage_info = vulkan_stage.pipeline_stage_info;
        pipeline_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipeline_stage_info.module = vulkan_stage.module;
        pipeline_stage_info.pName = vulkan_stage.stage_info.entry_point_name.c_str();

        if(stage.stage.type == ShaderStageType::kUnknown)
        {
            continue;
        }

        pipeline_stage_info.stage = static_cast<VkShaderStageFlagBits>(toVkShaderStageFlags(stage.stage.type));
    }

    return {};
}
