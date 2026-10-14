//
// Created by stuka on 23.09.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_command_buffer.h"

#include "pixelfield/rhi/vulkan/vulkan_buffer.h"
#include "pixelfield/rhi/vulkan/vulkan_context.h"
#include "pixelfield/rhi/vulkan/vulkan_descriptor_set.h"
#include "pixelfield/rhi/vulkan/vulkan_errors.h"
#include "pixelfield/rhi/vulkan/vulkan_framebuffer.h"
#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_pass.h"
#include "pixelfield/rhi/vulkan/vulkan_pipeline_state.h"
#include "pixelfield/rhi/vulkan/vulkan_types.h"

pf::rhi::VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept :
    logical_device_(std::exchange(other.logical_device_, nullptr)),
    pool_(std::exchange(other.pool_, VK_NULL_HANDLE)),
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE))
{
}

pf::rhi::VulkanCommandBuffer::~VulkanCommandBuffer() noexcept
{
    destroy();
}

std::error_code pf::rhi::VulkanCommandBuffer::begin() noexcept
{
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0; // Optional
    begin_info.pInheritanceInfo = nullptr; // Optional

    if(vkBeginCommandBuffer(handle_, &begin_info) != VK_SUCCESS)
    {
        return VulkanError::kCannotBeginCommandBuffer;
    }

    return {};
}

