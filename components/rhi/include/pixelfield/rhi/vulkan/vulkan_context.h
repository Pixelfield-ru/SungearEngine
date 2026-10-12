//
// Created by stuka on 10.09.2026.
//

#pragma once

#include <vector>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <memory>
#include <GLFW/glfw3.h>

#include "pixelfield/rhi/context.h"

namespace pf::rhi
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
        void* p_user_data);

    struct VulkanContextCreateInfo final : ContextCreateInfo
    {
        /// Optional.
        std::vector<std::string> validation_layers;
        /// Optional.
        std::vector<std::string> extensions {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        /// Optional.
        bool enable_validation_layers {};
        /// Optional. If nullptr then pf::rhi::debugCallback is used.
        VkBool32 (*api_debug_output_func)(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                         VkDebugUtilsMessageTypeFlagsEXT message_type,
                                         const VkDebugUtilsMessengerCallbackDataEXT*
                                         p_callback_data,
                                         void* p_user_data) { };
        /// Optional. Defaults to new/delete-backed host allocator.
        std::unique_ptr<VkAllocationCallbacks> allocator;
    };

    struct VulkanContext final : IContext
    {
        friend struct VulkanLogicalDevice;
        friend struct VulkanSwapchain;
        friend struct VulkanImage;
        friend struct VulkanImageView;
        friend struct VulkanSampler;
        friend struct VulkanShader;
        friend struct VulkanPipelineState;
        friend struct VulkanFramebuffer;
        friend struct VulkanPass;
        friend struct VulkanBuffer;
        friend struct VulkanDescriptorSet;

        VulkanContext() noexcept = default;
        VulkanContext(const VulkanContext&) = delete;
        VulkanContext(VulkanContext&& other) noexcept;
        ~VulkanContext() noexcept override;

        [[nodiscard]] bool valid() const noexcept override;

        [[nodiscard]] std::vector<std::shared_ptr<IPhysicalDevice>> physicalDevices() noexcept override;

        [[nodiscard]] std::expected<std::unique_ptr<ILogicalDevice>, std::error_code> createLogicalDevice(const LogicalDeviceCreateInfo& create_info) noexcept override;

        VulkanContext& operator=(const VulkanContext&) = delete;
        VulkanContext& operator=(VulkanContext&& other) noexcept;

        [[nodiscard]] static std::error_code create(VulkanContextCreateInfo create_info, VulkanContext& output) noexcept;

    private:
        PFN_vkCreateDebugUtilsMessengerEXT create_messenger_ {};
        PFN_vkDestroyDebugUtilsMessengerEXT destroy_messenger_ {};

        std::unique_ptr<VkAllocationCallbacks> allocator_;

        VkInstance instance_ {};
        VkSurfaceKHR surface_ {};
        VkDebugUtilsMessengerEXT messenger_ {};

        std::vector<std::string> enabled_validation_layers_;
        std::vector<std::string> enabled_extensions_;

        void destroy() noexcept;
    };
}
