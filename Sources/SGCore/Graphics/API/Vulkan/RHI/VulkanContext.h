//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <sgcore_export.h>
#include <vma/vk_mem_alloc.h>

#include "SGCore/Graphics/API/Vulkan/VulkanCommon.h"

struct GLFWwindow;

namespace SGCore
{
    /// Everything that exists exactly once per process for the Vulkan backend: instance, debug
    /// messenger, window surface, physical + logical device, the graphics/present queue and the
    /// VMA allocator. Owned by VkRenderer through a shared_ptr; GPU-owning RHI objects keep a
    /// shared reference (they may outlive the renderer in static destruction).
    struct SGCORE_EXPORT VulkanContext : std::enable_shared_from_this<VulkanContext>
    {
        VkInstance m_instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;
        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        std::uint32_t m_graphicsQueueFamily = 0;
        VmaAllocator m_allocator = VK_NULL_HANDLE;

        VkPhysicalDeviceProperties m_physicalDeviceProperties { };
        bool m_validationEnabled { };
        /// VK_EXT_depth_clip_control is available: pipelines can keep the GL [-1, 1] depth range.
        bool m_depthClipControl { };
        std::uint32_t m_apiVersion = VK_API_VERSION_1_3;

        /// Creates the instance (with validation layers when requested and installed) and enumerates
        /// physical devices. Returns false when Vulkan is unusable on this machine.
        bool createInstance(bool enableValidation) noexcept;
        bool createSurface(GLFWwindow* window) noexcept;
        /// Picks a physical device with a graphics queue that can present to m_surface.
        bool pickPhysicalDevice() noexcept;
        bool createDevice() noexcept;
        bool createAllocator() noexcept;

        void destroy() noexcept;

        [[nodiscard]] bool isReady() const noexcept { return m_device != VK_NULL_HANDLE; }
        [[nodiscard]] std::string getDeviceName() const noexcept { return m_physicalDeviceProperties.deviceName; }

        /// Names the object in validation messages / RenderDoc when the debug utils extension is on.
        void setObjectName(std::uint64_t handle, VkObjectType type, const std::string& name) const noexcept;

        /// Command-buffer debug regions. Without them a capture is one flat list of draws; with them
        /// RenderDoc/NSight and validation messages show which pass a command belongs to.
        void beginDebugLabel(VkCommandBuffer commandBuffer, const std::string& name) const noexcept;
        void endDebugLabel(VkCommandBuffer commandBuffer) const noexcept;
        void insertDebugLabel(VkCommandBuffer commandBuffer, const std::string& name) const noexcept;

        [[nodiscard]] bool hasDebugUtils() const noexcept { return m_debugUtilsEnabled; }

        /// Registry of live GPU-owning objects (textures, buffers). destroy() releases the GPU side
        /// of everything still registered before the allocator/device go away: legacy assets in the
        /// AssetManager may outlive the renderer, and VMA asserts on leaked allocations in Debug.
        void registerResource(void* owner, std::function<void()> releaseGPU) noexcept;
        void unregisterResource(void* owner) noexcept;

    private:
        std::unordered_map<void*, std::function<void()>> m_liveResources;
        bool m_debugUtilsEnabled { };
        PFN_vkSetDebugUtilsObjectNameEXT m_setDebugUtilsObjectName = nullptr;
        PFN_vkCmdBeginDebugUtilsLabelEXT m_cmdBeginDebugUtilsLabel = nullptr;
        PFN_vkCmdEndDebugUtilsLabelEXT m_cmdEndDebugUtilsLabel = nullptr;
        PFN_vkCmdInsertDebugUtilsLabelEXT m_cmdInsertDebugUtilsLabel = nullptr;
    };
}
