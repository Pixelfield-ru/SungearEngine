//
// Created by stuka on 06.10.2026.
//

#pragma once

#include <vulkan/vulkan.h>

#include "pixelfield/rhi/descriptor_set.h"

namespace pf::rhi
{
    struct VulkanContext;

    struct VulkanDescriptorSet final : IDescriptorSet
    {
        friend struct VulkanLogicalDevice;
        friend struct VulkanPipelineState;
        friend struct VulkanCommandBuffer;

        VulkanDescriptorSet() noexcept = default;
        VulkanDescriptorSet(const VulkanDescriptorSet&) noexcept = delete;
        VulkanDescriptorSet(VulkanDescriptorSet&& other) noexcept;
        ~VulkanDescriptorSet() noexcept override;

        void update(std::span<DescriptorInfo> descriptors) noexcept override;

        VulkanDescriptorSet& operator=(const VulkanDescriptorSet&) noexcept = delete;
        VulkanDescriptorSet& operator=(VulkanDescriptorSet&& other) noexcept;

    private:
        const VulkanContext* context_ {};
        VulkanLogicalDevice* logical_device_ {};

        /// Owner pool.
        VkDescriptorPool pool_ = VK_NULL_HANDLE;

        VkDescriptorSet handle_ = VK_NULL_HANDLE;
        VkDescriptorSetLayout layout_ = VK_NULL_HANDLE;

        void destroy() override;
        [[nodiscard]] std::error_code create(const DescriptorSetCreateInfo& create_info) override;
    };
}
