//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IShaderProgram.h"
#include "VulkanContext.h"

namespace SGCore
{
    class VulkanDevice;

    /// SPIR-V modules of every stage plus the descriptor set layouts and the pipeline layout derived
    /// from reflection. Stages without SPIR-V are compiled from their (vulkanized) GLSL here.
    class SGCORE_EXPORT VulkanShaderProgram final : public IShaderProgram
    {
    public:
        struct StageModule
        {
            SGSLESubShaderType m_type = SGSLESubShaderType::SST_NONE;
            VkShaderModule m_module = VK_NULL_HANDLE;
        };

        VulkanShaderProgram(VulkanDevice& device, const ShaderProgramDesc& desc) noexcept;
        ~VulkanShaderProgram() override;

        [[nodiscard]] bool isValid() const noexcept override { return m_valid; }

        [[nodiscard]] const std::vector<StageModule>& getStages() const noexcept { return m_stages; }
        [[nodiscard]] VkPipelineLayout getPipelineLayout() const noexcept { return m_pipelineLayout; }
        [[nodiscard]] const std::vector<VkDescriptorSetLayout>& getSetLayouts() const noexcept { return m_setLayouts; }
        /// Descriptor type of (set, binding) as declared by the shaders; nullptr when absent.
        [[nodiscard]] const ShaderReflection::DescriptorBinding* findBinding(std::uint32_t set, std::uint32_t binding) const noexcept;
        [[nodiscard]] bool hasStage(SGSLESubShaderType type) const noexcept;

        static VkShaderStageFlagBits stageToVk(SGSLESubShaderType type) noexcept;
        static VkShaderStageFlags stageMaskToVk(shader_stages_mask_t mask) noexcept;
        static VkDescriptorType descriptorTypeToVk(ShaderDescriptorType type) noexcept;

        /// Destroys the Vulkan objects; the program becomes an empty shell. Registered in the
        /// context so a program still referenced at shutdown releases them before the device dies.
        void releaseGPU() noexcept;

    private:
        bool buildLayouts() noexcept;

        /// Held by value, not as VulkanDevice&: shader assets outlive the renderer and are destroyed
        /// in static destruction, where a reference to the device would already dangle.
        std::shared_ptr<VulkanContext> m_context;
        std::vector<StageModule> m_stages;
        std::vector<VkDescriptorSetLayout> m_setLayouts;
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
        bool m_valid { };
    };
}
