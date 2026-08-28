//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12Texture2D.h"

#if defined(_WIN32)

#include <algorithm>
#include <cstring>
#include <vector>

#include "DX12Renderer.h"
#include "DX12TypesCaster.h"
#include "RHI/DX12Device.h"
#include "RHI/DX12GPUBuffer.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/RHI/TextureUnits.h"
#include "SGCore/Logger/Logger.h"

namespace
{
    SGCore::DX12Device* currentDevice() noexcept
    {
        // never through getInstance(): this runs from asset destructors, which happen in static
        // destruction after the renderer singleton is gone
        return SGCore::DX12Renderer::getLiveDevice();
    }
}

SGCore::DX12Texture2D::~DX12Texture2D() noexcept
{
    destroyOnGPU();
}

void SGCore::DX12Texture2D::create()
{
    auto* device = currentDevice();
    if(!device || !device->isReady()) return;
    if(m_width <= 0 || m_height <= 0) return;

    destroyOnGPU();

    // A texture buffer is not a 2D texture: GL exposes it as GL_TEXTURE_BUFFER, D3D12 as a typed
    // buffer SRV. As a texture its element count would be capped by the maximum texture dimension,
    // and the batch vertex buffer alone needs far more than that.
    if(m_type == SGTextureType::SG_TEXTURE_BUFFER)
    {
        createAsTexelBuffer();
        return;
    }

    DX12TextureDesc desc;
    desc.m_width = static_cast<std::uint32_t>(m_width);
    desc.m_height = static_cast<std::uint32_t>(m_height);
    desc.m_format = DX12TypesCaster::sggInternalFormatToDXGI(m_internalFormat);
    desc.m_debugName = "texture2d";
    m_dx12Texture = DX12Texture::create(device->getContextRef(), desc);
    if(!m_dx12Texture) return;

    if(m_textureData)
    {
        uploadRegion(m_textureData.get(), desc.m_width, desc.m_height, 0, 0);
    }
    else
    {
        // no pixels: still leave the image in the state shaders expect, with defined contents
        clearOnGPU(false);
    }
}

void SGCore::DX12Texture2D::createAsFrameBufferAttachment(IFrameBuffer* parentFrameBuffer, SGFrameBufferAttachmentType attachmentType)
{
    auto* device = currentDevice();
    if(!device || !device->isReady() || !parentFrameBuffer) return;

    destroyOnGPU();
    m_frameBufferAttachmentType = attachmentType;

    const auto format = DX12TypesCaster::sggInternalFormatToDXGI(m_internalFormat);
    const bool depth = isDepthAttachment(attachmentType) || isDepthStencilAttachment(attachmentType) ||
                       DX12TypesCaster::isDepthFormat(format);

    DX12TextureDesc desc;
    desc.m_width = static_cast<std::uint32_t>(std::max(1, m_width));
    desc.m_height = static_cast<std::uint32_t>(std::max(1, m_height));
    desc.m_format = format;
    desc.m_renderTarget = !depth;
    desc.m_depthStencil = depth;
    desc.m_samples = m_useMultisampling ? m_multisamplingSamplesCount : 1;
    // POINT filtering, as GL4Texture2D creates its attachments: later passes sample attachments
    // texel by texel and compare the result exactly (the layered FX gate gets its layer volume that
    // way), and an interpolated sample never equals the stored texel
    desc.m_filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    desc.m_addressMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    // the framebuffer address is part of the name: the engine has several framebuffers with the same
    // attachment slots, and identical names make a capture unreadable
    desc.m_debugName = "attachment_" + sgFrameBufferAttachmentTypeToString(attachmentType) + "@" +
                       std::to_string(reinterpret_cast<std::uintptr_t>(parentFrameBuffer) & 0xFFFFu) + "_" +
                       std::to_string(desc.m_width) + "x" + std::to_string(desc.m_height);
    m_dx12Texture = DX12Texture::create(device->getContextRef(), desc);
    if(!m_dx12Texture) return;

    // A fresh D3D12 resource holds whatever was in that memory, while a GL texture created without
    // data reads as zeros — and the engine relies on that: passes load their attachments
    // (LoadOp::SGG_LOAD) and the FX framebuffer's attachments are never cleared at all. Start cleared.
    clearOnGPU(depth);
}

