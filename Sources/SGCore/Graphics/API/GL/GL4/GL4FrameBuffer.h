//
// Created by stuka on 23.07.2023.
//

#ifndef SUNGEARENGINE_GL4FRAMEBUFFER_H
#define SUNGEARENGINE_GL4FRAMEBUFFER_H

#include <SGCore/pch.h>

#include "SGCore/Graphics/API/IFrameBuffer.h"

namespace SGCore
{
    class GL4FrameBuffer : public IFrameBuffer
    {
    private:
        GLuint m_handler = 0;

    public:
        void bindAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                                     const std::uint8_t& textureBlock) override;

        void bindAttachmentToReadFrom(const SGFrameBufferAttachmentType& attachmentType) override;
        void bindAttachmentToDrawIn(const SGFrameBufferAttachmentType& attachmentType) override;

        /**
         * OPENGL DOES NOT SUPPORT THIS.
         * @param attachmentsTypes
         * @return
         */
        void bindAttachmentsToReadFrom(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) override;
        void bindAttachmentsToDrawIn(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) override;
        void bindAttachmentsToDrawIn(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes) override;

        void unbindAttachmentToReadFrom() override;
        void unbindAttachmentToDrawIn() override;

        void bind() const override;
        void unbind() const override;

        void create() override;
        void destroy() override;
        
        void clear() override;
        void clearAttachment(const SGFrameBufferAttachmentType& attachmentType) override;
        
        void addAttachment(SGFrameBufferAttachmentType,
                           SGGColorFormat format,
                           SGGColorInternalFormat internalFormat,
                           const int& mipLevel,
                           const int& layer) override;
        
        void addAttachment(SGFrameBufferAttachmentType attachmentType,
                           SGGColorFormat format,
                           SGGColorInternalFormat internalFormat,
                           const int& mipLevel,
                           const int& layer,
                           bool useMultisampling,
                           std::uint8_t multisamplingSamplesCount) override;

        void attachAttachment(const Ref<ITexture2D>& otherAttachment) noexcept override;

        [[nodiscard]] glm::vec3 readPixelsFromAttachment(const glm::vec2& mousePos, SGFrameBufferAttachmentType attachmentType) const noexcept final;
    };
}

#endif //SUNGEARENGINE_GL4FRAMEBUFFER_H
