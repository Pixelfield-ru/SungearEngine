//
// Created by stuka on 23.09.2026.
//

#pragma once

#include <system_error>
#include <vector>
#include <vulkan/vulkan.h>

#include "pixelfield/rhi/command_buffer.h"

namespace pf::rhi
{
    struct VulkanContext;
    struct VulkanPipelineState;
    struct IDescriptorSet;

    struct VulkanCommandBuffer final : ICommandBuffer
    {
        friend struct VulkanLogicalDevice;

        VulkanCommandBuffer() noexcept = default;
        VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
        VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept;
        ~VulkanCommandBuffer() noexcept override;

        [[nodiscard]] std::error_code begin() noexcept override;
        [[nodiscard]] std::error_code beginRendering(const BeginRenderingInfo& info) noexcept override;

        void bindVertexBuffer(const IBuffer& buffer) noexcept override;
        void bindIndexBuffer(const IBuffer& buffer, IndexType index_type) noexcept override;
        void bindDescriptorSet(const IDescriptorSet& set, const IPipelineState& pipeline_state) noexcept override;

        void draw(std::size_t vertex_count, std::size_t instance_count,
                  std::size_t first_vertex, std::size_t first_instance) noexcept override;
        void drawIndexed(std::size_t index_count, std::size_t instance_count, std::size_t first_index,
                         std::int64_t vertex_offset, std::size_t first_instance) override;

        void setViewport(const ViewportInfo& viewport) noexcept override;
        void setScissor(const Rect<std::int32_t>& scissor) noexcept override;

        void copyBuffer(const IBuffer& src, IBuffer& dst, size_t src_offset, size_t dst_offset, size_t size) noexcept override;

        [[nodiscard]] std::error_code endRendering() noexcept override;
        [[nodiscard]] std::error_code end() noexcept override;

        [[nodiscard]] bool valid() const noexcept override;

        VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;
        VulkanCommandBuffer& operator=(VulkanCommandBuffer&& other) noexcept;

    private:
        const VulkanLogicalDevice* logical_device_ {};

        /// Owner commandPool.
        VkCommandPool pool_ {};

        VkCommandBuffer handle_ {};

        void destroy() noexcept override;
        [[nodiscard]] std::error_code create(const CommandBufferCreateInfo& create_info) noexcept override;
    };
}
