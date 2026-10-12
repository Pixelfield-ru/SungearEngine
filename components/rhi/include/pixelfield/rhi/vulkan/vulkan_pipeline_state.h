//
// Created by stuka on 18.09.2026.
//

#pragma once

#include <system_error>
#include <vulkan/vulkan_core.h>

#include "pixelfield/rhi/pipeline_state.h"

namespace pf::rhi
{
    struct VulkanContext;
    struct VulkanPass;

    struct VulkanPipelineState final : IPipelineState
    {
        friend struct VulkanLogicalDevice;
        friend struct VulkanCommandBuffer;
        
        VulkanPipelineState() noexcept = default;
        VulkanPipelineState(const VulkanPipelineState&) = delete;
        VulkanPipelineState(VulkanPipelineState&& other) noexcept;
        ~VulkanPipelineState() noexcept override;

        VulkanPipelineState& operator=(const VulkanPipelineState&) = delete;
        VulkanPipelineState& operator=(VulkanPipelineState&& other) noexcept;

    private:
        const VulkanContext* context_ {};
        const VulkanLogicalDevice* logical_device_ {};
        const VulkanPass* pass_ {};

        VkPipelineLayout pipeline_layout_ {};
        VkPipeline graphics_pipeline_ {};

        void destroy() noexcept override;
        [[nodiscard]] std::error_code create(const PipelineStateCreateInfo& create_info) noexcept override;
    };
}
