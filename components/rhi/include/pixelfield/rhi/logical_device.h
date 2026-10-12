//
// Created by stuka on 10.09.2026.
//

#pragma once

#include <span>

#include "buffer.h"
#include "command_buffer.h"
#include "descriptor_set.h"
#include "framebuffer.h"
#include "image.h"
#include "image_view.h"
#include "pass.h"
#include "pipeline_state.h"
#include "sampler.h"
#include "shader.h"
#include "swapchain.h"

namespace pf::rhi
{
    struct IContext;
    struct IPhysicalDevice;

    struct LogicalDeviceCreateInfo final
    {
        const IContext* context {};
        const IPhysicalDevice* physical_device {};
    };

    struct SubmitInfo final
    {
        ISwapchain* swapchain {};
        std::span<const ICommandBuffer*> command_buffers;
        QueueFamilyType queue_family = QueueFamilyType::kGraphics;
        const AcquiredFrame* wait_for_frame {};
    };

    struct ILogicalDevice
    {
        virtual ~ILogicalDevice() = default;

        [[nodiscard]] std::error_code recreate(const LogicalDeviceCreateInfo& create_info) noexcept
        {
            destroy();
            return create(create_info);
        }

        [[nodiscard]] virtual bool valid() const noexcept = 0;
        [[nodiscard]] virtual std::error_code submit(const SubmitInfo& submit_info) = 0;
        [[nodiscard]] virtual std::error_code waitIdle() = 0;

        [[nodiscard]] virtual std::expected<std::unique_ptr<ISwapchain>, std::error_code> createSwapchain(const SwapchainCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<IImage>, std::error_code> createImage(const ImageCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<IImageView>, std::error_code> createImageView(const ImageViewCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<ISampler>, std::error_code> createSampler(const SamplerCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<IShader>, std::error_code> createShader(const ShaderCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<IPass>, std::error_code> createPass(const PassCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<IPipelineState>, std::error_code> createPipelineState(const PipelineStateCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<IFramebuffer>, std::error_code> createFramebuffer(const FramebufferCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<ICommandBuffer>, std::error_code> createCommandBuffer(const CommandBufferCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<IBuffer>, std::error_code> createBuffer(const BufferCreateInfo& create_info) noexcept = 0;
        [[nodiscard]] virtual std::expected<std::unique_ptr<IDescriptorSet>, std::error_code> createDescriptorSet(const DescriptorSetCreateInfo& create_info) noexcept = 0;

    private:
        virtual void destroy() = 0;
        [[nodiscard]] virtual std::error_code create(const LogicalDeviceCreateInfo& create_info) = 0;
    };
}