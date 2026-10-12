//
// Created by stuka on 10.09.2026.
//

#include "pixelfield/rhi/vulkan/vulkan_context.h"

#include <iostream>
#include <utility>

#include "pixelfield/rhi/vulkan/vulkan_framebuffer.h"
#include "pixelfield/rhi/vulkan/vulkan_image.h"
#include "pixelfield/rhi/vulkan/vulkan_image_view.h"
#include "pixelfield/rhi/vulkan/vulkan_logical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_physical_device.h"
#include "pixelfield/rhi/vulkan/vulkan_pipeline_state.h"
#include "pixelfield/rhi/vulkan/vulkan_shader.h"
#include "pixelfield/rhi/vulkan/vulkan_swapchain.h"
#include "pixelfield/rhi/physical_device.h"

VkBool32 pf::rhi::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                            VkDebugUtilsMessageTypeFlagsEXT message_type,
                            const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
                            void* p_user_data)
{
    std::string msg_type = "unknown";
    if(message_type == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) { msg_type = "general"; }
    else if(message_type == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) { msg_type = "validation"; }
    else if(message_type == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) { msg_type = "performance"; }

    std::string msg_severity = "unknown";
    if(message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) { msg_severity = "info"; }
    else if(message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) { msg_severity = "verbose"; }
    else if(message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) { msg_severity = "warn"; }
    else if(message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) { msg_severity = "error"; }

    std::string objects_desc;
    for(std::uint32_t i = 0; i < p_callback_data->objectCount; ++i)
    {
        auto& object = p_callback_data->pObjects[i];

        objects_desc += "Handle: " + std::to_string(object.objectHandle) + ", type: " +
                std::to_string(object.objectType);

        if(object.pObjectName) { objects_desc += std::string(", name: ") + object.pObjectName; }

        if(i != p_callback_data->objectCount - 1) { objects_desc += '\n'; }
    }

    std::cout << "Vulkan: [" + msg_severity + "] [" + msg_type + "] " + p_callback_data->pMessage << std::endl;
    if(!objects_desc.empty())
    {
        std::cout << "Associated objects:\n" << objects_desc << std::endl;
    }

    return VK_FALSE;
}

pf::rhi::VulkanContext::VulkanContext(VulkanContext&& other) noexcept :
    create_messenger_(std::exchange(other.create_messenger_, VK_NULL_HANDLE)),
    destroy_messenger_(std::exchange(other.destroy_messenger_, VK_NULL_HANDLE)),
    allocator_(std::move(other.allocator_)),
    instance_(std::exchange(other.instance_, VK_NULL_HANDLE)),
    surface_(std::exchange(other.surface_, VK_NULL_HANDLE)),
    messenger_(std::exchange(other.messenger_, VK_NULL_HANDLE)),
    enabled_validation_layers_(std::move(other.enabled_validation_layers_)),
    enabled_extensions_(std::move(other.enabled_extensions_))
{
    rhi_debug_output_ = std::move(other.rhi_debug_output_);
    enable_rhi_debug_output_ = other.enable_rhi_debug_output_;
}

pf::rhi::VulkanContext::~VulkanContext() noexcept
{
    destroy();
}

