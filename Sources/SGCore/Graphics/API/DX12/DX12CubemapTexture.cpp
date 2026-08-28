//
// Created by 8bitniksis on 20.08.2026.
//

#include "DX12CubemapTexture.h"

#if defined(_WIN32)

#include <algorithm>
#include <cstring>

#include "DX12Renderer.h"
#include "DX12TypesCaster.h"
#include "RHI/DX12Device.h"
#include "RHI/DX12GPUBuffer.h"
#include "SGCore/Graphics/RHI/TextureUnits.h"
#include "SGCore/Logger/Logger.h"

namespace
{
    SGCore::DX12Device* currentDevice() noexcept
    {
        return SGCore::DX12Renderer::getLiveDevice();
    }
}

SGCore::DX12CubemapTexture::~DX12CubemapTexture() noexcept
{
    DX12CubemapTexture::destroyOnGPU();
}

void SGCore::DX12CubemapTexture::create()
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
        SG_LOG_E("DX12CubemapTexture: no face carries a size, the cubemap stays empty.");
        return;
    }

    DX12TextureDesc desc;
    desc.m_width = static_cast<std::uint32_t>(reference->getWidth());
    desc.m_height = static_cast<std::uint32_t>(reference->getHeight());
    desc.m_format = DX12TypesCaster::sggInternalFormatToDXGI(reference->m_internalFormat);
    desc.m_cube = true;
    // a skybox must not show seams at the face edges
    desc.m_addressMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    desc.m_debugName = "cubemap";
    m_dx12Texture = DX12Texture::create(device->getContextRef(), desc);
    if(!m_dx12Texture) return;

    const auto imageLayout = DX12TypesCaster::formatLayout(desc.m_format);
    const std::uint32_t texelSize = DX12TypesCaster::formatTexelSize(desc.m_format);
    if(texelSize == 0 || !imageLayout.isPlain())
    {
        SG_LOG_E("DX12CubemapTexture: image format {} has no plain channel layout.", static_cast<int>(desc.m_format));
        return;
    }

    const std::uint64_t rowPitch = (static_cast<std::uint64_t>(desc.m_width) * texelSize + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1) &
                                   ~static_cast<std::uint64_t>(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1);

    device->immediateSubmit([&](ID3D12GraphicsCommandList* commandList) {
        m_dx12Texture->recordTransition(commandList, D3D12_RESOURCE_STATE_COPY_DEST);
    });

    // upload every face that has pixels; faces without data stay as created
    for(std::size_t face = 0; face < m_parts.size() && face < 6; ++face)
    {
        const auto& part = m_parts[face];
        if(!part || !part->getData()) continue;

        const std::uint32_t partChannels = static_cast<std::uint32_t>(std::max(1, part->m_channelsCount));
        const std::uint32_t channelSize = std::max<std::uint32_t>(1, getSGGDataTypeSizeInBytes(part->m_dataType));
        const std::uint64_t sourceRowPitch = static_cast<std::uint64_t>(desc.m_width) * partChannels * channelSize;

        auto staging = device->createStagingBuffer(rowPitch * desc.m_height, "cubemap_face_upload");
        if(!staging) continue;

        auto* destination = static_cast<std::uint8_t*>(staging->getMappedPointer());
        const auto* source = part->getData();
        // skybox faces are RGB while DXGI has no 8-bit RGB format: the shared converter widens them
        for(std::uint32_t row = 0; row < desc.m_height; ++row)
        {
            PixelConversion::encodeToImage(source + row * sourceRowPitch, destination + row * rowPitch, desc.m_width,
                                           part->m_dataType, partChannels, imageLayout, texelSize);
        }

        device->immediateSubmit([&](ID3D12GraphicsCommandList* commandList) {
            D3D12_TEXTURE_COPY_LOCATION destinationLocation { };
            destinationLocation.pResource = m_dx12Texture->getResource();
            destinationLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            // one mip level per face: the subresource index is the face index
            destinationLocation.SubresourceIndex = static_cast<UINT>(face);

            D3D12_TEXTURE_COPY_LOCATION sourceLocation { };
            sourceLocation.pResource = staging->getResource();
            sourceLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            sourceLocation.PlacedFootprint.Footprint.Format = desc.m_format;
            sourceLocation.PlacedFootprint.Footprint.Width = desc.m_width;
            sourceLocation.PlacedFootprint.Footprint.Height = desc.m_height;
            sourceLocation.PlacedFootprint.Footprint.Depth = 1;
            sourceLocation.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(rowPitch);

            commandList->CopyTextureRegion(&destinationLocation, 0, 0, 0, &sourceLocation, nullptr);
        });
    }

    device->immediateSubmit([&](ID3D12GraphicsCommandList* commandList) {
        m_dx12Texture->recordTransition(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    });
}

void SGCore::DX12CubemapTexture::createAsFrameBufferAttachment(IFrameBuffer*, SGFrameBufferAttachmentType)
{
    // cube maps are not used as framebuffer attachments by the engine's passes
}

void SGCore::DX12CubemapTexture::subTextureBufferDataOnGAPISide(const size_t&, const size_t&)
{
}

void SGCore::DX12CubemapTexture::subTextureDataOnGAPISide(const std::uint8_t*, std::size_t, std::size_t, std::size_t, std::size_t)
{
    // faces are uploaded whole in create(); partial updates are not used for cube maps
}

void SGCore::DX12CubemapTexture::destroyOnGPU()
{
    if(!m_dx12Texture) return;
    if(auto* device = currentDevice()) device->destroyDeferred(m_dx12Texture);
    m_dx12Texture = nullptr;
}

void SGCore::DX12CubemapTexture::bind(const std::uint8_t& textureUnit) const
{
    // same unit model as DX12Texture2D: the shader facade joins unit and sampler at draw time
    TextureUnits::set(textureUnit, m_dx12Texture);
}

void* SGCore::DX12CubemapTexture::getTextureNativeHandler() const noexcept
{
    return m_dx12Texture.get();
}

void* SGCore::DX12CubemapTexture::getTextureBufferNativeHandler() const noexcept
{
    return nullptr;
}

SGCore::DX12CubemapTexture& SGCore::DX12CubemapTexture::operator=(const Ref<ITexture2D>& other)
{
    if(!other) return *this;
    m_type = other->m_type;
    m_internalFormat = other->m_internalFormat;
    m_format = other->m_format;
    m_dataType = other->m_dataType;
    m_channelsCount = other->m_channelsCount;
    return *this;
}

#endif
