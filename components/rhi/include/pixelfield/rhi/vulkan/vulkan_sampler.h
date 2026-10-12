//
// Created by stuka on 11.10.2026.
//

#pragma once

#include <vulkan/vulkan_core.h>

#include "pixelfield/rhi/sampler.h"

namespace pf::rhi
{
    struct VulkanContext;

    struct VulkanSampler final : ISampler
    {
        friend struct VulkanLogicalDevice;
        friend struct VulkanDescriptorSet;

        VulkanSampler() noexcept = default;
        VulkanSampler(const VulkanSampler&) = delete;
        VulkanSampler(VulkanSampler&& other) noexcept;
        ~VulkanSampler() noexcept override;

        VulkanSampler& operator=(const VulkanSampler&) = delete;
        VulkanSampler& operator=(VulkanSampler&& other) noexcept;

    private:
        const VulkanContext* context_ {};
        const VulkanLogicalDevice* logical_device_ {};

        VkSampler handle_ = VK_NULL_HANDLE;

        void destroy() noexcept override;
        [[nodiscard]] std::error_code create(const SamplerCreateInfo& create_info) override;
    };
}
