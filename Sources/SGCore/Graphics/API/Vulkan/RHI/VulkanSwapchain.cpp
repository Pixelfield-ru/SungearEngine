//
// Created by 8bitniksis on 17.08.2026.
//

#include "VulkanSwapchain.h"

#include <GLFW/glfw3.h>

#include <algorithm>

#include "SGCore/Logger/Logger.h"
#include "VulkanDevice.h"

SGCore::VulkanSwapchain::VulkanSwapchain(VulkanDevice& device, GLFWwindow* window) noexcept : m_device(device), m_window(window)
{
    m_debugName = "swapchain";

    auto& context = m_device.getContext();
    VkSemaphoreCreateInfo semaphoreInfo { };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    for(auto& slot : m_frameSlots)
    {
        SG_VK_CHECK(vkCreateSemaphore(context.m_device, &semaphoreInfo, nullptr, &slot.m_acquireSemaphore));
    }

    recreate();
}

SGCore::VulkanSwapchain::~VulkanSwapchain()
{
    auto& context = m_device.getContext();
    if(context.m_device == VK_NULL_HANDLE) return;

    vkDeviceWaitIdle(context.m_device);
    destroySwapchainObjects();
    if(m_swapchain != VK_NULL_HANDLE) vkDestroySwapchainKHR(context.m_device, m_swapchain, nullptr);
    for(auto& slot : m_frameSlots)
    {
        if(slot.m_acquireSemaphore != VK_NULL_HANDLE) vkDestroySemaphore(context.m_device, slot.m_acquireSemaphore, nullptr);
    }
}

void SGCore::VulkanSwapchain::destroySwapchainObjects() noexcept
{
    auto& context = m_device.getContext();
    m_images.clear();
    for(const auto semaphore : m_renderFinishedSemaphores)
    {
        vkDestroySemaphore(context.m_device, semaphore, nullptr);
    }
    m_renderFinishedSemaphores.clear();
}

bool SGCore::VulkanSwapchain::queryWindowExtent(VkExtent2D& outExtent) const noexcept
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    if(width <= 0 || height <= 0) return false;
    outExtent = { static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height) };
    return true;
}