std::error_code pf::rhi::VulkanCommandBuffer::beginRendering(const BeginRenderingInfo& info) noexcept
{
    const auto* vulkan_pipeline_state = static_cast<const VulkanPipelineState*>(info.pipeline_state);
    const auto* vulkan_framebuffer = static_cast<const VulkanFramebuffer*>(info.framebuffer);

    VkRenderPassBeginInfo render_pass_begin_info{};
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = vulkan_pipeline_state->pass_->handle_;
    render_pass_begin_info.framebuffer = vulkan_framebuffer->handle_;
    render_pass_begin_info.renderArea.offset = { info.render_area.x, info.render_area.y };
    render_pass_begin_info.renderArea.extent = { (std::uint32_t) info.render_area.width, (std::uint32_t) info.render_area.height };

    std::vector<VkClearValue> vulkan_clear(info.clear_infos.size());
    for(size_t i = 0; i < info.clear_infos.size(); ++i)
    {
        const auto& clear_info = info.clear_infos[i];
        if(clear_info.value.index() == 0)
        {
            const auto& value = std::get<glm::vec4>(clear_info.value);
            vulkan_clear[i].color = { value.x, value.y, value.z, value.w };
        }
        else if(clear_info.value.index() == 1)
        {
            const auto& value = std::get<DepthStencilClearInfo>(clear_info.value);
            vulkan_clear[i].depthStencil.depth = value.depth;
            vulkan_clear[i].depthStencil.stencil = value.stencil;
        }
        else
        {
            return VulkanError::kInvalidArgument;
        }
    }

    render_pass_begin_info.clearValueCount = vulkan_clear.size();
    render_pass_begin_info.pClearValues = vulkan_clear.data();

    vkCmdBeginRenderPass(handle_, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(handle_, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_pipeline_state->graphics_pipeline_);

    return {};
}

void pf::rhi::VulkanCommandBuffer::bindVertexBuffer(const IBuffer& buffer) noexcept
{
    VkBuffer vertex_buffers[] { static_cast<const VulkanBuffer&>(buffer).handle_ };
    VkDeviceSize offsets[] { 0 };
    vkCmdBindVertexBuffers(handle_, 0, 1, vertex_buffers, offsets);
}

void pf::rhi::VulkanCommandBuffer::bindIndexBuffer(const IBuffer& buffer, IndexType index_type) noexcept
{
    const VkBuffer index_buffer = static_cast<const VulkanBuffer&>(buffer).handle_;
    constexpr VkDeviceSize offset = 0;
    vkCmdBindIndexBuffer(handle_, index_buffer, offset, toVkIndexType(index_type));
}

void pf::rhi::VulkanCommandBuffer::bindDescriptorSet(const IDescriptorSet& set, const IPipelineState& pipeline_state) noexcept
{
    const auto& vulkan_descriptor_set = static_cast<const VulkanDescriptorSet&>(set);
    const auto& vulkan_pipeline_state = static_cast<const VulkanPipelineState&>(pipeline_state);

    vkCmdBindDescriptorSets(handle_,
                            toVkPipelineBindPoint(pipeline_state.bindPoint()),
                            vulkan_pipeline_state.pipeline_layout_,
                            0,
                            1,
                            &vulkan_descriptor_set.handle_,
                            0,
                            nullptr);
}

void pf::rhi::VulkanCommandBuffer::draw(std::size_t vertex_count, std::size_t instance_count,
                                    std::size_t first_vertex, std::size_t first_instance) noexcept
{
    vkCmdDraw(handle_, vertex_count, instance_count, first_vertex, first_instance);
}

void pf::rhi::VulkanCommandBuffer::drawIndexed(std::size_t index_count, std::size_t instance_count, std::size_t first_index,
                                           std::int64_t vertex_offset, std::size_t first_instance)
{
    vkCmdDrawIndexed(handle_, index_count, instance_count, first_index, vertex_offset, first_instance);
}

void pf::rhi::VulkanCommandBuffer::setViewport(const ViewportInfo& viewport) noexcept
{
    VkViewport vulkan_viewport{};
    vulkan_viewport.x = viewport.rect.x;
    vulkan_viewport.y = viewport.rect.y;
    vulkan_viewport.width = viewport.rect.width;
    vulkan_viewport.height = viewport.rect.height;
    vulkan_viewport.minDepth = 0.0f;
    vulkan_viewport.maxDepth = 1.0f;

    vkCmdSetViewport(handle_, 0, 1, &vulkan_viewport);
}

void pf::rhi::VulkanCommandBuffer::setScissor(const Rect<std::int32_t>& scissor) noexcept
{
    VkRect2D vulkan_scissor{};
    vulkan_scissor.offset = { scissor.x, scissor.y };
    vulkan_scissor.extent = { (std::uint32_t) scissor.width, (std::uint32_t) scissor.height };

    vkCmdSetScissor(handle_, 0, 1, &vulkan_scissor);
}

void pf::rhi::VulkanCommandBuffer::copyBuffer(const IBuffer& src, IBuffer& dst, size_t src_offset, size_t dst_offset,
                                          size_t size) noexcept
{
    const auto& vulkan_src_buffer = static_cast<const VulkanBuffer&>(src);
    auto& vulkan_dst_buffer = static_cast<VulkanBuffer&>(dst);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = src_offset;
    copyRegion.dstOffset = dst_offset;
    copyRegion.size = size;
    vkCmdCopyBuffer(handle_, vulkan_src_buffer.handle_, vulkan_dst_buffer.handle_, 1, &copyRegion);
}

std::error_code pf::rhi::VulkanCommandBuffer::endRendering() noexcept
{
    vkCmdEndRenderPass(handle_);

    return {};
}

std::error_code pf::rhi::VulkanCommandBuffer::end() noexcept
{
    if(vkEndCommandBuffer(handle_) != VK_SUCCESS)
    {
        return VulkanError::kCannotEndCommandBuffer;
    }

    return {};
}

bool pf::rhi::VulkanCommandBuffer::valid() const noexcept
{
    return handle_ != VK_NULL_HANDLE;
}

pf::rhi::VulkanCommandBuffer& pf::rhi::VulkanCommandBuffer::operator=(VulkanCommandBuffer&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    destroy();

    logical_device_ = std::exchange(other.logical_device_, nullptr);
    pool_ = std::exchange(other.pool_, VK_NULL_HANDLE);
    handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);

    return *this;
}

void pf::rhi::VulkanCommandBuffer::destroy() noexcept
{
    if(handle_)
    {
        vkFreeCommandBuffers(logical_device_->handle_, pool_, 1, &handle_);
        handle_ = VK_NULL_HANDLE;
    }
}

std::error_code pf::rhi::VulkanCommandBuffer::create(const CommandBufferCreateInfo& create_info) noexcept
{
    const auto* vulkan_context = static_cast<const VulkanContext*>(create_info.context);
    auto* vulkan_logical_device = static_cast<VulkanLogicalDevice*>(create_info.logical_device);

    const auto pool = vulkan_logical_device->commandPool(create_info.queue_family);
    if(!pool)
    {
        return pool.error();
    }

    logical_device_ = vulkan_logical_device;
    pool_ = *pool;

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = *pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    if(vkAllocateCommandBuffers(vulkan_logical_device->handle_, &alloc_info, &handle_) != VK_SUCCESS)
    {
        return VulkanError::kCannotAllocateCommandBuffer;
    }

    return {};
}
