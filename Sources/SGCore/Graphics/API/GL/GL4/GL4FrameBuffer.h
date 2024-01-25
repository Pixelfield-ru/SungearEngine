//
// Created by stuka on 23.07.2023.
//

#ifndef SUNGEARENGINE_GL4FRAMEBUFFER_H
#define SUNGEARENGINE_GL4FRAMEBUFFER_H

#include <glad/glad.h>
#include <map>

#include "SGCore/Graphics/API/IFrameBuffer.h"

namespace SGCore
{
    class GL4FrameBuffer : public IFrameBuffer
    {
    private:
        GLuint m_handler = 0;

    public:
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
    };
}

#endif //SUNGEARENGINE_GL4FRAMEBUFFER_H
