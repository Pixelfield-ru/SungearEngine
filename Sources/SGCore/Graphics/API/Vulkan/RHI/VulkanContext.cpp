//
// Created by 8bitniksis on 17.08.2026.
//

// the single translation unit that carries the VMA implementation
#define VMA_IMPLEMENTATION
#include "VulkanContext.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstring>
#include <optional>

#include "SGCore/Logger/Logger.h"
#include "SGCore/Main/CoreSettings.h"

namespace
{
    VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                          VkDebugUtilsMessageTypeFlagsEXT /*type*/,
                                                          const VkDebugUtilsMessengerCallbackDataEXT* data,
                                                          void* /*userData*/)
    {
        const char* message = data && data->pMessage ? data->pMessage : "(no message)";
        if(severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            SG_LOG_E("[Vulkan validation] {}", message);
        }
        else if(severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            SG_LOG_W("[Vulkan validation] {}", message);
        }
        else
        {
            SG_LOG_D("[Vulkan validation] {}", message);
        }
        return VK_FALSE;
    }

    bool hasLayer(const std::vector<VkLayerProperties>& layers, const char* name) noexcept
    {
        return std::any_of(layers.begin(), layers.end(), [name](const VkLayerProperties& layer) {
            return std::strcmp(layer.layerName, name) == 0;
        });
    }

    bool hasExtension(const std::vector<VkExtensionProperties>& extensions, const char* name) noexcept
    {
        return std::any_of(extensions.begin(), extensions.end(), [name](const VkExtensionProperties& extension) {
            return std::strcmp(extension.extensionName, name) == 0;
        });
    }
}

bool SGCore::VulkanContext::createInstance(bool enableValidation) noexcept
{
    if(m_instance != VK_NULL_HANDLE) return true;

    if(!glfwVulkanSupported())
    {
        SG_LOG_E("Vulkan: GLFW reports no Vulkan support (loader or ICD missing).");
        return false;
    }

    std::uint32_t instanceVersion = VK_API_VERSION_1_0;
    vkEnumerateInstanceVersion(&instanceVersion);
    if(instanceVersion < VK_API_VERSION_1_3)
    {
        SG_LOG_E("Vulkan: instance version {}.{} is below the required 1.3.",
                 VK_API_VERSION_MAJOR(instanceVersion), VK_API_VERSION_MINOR(instanceVersion));
        return false;
    }

    std::uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> layers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    std::uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::vector<const char*> enabledLayers;
    if(enableValidation)
    {
        if(hasLayer(layers, "VK_LAYER_KHRONOS_validation"))
        {
            enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
            m_validationEnabled = true;
        }
        else
        {
            SG_LOG_W("Vulkan: validation requested but VK_LAYER_KHRONOS_validation is not installed (install the Vulkan SDK).");
        }
    }

    std::uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> enabledExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if(hasExtension(extensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
    {
        enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        m_debugUtilsEnabled = true;
    }

    VkApplicationInfo applicationInfo { };
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "Sungear Engine";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(SG_CORE_MAJOR_VERSION, SG_CORE_MINOR_VERSION, SG_CORE_PATCH_VERSION);
    applicationInfo.pEngineName = "Sungear Engine";
    applicationInfo.engineVersion = applicationInfo.applicationVersion;
    applicationInfo.apiVersion = m_apiVersion;

    VkDebugUtilsMessengerCreateInfoEXT messengerInfo { };
    messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messengerInfo.pfnUserCallback = debugMessengerCallback;

    VkInstanceCreateInfo instanceInfo { };
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &applicationInfo;
    instanceInfo.enabledLayerCount = static_cast<std::uint32_t>(enabledLayers.size());
    instanceInfo.ppEnabledLayerNames = enabledLayers.data();
    instanceInfo.enabledExtensionCount = static_cast<std::uint32_t>(enabledExtensions.size());
    instanceInfo.ppEnabledExtensionNames = enabledExtensions.data();
    // catches problems in vkCreateInstance / vkDestroyInstance themselves
    if(m_validationEnabled && m_debugUtilsEnabled) instanceInfo.pNext = &messengerInfo;

    if(!SG_VK_CHECK(vkCreateInstance(&instanceInfo, nullptr, &m_instance)))
    {
        m_instance = VK_NULL_HANDLE;
        return false;
    }

    if(m_validationEnabled && m_debugUtilsEnabled)
    {
        const auto createMessenger = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
        if(createMessenger)
        {
            createMessenger(m_instance, &messengerInfo, nullptr, &m_debugMessenger);
        }
    }
    if(m_debugUtilsEnabled)
    {
        m_setDebugUtilsObjectName = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(
            vkGetInstanceProcAddr(m_instance, "vkSetDebugUtilsObjectNameEXT"));
        m_cmdBeginDebugUtilsLabel = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(m_instance, "vkCmdBeginDebugUtilsLabelEXT"));
        m_cmdEndDebugUtilsLabel = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(m_instance, "vkCmdEndDebugUtilsLabelEXT"));
        m_cmdInsertDebugUtilsLabel = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(m_instance, "vkCmdInsertDebugUtilsLabelEXT"));
    }

    SG_LOG_I("Vulkan: instance created (API {}.{}, validation: {}).",
             VK_API_VERSION_MAJOR(instanceVersion), VK_API_VERSION_MINOR(instanceVersion), m_validationEnabled ? "on" : "off");
    return true;
}

