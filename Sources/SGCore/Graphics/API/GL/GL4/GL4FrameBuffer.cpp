//
// Created by stuka on 23.07.2023.
//

#include "GL4FrameBuffer.h"

#include "SGCore/Logging/Log.h"
#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"
#include "SGCore/Main/CoreMain.h"

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::bindAttachment(const std::string& attachmentName)
{
    glActiveTexture(GL_TEXTURE0 + 30);
    glBindTexture(GL_TEXTURE_2D, m_attachments[attachmentName].m_handler);

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::bind()
{
    glViewport(0, 0, 1024 * 2, 1024 * 2);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handler);
    //glActiveTexture(GL_TEXTURE0);

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    int wndWidth;
    int wndHeight;
    Main::CoreMain::getWindow().getSize(wndWidth, wndHeight);
    glViewport(0, 0, wndWidth, wndHeight);

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::create()
{
    static bool staticInit = []() {
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
        SGCF_INFO("OpenGL max color attachments: " + std::to_string(maxColorAttachments), SG_LOG_GAPI_FILE);
        return true;
    }();

    glGenFramebuffers(1, &m_handler);

    glBindFramebuffer(GL_FRAMEBUFFER, m_handler);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    return shared_from_this();
}

void Core::Graphics::GL4FrameBuffer::destroy()
{
    glDeleteFramebuffers(1, &m_handler);
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::clear()
{
    glClear(GL_DEPTH_BUFFER_BIT);

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer>
Core::Graphics::GL4FrameBuffer::addAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                              const std::string& name,
                                              const int &width, const int& height,
                                              const SGGColorFormat& format,
                                              const SGGColorInternalFormat& internalFormat,
                                              const int& mipLevel,
                                              const int& layer)
{
    if(m_attachments.find(name) != m_attachments.end())
    {
        SGCF_ERROR("Error when adding an attachment '" + name + "' to the framebuffer: "
                   "an attachment with this name already exists.", SG_LOG_GAPI_FILE);

        return shared_from_this();
    }

    // TODO: MAKE VERIFY INTERNAL FORMAT
    // TODO: MAKE VERIFY SIZE TYPE

    switch(attachmentType)
    {
        case SGG_DEPTH_STENCIL_ATTACHMENT:
        {

            break;
        }

        case SGG_DEPTH_ATTACHMENT:
        {
            bool depthAttachmentExists = false;

            // finding depth attachment
            for(const auto& attachment : m_attachments)
            {
                if(attachment.second.m_type == SGG_DEPTH_ATTACHMENT)
                {
                    depthAttachmentExists = true;

                    SGCF_ERROR("Error adding depth attachment '" + name + "' to framebuffer: attachment already exists.", SG_LOG_GAPI_FILE);

                    break;
                }
            }

            // adding new depth attachment
            if(!depthAttachmentExists)
            {
                auto& newAttachment = m_attachments[name];

                newAttachment.m_type = attachmentType;

                newAttachment.m_name = name;
                newAttachment.m_ID = 0;
                newAttachment.m_width = width;
                newAttachment.m_height = height;
                newAttachment.m_format = format;
                newAttachment.m_internalFormat = internalFormat;
                newAttachment.m_mipLevel = mipLevel;
                newAttachment.m_layer = layer;

                glGenTextures(1, &newAttachment.m_handler);
                glBindTexture(GL_TEXTURE_2D, newAttachment.m_handler);

                glTexImage2D(GL_TEXTURE_2D,
                             mipLevel,
                             GLGraphicsTypesCaster::sggInternalFormatToGL(internalFormat),
                             width, height,
                             0,
                             GL_DEPTH_COMPONENT,
                             // todo: make customizable
                             GL_FLOAT,
                             nullptr);

                // TODO: make it customizable
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER    );

                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                       GL_DEPTH_ATTACHMENT,
                                       GL_TEXTURE_2D, newAttachment.m_handler, mipLevel);
            }

            break;
        }

        case SGG_COLOR_ATTACHMENT:
        {
            if(m_attachments.size() >= maxColorAttachments)
            {
                SGCF_ERROR("It is not possible to add a new color attachment '" + name + "' to the framebuffer: there are no free blocks.",
                           SG_LOG_GAPI_FILE);

                break;
            }

            int foundAttachment = -1;

            // find free attachment id
            for(int i = 0; i < maxColorAttachments; i++)
            {
                foundAttachment = i;

                for(const auto& attachment : m_attachments)
                {
                    if(attachment.second.m_type == SGG_COLOR_ATTACHMENT)
                    {
                        if(attachment.second.m_ID == i)
                        {
                            // already exists
                            foundAttachment = -1;
                        }
                    }
                }

                // free attachment found
                if(foundAttachment != -1) break;
            }

            // creating texture
            if(foundAttachment != -1)
            {
                GLFrameBufferAttachment& newAttachment = m_attachments[name];

                newAttachment.m_type = attachmentType;

                newAttachment.m_name = name;
                newAttachment.m_ID = foundAttachment;
                newAttachment.m_width = width;
                newAttachment.m_height = height;
                newAttachment.m_format = format;
                newAttachment.m_internalFormat = internalFormat;
                newAttachment.m_mipLevel = mipLevel;
                newAttachment.m_layer = layer;

                glGenTextures(1, &newAttachment.m_handler);
                glBindTexture(GL_TEXTURE_2D, newAttachment.m_handler);

                glTexImage2D(GL_TEXTURE_2D,
                             mipLevel,
                             GLGraphicsTypesCaster::sggInternalFormatToGL(internalFormat),
                             width, height,
                             0,
                             GLGraphicsTypesCaster::sggFormatToGL(format),
                             GL_UNSIGNED_BYTE,
                             nullptr);

                // TODO: make it customizable
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                       GL_COLOR_ATTACHMENT0 + newAttachment.m_ID,
                                       GL_TEXTURE_2D, newAttachment.m_handler, mipLevel);
            }

            break;
        }

        case SGG_RENDER_ATTACHMENT:
            break;
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        SGCF_ERROR("Error when adding attachment '" + name + "' to framebuffer: framebuffer is not completed", SG_LOG_GAPI_FILE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    return shared_from_this();
}
