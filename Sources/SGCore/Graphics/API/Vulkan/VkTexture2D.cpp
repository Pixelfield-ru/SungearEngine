//
// Created by stuka on 07.07.2023.
//

#include "VkTexture2D.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

#include <glm/gtc/packing.hpp>

#include "RHI/VulkanDevice.h"
#include "RHI/VulkanGPUBuffer.h"
#include "SGCore/Graphics/RHI/TextureUnits.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Logger/Logger.h"
#include "VkRenderer.h"
#include "VulkanTypesCaster.h"

namespace
{
    SGCore::VulkanDevice* currentDevice() noexcept
    {
        // never through getInstance(): this runs from asset destructors, which happen in static
        // destruction after the renderer singleton is gone
        return SGCore::VkRenderer::getLiveDevice();
    }

}

SGCore::VkTexture2D::~VkTexture2D() noexcept
{
    destroyOnGPU();
}

void SGCore::VkTexture2D::create()
{
    auto* device = currentDevice();
    if(!device || !device->isReady()) return;
    if(m_width <= 0 || m_height <= 0) return;

    destroyOnGPU();

    // A texture buffer is not an image: GL exposes it as GL_TEXTURE_BUFFER, Vulkan as a uniform
    // texel buffer. Creating an image for it caps the element count at maxImageDimension2D (32768
    // here), and the batch vertex buffer alone needs 40440 texels.
    if(m_type == SGTextureType::SG_TEXTURE_BUFFER)
    {
        createAsTexelBuffer();
        return;
    }

    VulkanTextureDesc desc;
    desc.m_width = static_cast<std::uint32_t>(m_width);
    desc.m_height = static_cast<std::uint32_t>(m_height);
    desc.m_format = VulkanTypesCaster::sggInternalFormatToVk(m_internalFormat);
    desc.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    desc.m_mipLevels = 1;
    desc.m_debugName = "texture2d";
    m_vulkanTexture = VulkanTexture::create(device->getContext(), desc);
    if(!m_vulkanTexture) return;

    if(m_textureData)
    {
        uploadRegion(m_textureData.get(), desc.m_width, desc.m_height, 0, 0);
    }
    else
    {
        // no pixels: still leave the image in the layout shaders expect
        device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
            m_vulkanTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        });
    }
}

void SGCore::VkTexture2D::createAsFrameBufferAttachment(IFrameBuffer* parentFrameBuffer, SGFrameBufferAttachmentType attachmentType)
{
    auto* device = currentDevice();
    if(!device || !device->isReady() || !parentFrameBuffer) return;

    destroyOnGPU();
    m_frameBufferAttachmentType = attachmentType;

    const bool depth = isDepthAttachment(attachmentType) || isDepthStencilAttachment(attachmentType) ||
                       VulkanTypesCaster::isDepthFormat(m_internalFormat);

    VulkanTextureDesc desc;
    desc.m_width = static_cast<std::uint32_t>(std::max(1, m_width));
    desc.m_height = static_cast<std::uint32_t>(std::max(1, m_height));
    desc.m_format = VulkanTypesCaster::sggInternalFormatToVk(m_internalFormat);
    desc.m_usage = (depth ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) |
                   VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    desc.m_samples = m_useMultisampling ? static_cast<VkSampleCountFlagBits>(m_multisamplingSamplesCount) : VK_SAMPLE_COUNT_1_BIT;
    // NEAREST, as GL4Texture2D creates its attachments: later passes sample attachments texel by
    // texel and compare the result exactly (the layered FX gate gets its layer volume that way), and
    // an interpolated sample never equals the stored texel
    desc.m_filter = VK_FILTER_NEAREST;
    desc.m_addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    // the framebuffer address is part of the name: the engine has several framebuffers with the same
    // attachment slots, and identical names make a RenderDoc capture unreadable
    desc.m_debugName = "attachment_" + sgFrameBufferAttachmentTypeToString(attachmentType) + "@" +
                       std::to_string(reinterpret_cast<std::uintptr_t>(parentFrameBuffer) & 0xFFFFu) + "_" +
                       std::to_string(desc.m_width) + "x" + std::to_string(desc.m_height);
    m_vulkanTexture = VulkanTexture::create(device->getContext(), desc);
    if(!m_vulkanTexture) return;

    device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
        // A fresh Vulkan image holds whatever was in that memory, while a GL texture created without
        // data reads as zeros — and the engine relies on that: render passes load their attachments
        // (LoadOp::SGG_LOAD) and the FX framebuffer's attachments are never cleared at all, so
        // undefined contents survived into the final composite as sparse bright noise. Start cleared.
        m_vulkanTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        VkImageSubresourceRange range { };
        range.aspectMask = m_vulkanTexture->getAspect();
        range.levelCount = VK_REMAINING_MIP_LEVELS;
        range.layerCount = VK_REMAINING_ARRAY_LAYERS;

        if(m_vulkanTexture->isDepth())
        {
            // the far plane, not zero: a pass that forgets to clear depth should reject nothing
            const VkClearDepthStencilValue value { 1.0f, 0 };
            vkCmdClearDepthStencilImage(commandBuffer, m_vulkanTexture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
        }
        else
        {
            const VkClearColorValue value { };
            vkCmdClearColorImage(commandBuffer, m_vulkanTexture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
        }

        // rest in SHADER_READ_ONLY so an attachment can be sampled even before its first render pass
        m_vulkanTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    });
}

