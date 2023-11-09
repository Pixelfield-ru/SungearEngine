//
// Created by stuka on 23.07.2023.
//

#ifndef SUNGEARENGINE_GL4FRAMEBUFFER_H
#define SUNGEARENGINE_GL4FRAMEBUFFER_H

#include <tsl/robin_map.h>
#include <glad/glad.h>
#include <map>

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/GL/GLFrameBufferAttachment.h"
#include "SGCore/Graphics/API/ITexture2D.h"

namespace Core::Graphics
{
    class GL4FrameBuffer : public IFrameBuffer
    {
    private:
        static inline GLint maxColorAttachments = 0;

        GLuint m_handler = 0;
        // first - name
        std::unordered_map<SGFrameBufferAttachmentType, GLFrameBufferAttachment> m_attachments;

    public:
        std::shared_ptr<IFrameBuffer> bindAttachments(const MarkedFrameBufferAttachmentsBlock& markedFrameBufferAttachmentsBlock) override;

        std::shared_ptr<IFrameBuffer> bindAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                                     const std::uint8_t& textureBlock) override;

        std::shared_ptr<IFrameBuffer> bindAttachmentToRead() override;
        std::shared_ptr<IFrameBuffer> bindAttachmentToDraw() override;

        std::shared_ptr<IFrameBuffer> unbindAttachmentToRead() override;
        std::shared_ptr<IFrameBuffer> unbindAttachmentToDraw() override;

        std::shared_ptr<IFrameBuffer> bind() override;
        std::shared_ptr<IFrameBuffer> unbind() override;

        std::shared_ptr<IFrameBuffer> create() override;
        void destroy() override;

        std::shared_ptr<IFrameBuffer> clear() override;

        std::shared_ptr<IFrameBuffer> addAttachment(const SGFrameBufferAttachmentType&,
                                                    const SGGColorFormat& format, const SGGColorInternalFormat& internalFormat,
                                                    const int& mipLevel,
                                                    const int& layer) override;
    };
}

#endif //SUNGEARENGINE_GL4FRAMEBUFFER_H
