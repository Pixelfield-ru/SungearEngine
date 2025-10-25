//
// Created by stuka on 29.08.2023.
//

#ifndef SUNGEARENGINE_GL4CUBEMAPTEXTURE_H
#define SUNGEARENGINE_GL4CUBEMAPTEXTURE_H

#include <glad/glad.h>

#include "SGCore/Graphics/API/ICubemapTexture.h"

namespace SGCore
{
    class GL4CubemapTexture : public ICubemapTexture
    {
    public:
        void create() override;
        void createAsFrameBufferAttachment(const Ref<IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;
        
        void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) override;
        void subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY) override;
        
        void destroyOnGPU() override;
        
        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;
        
        void bind(const std::uint8_t& textureUnit) const override;

        GL4CubemapTexture& operator=(const Ref<ITexture2D>& other) override;

    private:
        GLuint m_cubemapHandler = 0;
    };
}

#endif // SUNGEARENGINE_GL4CUBEMAPTEXTURE_H