void SGCore::VkTexture2D::uploadRegion(const std::uint8_t* data, std::uint32_t width, std::uint32_t height, std::uint32_t x, std::uint32_t y) noexcept
{
    auto* device = currentDevice();
    if(!device || !m_vulkanTexture || !data || width == 0 || height == 0) return;

    const std::uint32_t srcChannels = static_cast<std::uint32_t>(std::max(1, m_channelsCount));
    const std::uint64_t pixelCount = static_cast<std::uint64_t>(width) * height;

    // The copy is sized by the image format, and glTexImage2D converts the CPU buffer into the internal
    // format on the way in while vkCmdCopyBufferToImage copies bytes as they are. The engine's
    // (channels, dataType) pair does not always agree with the declared internal format — the SSAO
    // noise hands 32-bit floats to an RGB16F image — so the staging buffer is built in the image's
    // layout, converting channel by channel. This is the upload counterpart of the decoding
    // VkFrameBuffer::readAttachmentPixels does.
    const auto imageLayout = VulkanTypesCaster::formatLayout(m_vulkanTexture->getFormat());
    const std::uint32_t texelSize = VulkanTypesCaster::formatTexelSize(m_vulkanTexture->getFormat());
    if(texelSize == 0 || !imageLayout.isPlain())
    {
        SG_LOG_E("VkTexture2D: can not upload '{}': image format {} has no plain channel layout.",
                 m_vulkanTexture->getDebugName(), static_cast<int>(m_vulkanTexture->getFormat()));

        // an image left in UNDEFINED is still bound and sampled by the passes, which is a validation
        // error on top of the refused upload: give it defined contents and the layout shaders expect
        device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
            m_vulkanTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            VkImageSubresourceRange range { };
            range.aspectMask = m_vulkanTexture->getAspect();
            range.levelCount = VK_REMAINING_MIP_LEVELS;
            range.layerCount = VK_REMAINING_ARRAY_LAYERS;
            const VkClearColorValue value { };
            vkCmdClearColorImage(commandBuffer, m_vulkanTexture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);

            m_vulkanTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        });
        return;
    }

    auto staging = device->createStagingBuffer(pixelCount * texelSize, "texture_upload");
    if(!staging) return;

    auto* dst = static_cast<std::uint8_t*>(staging->getMappedPointer());
    PixelConversion::encodeToImage(data, dst, pixelCount, m_dataType, srcChannels, imageLayout, texelSize);

    device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
        m_vulkanTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        VkBufferImageCopy region { };
        region.bufferOffset = 0;
        region.imageSubresource.aspectMask = m_vulkanTexture->getAspect();
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { static_cast<std::int32_t>(x), static_cast<std::int32_t>(y), 0 };
        region.imageExtent = { width, height, 1 };
        vkCmdCopyBufferToImage(commandBuffer, staging->getHandle(), m_vulkanTexture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        m_vulkanTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    });
}

