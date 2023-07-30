//
// Created by stuka on 23.07.2023.
//

#ifndef SUNGEARENGINE_GL4FRAMEBUFFER_H
#define SUNGEARENGINE_GL4FRAMEBUFFER_H

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/GL/GLFrameBufferAttachment.h"

#include <glad/glad.h>
#include <map>

namespace Core::Graphics
{
    class GL4FrameBuffer : public IFrameBuffer
    {
    private:
        static inline GLint maxColorAttachments = 0;

        GLuint m_handler = 0;
        // first - name
        std::map<std::string, GLFrameBufferAttachment> m_attachments;

    public:
        std::shared_ptr<IFrameBuffer> bind() override;
        std::shared_ptr<IFrameBuffer> unbind() override;

        std::shared_ptr<IFrameBuffer> create() override;
        void destroy() override;

        std::shared_ptr<IFrameBuffer> addAttachment(const SGFrameBufferAttachmentType&,
                                                    const std::string& name,
                                                    const int& width, const int& height,
                                                    const SGGColorFormat& format, const SGGColorInternalFormat& internalFormat,
                                                    const int& mipLevel,
                                                    const int& layer) override;
    };
}

#endif //SUNGEARENGINE_GL4FRAMEBUFFER_H
