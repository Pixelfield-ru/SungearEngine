//
// Created by stuka on 07.07.2023.
//

#include "VkFrameBuffer.h"

#include <algorithm>
#include <cmath>
#include <cstring>

#include <glm/gtc/packing.hpp>

#include "RHI/VulkanDevice.h"
#include "RHI/VulkanGPUBuffer.h"
#include "SGCore/Graphics/API/AttachmentReadback.h"
#include "SGCore/Graphics/RHI/ICommandList.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Main/CoreMain.h"
#include "VkRenderer.h"
#include "VkTexture2D.h"
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

SGCore::VkFrameBuffer::~VkFrameBuffer()
{
    VkFrameBuffer::destroy();
}

SGCore::ICommandList* SGCore::VkFrameBuffer::commandList() noexcept
{
    // no command list once the device is gone (see currentDevice())
    if(!VkRenderer::getLiveDevice()) return nullptr;
    return VkRenderer::getInstance()->getFrameBufferCommandList();
}

void SGCore::VkFrameBuffer::create()
{
    // attachments carry the GPU objects; nothing to allocate for the framebuffer itself
}

void SGCore::VkFrameBuffer::destroy()
{
    if(m_boundThroughRHI) unbind();
    m_attachments.clear();
}

void SGCore::VkFrameBuffer::beginPass() const noexcept
{
    auto* list = commandList();
    if(!list) return;

    RenderPassBeginDesc desc;
    desc.m_frameBuffer = const_cast<VkFrameBuffer*>(this);
    desc.m_colorAttachments = m_drawAttachments;
    desc.m_colorLoadOp = LoadOp::SGG_LOAD;
    desc.m_depthLoadOp = LoadOp::SGG_LOAD;
    desc.m_width = m_width;
    desc.m_height = m_height;

    list->begin();
    list->beginRenderPass(desc);
    m_boundThroughRHI = true;
}

void SGCore::VkFrameBuffer::bind() const
{
    if(m_attachments.empty()) return;
    beginPass();
}

void SGCore::VkFrameBuffer::unbind() const
{
    auto* list = commandList();
    if(!list || !m_boundThroughRHI)
    {
        m_boundThroughRHI = false;
        return;
    }
    list->endRenderPass();
    list->end();
    if(auto* device = currentDevice())
    {
        device->submit(VkRenderer::getInstance()->getFrameBufferCommandListRef());
    }
    m_boundThroughRHI = false;
}

void SGCore::VkFrameBuffer::bindAttachment(const SGFrameBufferAttachmentType& attachmentType, const std::uint8_t& textureBlock)
{
    // same contract as GL4FrameBuffer: hand the unit to the attachment itself
    if(const auto attachment = getAttachment(attachmentType))
    {
        attachment->bind(textureBlock);
    }
}

void SGCore::VkFrameBuffer::bindAttachmentToDrawIn(const SGFrameBufferAttachmentType& attachmentType)
{
    bindAttachmentsToDrawIn(std::vector<SGFrameBufferAttachmentType> { attachmentType });
}

void SGCore::VkFrameBuffer::bindAttachmentsToDrawIn(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    m_drawAttachments = attachmentsTypes;
    // draw buffers are part of the render pass: re-open it with the new set (load ops keep contents)
    if(m_boundThroughRHI)
    {
        unbind();
        beginPass();
    }
}

void SGCore::VkFrameBuffer::bindAttachmentsToDrawIn(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    bindAttachmentsToDrawIn(std::vector<SGFrameBufferAttachmentType>(attachmentsTypes.begin(), attachmentsTypes.end()));
}

void SGCore::VkFrameBuffer::clear()
{
    for(const auto& attachment : m_attachments)
    {
        clearAttachment(attachment.first);
    }
}

void SGCore::VkFrameBuffer::clearAttachment(const SGFrameBufferAttachmentType& attachmentType)
{
    auto* list = commandList();
    if(!list || !m_boundThroughRHI)
    {
        clearAttachmentImmediate(attachmentType);
        return;
    }

    if(isColorAttachment(attachmentType))
    {
        // clearColorAttachment addresses the pass's color attachments by index in the draw set
        const auto& draw = m_drawAttachments;
        std::uint32_t index = 0;
        if(!draw.empty())
        {
            const auto it = std::find(draw.begin(), draw.end(), attachmentType);
            if(it == draw.end()) return;
            index = static_cast<std::uint32_t>(std::distance(draw.begin(), it));
        }
        else
        {
            std::vector<SGFrameBufferAttachmentType> all;
            for(const auto& [type, texture] : m_attachments) if(isColorAttachment(type)) all.push_back(type);
            std::sort(all.begin(), all.end());
            const auto it = std::find(all.begin(), all.end(), attachmentType);
            if(it == all.end()) return;
            index = static_cast<std::uint32_t>(std::distance(all.begin(), it));
        }
        if(const auto attachment = getAttachment(attachmentType))
        {
            list->clearColorAttachment(index, attachment->m_clearColor);
        }
    }
    else if(isDepthAttachment(attachmentType))
    {
        list->clearDepthStencil(1.0f, false, 0);
    }
    else if(isDepthStencilAttachment(attachmentType))
    {
        list->clearDepthStencil(1.0f, true, 0);
    }
}

