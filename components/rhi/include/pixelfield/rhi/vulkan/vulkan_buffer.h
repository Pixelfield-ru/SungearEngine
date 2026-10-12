//
// Created by stuka on 02.10.2026.
//

#pragma once

#include "vulkan_command_buffer.h"
#include "pixelfield/rhi/buffer.h"

namespace pf::rhi
{
    struct VulkanContext;

    struct VulkanBuffer final : IBuffer
    {
        friend struct VulkanLogicalDevice;
        friend struct VulkanCommandBuffer;
        friend struct VulkanDescriptorSet;

        VulkanBuffer() noexcept = default;
        VulkanBuffer(const VulkanBuffer&) = delete;
        VulkanBuffer(VulkanBuffer&& other) noexcept;
        ~VulkanBuffer() noexcept override;

        bool valid() const noexcept override;

        std::error_code update(void* data, size_t offset, size_t size) override;
        size_t size() override;

        VulkanBuffer& operator=(const VulkanBuffer&) = delete;
        VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

    private:
        const VulkanContext* context_ {};
        VulkanLogicalDevice* logical_device_ {};

        VkBuffer handle_ = VK_NULL_HANDLE;
        VkDeviceMemory memory_ = VK_NULL_HANDLE;

        size_t size_ {};

        std::error_code createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& out_buffer, VkDeviceMemory& out_memory) const noexcept;

        void destroy() override;
        [[nodiscard]] std::error_code create(const BufferCreateInfo& create_info) override;
    };
}
