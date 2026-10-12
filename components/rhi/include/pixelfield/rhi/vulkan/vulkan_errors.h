//
// Created by stuka on 10.09.2026.
//

#pragma once

#include <system_error>

namespace pf::rhi
{
    enum class VulkanError
    {
        kUnsupportedValidationLayer = 1,
        kCannotCreateInstance,
        kCannotSetupDebug,
        kCannotCreateSurface,
        kCannotCreateSemaphore,
        kCannotCreateFence,
        kCannotCreateDevice,
        kCannotCreateSwapchain,
        kCannotCreateImage,
        kCannotCreateImageView,
        kCannotCreateShaderModule,
        kCannotCreatePipelineLayout,
        kCannotCreateRenderPass,
        kCannotCreatePipeline,
        kCannotCreateFramebuffer,
        kCannotCreateCommandPool,
        kCannotCreateBuffer,
        kCannotCreateDescriptorPool,
        kCannotCreateDescriptorSetLayout,
        kCannotCreateDescriptorSet,
        kCannotCreateSampler,
        kCannotAllocateCommandBuffer,
        kCannotAllocateMemory,
        kCannotBindMemory,
        kCannotMapMemory,
        kCannotBeginCommandBuffer,
        kCannotEndCommandBuffer,
        kCannotSubmit,
        kCannotAcquireNextImage,
        kCannotPresentFrame,
        kCannotWaitDeviceIdle,
        kInvalidDevice,
        kInvalidContext,
        kInvalidShader,
        kInvalidPipelineState,
        kInvalidImageView,
        kInvalidImage,
        kInvalidFramebuffer,
        kInvalidArgument,
        kIncompatibleDevice,
        kUnexpectedError
    };

    struct VulkanErrorCategory final : public std::error_category
    {
        [[nodiscard]] const char* name() const noexcept override { return "RHI (Vulkan)"; }

        [[nodiscard]] std::string message(int ev) const override
        {
            switch(static_cast<VulkanError>(ev))
            {
                case VulkanError::kUnsupportedValidationLayer: return "Some of validation layers are not supported.";
                case VulkanError::kCannotCreateInstance: return "Failed to create Vulkan instance.";
                case VulkanError::kCannotSetupDebug: return "Failed to create setup Vulkan debug messenger.";
                case VulkanError::kCannotCreateSurface: return "Failed to create Vulkan surface.";
                case VulkanError::kCannotCreateSemaphore: return "Failed to create Vulkan semaphore.";
                case VulkanError::kCannotCreateFence: return "Failed to create Vulkan fence.";
                case VulkanError::kCannotCreateDevice: return "Failed to create Vulkan device.";
                case VulkanError::kCannotCreateSwapchain: return "Failed to create Vulkan swapchain.";
                case VulkanError::kCannotCreateImage: return "Failed to create Vulkan image.";
                case VulkanError::kCannotCreateImageView: return "Failed to create Vulkan image view.";
                case VulkanError::kCannotCreateShaderModule: return "Failed to create Vulkan shader module.";
                case VulkanError::kCannotCreatePipelineLayout: return "Failed to create Vulkan pipeline layout.";
                case VulkanError::kCannotCreateRenderPass: return "Failed to create Vulkan render pass.";
                case VulkanError::kCannotCreatePipeline: return "Failed to create Vulkan pipeline.";
                case VulkanError::kCannotCreateFramebuffer: return "Failed to create Vulkan framebuffer.";
                case VulkanError::kCannotCreateCommandPool: return "Failed to create Vulkan command pool.";
                case VulkanError::kCannotCreateBuffer: return "Failed to create Vulkan buffer.";
                case VulkanError::kCannotCreateDescriptorPool: return "Failed to create Vulkan descriptor pool.";
                case VulkanError::kCannotCreateDescriptorSetLayout: return "Failed to create Vulkan descriptor set layout.";
                case VulkanError::kCannotCreateDescriptorSet: return "Failed to create Vulkan descriptor set.";
                case VulkanError::kCannotCreateSampler: return "Failed to create Vulkan sampler.";
                case VulkanError::kCannotAllocateCommandBuffer: return "Failed to allocate Vulkan command buffer.";
                case VulkanError::kCannotAllocateMemory: return "Failed to allocate GPU memory using Vulkan.";
                case VulkanError::kCannotBindMemory: return "Failed to bind GPU memory using Vulkan.";
                case VulkanError::kCannotMapMemory: return "Failed to map device memory using Vulkan.";
                case VulkanError::kCannotBeginCommandBuffer: return "Failed to begin Vulkan command buffer.";
                case VulkanError::kCannotEndCommandBuffer: return "Failed to end Vulkan command buffer.";
                case VulkanError::kCannotSubmit: return "Failed to submit command buffers to Vulkan queue.";
                case VulkanError::kCannotAcquireNextImage: return "Failed to acquire next image in Vulkan swapchain.";
                case VulkanError::kCannotPresentFrame: return "Failed to present frame in Vulkan swapchain.";
                case VulkanError::kCannotWaitDeviceIdle: return "Failed to wait Vulkan device idle.";
                case VulkanError::kInvalidDevice: return "Invalid Vulkan device.";
                case VulkanError::kInvalidContext: return "Invalid Vulkan context.";
                case VulkanError::kInvalidShader: return "Invalid Vulkan shader.";
                case VulkanError::kInvalidPipelineState: return "Invalid Vulkan pipeline state.";
                case VulkanError::kInvalidImageView: return "Invalid Vulkan image view.";
                case VulkanError::kInvalidImage: return "Invalid Vulkan image.";
                case VulkanError::kInvalidFramebuffer: return "Invalid Vulkan framebuffer.";
                case VulkanError::kInvalidArgument: return "Invalid argument.";
                case VulkanError::kIncompatibleDevice: return "Incompatible device.";
                case VulkanError::kUnexpectedError: return "Unexpected Vulkan error.";
                default: return "Unknown error";
            }
        }
    };

    inline const std::error_category& vulkan_error_category()
    {
        static const VulkanErrorCategory cat;
        return cat;
    }

    inline std::error_code make_error_code(VulkanError e) noexcept
    {
        return { static_cast<int>(e), vulkan_error_category() };
    }
}

namespace std
{
    template<>
    struct is_error_code_enum<pf::rhi::VulkanError> : true_type {};
}
