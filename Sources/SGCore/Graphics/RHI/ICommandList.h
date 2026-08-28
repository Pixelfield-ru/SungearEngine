//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <sgcore_export.h>

#include "IDescriptorSet.h"
#include "IGPUBuffer.h"
#include "IGPUObject.h"
#include "IPipelineState.h"
#include "RHITypes.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class ITexture2D;

    /**
     * Records GPU work. Explicit backends record into a real command buffer submitted by
     * IGPUQueue; the GL backend executes every call immediately (GL is an immediate-mode API,
     * emulating deferral there would be pointless) — the interface allows both.
     */
    class SGCORE_EXPORT ICommandList : public IGPUObject
    {
    public:
        virtual void begin() noexcept = 0;
        virtual void end() noexcept = 0;

        virtual void beginRenderPass(const RenderPassBeginDesc& desc) noexcept = 0;
        virtual void endRenderPass() noexcept = 0;

        virtual void bindPipeline(const Ref<IPipelineState>& pipeline) noexcept = 0;
        virtual void bindDescriptorSet(std::uint32_t setIndex, const Ref<IDescriptorSet>& set) noexcept = 0;
        virtual void pushConstants(const void* data, std::uint32_t size, std::uint32_t offset = 0) noexcept = 0;

        virtual void bindVertexBuffer(std::uint32_t slot, const Ref<IGPUBuffer>& buffer, std::uint64_t offset = 0) noexcept = 0;
        virtual void bindIndexBuffer(const Ref<IGPUBuffer>& buffer, SGIndexType indexType, std::uint64_t offset = 0) noexcept = 0;

        virtual void setViewport(const Viewport& viewport) noexcept = 0;
        virtual void setScissor(const Scissor& scissor) noexcept = 0;

        virtual void draw(std::uint32_t vertexCount, std::uint32_t instanceCount = 1,
                          std::uint32_t firstVertex = 0, std::uint32_t firstInstance = 0) noexcept = 0;
        virtual void drawIndexed(std::uint32_t indexCount, std::uint32_t instanceCount = 1,
                                 std::uint32_t firstIndex = 0, std::int32_t vertexOffset = 0,
                                 std::uint32_t firstInstance = 0) noexcept = 0;

        /// Clears inside a render pass (Vulkan: vkCmdClearAttachments; DX12: Clear*View).
        /// p colorIndex is the index in RenderPassBeginDesc::m_colorAttachments / the framebuffer's color attachment index.
        virtual void clearColorAttachment(std::uint32_t colorIndex, const glm::vec4& color) noexcept = 0;
        virtual void clearDepthStencil(float depth = 1.0f, bool clearStencil = false, std::uint32_t stencil = 0) noexcept = 0;

        /// Explicit resource state transition; a no-op on GL (see DeviceProperties::m_supportsExplicitBarriers).
        virtual void transition(const Ref<IGPUObject>& resource, GPUResourceState newState) noexcept = 0;

        /// True while commands can be recorded into this list. Immediate backends are always ready.
        [[nodiscard]] virtual bool isRecording() const noexcept { return true; }

        /// Copies data into a device-local buffer (staging is the backend's business).
        virtual void uploadData(const Ref<IGPUBuffer>& dst, const void* data, std::uint64_t size, std::uint64_t offset = 0) noexcept = 0;
    };
}
