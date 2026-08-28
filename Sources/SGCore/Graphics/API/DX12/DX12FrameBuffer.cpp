//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12FrameBuffer.h"

#if defined(_WIN32)

#include <algorithm>
#include <cstring>

#include "DX12Renderer.h"
#include "DX12Texture2D.h"
#include "DX12TypesCaster.h"
#include "RHI/DX12Device.h"
#include "SGCore/Graphics/API/AttachmentReadback.h"
#include "SGCore/Graphics/RHI/ICommandList.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Main/CoreMain.h"

namespace
{
    SGCore::DX12Device* currentDevice() noexcept
    {
        // never through getInstance(): this runs from asset destructors, which happen in static
        // destruction after the renderer singleton is gone
        return SGCore::DX12Renderer::getLiveDevice();
    }
}

SGCore::DX12FrameBuffer::~DX12FrameBuffer()
{
    DX12FrameBuffer::destroy();
}

SGCore::ICommandList* SGCore::DX12FrameBuffer::commandList() noexcept
{
    if(!DX12Renderer::getLiveDevice()) return nullptr;
    return DX12Renderer::getInstance()->getFrameBufferCommandList();
}

void SGCore::DX12FrameBuffer::create()
{
    // attachments carry the GPU objects; nothing to allocate for the framebuffer itself
}

void SGCore::DX12FrameBuffer::destroy()
{
    if(m_boundThroughRHI) unbind();
    m_attachments.clear();
}

void SGCore::DX12FrameBuffer::beginPass() const noexcept
{
    auto* list = commandList();
    if(!list) return;

    RenderPassBeginDesc desc;
    desc.m_frameBuffer = const_cast<DX12FrameBuffer*>(this);
    desc.m_colorAttachments = m_drawAttachments;
    desc.m_colorLoadOp = LoadOp::SGG_LOAD;
    desc.m_depthLoadOp = LoadOp::SGG_LOAD;
    desc.m_width = m_width;
    desc.m_height = m_height;

    list->begin();
    list->beginRenderPass(desc);
    m_boundThroughRHI = true;
}

void SGCore::DX12FrameBuffer::bind() const
{
    if(m_attachments.empty()) return;
    beginPass();
}

void SGCore::DX12FrameBuffer::unbind() const
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
        device->submit(DX12Renderer::getInstance()->getFrameBufferCommandListRef());
    }
    m_boundThroughRHI = false;
}

void SGCore::DX12FrameBuffer::bindAttachment(const SGFrameBufferAttachmentType& attachmentType, const std::uint8_t& textureBlock)
{
    // same contract as GL4FrameBuffer: hand the unit to the attachment itself
    if(const auto attachment = getAttachment(attachmentType))
    {
        attachment->bind(textureBlock);
    }
}

void SGCore::DX12FrameBuffer::bindAttachmentToDrawIn(const SGFrameBufferAttachmentType& attachmentType)
{
    bindAttachmentsToDrawIn(std::vector<SGFrameBufferAttachmentType> { attachmentType });
}

void SGCore::DX12FrameBuffer::bindAttachmentsToDrawIn(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    m_drawAttachments = attachmentsTypes;
    // the render target set is part of the pass: re-open it with the new one (load ops keep contents)
    if(m_boundThroughRHI)
    {
        unbind();
        beginPass();
    }
}

void SGCore::DX12FrameBuffer::bindAttachmentsToDrawIn(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    bindAttachmentsToDrawIn(std::vector<SGFrameBufferAttachmentType>(attachmentsTypes.begin(), attachmentsTypes.end()));
}

void SGCore::DX12FrameBuffer::clear()
{
    for(const auto& attachment : m_attachments)
    {
        clearAttachment(attachment.first);
    }
}

