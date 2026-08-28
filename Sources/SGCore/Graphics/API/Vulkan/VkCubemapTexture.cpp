//
// Created by stuka on 27.07.2023.
//

#include "VkCubemapTexture.h"

#include <cstring>

#include "RHI/VulkanDevice.h"
#include "RHI/VulkanGPUBuffer.h"
#include "SGCore/Graphics/RHI/TextureUnits.h"
#include "SGCore/Logger/Logger.h"
#include "VkRenderer.h"
#include "VulkanTypesCaster.h"

namespace
{
    SGCore::VulkanDevice* currentDevice() noexcept
    {
        return SGCore::VkRenderer::getLiveDevice();
    }
}

SGCore::VkCubemapTexture::~VkCubemapTexture() noexcept
{
    VkCubemapTexture::destroyOnGPU();
}

void SGCore::VkCubemapTexture::create()
{
    auto* device = currentDevice();
    if(!device || !device->isReady()) return;

    destroyOnGPU();

    // the faces carry the real size and format; the cubemap asset itself may not have them set
    const ITexture2D* reference = this;
    for(const auto& part : m_parts)
    {
        if(part && part->getWidth() > 0) { reference = part.get(); break; }
    }
    if(reference->getWidth() <= 0 || reference->getHeight() <= 0)
    {
        SG_LOG_E("VkCubemapTexture: no face carries a size, the cubemap stays empty.");
        return;
    }

    VulkanTextureDesc desc;
    desc.m_width = static_cast<std::uint32_t>(reference->getWidth());
    desc.m_height = static_cast<std::uint32_t>(reference->getHeight());
    desc.m_format = VulkanTypesCaster::sggInternalFormatToVk(reference->m_internalFormat);
    desc.m_usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    desc.m_cube = true;
    // a skybox must not show seams at the face edges
    desc.m_addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    desc.m_debugName = "cubemap";
    m_vulkanTexture = VulkanTexture::create(device->getContext(), desc);
    if(!m_vulkanTexture) return;

    const std::uint32_t texelSize = VulkanTypesCaster::formatTexelSize(desc.m_format);
    const std::uint64_t faceSize = static_cast<std::uint64_t>(desc.m_width) * desc.m_height * texelSize;

    // upload every face that has pixels; faces without data stay cleared
    device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
        m_vulkanTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    });

    for(std::size_t face = 0; face < m_parts.size() && face < 6; ++face)
    {
        const auto& part = m_parts[face];
        if(!part || !part->getData() || texelSize == 0) continue;

        const std::uint32_t partChannels = static_cast<std::uint32_t>(std::max(1, part->m_channelsCount));
        const std::uint32_t channelSize = getSGGDataTypeSizeInBytes(part->m_dataType);
        const std::uint64_t srcPixel = static_cast<std::uint64_t>(partChannels) * channelSize;
        // skybox faces are RGB while Vulkan widens RGB formats to RGBA (see sggInternalFormatToVk)
        const bool expand = VulkanTypesCaster::needsAlphaExpansion(reference->m_internalFormat) &&
                            partChannels == 3 && texelSize == 4 * channelSize;
        if(srcPixel != texelSize && !expand)
        {
            SG_LOG_E("VkCubemapTexture: face {} is {} byte(s) per pixel but the image format needs {}; face skipped.",
                     face, srcPixel, texelSize);
            continue;
        }

        auto staging = device->createStagingBuffer(faceSize, "cubemap_face_upload");
        if(!staging) continue;

        auto* destination = static_cast<std::uint8_t*>(staging->getMappedPointer());
        const auto* source = part->getData();
        if(!expand)
        {
            std::memcpy(destination, source, faceSize);
        }
        else
        {
            const std::uint64_t pixels = static_cast<std::uint64_t>(desc.m_width) * desc.m_height;
            for(std::uint64_t i = 0; i < pixels; ++i)
            {
                std::memcpy(destination + i * texelSize, source + i * srcPixel, srcPixel);
                std::memset(destination + i * texelSize + srcPixel, 0xFF, channelSize);
            }
        }

        device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
            VkBufferImageCopy region { };
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = static_cast<std::uint32_t>(face);
            region.imageSubresource.layerCount = 1;
            region.imageExtent = { desc.m_width, desc.m_height, 1 };
            vkCmdCopyBufferToImage(commandBuffer, staging->getHandle(), m_vulkanTexture->getImage(),
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        });
    }

    device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
        m_vulkanTexture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    });
}

void SGCore::VkCubemapTexture::createAsFrameBufferAttachment(IFrameBuffer*, SGFrameBufferAttachmentType)
{
    // cube maps are not used as framebuffer attachments by the engine's passes
}

void SGCore::VkCubemapTexture::subTextureBufferDataOnGAPISide(const size_t&, const size_t&)
{
}

void SGCore::VkCubemapTexture::subTextureDataOnGAPISide(const std::uint8_t*, std::size_t, std::size_t, std::size_t, std::size_t)
{
    // faces are uploaded whole in create(); partial updates are not used for cube maps
}

void SGCore::VkCubemapTexture::destroyOnGPU()
{
    if(!m_vulkanTexture) return;
    if(auto* device = currentDevice()) device->destroyDeferred(m_vulkanTexture);
    m_vulkanTexture = nullptr;
}

void SGCore::VkCubemapTexture::bind(const std::uint8_t& textureUnit) const
{
    // same unit model as VkTexture2D: VkShader joins unit and sampler at draw time
    TextureUnits::set(textureUnit, m_vulkanTexture);
}

void* SGCore::VkCubemapTexture::getTextureNativeHandler() const noexcept
{
    return m_vulkanTexture.get();
}

void* SGCore::VkCubemapTexture::getTextureBufferNativeHandler() const noexcept
{
    return nullptr;
}

SGCore::VkCubemapTexture& SGCore::VkCubemapTexture::operator=(const Ref<ITexture2D>& other)
{
    if(!other) return *this;
    m_type = other->m_type;
    m_internalFormat = other->m_internalFormat;
    m_format = other->m_format;
    m_dataType = other->m_dataType;
    m_channelsCount = other->m_channelsCount;
    return *this;
}
