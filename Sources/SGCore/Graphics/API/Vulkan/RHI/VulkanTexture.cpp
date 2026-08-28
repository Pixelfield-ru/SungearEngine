//
// Created by 8bitniksis on 17.08.2026.
//

#include "VulkanTexture.h"

#include "SGCore/Logger/Logger.h"

SGCore::VulkanTexture::~VulkanTexture()
{
    if(m_context) m_context->unregisterResource(this);
    releaseGPU();
}

void SGCore::VulkanTexture::releaseGPU() noexcept
{
    if(!m_context || m_context->m_device == VK_NULL_HANDLE)
    {
        m_image = VK_NULL_HANDLE;
        m_view = VK_NULL_HANDLE;
        m_sampler = VK_NULL_HANDLE;
        return;
    }

    if(m_sampler != VK_NULL_HANDLE) vkDestroySampler(m_context->m_device, m_sampler, nullptr);
    if(m_view != VK_NULL_HANDLE) vkDestroyImageView(m_context->m_device, m_view, nullptr);
    if(m_ownsImage && m_image != VK_NULL_HANDLE && m_context->m_allocator != VK_NULL_HANDLE)
    {
        vmaDestroyImage(m_context->m_allocator, m_image, m_allocation);
    }
    m_sampler = VK_NULL_HANDLE;
    m_view = VK_NULL_HANDLE;
    m_image = VK_NULL_HANDLE;
    m_allocation = VK_NULL_HANDLE;
}

VkImageAspectFlags SGCore::VulkanTexture::aspectOfFormat(VkFormat format) noexcept
{
    switch(format)
    {
        case VK_FORMAT_D16_UNORM:
        case VK_FORMAT_X8_D24_UNORM_PACK32:
        case VK_FORMAT_D32_SFLOAT:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        case VK_FORMAT_D16_UNORM_S8_UINT:
        case VK_FORMAT_D24_UNORM_S8_UINT:
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        case VK_FORMAT_S8_UINT:
            return VK_IMAGE_ASPECT_STENCIL_BIT;
        default:
            return VK_IMAGE_ASPECT_COLOR_BIT;
    }
}

SGCore::Ref<SGCore::VulkanTexture> SGCore::VulkanTexture::create(VulkanContext& context, const VulkanTextureDesc& desc) noexcept
{
    Ref<VulkanTexture> texture(new VulkanTexture());
    texture->m_context = context.shared_from_this();
    texture->m_format = desc.m_format;
    texture->m_extent = { desc.m_width, desc.m_height };
    texture->m_aspect = aspectOfFormat(desc.m_format);
    texture->m_mipLevels = desc.m_mipLevels == 0 ? 1 : desc.m_mipLevels;
    texture->m_arrayLayers = desc.m_arrayLayers == 0 ? 1 : desc.m_arrayLayers;
    texture->m_ownsImage = true;
    texture->m_debugName = desc.m_debugName;

    VkImageCreateInfo imageInfo { };
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = desc.m_format;
    imageInfo.extent = { desc.m_width, desc.m_height, 1 };
    imageInfo.mipLevels = texture->m_mipLevels;
    imageInfo.arrayLayers = texture->m_arrayLayers;
    imageInfo.samples = desc.m_samples;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = desc.m_usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if(desc.m_cube)
    {
        imageInfo.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        imageInfo.arrayLayers = 6;
        texture->m_arrayLayers = 6;
    }

    VmaAllocationCreateInfo allocationInfo { };
    allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
    if(desc.m_usage & (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT))
    {
        allocationInfo.flags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    }

    if(!SG_VK_CHECK(vmaCreateImage(context.m_allocator, &imageInfo, &allocationInfo, &texture->m_image, &texture->m_allocation, nullptr)))
    {
        return nullptr;
    }
    context.setObjectName(reinterpret_cast<std::uint64_t>(texture->m_image), VK_OBJECT_TYPE_IMAGE, desc.m_debugName);

    VkImageViewCreateInfo viewInfo { };
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = texture->m_image;
    viewInfo.viewType = desc.m_cube ? VK_IMAGE_VIEW_TYPE_CUBE
                                    : (texture->m_arrayLayers > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D);
    viewInfo.format = desc.m_format;
    // TODO(vulkan): a depth-stencil image needs a depth-only view for sampling; the full view here
    // serves attachment use, sampling such images gets its own view when shadows move to Vulkan
    viewInfo.subresourceRange.aspectMask = texture->m_aspect;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = texture->m_mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = texture->m_arrayLayers;
    if(!SG_VK_CHECK(vkCreateImageView(context.m_device, &viewInfo, nullptr, &texture->m_view)))
    {
        return nullptr;
    }

    if(desc.m_usage & VK_IMAGE_USAGE_SAMPLED_BIT)
    {
        VkSamplerCreateInfo samplerInfo { };
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = desc.m_filter;
        samplerInfo.minFilter = desc.m_filter;
        samplerInfo.mipmapMode = desc.m_filter == VK_FILTER_NEAREST ? VK_SAMPLER_MIPMAP_MODE_NEAREST : VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.addressModeU = desc.m_addressMode;
        samplerInfo.addressModeV = desc.m_addressMode;
        samplerInfo.addressModeW = desc.m_addressMode;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = static_cast<float>(texture->m_mipLevels);
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        if(!SG_VK_CHECK(vkCreateSampler(context.m_device, &samplerInfo, nullptr, &texture->m_sampler)))
        {
            return nullptr;
        }
    }

    context.registerResource(texture.get(), [raw = texture.get()] { raw->releaseGPU(); });
    return texture;
}

