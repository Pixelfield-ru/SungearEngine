//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include "SGCore/Graphics/API/ITexture2D.h"
#include "RHI/DX12Texture.h"
#include "RHI/DX12GPUBuffer.h"

namespace SGCore
{
    /// Legacy ITexture2D facade over a DX12Texture, the twin of VkTexture2D. Unit-model bind() only
    /// records "unit N holds this texture": the texture reaches shaders through a descriptor table
    /// (see docs/RHI_DESIGN.md, textures).
    class DX12Texture2D : public ITexture2D
    {
    public:
        ~DX12Texture2D() noexcept override;

        void create() final;
        void createAsFrameBufferAttachment(IFrameBuffer* parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;

        void destroyOnGPU() noexcept final;

        void bind(const std::uint8_t& textureUnit) const noexcept final;

        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;

        DX12Texture2D& operator=(const Ref<ITexture2D>& other) final;

        [[nodiscard]] const Ref<DX12Texture>& getDX12Texture() const noexcept { return m_dx12Texture; }

    private:
        void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept override;

        /// SG_TEXTURE_BUFFER path: a typed buffer with an SRV, not a 2D texture. An image would cap the
        /// element count at the maximum texture dimension, and the batch buffers exceed it.
        void createAsTexelBuffer() noexcept;

        void subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight,
                                      std::size_t areaOffsetX, std::size_t areaOffsetY) noexcept override;

        /// Copies pixels (in the texture's declared format) into the image region and leaves it in
        /// PIXEL_SHADER_RESOURCE. Converts into the image's channel layout on the way, as the
        /// Vulkan facade does — an explicit API copies bytes, glTexImage2D converted them.
        void uploadRegion(const std::uint8_t* data, std::uint32_t width, std::uint32_t height, std::uint32_t x, std::uint32_t y) noexcept;
        /// Gives the image defined contents (zeros / far depth) and the state shaders expect.
        void clearOnGPU(bool depth) noexcept;

        Ref<DX12Texture> m_dx12Texture;
        /// Set instead of m_dx12Texture when m_type == SG_TEXTURE_BUFFER.
        Ref<DX12GPUBuffer> m_texelBuffer;
    };
}

#endif