void SGCore::DX12FrameBuffer::clearAttachment(const SGFrameBufferAttachmentType& attachmentType)
{
    auto* list = commandList();
    if(!list || !m_boundThroughRHI)
    {
        clearAttachmentImmediate(attachmentType);
        return;
    }

    if(isColorAttachment(attachmentType))
    {
        // clearColorAttachment addresses the pass's colour attachments by index in the draw set
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

void SGCore::DX12FrameBuffer::clearAttachmentImmediate(const SGFrameBufferAttachmentType& attachmentType) const noexcept
{
    auto* device = currentDevice();
    const auto attachment = getAttachment(attachmentType);
    const auto* dxAttachment = dynamic_cast<DX12Texture2D*>(attachment.get());
    if(!device || !dxAttachment || !dxAttachment->getDX12Texture()) return;
    auto texture = dxAttachment->getDX12Texture();

    device->immediateSubmit([&](ID3D12GraphicsCommandList* list) {
        const D3D12_RESOURCE_STATES restore = texture->getState();
        if(texture->isDepth())
        {
            texture->recordTransition(list, D3D12_RESOURCE_STATE_DEPTH_WRITE);
            list->ClearDepthStencilView(texture->getDSV(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
        }
        else
        {
            texture->recordTransition(list, D3D12_RESOURCE_STATE_RENDER_TARGET);
            const auto& c = attachment->m_clearColor;
            const float color[4] = { c.r, c.g, c.b, c.a };
            list->ClearRenderTargetView(texture->getRTV(), color, 0, nullptr);
        }
        texture->recordTransition(list, restore);
    });
}

void SGCore::DX12FrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
                                            SGGColorFormat format,
                                            SGGColorInternalFormat internalFormat,
                                            SGGDataType dataType,
                                            const int& mipLevel,
                                            const int& layer)
{
    addAttachment(attachmentType, format, internalFormat, dataType, mipLevel, layer, false, 1);
}

void SGCore::DX12FrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
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
        SG_LOG_E("DX12FrameBuffer: an attachment of type {} already exists.", sgFrameBufferAttachmentTypeToString(attachmentType));
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

void SGCore::DX12FrameBuffer::attachAttachment(const Ref<ITexture2D>& otherAttachment) noexcept
{
    if(!otherAttachment) return;
    // shares the other framebuffer's image; the type is whatever it was created as
    const auto type = otherAttachment->getFrameBufferAttachmentType();
    if(type == SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT) return;
    m_attachments[type] = otherAttachment;
}

void SGCore::DX12FrameBuffer::removeAttachment(SGFrameBufferAttachmentType attachmentType) noexcept
{
    const bool wasBound = m_boundThroughRHI;
    if(wasBound) unbind();
    m_attachments.erase(attachmentType);
    if(wasBound && !m_attachments.empty()) beginPass();
}

bool SGCore::DX12FrameBuffer::readAttachmentPixels(SGFrameBufferAttachmentType attachmentType, AttachmentReadback& out) const noexcept
{
    auto* device = currentDevice();
    if(!device || m_width <= 0 || m_height <= 0) return false;

    const auto attachment = getAttachment(attachmentType);
    const auto* dxAttachment = dynamic_cast<DX12Texture2D*>(attachment.get());
    if(!dxAttachment || !dxAttachment->getDX12Texture()) return false;
    auto texture = dxAttachment->getDX12Texture();

    // the caller wants the attachment in the format and data type it was declared with, but a
    // texture copy is raw memory: the image's own channel layout is what lands in the readback
    // resource, and the conversion glReadPixels does implicitly has to happen here
    const std::int8_t outChannels = getSGGFormatChannelsCount(attachment->m_format);
    const std::uint16_t channelSize = getSGGDataTypeSizeInBytes(attachment->m_dataType);
    if(outChannels <= 0 || channelSize == 0) return false;

    const auto imageLayout = DX12TypesCaster::formatLayout(texture->getFormat());
    const std::uint32_t texelSize = DX12TypesCaster::formatTexelSize(texture->getFormat());
    if(!imageLayout.isPlain() || texelSize == 0)
    {
        SG_LOG_E("DX12FrameBuffer: attachment image format {} has no plain channel layout and can not be read back.",
                 static_cast<int>(texture->getFormat()));
        return false;
    }

    if(m_boundThroughRHI) unbind();
    device->waitIdle();

    const D3D12_RESOURCE_DESC resourceDesc = texture->getResource()->GetDesc();
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint { };
    UINT rowsCount = 0;
    UINT64 rowSizeInBytes = 0;
    UINT64 totalBytes = 0;
    device->getContext().m_device->GetCopyableFootprints(&resourceDesc, 0, 1, 0, &footprint, &rowsCount, &rowSizeInBytes, &totalBytes);

    auto readback = device->createReadbackResource(totalBytes, "attachment_readback");
    if(!readback) return false;

    device->immediateSubmit([&](ID3D12GraphicsCommandList* list) {
        const D3D12_RESOURCE_STATES restore = texture->getState();
        texture->recordTransition(list, D3D12_RESOURCE_STATE_COPY_SOURCE);

        D3D12_TEXTURE_COPY_LOCATION destination { };
        destination.pResource = readback.Get();
        destination.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        destination.PlacedFootprint = footprint;

        D3D12_TEXTURE_COPY_LOCATION source { };
        source.pResource = texture->getResource();
        source.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        source.SubresourceIndex = 0;

        list->CopyTextureRegion(&destination, 0, 0, 0, &source, nullptr);
        texture->recordTransition(list, restore);
    });

    void* mapped = nullptr;
    const D3D12_RANGE readRange { 0, static_cast<SIZE_T>(totalBytes) };
    if(!SG_DX_CHECK(readback->Map(0, &readRange, &mapped)) || !mapped) return false;

    const std::uint32_t width = texture->getWidth();
    const std::uint32_t height = texture->getHeight();
    const std::uint64_t outRowPitch = static_cast<std::uint64_t>(width) * outChannels * channelSize;

    out.m_width = static_cast<std::int32_t>(width);
    out.m_height = static_cast<std::int32_t>(height);
    out.m_format = attachment->m_format;
    out.m_dataType = attachment->m_dataType;
    out.m_channelsCount = outChannels;
    out.m_data.resize(outRowPitch * height);

    // offscreen passes render with a flipped viewport, so row 0 of the image is NDC y = -1 — the row
    // order glReadPixels returns. The copy pads its rows to 256 bytes, so it is walked row by row.
    const auto* source = static_cast<const std::uint8_t*>(mapped);
    for(std::uint32_t row = 0; row < height; ++row)
    {
        PixelConversion::decodeFromImage(source + static_cast<std::uint64_t>(row) * footprint.Footprint.RowPitch,
                                         out.m_data.data() + row * outRowPitch, width, imageLayout, texelSize,
                                         attachment->m_dataType, static_cast<std::uint32_t>(outChannels));
    }

    const D3D12_RANGE writtenRange { 0, 0 };
    readback->Unmap(0, &writtenRange);
    return true;
}

glm::vec3 SGCore::DX12FrameBuffer::readPixelsFromAttachment(const glm::vec2& mousePos, SGFrameBufferAttachmentType attachmentType) const noexcept
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
        result[c] = PixelConversion::decodeSourceChannel(readback.m_data.data() + pixelIndex + static_cast<std::size_t>(c) * channelSize,
                                                         readback.m_dataType);
    }
    return result;
}

#endif
