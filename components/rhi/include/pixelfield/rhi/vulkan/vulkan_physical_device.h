//
// Created by stuka on 10.09.2026.
//

#pragma once

#include "pixelfield/rhi/physical_device.h"

#include <optional>
#include <array>
#include <utility>
#include <vector>

#include <vulkan/vulkan.h>

namespace pf::rhi
{
    struct VulkanPhysicalDevice final : IPhysicalDevice
    {
        friend struct VulkanContext;
        friend struct VulkanLogicalDevice;
        friend struct VulkanSwapchain;
        friend struct VulkanBuffer;
        friend struct VulkanImage;

        VulkanPhysicalDevice(const VulkanPhysicalDevice&) = delete;
        VulkanPhysicalDevice(VulkanPhysicalDevice&& other) noexcept;

        [[nodiscard]] bool valid() const noexcept override;

        VulkanPhysicalDevice& operator=(const VulkanPhysicalDevice&) = delete;
        VulkanPhysicalDevice& operator=(VulkanPhysicalDevice&& other) noexcept;

    private:
        struct SurfaceSupport
        {
            std::vector<VkSurfaceFormatKHR> formats_;
            std::vector<VkPresentModeKHR> present_modes_;
        };

        // destroys with VkInstance
        VkPhysicalDevice handle_ {};

        VkPhysicalDeviceFeatures features_ {};
        SurfaceSupport surface_support_;

        std::array<std::optional<std::uint32_t>, std::to_underlying(QueueFamilyType::kCount)> queue_families_indices_ {};

        [[nodiscard]] std::int32_t findMemoryIndex(VkMemoryRequirements requirements, VkMemoryPropertyFlags properties) const noexcept;

        VulkanPhysicalDevice() = default;
    };
}
