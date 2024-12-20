//
// Created by stuka on 28.05.2023.
//

#ifndef SUNGEARENGINE_GL4TEXTURE2D_H
#define SUNGEARENGINE_GL4TEXTURE2D_H

#include "SGCore/Graphics/API/ITexture2D.h"

#include <SGCore/pch.h>

namespace SGCore
{
    class GL4Texture2D : public ITexture2D
    {
    protected:
        GLuint m_textureHandler = 0;
        GLuint m_textureBufferHandler = 0;

    public:
        friend class AssetManager;

        ~GL4Texture2D() noexcept override;

        void create() noexcept override;
        void createAsFrameBufferAttachment(const Ref<IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) override;
        
        void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept override;
        void subTextureDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept override;
        
        void destroy() noexcept override;

        void bind(const std::uint8_t& textureUnit) const noexcept override;
        
        void* getTextureNativeHandler() const noexcept override;
        void* getTextureBufferNativeHandler() const noexcept override;

        [[nodiscard]] glm::vec3 pickColor(const glm::vec2& inPosition) const noexcept final;
        
        GL4Texture2D& operator=(const Ref<ITexture2D>& other) override;
    };
}

#endif //SUNGEARENGINE_GL4TEXTURE2D_H
