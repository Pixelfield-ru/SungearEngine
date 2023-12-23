//
// Created by stuka on 23.07.2023.
//

#include "GL4FrameBuffer.h"

#include "SGCore/Logging/Log.h"
#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"
#include "SGCore/Main/CoreMain.h"
#include "GL4Texture2D.h"

#include "SGCore/Graphics/API/GL/DeviceGLInfo.h"

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachment
(const SGFrameBufferAttachmentType& attachmentType, const std::uint8_t& textureBlock)
{
    GLenum glTextureType = GL_TEXTURE_2D;
    GLint handler = -1;
    auto foundAttachment = m_depthAttachments.end();

    if(isDepthAttachment(attachmentType))
    {
        foundAttachment = m_depthAttachments.find(attachmentType);
    }
    else if(isDepthStencilAttachment(attachmentType))
    {
        foundAttachment = m_depthStencilAttachments.find(attachmentType);
    }
    else if(isColorAttachment(attachmentType))
    {
        foundAttachment = m_colorAttachments.find(attachmentType);
    }
    else if(isRenderAttachment(attachmentType))
    {
        foundAttachment = m_renderAttachments.find(attachmentType);
    }

    if(foundAttachment != m_depthAttachments.end() || foundAttachment != m_depthStencilAttachments.end() ||
       foundAttachment != m_colorAttachments.end() || foundAttachment != m_renderAttachments.end())
    {
        const auto& attachment = foundAttachment->second;

        handler = attachment.m_handler;
        glTextureType = !attachment.m_useMultisampling ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;

        glActiveTexture(GL_TEXTURE0 + textureBlock);
        glBindTexture(glTextureType, handler);
    }

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentToRead
(const SGFrameBufferAttachmentType& attachmentType)
{
    if(attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
       attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + (attachmentType - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
    }

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentToDraw
(const SGFrameBufferAttachmentType& attachmentType)
{
    if(attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
       attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
    {
        glDrawBuffer(GL_COLOR_ATTACHMENT0 + (attachmentType - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
    }

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentsToRead
(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentsToDraw
(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    GLenum attachmentsToBind[attachmentsTypes.size()];

    std::uint8_t curAttachment = 0;
    for(const auto& type: attachmentsTypes)
    {
        if(type >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
           type <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
        {
            attachmentsToBind[curAttachment] = GL_COLOR_ATTACHMENT0 + (type - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0);
        }

        ++curAttachment;
    }

    glDrawBuffers(attachmentsTypes.size(), attachmentsToBind);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bindAttachmentsToDraw
(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    GLenum attachmentsToBind[attachmentsTypes.size()];

    std::uint8_t curAttachment = 0;
    for(const auto& type: attachmentsTypes)
    {
        if(type >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
           type <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
        {
            attachmentsToBind[curAttachment] = GL_COLOR_ATTACHMENT0 + (type - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0);
        }

        ++curAttachment;
    }

    glDrawBuffers(attachmentsTypes.size(), attachmentsToBind);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::unbindAttachmentToRead()
{
    glReadBuffer(GL_NONE);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::unbindAttachmentToDraw()
{
    glDrawBuffer(GL_NONE);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::bind()
{
    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handler);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int wndWidth;
    int wndHeight;
    CoreMain::getWindow().getSize(wndWidth, wndHeight);
    glViewport(0, 0, wndWidth, wndHeight);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::create()
{
    glGenFramebuffers(1, &m_handler);

    glBindFramebuffer(GL_FRAMEBUFFER, m_handler);

    return shared_from_this();
}

void SGCore::GL4FrameBuffer::destroy()
{
    glDeleteFramebuffers(1, &m_handler);
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::clear()
{
    glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::GL4FrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
                                                                            SGGColorFormat format,
                                                                            SGGColorInternalFormat internalFormat,
                                                                            const int& mipLevel,
                                                                            const int& layer,
                                                                            bool useMultisampling,
                                                                            std::uint8_t multisamplingSamplesCount)
{
    // TODO: MAKE VERIFY INTERNAL FORMAT
    // TODO: MAKE VERIFY SIZE TYPE

    if(isDepthAttachment(attachmentType))
    {
        if(m_depthAttachments.find(attachmentType) != m_depthAttachments.end())
        {
            SGCF_ERROR("Error when adding an attachment to the framebuffer: "
                       "an attachment with this type already exists.", SG_LOG_GAPI_FILE);

            return shared_from_this();
        }

        auto& newAttachment = m_depthAttachments[attachmentType];

        newAttachment.m_format = format;
        newAttachment.m_internalFormat = internalFormat;
        newAttachment.m_mipLevel = mipLevel;
        newAttachment.m_layer = layer;
        newAttachment.m_useMultisampling = useMultisampling;
        newAttachment.m_multisamplingSamplesCount = multisamplingSamplesCount;

        glGenTextures(1, &newAttachment.m_handler);
        glBindTexture(GL_TEXTURE_2D, newAttachment.m_handler);

        glTexImage2D(GL_TEXTURE_2D,
                     mipLevel,
                     GLGraphicsTypesCaster::sggInternalFormatToGL(internalFormat),
                     m_width, m_height,
                     0,
                     GLGraphicsTypesCaster::sggFormatToGL(format),
                // todo: make customizable
                     GL_FLOAT,
                     nullptr
        );

        // TODO: make it customizable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // todo: make it customizable
        float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D, newAttachment.m_handler, mipLevel
        );
    }
    else if(attachmentType >= SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT0 &&
            attachmentType <= SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT9)
    {
        if(m_depthStencilAttachments.find(attachmentType) != m_depthStencilAttachments.end())
        {
            SGCF_ERROR("Error when adding an attachment to the framebuffer: "
                       "an attachment with this type already exists.", SG_LOG_GAPI_FILE);

            return shared_from_this();
        }

        auto& newAttachment = m_depthStencilAttachments[attachmentType];

        newAttachment.m_format = format;
        newAttachment.m_internalFormat = internalFormat;
        newAttachment.m_mipLevel = mipLevel;
        newAttachment.m_layer = layer;
        newAttachment.m_useMultisampling = useMultisampling;
        newAttachment.m_multisamplingSamplesCount = multisamplingSamplesCount;

        glGenTextures(1, &newAttachment.m_handler);
        glBindTexture(GL_TEXTURE_2D, newAttachment.m_handler);

        glTexImage2D(GL_TEXTURE_2D,
                     mipLevel,
                     GLGraphicsTypesCaster::sggInternalFormatToGL(internalFormat),
                     m_width, m_height,
                     0,
                     GLGraphicsTypesCaster::sggFormatToGL(format),
                // todo: make customizable
                     GL_UNSIGNED_INT_24_8,
                     nullptr
        );

        // TODO: make it customizable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // todo: make it customizable
        //float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_STENCIL_ATTACHMENT,
                               GL_TEXTURE_2D, newAttachment.m_handler, mipLevel);
    }
    else if(attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
            attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
    {
        if(m_colorAttachments.size() >= DeviceGLInfo::getMaxFBColorAttachments())
        {
            SGCF_ERROR(
                    "It is not possible to add more color attachments for framebuffer. Current color attachments count: " +
                    std::to_string(m_colorAttachments.size()) + ". Max color attachments count: " +
                    std::to_string(DeviceGLInfo::getMaxFBColorAttachments()), SG_LOG_GAPI_FILE);

            return shared_from_this();
        }

        if(m_colorAttachments.find(attachmentType) != m_colorAttachments.end())
        {
            SGCF_ERROR("Error when adding an attachment to the framebuffer: "
                       "an attachment with this type already exists.", SG_LOG_GAPI_FILE);

            return shared_from_this();
        }

        auto& newAttachment = m_colorAttachments[attachmentType];

        newAttachment.m_format = format;
        newAttachment.m_internalFormat = internalFormat;
        newAttachment.m_mipLevel = mipLevel;
        newAttachment.m_layer = layer;
        newAttachment.m_useMultisampling = useMultisampling;
        newAttachment.m_multisamplingSamplesCount = multisamplingSamplesCount;

        GLenum glType = !useMultisampling ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;

        glGenTextures(1, &newAttachment.m_handler);
        glBindTexture(glType, newAttachment.m_handler);

        if(!useMultisampling)
        {
            glTexImage2D(glType,
                         mipLevel,
                         GLGraphicsTypesCaster::sggInternalFormatToGL(internalFormat),
                         m_width, m_height,
                         0,
                         GLGraphicsTypesCaster::sggFormatToGL(format),
                         GL_FLOAT,
                         nullptr
            );
        }
        else
        {
            glTexImage2DMultisample(glType,
                                    multisamplingSamplesCount,
                                    GLGraphicsTypesCaster::sggFormatToGL(format),
                                    m_width, m_height,
                                    GL_FALSE
            );
        }

        // TODO: make it customizable
        glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + (attachmentType - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0),
                               glType, newAttachment.m_handler, mipLevel);
    }
    else if(attachmentType >= SGFrameBufferAttachmentType::SGG_RENDER_ATTACHMENT0 &&
            attachmentType <= SGFrameBufferAttachmentType::SGG_RENDER_ATTACHMENT9)
    {
        // todo: make
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        SGCF_ERROR("Error when adding attachment to framebuffer: framebuffer is not completed", SG_LOG_GAPI_FILE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer>
SGCore::GL4FrameBuffer::addAttachment(SGFrameBufferAttachmentType attachmentType,
                                      SGGColorFormat format,
                                      SGGColorInternalFormat internalFormat,
                                      const int& mipLevel,
                                      const int& layer)
{
    return addAttachment(attachmentType, format, internalFormat, mipLevel, layer, false, 8);
}