void SGCore::DX12Texture2D::clearOnGPU(bool depth) noexcept
{
    auto* device = currentDevice();
    if(!device || !m_dx12Texture) return;

    device->immediateSubmit([&](ID3D12GraphicsCommandList* commandList) {
        if(depth)
        {
            m_dx12Texture->recordTransition(commandList, D3D12_RESOURCE_STATE_DEPTH_WRITE);
            // the far plane, not zero: a pass that forgets to clear depth should reject nothing
            commandList->ClearDepthStencilView(m_dx12Texture->getDSV(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
        }
        else
        {
            m_dx12Texture->recordTransition(commandList, D3D12_RESOURCE_STATE_RENDER_TARGET);
            const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            commandList->ClearRenderTargetView(m_dx12Texture->getRTV(), zero, 0, nullptr);
        }
        // rest in the shader-read state so an attachment can be sampled even before its first pass
        m_dx12Texture->recordTransition(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    });
}

void SGCore::DX12Texture2D::uploadRegion(const std::uint8_t* data, std::uint32_t width, std::uint32_t height,
                                         std::uint32_t x, std::uint32_t y) noexcept
{
    auto* device = currentDevice();
    if(!device || !m_dx12Texture || !data || width == 0 || height == 0) return;

    const auto imageLayout = DX12TypesCaster::formatLayout(m_dx12Texture->getFormat());
    const std::uint32_t texelSize = DX12TypesCaster::formatTexelSize(m_dx12Texture->getFormat());
    if(texelSize == 0 || !imageLayout.isPlain())
    {
        SG_LOG_E("DX12Texture2D: can not upload '{}': image format {} has no plain channel layout.",
                 m_dx12Texture->getDebugName(), static_cast<int>(m_dx12Texture->getFormat()));
        // an image with undefined contents is still bound and sampled by the passes
        clearOnGPU(false);
        return;
    }

    // a texture copy reads its source rows at a 256-byte aligned pitch, so the staging buffer is
    // built row by row rather than as one tight block
    const std::uint32_t sourceChannels = static_cast<std::uint32_t>(std::max(1, m_channelsCount));
    const std::uint32_t sourceChannelSize = std::max<std::uint32_t>(1, getSGGDataTypeSizeInBytes(m_dataType));
    const std::uint64_t sourceRowPitch = static_cast<std::uint64_t>(width) * sourceChannels * sourceChannelSize;
    const std::uint64_t rowPitch = (static_cast<std::uint64_t>(width) * texelSize + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1) &
                                   ~static_cast<std::uint64_t>(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1);

    auto staging = device->createStagingBuffer(rowPitch * height, "texture_upload");
    if(!staging) return;

    auto* destination = static_cast<std::uint8_t*>(staging->getMappedPointer());
    for(std::uint32_t row = 0; row < height; ++row)
    {
        PixelConversion::encodeToImage(data + row * sourceRowPitch, destination + row * rowPitch, width,
                                       m_dataType, sourceChannels, imageLayout, texelSize);
    }

    device->immediateSubmit([&](ID3D12GraphicsCommandList* commandList) {
        m_dx12Texture->recordTransition(commandList, D3D12_RESOURCE_STATE_COPY_DEST);

        D3D12_TEXTURE_COPY_LOCATION destinationLocation { };
        destinationLocation.pResource = m_dx12Texture->getResource();
        destinationLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        destinationLocation.SubresourceIndex = 0;

        D3D12_TEXTURE_COPY_LOCATION sourceLocation { };
        sourceLocation.pResource = staging->getResource();
        sourceLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        sourceLocation.PlacedFootprint.Footprint.Format = m_dx12Texture->getFormat();
        sourceLocation.PlacedFootprint.Footprint.Width = width;
        sourceLocation.PlacedFootprint.Footprint.Height = height;
        sourceLocation.PlacedFootprint.Footprint.Depth = 1;
        sourceLocation.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(rowPitch);

        commandList->CopyTextureRegion(&destinationLocation, x, y, 0, &sourceLocation, nullptr);
        m_dx12Texture->recordTransition(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    });
}

void SGCore::DX12Texture2D::subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight,
                                                     std::size_t areaOffsetX, std::size_t areaOffsetY) noexcept
{
    uploadRegion(data, static_cast<std::uint32_t>(areaWidth), static_cast<std::uint32_t>(areaHeight),
                 static_cast<std::uint32_t>(areaOffsetX), static_cast<std::uint32_t>(areaOffsetY));
}

void SGCore::DX12Texture2D::destroyOnGPU() noexcept
{
    // an ID3D12Resource keeps the device alive through COM, so simply dropping the reference is safe
    m_texelBuffer.reset();
    if(!m_dx12Texture) return;
    if(auto* device = currentDevice())
    {
        // the GPU may still read it: released with the next retired submission
        device->destroyDeferred(m_dx12Texture);
    }
    m_dx12Texture.reset();
}

void SGCore::DX12Texture2D::bind(const std::uint8_t& textureUnit) const noexcept
{
    // D3D12 has no texture units: record "unit N holds this texture" and let DX12Shader join it
    // with the sampler->unit half at draw time (see TextureUnits)
    if(m_texelBuffer)
    {
        TextureUnits::set(textureUnit, m_texelBuffer);
        return;
    }
    TextureUnits::set(textureUnit, m_dx12Texture);
}

void* SGCore::DX12Texture2D::getTextureNativeHandler() const noexcept
{
    return m_dx12Texture.get();
}

void* SGCore::DX12Texture2D::getTextureBufferNativeHandler() const noexcept
{
    return nullptr;
}

SGCore::DX12Texture2D& SGCore::DX12Texture2D::operator=(const Ref<ITexture2D>& other)
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


void SGCore::DX12Texture2D::createAsTexelBuffer() noexcept
{
    auto* device = currentDevice();
    if(!device) return;

    const std::uint8_t texelSize = getSGGInternalFormatChannelsSizeInBytes(m_internalFormat);
    const std::uint64_t elements = static_cast<std::uint64_t>(m_width) * m_height;
    const std::uint64_t bytes = elements * texelSize;
    if(bytes == 0) return;

    GPUBufferDesc desc;
    desc.m_usage = GPUBufferUsage::SGG_STORAGE_BUFFER | GPUBufferUsage::SGG_TRANSFER_DST;
    desc.m_access = GPUMemoryAccess::SGG_HOST_VISIBLE;
    desc.m_size = bytes;
    desc.m_debugName = "texture_buffer";

    auto buffer = device->createBuffer(desc);
    if(!buffer) return;

    m_texelBuffer = std::static_pointer_cast<DX12GPUBuffer>(buffer);
    m_texelBuffer->setTexelFormat(DX12TypesCaster::sggInternalFormatToDXGITexel(m_internalFormat),
                                  static_cast<std::uint32_t>(elements));

    if(m_textureData) m_texelBuffer->write(m_textureData.get(), bytes);
}

void SGCore::DX12Texture2D::subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept
{
    if(!m_texelBuffer || !m_textureData) return;

    m_texelBuffer->write(m_textureData.get() + bytesOffset, bytesCount, bytesOffset);
}

#endif