bool SGCore::VulkanContext::createSurface(GLFWwindow* window) noexcept
{
    if(m_surface != VK_NULL_HANDLE) return true;
    if(!window) return false;

    return SG_VK_CHECK(glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface));
}

bool SGCore::VulkanContext::pickPhysicalDevice() noexcept
{
    std::uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if(deviceCount == 0)
    {
        SG_LOG_E("Vulkan: no physical devices found.");
        return false;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    VkPhysicalDevice best = VK_NULL_HANDLE;
    std::uint32_t bestFamily = 0;
    int bestScore = -1;

    for(const auto device : devices)
    {
        VkPhysicalDeviceProperties properties { };
        vkGetPhysicalDeviceProperties(device, &properties);
        if(properties.apiVersion < VK_API_VERSION_1_3) continue;

        std::uint32_t familyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
        std::vector<VkQueueFamilyProperties> families(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

        std::optional<std::uint32_t> family;
        for(std::uint32_t i = 0; i < familyCount; ++i)
        {
            if(!(families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) continue;
            VkBool32 presentSupported = VK_FALSE;
            if(m_surface != VK_NULL_HANDLE)
            {
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupported);
                if(!presentSupported) continue;
            }
            family = i;
            break;
        }
        if(!family) continue;

        std::uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());
        if(!hasExtension(extensions, VK_KHR_SWAPCHAIN_EXTENSION_NAME)) continue;

        VkPhysicalDeviceVulkan13Features features13 { };
        features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
        VkPhysicalDeviceFeatures2 features2 { };
        features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        features2.pNext = &features13;
        vkGetPhysicalDeviceFeatures2(device, &features2);
        if(!features13.dynamicRendering || !features13.synchronization2) continue;

        int score = 0;
        if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;
        else if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) score += 100;
        if(score > bestScore)
        {
            bestScore = score;
            best = device;
            bestFamily = *family;
        }
    }

    if(best == VK_NULL_HANDLE)
    {
        SG_LOG_E("Vulkan: no physical device with Vulkan 1.3, dynamic rendering, synchronization2 and a presentable graphics queue.");
        return false;
    }

    m_physicalDevice = best;
    m_graphicsQueueFamily = bestFamily;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);

    SG_LOG_I("Vulkan: using '{}' (driver {}.{}.{}, API {}.{}.{}).", m_physicalDeviceProperties.deviceName,
             VK_API_VERSION_MAJOR(m_physicalDeviceProperties.driverVersion), VK_API_VERSION_MINOR(m_physicalDeviceProperties.driverVersion),
             VK_API_VERSION_PATCH(m_physicalDeviceProperties.driverVersion),
             VK_API_VERSION_MAJOR(m_physicalDeviceProperties.apiVersion), VK_API_VERSION_MINOR(m_physicalDeviceProperties.apiVersion),
             VK_API_VERSION_PATCH(m_physicalDeviceProperties.apiVersion));
    return true;
}

bool SGCore::VulkanContext::createDevice() noexcept
{
    if(m_device != VK_NULL_HANDLE) return true;

    std::uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, extensions.data());

    std::vector<const char*> enabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkPhysicalDeviceDepthClipControlFeaturesEXT depthClipControl { };
    depthClipControl.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_CONTROL_FEATURES_EXT;
    if(hasExtension(extensions, VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME))
    {
        VkPhysicalDeviceFeatures2 query { };
        query.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        query.pNext = &depthClipControl;
        vkGetPhysicalDeviceFeatures2(m_physicalDevice, &query);
        if(depthClipControl.depthClipControl)
        {
            enabledExtensions.push_back(VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME);
            m_depthClipControl = true;
        }
    }
    if(!m_depthClipControl)
    {
        SG_LOG_W("Vulkan: VK_EXT_depth_clip_control is unavailable; GL-style projection matrices lose the near half of the depth range until the projections are made API-aware.");
    }

    VkPhysicalDeviceFeatures supported { };
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &supported);

    VkPhysicalDeviceFeatures features { };
    features.independentBlend = supported.independentBlend;
    features.fillModeNonSolid = supported.fillModeNonSolid;
    features.wideLines = supported.wideLines;
    features.largePoints = supported.largePoints;
    features.samplerAnisotropy = supported.samplerAnisotropy;
    features.tessellationShader = supported.tessellationShader;
    features.geometryShader = supported.geometryShader;
    features.fragmentStoresAndAtomics = supported.fragmentStoresAndAtomics;
    features.vertexPipelineStoresAndAtomics = supported.vertexPipelineStoresAndAtomics;

    VkPhysicalDeviceVulkan12Features features12 { };
    features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    features12.timelineSemaphore = VK_TRUE;

    VkPhysicalDeviceVulkan13Features features13 { };
    features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    features13.dynamicRendering = VK_TRUE;
    features13.synchronization2 = VK_TRUE;
    features13.pNext = &features12;

    depthClipControl.depthClipControl = m_depthClipControl ? VK_TRUE : VK_FALSE;
    depthClipControl.pNext = &features13;

    VkPhysicalDeviceFeatures2 features2 { };
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features2.features = features;
    features2.pNext = m_depthClipControl ? static_cast<void*>(&depthClipControl) : static_cast<void*>(&features13);

    const float priority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo { };
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = m_graphicsQueueFamily;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;

    VkDeviceCreateInfo deviceInfo { };
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = &features2;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = static_cast<std::uint32_t>(enabledExtensions.size());
    deviceInfo.ppEnabledExtensionNames = enabledExtensions.data();

    if(!SG_VK_CHECK(vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device)))
    {
        m_device = VK_NULL_HANDLE;
        return false;
    }

    vkGetDeviceQueue(m_device, m_graphicsQueueFamily, 0, &m_graphicsQueue);
    return true;
}

