//
// Created by stuka on 28.05.2023.
//

#ifndef SUNGEARENGINE_GL4TEXTURE2D_H
#define SUNGEARENGINE_GL4TEXTURE2D_H

#include <glad/glad.h>

#include "SGCore/Graphics/API/ITexture2D.h"

namespace SGCore
{
    class GL4Texture2D : public ITexture2D
    {
    public:
        friend class AssetManager;
        friend struct GL4Renderer;

        ~GL4Texture2D() noexcept override;

        void create() noexcept override;
        void createAsFrameBufferAttachment(IFrameBuffer* parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;
        
        void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept override;
        void subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY) noexcept override;
        
        void destroyOnGPU() noexcept override;

        void bind(const std::uint8_t& textureUnit) const noexcept override;
        
        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;
        
        GL4Texture2D& operator=(const Ref<ITexture2D>& other) override;

    protected:
        GLuint m_textureHandler = 0;
        GLuint m_textureBufferHandler = 0;

        GL4Texture2D() noexcept = default;
    };
}

#endif //SUNGEARENGINE_GL4TEXTURE2D_H