SGCore::Ref<SGCore::VulkanTexture> SGCore::VulkanTexture::wrap(VulkanContext& context, VkImage image, VkFormat format,
                                                               std::uint32_t width, std::uint32_t height, const std::string& debugName) noexcept
{
    Ref<VulkanTexture> texture(new VulkanTexture());
    texture->m_context = context.shared_from_this();
    texture->m_image = image;
    texture->m_format = format;
    texture->m_extent = { width, height };
    texture->m_aspect = aspectOfFormat(format);
    texture->m_ownsImage = false;
    texture->m_debugName = debugName;

    VkImageViewCreateInfo viewInfo { };
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = texture->m_aspect;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.layerCount = 1;
    if(!SG_VK_CHECK(vkCreateImageView(context.m_device, &viewInfo, nullptr, &texture->m_view)))
    {
        return nullptr;
    }
    context.setObjectName(reinterpret_cast<std::uint64_t>(image), VK_OBJECT_TYPE_IMAGE, debugName);

    context.registerResource(texture.get(), [raw = texture.get()] { raw->releaseGPU(); });
    return texture;
}

void SGCore::VulkanTexture::layoutToStageAccess(VkImageLayout layout, VkPipelineStageFlags2& stage, VkAccessFlags2& access) noexcept
{
    switch(layout)
    {
        case VK_IMAGE_LAYOUT_UNDEFINED:
        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            stage = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
            access = VK_ACCESS_2_NONE;
            break;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            stage = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
            access = VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
            stage = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
            access = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
        case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
            stage = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT | VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
            access = VK_ACCESS_2_SHADER_SAMPLED_READ_BIT | VK_ACCESS_2_SHADER_READ_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            stage = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
            access = VK_ACCESS_2_TRANSFER_READ_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            stage = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
            access = VK_ACCESS_2_TRANSFER_WRITE_BIT;
            break;
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            stage = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
            access = VK_ACCESS_2_NONE;
            break;
        case VK_IMAGE_LAYOUT_GENERAL:
        default:
            stage = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
            access = VK_ACCESS_2_MEMORY_READ_BIT | VK_ACCESS_2_MEMORY_WRITE_BIT;
            break;
    }
}

void SGCore::VulkanTexture::recordTransition(VkCommandBuffer commandBuffer, VkImageLayout newLayout) noexcept
{
    if(m_layout == newLayout || commandBuffer == VK_NULL_HANDLE) return;

    VkImageMemoryBarrier2 barrier { };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    layoutToStageAccess(m_layout, barrier.srcStageMask, barrier.srcAccessMask);
    layoutToStageAccess(newLayout, barrier.dstStageMask, barrier.dstAccessMask);
    barrier.oldLayout = m_layout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = m_image;
    barrier.subresourceRange.aspectMask = m_aspect;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = m_mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = m_arrayLayers;

    VkDependencyInfo dependency { };
    dependency.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency.imageMemoryBarrierCount = 1;
    dependency.pImageMemoryBarriers = &barrier;
    vkCmdPipelineBarrier2(commandBuffer, &dependency);

    m_layout = newLayout;
}