bool SGCore::VulkanSwapchain::recreate() noexcept
{
    auto& context = m_device.getContext();
    m_needsRecreate = false;
    m_acquired = false;

    VkExtent2D windowExtent { };
    if(!queryWindowExtent(windowExtent))
    {
        // minimized: keep the old swapchain around, try again next frame
        m_needsRecreate = true;
        return false;
    }

    VkSurfaceCapabilitiesKHR capabilities { };
    if(!SG_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context.m_physicalDevice, context.m_surface, &capabilities))) return false;

    std::uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(context.m_physicalDevice, context.m_surface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(context.m_physicalDevice, context.m_surface, &formatCount, formats.data());

    // UNORM (not sRGB): the engine's screen pass writes display-ready values, as on GL
    VkSurfaceFormatKHR chosenFormat = formats.empty() ? VkSurfaceFormatKHR { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR } : formats[0];
    for(const auto& candidate : formats)
    {
        if((candidate.format == VK_FORMAT_B8G8R8A8_UNORM || candidate.format == VK_FORMAT_R8G8B8A8_UNORM) &&
           candidate.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            chosenFormat = candidate;
            break;
        }
    }
    m_format = chosenFormat.format;
    m_colorSpace = chosenFormat.colorSpace;

    if(capabilities.currentExtent.width != UINT32_MAX)
    {
        m_extent = capabilities.currentExtent;
    }
    else
    {
        m_extent.width = std::clamp(windowExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        m_extent.height = std::clamp(windowExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }
    if(m_extent.width == 0 || m_extent.height == 0)
    {
        m_needsRecreate = true;
        return false;
    }

    std::uint32_t imageCount = capabilities.minImageCount + 1;
    if(capabilities.maxImageCount > 0) imageCount = std::min(imageCount, capabilities.maxImageCount);

    VkSwapchainKHR oldSwapchain = m_swapchain;

    VkSwapchainCreateInfoKHR swapchainInfo { };
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = context.m_surface;
    swapchainInfo.minImageCount = imageCount;
    swapchainInfo.imageFormat = m_format;
    swapchainInfo.imageColorSpace = m_colorSpace;
    swapchainInfo.imageExtent = m_extent;
    swapchainInfo.imageArrayLayers = 1;
    // TRANSFER_SRC for readScreenPixels(), TRANSFER_DST for clears/blits outside a render pass
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainInfo.preTransform = capabilities.currentTransform;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapchainInfo.clipped = VK_TRUE;
    swapchainInfo.oldSwapchain = oldSwapchain;

    vkDeviceWaitIdle(context.m_device);
    m_device.retire(true);
    destroySwapchainObjects();

    if(!SG_VK_CHECK(vkCreateSwapchainKHR(context.m_device, &swapchainInfo, nullptr, &m_swapchain)))
    {
        m_swapchain = VK_NULL_HANDLE;
        if(oldSwapchain != VK_NULL_HANDLE) vkDestroySwapchainKHR(context.m_device, oldSwapchain, nullptr);
        return false;
    }
    if(oldSwapchain != VK_NULL_HANDLE) vkDestroySwapchainKHR(context.m_device, oldSwapchain, nullptr);

    std::uint32_t actualCount = 0;
    vkGetSwapchainImagesKHR(context.m_device, m_swapchain, &actualCount, nullptr);
    std::vector<VkImage> images(actualCount);
    vkGetSwapchainImagesKHR(context.m_device, m_swapchain, &actualCount, images.data());

    VkSemaphoreCreateInfo semaphoreInfo { };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    for(std::uint32_t i = 0; i < actualCount; ++i)
    {
        m_images.push_back(VulkanTexture::wrap(context, images[i], m_format, m_extent.width, m_extent.height,
                                               "swapchain_image_" + std::to_string(i)));
        VkSemaphore semaphore = VK_NULL_HANDLE;
        SG_VK_CHECK(vkCreateSemaphore(context.m_device, &semaphoreInfo, nullptr, &semaphore));
        m_renderFinishedSemaphores.push_back(semaphore);
    }

    for(auto& slot : m_frameSlots) slot.m_acquireSemaphorePending = false;

    SG_LOG_I("Vulkan: swapchain {}x{} with {} images (format {}).", m_extent.width, m_extent.height, actualCount, static_cast<int>(m_format));
    return true;
}

bool SGCore::VulkanSwapchain::beginFrame() noexcept
{
    m_device.retire(false);

    // window resized since the swapchain was built
    VkExtent2D windowExtent { };
    if(queryWindowExtent(windowExtent) && (windowExtent.width != m_extent.width || windowExtent.height != m_extent.height))
    {
        m_needsRecreate = true;
    }
    if(m_needsRecreate || m_swapchain == VK_NULL_HANDLE)
    {
        recreate();
    }
    return isValid();
}

bool SGCore::VulkanSwapchain::ensureAcquired() noexcept
{
    if(m_acquired) return true;
    if(m_needsRecreate || m_swapchain == VK_NULL_HANDLE)
    {
        if(!recreate()) return false;
    }

    auto& context = m_device.getContext();
    auto& slot = m_frameSlots[m_frameSlot];

    // bound the frames in flight: this slot's previous present must be done before its semaphore is reused
    m_device.waitForSubmission(slot.m_lastSubmission);

    if(slot.m_acquireSemaphorePending)
    {
        // an acquire signalled it and nothing consumed it (a frame without any submit): flush with an empty submit
        VulkanSubmission flush;
        flush.m_waitSemaphores.push_back(slot.m_acquireSemaphore);
        flush.m_waitStages.push_back(VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT);
        m_device.submitRaw(std::move(flush));
        m_device.retire(true);
        slot.m_acquireSemaphorePending = false;
    }

    const VkResult result = vkAcquireNextImageKHR(context.m_device, m_swapchain, UINT64_MAX, slot.m_acquireSemaphore, VK_NULL_HANDLE, &m_imageIndex);
    if(result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        m_needsRecreate = true;
        return recreate() && ensureAcquired();
    }
    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        SG_VK_CHECK(result);
        return false;
    }
    if(result == VK_SUBOPTIMAL_KHR) m_needsRecreate = true;

    slot.m_acquireSemaphorePending = true;
    m_acquired = true;
    // the presentation engine gives the image back with undefined contents
    m_images[m_imageIndex]->setLayout(VK_IMAGE_LAYOUT_UNDEFINED);
    return true;
}

SGCore::Ref<SGCore::VulkanTexture> SGCore::VulkanSwapchain::getCurrentTexture() const noexcept
{
    if(!m_acquired || m_imageIndex >= m_images.size()) return nullptr;
    return m_images[m_imageIndex];
}

VkSemaphore SGCore::VulkanSwapchain::takeAcquireSemaphore() noexcept
{
    auto& slot = m_frameSlots[m_frameSlot];
    if(!m_acquired || !slot.m_acquireSemaphorePending) return VK_NULL_HANDLE;
    slot.m_acquireSemaphorePending = false;
    return slot.m_acquireSemaphore;
}

void SGCore::VulkanSwapchain::present() noexcept
{
    ++m_frameNumber;
    if(!m_acquired)
    {
        // nothing rendered to the window this frame
        return;
    }

    auto& context = m_device.getContext();
    auto& slot = m_frameSlots[m_frameSlot];
    auto image = m_images[m_imageIndex];

    // transition to PRESENT_SRC and signal the per-image semaphore the presentation engine waits on
    VulkanSubmission submission;
    const VkCommandBuffer commandBuffer = m_device.acquireCommandBuffer();
    VkCommandBufferBeginInfo beginInfo { };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    image->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    vkEndCommandBuffer(commandBuffer);
    submission.m_commandBuffers.push_back(commandBuffer);
    if(const auto acquireSemaphore = takeAcquireSemaphore(); acquireSemaphore != VK_NULL_HANDLE)
    {
        submission.m_waitSemaphores.push_back(acquireSemaphore);
        submission.m_waitStages.push_back(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT);
    }
    submission.m_signalSemaphores.push_back(m_renderFinishedSemaphores[m_imageIndex]);
    submission.m_keepAlive.push_back(image);
    slot.m_lastSubmission = m_device.submitRaw(std::move(submission));

    VkPresentInfoKHR presentInfo { };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_renderFinishedSemaphores[m_imageIndex];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapchain;
    presentInfo.pImageIndices = &m_imageIndex;

    const VkResult result = vkQueuePresentKHR(context.m_graphicsQueue, &presentInfo);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        m_needsRecreate = true;
    }
    else
    {
        SG_VK_CHECK(result);
    }

    m_acquired = false;
    m_frameSlot = (m_frameSlot + 1) % frames_in_flight;
}
