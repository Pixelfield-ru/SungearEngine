//
// Created by 8bitniksis on 17.08.2026.
//

#include "VulkanShaderProgram.h"

#include <algorithm>
#include <map>

#include "SGCore/Graphics/SPIRV/SPIRVCompiler.h"
#include "SGCore/Logger/Logger.h"
#include "VulkanDevice.h"

VkShaderStageFlagBits SGCore::VulkanShaderProgram::stageToVk(SGSLESubShaderType type) noexcept
{
    switch(type)
    {
        case SGSLESubShaderType::SST_VERTEX: return VK_SHADER_STAGE_VERTEX_BIT;
        case SGSLESubShaderType::SST_FRAGMENT: return VK_SHADER_STAGE_FRAGMENT_BIT;
        case SGSLESubShaderType::SST_GEOMETRY: return VK_SHADER_STAGE_GEOMETRY_BIT;
        case SGSLESubShaderType::SST_COMPUTE: return VK_SHADER_STAGE_COMPUTE_BIT;
        case SGSLESubShaderType::SST_TESS_CONTROL: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case SGSLESubShaderType::SST_TESS_EVALUATION: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        default: return VK_SHADER_STAGE_ALL;
    }
}

VkShaderStageFlags SGCore::VulkanShaderProgram::stageMaskToVk(shader_stages_mask_t mask) noexcept
{
    VkShaderStageFlags flags = 0;
    for(std::uint32_t i = 1; i <= static_cast<std::uint32_t>(SGSLESubShaderType::SST_TESS_EVALUATION); ++i)
    {
        if(mask & (1u << i)) flags |= stageToVk(static_cast<SGSLESubShaderType>(i));
    }
    return flags == 0 ? VK_SHADER_STAGE_ALL_GRAPHICS : flags;
}

VkDescriptorType SGCore::VulkanShaderProgram::descriptorTypeToVk(ShaderDescriptorType type) noexcept
{
    switch(type)
    {
        case ShaderDescriptorType::UNIFORM_BUFFER: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case ShaderDescriptorType::STORAGE_BUFFER: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case ShaderDescriptorType::COMBINED_IMAGE_SAMPLER: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case ShaderDescriptorType::SAMPLED_IMAGE: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case ShaderDescriptorType::SAMPLER: return VK_DESCRIPTOR_TYPE_SAMPLER;
        case ShaderDescriptorType::STORAGE_IMAGE: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case ShaderDescriptorType::UNIFORM_TEXEL_BUFFER: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        case ShaderDescriptorType::STORAGE_TEXEL_BUFFER: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        case ShaderDescriptorType::INPUT_ATTACHMENT: return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        default: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    }
}

SGCore::VulkanShaderProgram::VulkanShaderProgram(VulkanDevice& device, const ShaderProgramDesc& desc) noexcept
{
    m_debugName = desc.m_debugName;
    auto& context = device.getContext();
    m_context = context.shared_from_this();

    // stages lacking SPIR-V get compiled from their GLSL (already vulkanized by the caller)
    std::vector<SPIRVCompiler::StageResult> compiledStages;
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
            SG_LOG_E("VulkanShaderProgram '{}': SPIR-V compilation failed:\n{}", m_debugName, m_log);
            return;
        }
    }

    std::size_t compiledIndex = 0;
    for(const auto& stage : desc.m_stages)
    {
        SPIRVCompiler::StageResult result;
        result.m_type = stage.m_type;
        result.m_success = true;
        if(stage.m_spirv.empty())
        {
            result.m_spirv = compiled.m_stages[compiledIndex++].m_spirv;
        }
        else
        {
            result.m_spirv = stage.m_spirv;
        }
        compiledStages.push_back(std::move(result));
    }

    std::string reflectLog;
    if(!SPIRVCompiler::reflect(compiledStages, m_reflection, reflectLog))
    {
        m_log = reflectLog;
        SG_LOG_E("VulkanShaderProgram '{}': SPIR-V reflection failed:\n{}", m_debugName, reflectLog);
        return;
    }

    for(const auto& stage : compiledStages)
    {
        VkShaderModuleCreateInfo moduleInfo { };
        moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleInfo.codeSize = stage.m_spirv.size() * sizeof(std::uint32_t);
        moduleInfo.pCode = stage.m_spirv.data();

        VkShaderModule module = VK_NULL_HANDLE;
        if(!SG_VK_CHECK(vkCreateShaderModule(context.m_device, &moduleInfo, nullptr, &module)))
        {
            m_log = "vkCreateShaderModule failed";
            return;
        }
        context.setObjectName(reinterpret_cast<std::uint64_t>(module), VK_OBJECT_TYPE_SHADER_MODULE, m_debugName);
        m_stages.push_back({ stage.m_type, module });
    }

    if(!buildLayouts()) return;

    m_valid = true;
    context.registerResource(this, [this] { releaseGPU(); });
}

