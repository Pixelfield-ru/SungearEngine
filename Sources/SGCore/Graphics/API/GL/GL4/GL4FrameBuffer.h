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

namespace SGCore
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

        std::shared_ptr<IFrameBuffer> bindAttachmentToRead(const SGFrameBufferAttachmentType& attachmentType) override;
        std::shared_ptr<IFrameBuffer> bindAttachmentToDraw(const SGFrameBufferAttachmentType& attachmentType) override;

        /**
         * OPENGL DOES NOT SUPPORT THIS.
         * @param attachmentsTypes
         * @return
         */
        std::shared_ptr<IFrameBuffer> bindAttachmentsToRead(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) override;
        std::shared_ptr<IFrameBuffer> bindAttachmentsToDraw(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) override;
        std::shared_ptr<IFrameBuffer> bindAttachmentsToDraw(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes) override;

        std::shared_ptr<IFrameBuffer> unbindAttachmentToRead() override;
        std::shared_ptr<IFrameBuffer> unbindAttachmentToDraw() override;

        std::shared_ptr<IFrameBuffer> bind() override;
        std::shared_ptr<IFrameBuffer> unbind() override;

        std::shared_ptr<IFrameBuffer> create() override;
        void destroy() override;

        std::shared_ptr<IFrameBuffer> clear() override;

        std::shared_ptr<IFrameBuffer> addAttachment(SGFrameBufferAttachmentType,
                                                    SGGColorFormat format,
                                                    SGGColorInternalFormat internalFormat,
                                                    const int& mipLevel,
                                                    const int& layer) override;

        std::shared_ptr<IFrameBuffer> addAttachment(SGFrameBufferAttachmentType attachmentType,
                                                    SGGColorFormat format,
                                                    SGGColorInternalFormat internalFormat,
                                                    const int& mipLevel,
                                                    const int& layer,
                                                    bool useMultisampling,
                                                    std::uint8_t multisamplingSamplesCount) override;

        void getAttachmentsCount(std::uint16_t& depthAttachmentsCount,
                                 std::uint16_t& depthStencilAttachmentsCount,
                                 std::uint16_t& colorAttachmentsCount,
                                 std::uint16_t& renderAttachmentsCount) const noexcept override;
    };
}

#endif //SUNGEARENGINE_GL4FRAMEBUFFER_H
