//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include "SGCore/Graphics/API/ICubemapTexture.h"
#include "RHI/DX12Texture.h"

namespace SGCore
{
    /// Legacy ICubemapTexture facade: the six m_parts are uploaded into the array slices of one cube
    /// texture. Like DX12Texture2D, bind() only records the texture unit — the descriptor table is
    /// assembled by the shader facade at draw time.
    class DX12CubemapTexture : public ICubemapTexture
    {
    public:
        ~DX12CubemapTexture() noexcept override;

        void create() override;
        void createAsFrameBufferAttachment(IFrameBuffer* parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;
        void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) override;
        void subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight,
                                      std::size_t areaOffsetX, std::size_t areaOffsetY) override;
        void destroyOnGPU() override;
        void bind(const std::uint8_t& textureUnit) const override;
        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;
        DX12CubemapTexture& operator=(const Ref<ITexture2D>& other) override;

        [[nodiscard]] const Ref<DX12Texture>& getDX12Texture() const noexcept { return m_dx12Texture; }

    private:
        Ref<DX12Texture> m_dx12Texture;
    };
}

#endif
