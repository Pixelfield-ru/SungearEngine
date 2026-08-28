//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/ICommandList.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanPipelineState.h"
#include "VulkanTexture.h"

namespace SGCore
{
    class VulkanDescriptorSet;
    class VulkanGPUBuffer;

    /// Records into a primary command buffer; uploads go into a second one that the device submits
    /// first, so uploadData() works inside a render pass exactly as on GL. Pipeline, descriptor sets,
    /// viewport and scissor are resolved lazily at the first draw (flushState()) because the
    /// VkPipeline variant depends on the attachment formats of the active pass.
    class SGCORE_EXPORT VulkanCommandList final : public ICommandList
    {
    public:
        static constexpr std::uint32_t max_descriptor_sets = 4;

        explicit VulkanCommandList(VulkanDevice& device) noexcept;
        ~VulkanCommandList() override;

        // ---- ICommandList
        void begin() noexcept override;
        void end() noexcept override;
        void beginRenderPass(const RenderPassBeginDesc& desc) noexcept override;
        void endRenderPass() noexcept override;
        void bindPipeline(const Ref<IPipelineState>& pipeline) noexcept override;
        void bindDescriptorSet(std::uint32_t setIndex, const Ref<IDescriptorSet>& set) noexcept override;
        void pushConstants(const void* data, std::uint32_t size, std::uint32_t offset = 0) noexcept override;
        void bindVertexBuffer(std::uint32_t slot, const Ref<IGPUBuffer>& buffer, std::uint64_t offset = 0) noexcept override;
        void bindIndexBuffer(const Ref<IGPUBuffer>& buffer, SGIndexType indexType, std::uint64_t offset = 0) noexcept override;
        void setViewport(const Viewport& viewport) noexcept override;
        void setScissor(const Scissor& scissor) noexcept override;
        void draw(std::uint32_t vertexCount, std::uint32_t instanceCount = 1,
                  std::uint32_t firstVertex = 0, std::uint32_t firstInstance = 0) noexcept override;
        void drawIndexed(std::uint32_t indexCount, std::uint32_t instanceCount = 1,
                         std::uint32_t firstIndex = 0, std::int32_t vertexOffset = 0,
                         std::uint32_t firstInstance = 0) noexcept override;
        void clearColorAttachment(std::uint32_t colorIndex, const glm::vec4& color) noexcept override;
        void clearDepthStencil(float depth = 1.0f, bool clearStencil = false, std::uint32_t stencil = 0) noexcept override;
        void transition(const Ref<IGPUObject>& resource, GPUResourceState newState) noexcept override;
        void uploadData(const Ref<IGPUBuffer>& dst, const void* data, std::uint64_t size, std::uint64_t offset = 0) noexcept override;

        // ---- backend
        /// Moves the recorded work out for VulkanDevice::submit(). Empty when nothing was recorded.
        [[nodiscard]] VulkanSubmission takeSubmission() noexcept;
        [[nodiscard]] bool isRecording() const noexcept { return m_recording; }
        [[nodiscard]] bool hasRecordedWork() const noexcept { return m_ended && !m_submitted; }
        [[nodiscard]] bool touchedSwapchain() const noexcept { return m_touchedSwapchain; }
        [[nodiscard]] VkCommandBuffer getCommandBuffer() const noexcept { return m_commandBuffer; }
        void markSubmitted() noexcept { m_submitted = true; }

    private:
        struct SetSlot
        {
            Ref<VulkanDescriptorSet> m_set;
            std::uint64_t m_materializedVersion = ~0ull;
            VkDescriptorSet m_vkSet = VK_NULL_HANDLE;
            VkPipelineLayout m_materializedLayout = VK_NULL_HANDLE;
        };
        struct PendingPushConstants
        {
            std::vector<std::uint8_t> m_data;
            std::uint32_t m_offset { };
        };

        void resetState() noexcept;
        /// Submits work recorded but not yet handed to the device (see begin()).
        void flushRecordedWork() noexcept;
        /// Binds pipeline / sets / viewport / scissor / push constants that changed since the last draw.
        bool flushState() noexcept;
        [[nodiscard]] VkDescriptorSet materializeSet(SetSlot& slot, std::uint32_t setIndex) noexcept;
        /// True when the texture is a color or depth attachment of the pass being recorded.
        [[nodiscard]] bool isPassAttachment(const Ref<VulkanTexture>& texture) const noexcept;
        void applyViewportScissor() noexcept;
        void endUploadRecording() noexcept;

        VulkanDevice& m_device;
        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
        VkCommandBuffer m_uploadCommandBuffer = VK_NULL_HANDLE;
        bool m_recording { };
        bool m_ended { };
        bool m_submitted = true;
        bool m_uploadUsed { };
        VulkanSubmission m_submission;

        // active render pass
        bool m_inPass { };
        bool m_targetIsSwapchain { };
        bool m_touchedSwapchain { };
        VulkanPassFormats m_passFormats;
        VkExtent2D m_passExtent { };
        std::vector<Ref<VulkanTexture>> m_passColorTextures;
        Ref<VulkanTexture> m_passDepthTexture;

        // deferred state
        Ref<VulkanPipelineState> m_pipeline;
        VkPipeline m_boundPipeline = VK_NULL_HANDLE;
        VkPipelineLayout m_boundLayout = VK_NULL_HANDLE;
        std::array<SetSlot, max_descriptor_sets> m_sets;
        Viewport m_viewport { };
        bool m_viewportExplicit { };
        Scissor m_scissor { };
        bool m_scissorExplicit { };
        bool m_viewportDirty = true;
        std::vector<PendingPushConstants> m_pendingPushConstants;
    };
}