bool SGCore::VulkanContext::createAllocator() noexcept
{
    if(m_allocator != VK_NULL_HANDLE) return true;

    VmaAllocatorCreateInfo allocatorInfo { };
    allocatorInfo.instance = m_instance;
    allocatorInfo.physicalDevice = m_physicalDevice;
    allocatorInfo.device = m_device;
    allocatorInfo.vulkanApiVersion = m_apiVersion;

    return SG_VK_CHECK(vmaCreateAllocator(&allocatorInfo, &m_allocator));
}

void SGCore::VulkanContext::registerResource(void* owner, std::function<void()> releaseGPU) noexcept
{
    m_liveResources[owner] = std::move(releaseGPU);
}

void SGCore::VulkanContext::unregisterResource(void* owner) noexcept
{
    m_liveResources.erase(owner);
}

void SGCore::VulkanContext::destroy() noexcept
{
    if(m_device != VK_NULL_HANDLE) vkDeviceWaitIdle(m_device);

    // whatever is still alive (legacy assets, caches) drops its GPU objects now; the C++ shells stay
    if(!m_liveResources.empty())
    {
        SG_LOG_D("Vulkan: releasing {} GPU resources still alive at context shutdown.", m_liveResources.size());
        auto resources = std::move(m_liveResources);
        m_liveResources.clear();
        for(auto& [owner, release] : resources) release();
    }

    if(m_allocator != VK_NULL_HANDLE)
    {
        vmaDestroyAllocator(m_allocator);
        m_allocator = VK_NULL_HANDLE;
    }
    if(m_device != VK_NULL_HANDLE)
    {
        vkDestroyDevice(m_device, nullptr);
        m_device = VK_NULL_HANDLE;
    }
    if(m_surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        m_surface = VK_NULL_HANDLE;
    }
    if(m_debugMessenger != VK_NULL_HANDLE)
    {
        const auto destroyMessenger = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
        if(destroyMessenger) destroyMessenger(m_instance, m_debugMessenger, nullptr);
        m_debugMessenger = VK_NULL_HANDLE;
    }
    if(m_instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }
}

void SGCore::VulkanContext::beginDebugLabel(VkCommandBuffer commandBuffer, const std::string& name) const noexcept
{
    if(!m_cmdBeginDebugUtilsLabel || commandBuffer == VK_NULL_HANDLE) return;

    VkDebugUtilsLabelEXT label { };
    label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
    label.pLabelName = name.c_str();
    m_cmdBeginDebugUtilsLabel(commandBuffer, &label);
}

void SGCore::VulkanContext::endDebugLabel(VkCommandBuffer commandBuffer) const noexcept
{
    if(!m_cmdEndDebugUtilsLabel || commandBuffer == VK_NULL_HANDLE) return;
    m_cmdEndDebugUtilsLabel(commandBuffer);
}

void SGCore::VulkanContext::insertDebugLabel(VkCommandBuffer commandBuffer, const std::string& name) const noexcept
{
    if(!m_cmdInsertDebugUtilsLabel || commandBuffer == VK_NULL_HANDLE) return;

    VkDebugUtilsLabelEXT label { };
    label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
    label.pLabelName = name.c_str();
    m_cmdInsertDebugUtilsLabel(commandBuffer, &label);
}

void SGCore::VulkanContext::setObjectName(std::uint64_t handle, VkObjectType type, const std::string& name) const noexcept
{
    if(!m_setDebugUtilsObjectName || name.empty() || m_device == VK_NULL_HANDLE) return;

    VkDebugUtilsObjectNameInfoEXT nameInfo { };
    nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    nameInfo.objectType = type;
    nameInfo.objectHandle = handle;
    nameInfo.pObjectName = name.c_str();
    m_setDebugUtilsObjectName(m_device, &nameInfo);
}