void SGCore::VkFrameBuffer::clearAttachmentImmediate(const SGFrameBufferAttachmentType& attachmentType) const noexcept
{
    auto* device = currentDevice();
    const auto attachment = getAttachment(attachmentType);
    const auto* vkAttachment = dynamic_cast<VkTexture2D*>(attachment.get());
    if(!device || !vkAttachment || !vkAttachment->getVulkanTexture()) return;
    auto texture = vkAttachment->getVulkanTexture();

    device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
        const VkImageLayout restore = texture->getLayout() == VK_IMAGE_LAYOUT_UNDEFINED ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : texture->getLayout();
        texture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        VkImageSubresourceRange range { };
        range.aspectMask = texture->getAspect();
        range.levelCount = VK_REMAINING_MIP_LEVELS;
        range.layerCount = VK_REMAINING_ARRAY_LAYERS;
        if(texture->isDepth())
        {
            VkClearDepthStencilValue value { 1.0f, 0 };
            vkCmdClearDepthStencilImage(commandBuffer, texture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
        }
        else
        {
            const auto& c = attachment->m_clearColor;
            VkClearColorValue value { { c.r, c.g, c.b, c.a } };
            vkCmdClearColorImage(commandBuffer, texture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
        }
        texture->recordTransition(commandBuffer, restore);
    });
}

void SGCore::VkFrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
                                          SGGColorFormat format,
                                          SGGColorInternalFormat internalFormat,
                                          SGGDataType dataType,
                                          const int& mipLevel,
                                          const int& layer)
{
    addAttachment(attachmentType, format, internalFormat, dataType, mipLevel, layer, false, 1);
}

void SGCore::VkFrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
                                          SGGColorFormat format,
                                          SGGColorInternalFormat internalFormat,
                                          SGGDataType dataType,
                                          const int& mipLevel,
                                          const int& layer,
                                          bool useMultisampling,
                                          std::uint8_t multisamplingSamplesCount)
{
    if(m_attachments.contains(attachmentType))
    {
        SG_LOG_E("VkFrameBuffer: an attachment of type {} already exists.", sgFrameBufferAttachmentTypeToString(attachmentType));
        return;
    }

    // adding an attachment while the pass is open (legacy flow: bind -> addAttachment) restarts the pass
    const bool wasBound = m_boundThroughRHI;
    if(wasBound) unbind();

    auto& newAttachment = m_attachments[attachmentType];
    newAttachment = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    newAttachment->resize(m_width, m_height, true);
    newAttachment->m_format = format;
    newAttachment->m_internalFormat = internalFormat;
    newAttachment->m_mipLevel = mipLevel;
    newAttachment->m_layer = layer;
    newAttachment->m_useMultisampling = useMultisampling;
    newAttachment->m_multisamplingSamplesCount = multisamplingSamplesCount;
    newAttachment->m_dataType = dataType;
    newAttachment->m_channelsCount = getSGGFormatChannelsCount(format);
    newAttachment->createAsFrameBufferAttachment(this, attachmentType);

    if(wasBound) beginPass();
}

void SGCore::VkFrameBuffer::attachAttachment(const Ref<ITexture2D>& otherAttachment) noexcept
{
    if(!otherAttachment) return;
    // shares the other framebuffer's image; the type is whatever it was created as
    const auto type = otherAttachment->getFrameBufferAttachmentType();
    if(type == SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT) return;
    m_attachments[type] = otherAttachment;
}

void SGCore::VkFrameBuffer::removeAttachment(SGFrameBufferAttachmentType attachmentType) noexcept
{
    const bool wasBound = m_boundThroughRHI;
    if(wasBound) unbind();
    m_attachments.erase(attachmentType);
    if(wasBound && !m_attachments.empty()) beginPass();
}

