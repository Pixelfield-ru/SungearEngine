//
// Created by stuka on 29.08.2023.
//

#ifndef SUNGEARENGINE_GL4CUBEMAPTEXTURE_H
#define SUNGEARENGINE_GL4CUBEMAPTEXTURE_H

#include <SGCore/pch.h>

#include "SGCore/Graphics/API/ICubemapTexture.h"

namespace SGCore
{
    class GL4CubemapTexture : public ICubemapTexture
    {
    public:
        void create() override;
        void createAsFrameBufferAttachment(const Ref<IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;
        
        void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) override;
        void subTextureDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) override;
        
        void destroy() override;
        
        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;
        
        void bind(const std::uint8_t& textureUnit) const override;

        [[nodiscard]] glm::vec3 pickColor(const glm::vec2& inPosition) const noexcept final;

        GL4CubemapTexture& operator=(const Ref<ITexture2D>& other) override;

    private:
        GLuint m_cubemapHandler = 0;
    };
}

#endif // SUNGEARENGINE_GL4CUBEMAPTEXTURE_H
