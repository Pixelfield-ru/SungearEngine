//
// Created by stuka on 16.09.2026.
//

#pragma once

#include <vulkan/vulkan.h>

#include "pixelfield/rhi/shader.h"
#include "vulkan_errors.h"

namespace pf::rhi
{
    struct VulkanContext;

    struct VulkanShader final : IShader
    {
        friend struct VulkanLogicalDevice;
        friend struct VulkanPipelineState;

        VulkanShader() noexcept = default;
        VulkanShader(const VulkanShader&) = delete;
        VulkanShader(VulkanShader&& other) noexcept;
        ~VulkanShader() noexcept override;

        [[nodiscard]] bool valid() const noexcept override;

        VulkanShader& operator=(const VulkanShader&) = delete;
        VulkanShader& operator=(VulkanShader&&) noexcept;

    private:
        struct Stage final
        {
            VkShaderModule module = VK_NULL_HANDLE;
            ShaderStage stage_info;
            VkPipelineShaderStageCreateInfo pipeline_stage_info {};
        };

        const VulkanContext* context_ {};
        const VulkanLogicalDevice* logical_device_ {};

        std::vector<Stage> stages_;

        void destroy() noexcept override;
        [[nodiscard]] std::error_code create(const ShaderCreateInfo& create_info) noexcept override;
    };
}