bool SGCore::VkFrameBuffer::readAttachmentPixels(SGFrameBufferAttachmentType attachmentType, AttachmentReadback& out) const noexcept
{
    auto* device = currentDevice();
    if(!device || m_width <= 0 || m_height <= 0) return false;

    const auto attachment = getAttachment(attachmentType);
    const auto* vkAttachment = dynamic_cast<VkTexture2D*>(attachment.get());
    if(!vkAttachment || !vkAttachment->getVulkanTexture()) return false;
    auto texture = vkAttachment->getVulkanTexture();

    // the caller wants the attachment in the format and data type it was declared with, but
    // vkCmdCopyImageToBuffer is a raw memory copy: the image's own layout (its channel count, channel
    // size and numeric kind) is what lands in the staging buffer, and the conversion glReadPixels
    // does implicitly has to happen here. Sizing the copy by the declared data type instead reads
    // half of a 16-bit-float attachment and decodes it shifted (observed as black G-buffer readbacks).
    const std::int8_t outChannels = getSGGFormatChannelsCount(attachment->m_format);
    const std::uint16_t channelSize = getSGGDataTypeSizeInBytes(attachment->m_dataType);
    if(outChannels <= 0 || channelSize == 0) return false;

    const auto imageLayout = VulkanTypesCaster::formatLayout(texture->getFormat());
    if(!imageLayout.isPlain())
    {
        SG_LOG_E("VkFrameBuffer: attachment image format {} has no plain channel layout and can not be read back.",
                 static_cast<int>(texture->getFormat()));
        return false;
    }

    const auto extent = texture->getExtent();
    const std::uint64_t imagePixel = static_cast<std::uint64_t>(imageLayout.m_channels) * imageLayout.m_channelSize;
    const std::uint64_t outPixel = static_cast<std::uint64_t>(outChannels) * channelSize;
    const std::uint64_t pixelCount = static_cast<std::uint64_t>(extent.width) * extent.height;

    if(m_boundThroughRHI) unbind();
    device->waitIdle();

    auto staging = device->createStagingBuffer(pixelCount * imagePixel, "attachment_readback");
    if(!staging) return false;

    device->immediateSubmit([&](VkCommandBuffer commandBuffer) {
        const VkImageLayout restore = texture->getLayout() == VK_IMAGE_LAYOUT_UNDEFINED ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : texture->getLayout();
        texture->recordTransition(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        VkBufferImageCopy region { };
        region.imageSubresource.aspectMask = texture->isDepth() ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.layerCount = 1;
        region.imageExtent = { extent.width, extent.height, 1 };
        vkCmdCopyImageToBuffer(commandBuffer, texture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, staging->getHandle(), 1, &region);
        texture->recordTransition(commandBuffer, restore);
    });

    out.m_width = static_cast<std::int32_t>(extent.width);
    out.m_height = static_cast<std::int32_t>(extent.height);
    out.m_format = attachment->m_format;
    out.m_dataType = attachment->m_dataType;
    out.m_channelsCount = outChannels;
    out.m_data.resize(pixelCount * outPixel);

    // offscreen passes are not flipped: row 0 of the image is NDC y = -1, the same row order glReadPixels returns
    const auto* src = static_cast<const std::uint8_t*>(staging->getMappedPointer());
    PixelConversion::decodeFromImage(src, out.m_data.data(), pixelCount, imageLayout, static_cast<std::uint32_t>(imagePixel),
                                     attachment->m_dataType, static_cast<std::uint32_t>(outChannels));
    return true;
}

glm::vec3 SGCore::VkFrameBuffer::readPixelsFromAttachment(const glm::vec2& mousePos, SGFrameBufferAttachmentType attachmentType) const noexcept
{
    AttachmentReadback readback;
    if(!readAttachmentPixels(attachmentType, readback)) return { };

    const std::int32_t x = std::clamp(static_cast<std::int32_t>(mousePos.x), 0, readback.m_width - 1);
    const std::int32_t y = std::clamp(static_cast<std::int32_t>(mousePos.y), 0, readback.m_height - 1);
    const std::uint16_t channelSize = getSGGDataTypeSizeInBytes(readback.m_dataType);
    const std::size_t pixelIndex = (static_cast<std::size_t>(y) * readback.m_width + x) * readback.m_channelsCount * channelSize;

    glm::vec3 result { };
    for(std::int32_t c = 0; c < std::min<std::int32_t>(3, readback.m_channelsCount); ++c)
    {
        const std::uint8_t* channel = readback.m_data.data() + pixelIndex + static_cast<std::size_t>(c) * channelSize;
        switch(readback.m_dataType)
        {
            case SGGDataType::SGG_FLOAT: { float value; std::memcpy(&value, channel, sizeof(value)); result[c] = value; break; }
            case SGGDataType::SGG_UNSIGNED_BYTE: result[c] = static_cast<float>(*channel) / 255.0f; break;
            case SGGDataType::SGG_BYTE: result[c] = static_cast<float>(*reinterpret_cast<const std::int8_t*>(channel)) / 127.0f; break;
            case SGGDataType::SGG_UNSIGNED_INT: { std::uint32_t value; std::memcpy(&value, channel, sizeof(value)); result[c] = static_cast<float>(value); break; }
            case SGGDataType::SGG_INT: { std::int32_t value; std::memcpy(&value, channel, sizeof(value)); result[c] = static_cast<float>(value); break; }
            case SGGDataType::SGG_UNSIGNED_SHORT: { std::uint16_t value; std::memcpy(&value, channel, sizeof(value)); result[c] = static_cast<float>(value); break; }
            case SGGDataType::SGG_SHORT: { std::int16_t value; std::memcpy(&value, channel, sizeof(value)); result[c] = static_cast<float>(value); break; }
            default: break;
        }
    }
    return result;
}