SGCore::VulkanShaderProgram::~VulkanShaderProgram()
{
    if(m_context) m_context->unregisterResource(this);
    releaseGPU();
}

void SGCore::VulkanShaderProgram::releaseGPU() noexcept
{
    if(!m_context || m_context->m_device == VK_NULL_HANDLE)
    {
        m_pipelineLayout = VK_NULL_HANDLE;
        m_setLayouts.clear();
        m_stages.clear();
        m_valid = false;
        return;
    }

    if(m_pipelineLayout != VK_NULL_HANDLE) vkDestroyPipelineLayout(m_context->m_device, m_pipelineLayout, nullptr);
    for(const auto layout : m_setLayouts)
    {
        if(layout != VK_NULL_HANDLE) vkDestroyDescriptorSetLayout(m_context->m_device, layout, nullptr);
    }
    for(const auto& stage : m_stages)
    {
        if(stage.m_module != VK_NULL_HANDLE) vkDestroyShaderModule(m_context->m_device, stage.m_module, nullptr);
    }
    m_pipelineLayout = VK_NULL_HANDLE;
    m_setLayouts.clear();
    m_stages.clear();
    m_valid = false;
}

bool SGCore::VulkanShaderProgram::buildLayouts() noexcept
{
    auto& context = *m_context;

    std::uint32_t setCount = 0;
    for(const auto& binding : m_reflection.m_bindings) setCount = std::max(setCount, binding.m_set + 1);

    // sets are contiguous in the pipeline layout: gaps get empty layouts
    m_setLayouts.assign(setCount, VK_NULL_HANDLE);
    for(std::uint32_t set = 0; set < setCount; ++set)
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        for(const auto& binding : m_reflection.m_bindings)
        {
            if(binding.m_set != set) continue;
            VkDescriptorSetLayoutBinding layoutBinding { };
            layoutBinding.binding = binding.m_binding;
            layoutBinding.descriptorType = descriptorTypeToVk(binding.m_type);
            layoutBinding.descriptorCount = binding.m_count == 0 ? 1 : binding.m_count;
            layoutBinding.stageFlags = stageMaskToVk(binding.m_stages);
            bindings.push_back(layoutBinding);
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo { };
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<std::uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();
        if(!SG_VK_CHECK(vkCreateDescriptorSetLayout(context.m_device, &layoutInfo, nullptr, &m_setLayouts[set])))
        {
            m_log = "vkCreateDescriptorSetLayout failed";
            return false;
        }
    }

    std::vector<VkPushConstantRange> pushConstants;
    for(const auto& range : m_reflection.m_pushConstants)
    {
        VkPushConstantRange vkRange { };
        vkRange.stageFlags = stageMaskToVk(range.m_stages);
        vkRange.offset = range.m_offset;
        vkRange.size = range.m_size;
        pushConstants.push_back(vkRange);
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo { };
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<std::uint32_t>(m_setLayouts.size());
    pipelineLayoutInfo.pSetLayouts = m_setLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = static_cast<std::uint32_t>(pushConstants.size());
    pipelineLayoutInfo.pPushConstantRanges = pushConstants.data();
    if(!SG_VK_CHECK(vkCreatePipelineLayout(context.m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout)))
    {
        m_log = "vkCreatePipelineLayout failed";
        return false;
    }
    context.setObjectName(reinterpret_cast<std::uint64_t>(m_pipelineLayout), VK_OBJECT_TYPE_PIPELINE_LAYOUT, m_debugName);
    return true;
}

const SGCore::ShaderReflection::DescriptorBinding* SGCore::VulkanShaderProgram::findBinding(std::uint32_t set, std::uint32_t binding) const noexcept
{
    return m_reflection.findBinding(set, binding);
}

bool SGCore::VulkanShaderProgram::hasStage(SGSLESubShaderType type) const noexcept
{
    return std::any_of(m_stages.begin(), m_stages.end(), [type](const StageModule& stage) { return stage.m_type == type; });
}
