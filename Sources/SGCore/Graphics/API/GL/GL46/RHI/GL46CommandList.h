//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <glad/glad.h>

#include "GL46PipelineState.h"
#include "SGCore/Graphics/RHI/ICommandList.h"

namespace SGCore
{
    class GL4Renderer;

    /// Immediate-mode command list: every call is executed on the GL context right away.
    /// Fixed-function state goes through the renderer's cached useState/useMeshRenderState so the
    /// legacy passes and the RHI path share one notion of "current state" during the migration.
    class GL46CommandList final : public ICommandList
    {
    public:
        explicit GL46CommandList(GL4Renderer& renderer) noexcept;

        void begin() noexcept override;
        void end() noexcept override;

        void beginRenderPass(const RenderPassBeginDesc& desc) noexcept override;
        void endRenderPass() noexcept override;

        void bindPipeline(const Ref<IPipelineState>& pipeline) noexcept override;
        void bindDescriptorSet(std::uint32_t setIndex, const Ref<IDescriptorSet>& set) noexcept override;
        void pushConstants(const void* data, std::uint32_t size, std::uint32_t offset) noexcept override;

        void bindVertexBuffer(std::uint32_t slot, const Ref<IGPUBuffer>& buffer, std::uint64_t offset) noexcept override;
        void bindIndexBuffer(const Ref<IGPUBuffer>& buffer, SGIndexType indexType, std::uint64_t offset) noexcept override;

        void setViewport(const Viewport& viewport) noexcept override;
        void setScissor(const Scissor& scissor) noexcept override;

        void draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance) noexcept override;
        void drawIndexed(std::uint32_t indexCount, std::uint32_t instanceCount, std::uint32_t firstIndex, std::int32_t vertexOffset, std::uint32_t firstInstance) noexcept override;

        void clearColorAttachment(std::uint32_t colorIndex, const glm::vec4& color) noexcept override;
        void clearDepthStencil(float depth, bool clearStencil, std::uint32_t stencil) noexcept override;

        void transition(const Ref<IGPUObject>& resource, GPUResourceState newState) noexcept override;
        void uploadData(const Ref<IGPUBuffer>& dst, const void* data, std::uint64_t size, std::uint64_t offset) noexcept override;

    private:
        GL4Renderer& m_renderer;
        Ref<GL46PipelineState> m_pipeline;
        GLenum m_indexType = GL_UNSIGNED_INT;
        std::uint64_t m_indexOffset { };
        std::uint32_t m_indexSize = 4;
        bool m_pushConstantsWarned { };
    };
}