void SGCore::VkTexture2D::subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight,
                                                   std::size_t areaOffsetX, std::size_t areaOffsetY) noexcept
{
    uploadRegion(data, static_cast<std::uint32_t>(areaWidth), static_cast<std::uint32_t>(areaHeight),
                 static_cast<std::uint32_t>(areaOffsetX), static_cast<std::uint32_t>(areaOffsetY));
}

void SGCore::VkTexture2D::destroyOnGPU() noexcept
{
    if(m_texelBuffer)
    {
        // the buffer holds the context itself, so it can outlive this facade safely
        m_texelBuffer.reset();
    }
    if(!m_vulkanTexture) return;
    if(auto* device = currentDevice())
    {
        // the GPU may still read it: released with the next retired submission
        device->destroyDeferred(m_vulkanTexture);
    }
    m_vulkanTexture.reset();
}

void SGCore::VkTexture2D::bind(const std::uint8_t& textureUnit) const noexcept
{
    // Vulkan has no texture units: record "unit N holds this texture" and let VkShader join it with
    // the sampler->unit half at draw time (see TextureUnits)
    if(m_texelBuffer)
    {
        TextureUnits::set(textureUnit, m_texelBuffer);
        return;
    }
    TextureUnits::set(textureUnit, m_vulkanTexture);
}

void* SGCore::VkTexture2D::getTextureNativeHandler() const noexcept
{
    return m_vulkanTexture.get();
}

void* SGCore::VkTexture2D::getTextureBufferNativeHandler() const noexcept
{
    return nullptr;
}

SGCore::VkTexture2D& SGCore::VkTexture2D::operator=(const Ref<ITexture2D>& other)
{
    // same contract as GL4Texture2D: the GPU object is not shared, only the CPU-side description
    if(!other) return *this;
    m_type = other->m_type;
    m_internalFormat = other->m_internalFormat;
    m_format = other->m_format;
    m_dataType = other->m_dataType;
    m_channelsCount = other->m_channelsCount;
    return *this;
}

void SGCore::VkTexture2D::createAsTexelBuffer() noexcept
{
    auto* device = currentDevice();
    if(!device) return;

    const std::uint8_t texelSize = getSGGInternalFormatChannelsSizeInBytes(m_internalFormat);
    const std::uint64_t bytes = static_cast<std::uint64_t>(m_width) * m_height * texelSize;
    if(bytes == 0) return;

    GPUBufferDesc desc;
    // storage usage is what carries VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT (VulkanGPUBuffer::toVkUsage)
    desc.m_usage = GPUBufferUsage::SGG_STORAGE_BUFFER | GPUBufferUsage::SGG_TRANSFER_DST;
    desc.m_access = GPUMemoryAccess::SGG_HOST_VISIBLE;
    desc.m_size = bytes;
    desc.m_debugName = "texture_buffer";

    auto buffer = device->createBuffer(desc);
    if(!buffer) return;

    m_texelBuffer = std::static_pointer_cast<VulkanGPUBuffer>(buffer);

    if(m_textureData) m_texelBuffer->write(m_textureData.get(), bytes);

    if(!m_texelBuffer->createTexelView(VulkanTypesCaster::sggInternalFormatToVkTexel(m_internalFormat)))
    {
        SG_LOG_E("VkTexture2D: could not create a texel buffer view; a samplerBuffer reading this will get the dummy view.");
    }
}

void SGCore::VkTexture2D::subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept
{
    if(!m_texelBuffer || !m_textureData) return;

    m_texelBuffer->write(m_textureData.get() + bytesOffset, bytesCount, bytesOffset);
}
