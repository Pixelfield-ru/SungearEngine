//
// Created by stuka on 23.09.2026.
//

#pragma once

#include <variant>
#include <glm/vec4.hpp>
#include <system_error>

#include "types.h"

namespace pf::rhi
{
    struct IContext;
    struct ILogicalDevice;
    struct IPipelineState;
    struct IFramebuffer;
    struct IBuffer;
    struct IDescriptorSet;

    struct CommandBufferCreateInfo final
    {
        const IContext* context {};
        ILogicalDevice* logical_device {};

        QueueFamilyType queue_family = QueueFamilyType::kGraphics;
    };

    struct DepthStencilClearInfo final
    {
        float depth {};
        std::uint32_t stencil {};
    };

    struct ClearInfo final
    {
        /// First - color, second - depth and stencil
        std::variant<glm::vec4, DepthStencilClearInfo> value;
    };

    struct BeginRenderingInfo final
    {
        const IPipelineState* pipeline_state {};
        const IFramebuffer* framebuffer {};

        Rect<std::int32_t> render_area {};
        std::vector<ClearInfo> clear_infos;
    };

    // todo: make bind descriptors set
    struct ICommandBuffer
    {
        virtual ~ICommandBuffer() noexcept = default;

        [[nodiscard]] std::error_code recreate(const CommandBufferCreateInfo& create_info) noexcept
        {
            destroy();
            return create(create_info);
        }

        [[nodiscard]] virtual std::error_code begin() = 0;
        [[nodiscard]] virtual std::error_code beginRendering(const BeginRenderingInfo& info) = 0;

        virtual void bindVertexBuffer(const IBuffer& buffer) = 0;
        virtual void bindIndexBuffer(const IBuffer& buffer, IndexType index_type) = 0;
        virtual void bindDescriptorSet(const IDescriptorSet& set, const IPipelineState& pipeline_state) = 0;

        virtual void draw(std::size_t vertex_count, std::size_t instance_count, std::size_t first_vertex, std::size_t first_instance) = 0;
        virtual void drawIndexed(std::size_t index_count, std::size_t instance_count, std::size_t first_index, std::int64_t vertex_offset, std::size_t first_instance) = 0;

        virtual void setViewport(const ViewportInfo& viewport) = 0;
        virtual void setScissor(const Rect<std::int32_t>& scissor) = 0;

        virtual void copyBuffer(const IBuffer& src, IBuffer& dst, size_t src_offset, size_t dst_offset, size_t size) = 0;

        [[nodiscard]] virtual std::error_code endRendering() = 0;
        [[nodiscard]] virtual std::error_code end() = 0;

        [[nodiscard]] virtual bool valid() const noexcept = 0;

    private:
        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const CommandBufferCreateInfo& create_info) = 0;
    };
}