std::error_code pf::rhi::VulkanContext::create(VulkanContextCreateInfo create_info,
                                               VulkanContext& output) noexcept
{
    output.allocator_ = std::move(create_info.allocator);
    output.enabled_extensions_ = std::move(create_info.extensions);
    output.enabled_validation_layers_ = std::move(create_info.validation_layers);
    output.rhi_debug_output_ = std::move(create_info.rhi_debug_output_func);
    output.enable_rhi_debug_output_ = create_info.enable_rhi_debug_output;

    // getting count of extensions that are supported by instance
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    // getting extensions supported by instance
    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;

    // getting required by glfw extensions
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> required_extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
    if(create_info.enable_api_debug_output)
    {
        required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    std::vector<const char*> validation_layers;

    if(create_info.enable_validation_layers)
    {
        uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

        for(const auto& layer : output.enabled_validation_layers_)
        {
            const auto it = std::ranges::find_if(available_layers, [&layer](const VkLayerProperties& available_layer) {
                return std::strcmp(layer.c_str(), available_layer.layerName) == 0;
            });

            if(it == available_layers.end())
            {
                return VulkanError::kUnsupportedValidationLayer;
            }

            validation_layers.push_back(layer.c_str());
        }
    }

    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{};
    debug_messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_messenger_create_info.pfnUserCallback = create_info.api_debug_output_func ? create_info.api_debug_output_func : debugCallback;
    debug_messenger_create_info.pUserData = nullptr;

    VkApplicationInfo app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = create_info.app_name.c_str();
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = create_info.engine_name.c_str();
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo instance_create_info {};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.enabledExtensionCount = required_extensions.size();
    instance_create_info.ppEnabledExtensionNames = required_extensions.data();
    instance_create_info.enabledLayerCount = validation_layers.size();
    instance_create_info.ppEnabledLayerNames = validation_layers.data();
    instance_create_info.pNext = &debug_messenger_create_info;

    if(vkCreateInstance(&instance_create_info, output.allocator_.get(), &output.instance_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreateInstance;
    }

    if(create_info.enable_api_debug_output)
    {
        output.create_messenger_ = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(output.instance_, "vkCreateDebugUtilsMessengerEXT");
        if(!output.create_messenger_)
        {
            return VulkanError::kCannotSetupDebug;
        }

        output.destroy_messenger_ = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(output.instance_, "vkDestroyDebugUtilsMessengerEXT");
        if(!output.destroy_messenger_)
        {
            return VulkanError::kCannotSetupDebug;
        }

        if(output.create_messenger_(output.instance_, &debug_messenger_create_info, output.allocator_.get(), &output.messenger_) != VK_SUCCESS)
        {
            return VulkanError::kCannotSetupDebug;
        }
    }

    // creating surface to present image
    if(glfwCreateWindowSurface(output.instance_, create_info.window, output.allocator_.get(), &output.surface_) != VK_SUCCESS)
    {
        return VulkanError::kCannotCreateSurface;
    }

    return {};
}

bool pf::rhi::VulkanContext::valid() const noexcept
{
    return instance_ != nullptr;
}

std::vector<std::shared_ptr<pf::rhi::IPhysicalDevice>> pf::rhi::VulkanContext::physicalDevices() noexcept
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance_, &device_count, nullptr);

    if(device_count == 0) return {};

    std::vector<VkPhysicalDevice> vulkan_devices(device_count);
    vkEnumeratePhysicalDevices(instance_, &device_count, vulkan_devices.data());

    std::vector<std::shared_ptr<IPhysicalDevice>> physical_devices(vulkan_devices.size());

    for(size_t i = 0; i < vulkan_devices.size(); ++i)
    {
        const auto& vulkan_device = vulkan_devices[i];

        VkPhysicalDeviceProperties device_properties;
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceProperties(vulkan_device, &device_properties);
        vkGetPhysicalDeviceFeatures(vulkan_device, &device_features);

        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(vulkan_device, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(vulkan_device, nullptr, &extension_count, available_extensions.data());

        auto physical_device = std::shared_ptr<VulkanPhysicalDevice>(new VulkanPhysicalDevice);
        physical_device->name_ = std::string(device_properties.deviceName);
        switch(device_properties.deviceType)
        {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                physical_device->type_ = DeviceType::kOther;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                physical_device->type_ = DeviceType::kIntegratedGpu;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                physical_device->type_ = DeviceType::kDiscreteGpu;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                physical_device->type_ = DeviceType::kVirtualGpu;
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                physical_device->type_ = DeviceType::kCpu;
                break;
            default:
                physical_device->type_ = DeviceType::kOther;
                break;
        }

        physical_device->features_ = device_features;
        physical_device->handle_ = vulkan_device;
        auto& surface_support = physical_device->surface_support_;

        // =========================================

        std::uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->handle_, surface_, &format_count, nullptr);

        if(format_count != 0)
        {
            surface_support.formats_.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->handle_, surface_, &format_count, surface_support.formats_.data());
        }

        std::uint32_t present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->handle_, surface_, &present_mode_count, nullptr);

        if(present_mode_count != 0)
        {
            surface_support.present_modes_.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->handle_, surface_, &present_mode_count, surface_support.present_modes_.data());
        }

        // =========================================

        // getting device queues...
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vulkan_device, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(vulkan_device, &queue_family_count, queue_families.data());

        for(size_t i = 0; i < queue_families.size(); ++i)
        {
            const auto& family = queue_families[i];
            if(family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kGraphics)] = i;
            }

            if(family.queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kCompute)] = i;
            }

            if(family.queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kTransfer)] = i;
            }

            if(family.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
            {
                physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kSparseBinding)] = i;
            }

            if(family.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR)
            {
                physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kVideoDecode)] = i;
            }

            if(family.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR)
            {
                physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kVideoEncode)] = i;
            }

            if(family.queueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV)
            {
                physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kOpticalFlow)] = i;
            }

            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(vulkan_device, i, surface_, &present_support);

            if(present_support)
            {
                physical_device->queue_families_indices_[std::to_underlying(QueueFamilyType::kPresent)] = i;
            }
        }

        // todo: select swapchain support

        physical_devices[i] = std::move(physical_device);
    }

    return physical_devices;
}

std::expected<std::unique_ptr<pf::rhi::ILogicalDevice>, std::error_code> pf::rhi::VulkanContext::createLogicalDevice(
    const LogicalDeviceCreateInfo& create_info) noexcept
{
    auto output = std::make_unique<VulkanLogicalDevice>();

    if(const auto err = output->create(create_info))
    {
        return std::unexpected(err);
    }

    return output;
}

pf::rhi::VulkanContext& pf::rhi::VulkanContext::operator=(VulkanContext&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    destroy();

    create_messenger_ = std::exchange(other.create_messenger_, VK_NULL_HANDLE);
    destroy_messenger_ = std::exchange(other.destroy_messenger_, VK_NULL_HANDLE);
    allocator_ = std::move(other.allocator_);
    instance_ = std::exchange(other.instance_, VK_NULL_HANDLE);
    surface_ = std::exchange(other.surface_, VK_NULL_HANDLE);
    messenger_ = std::exchange(other.messenger_, VK_NULL_HANDLE);
    enabled_validation_layers_ = std::move(other.enabled_validation_layers_);
    enabled_extensions_ = std::move(other.enabled_extensions_);
    rhi_debug_output_ = std::move(other.rhi_debug_output_);
    enable_rhi_debug_output_ = other.enable_rhi_debug_output_;

    return *this;
}

void pf::rhi::VulkanContext::destroy() noexcept
{
    if(surface_)
    {
        vkDestroySurfaceKHR(instance_, surface_, allocator_.get());
        surface_ = VK_NULL_HANDLE;
    }

    if(destroy_messenger_)
    {
        destroy_messenger_(instance_, messenger_, allocator_.get());
        destroy_messenger_ = VK_NULL_HANDLE;
        messenger_ = VK_NULL_HANDLE;
    }

    if(instance_)
    {
        vkDestroyInstance(instance_, allocator_.get());
        instance_ = VK_NULL_HANDLE;
    }
}
