//
// Created by stuka on 11.09.2026.
//

#pragma once

#include <array>
#include <expected>
#include <memory>
#include <utility>

#include <vulkan/vulkan.h>

#include "vulkan_buffer.h"
#include "pixelfield/rhi/logical_device.h"
#include "pixelfield/rhi/types.h"

namespace pf::rhi
{
    struct VulkanPhysicalDevice;

    struct VulkanLogicalDevice final : ILogicalDevice
    {
        friend struct VulkanContext;
        friend struct VulkanSwapchain;
        friend struct VulkanImage;
        friend struct VulkanImageView;
        friend struct VulkanShader;
        friend struct VulkanPipelineState;
        friend struct VulkanFramebuffer;
        friend struct VulkanCommandBuffer;
        friend struct VulkanPass;
        friend struct VulkanBuffer;
        friend struct VulkanDescriptorSet;
        friend struct VulkanSampler;

        VulkanLogicalDevice() noexcept = default;
        VulkanLogicalDevice(const VulkanLogicalDevice&) = delete;
        VulkanLogicalDevice(VulkanLogicalDevice&& other) noexcept;
        ~VulkanLogicalDevice() override;

        [[nodiscard]] bool valid() const noexcept override;
        [[nodiscard]] std::error_code submit(const SubmitInfo& submit_info) noexcept override;
        [[nodiscard]] std::error_code waitIdle() noexcept override;

        [[nodiscard]] std::expected<std::unique_ptr<ISwapchain>, std::error_code> createSwapchain(const SwapchainCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<IImage>, std::error_code> createImage(const ImageCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<IImageView>, std::error_code> createImageView(const ImageViewCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<ISampler>, std::error_code> createSampler(const SamplerCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<IShader>, std::error_code> createShader(const ShaderCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<IPass>, std::error_code> createPass(const PassCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<IPipelineState>, std::error_code> createPipelineState(const PipelineStateCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<IFramebuffer>, std::error_code> createFramebuffer(const FramebufferCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<ICommandBuffer>, std::error_code> createCommandBuffer(const CommandBufferCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<IBuffer>, std::error_code> createBuffer(const BufferCreateInfo& create_info) noexcept override;
        [[nodiscard]] std::expected<std::unique_ptr<IDescriptorSet>, std::error_code> createDescriptorSet(const DescriptorSetCreateInfo& create_info) noexcept override;

        VulkanLogicalDevice& operator=(const VulkanLogicalDevice&) = delete;
        VulkanLogicalDevice& operator=(VulkanLogicalDevice&& other) noexcept;

    private:
        const VulkanContext* context_ {};
        const VulkanPhysicalDevice* physical_device_ {};

        VkDevice handle_ = VK_NULL_HANDLE;

        std::array<VkQueue, std::to_underlying(QueueFamilyType::kCount)> queues_ {};
        std::array<VkCommandPool, std::to_underlying(QueueFamilyType::kCount)> command_pools_ {};
        VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;

        VulkanBuffer staging_buffer_;
        VulkanCommandBuffer staging_command_buffer_;
        size_t staging_buffer_min_size_ = 1024;

        /**
         * Get or create command commandPool.
         * @param queue_type Type queue.
         * @return New or old pool.
         */
        [[nodiscard]] std::expected<VkCommandPool, std::error_code> commandPool(QueueFamilyType queue_type) noexcept;
        // todo: maybe allow user to create his own pools
        [[nodiscard]] std::expected<VkDescriptorPool, std::error_code> descriptorPool() noexcept;

        [[nodiscard]] std::error_code assureStagingBuffer(size_t size) noexcept;
        [[nodiscard]] std::error_code assureStagingCommandBuffer() noexcept;

        [[nodiscard]] std::error_code updateBuffer(void* data, VulkanBuffer& dst_buffer, size_t dst_offset, size_t size) noexcept;
        [[nodiscard]] std::error_code updateImage(IImage& dst_image, const ImageUpdateInfo& update_info) noexcept;

        void destroy() noexcept override;
        [[nodiscard]] std::error_code create(const LogicalDeviceCreateInfo& create_info) noexcept override;
    };
}
