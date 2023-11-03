//
// Created by stuka on 23.07.2023.
//

#include "GL4FrameBuffer.h"

#include "SGCore/Logging/Log.h"
#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"
#include "SGCore/Main/CoreMain.h"
#include "GL4Texture2D.h"

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::bindAttachments
(const std::shared_ptr<Memory::Assets::IMaterial>& material, const std::uint8_t& frameBufferIndex)
{
    /*const auto& blocks = material->getBlocks();

    std::uint8_t fbDepthAttachmentsCnt = 0;
    std::uint8_t fbDepthStencilAttachmentsCnt = 0;
    std::uint8_t fbColorAttachmentsCnt = 0;
    std::uint8_t fbRenderAttachmentsCnt = 0;

    std::string fbDepthTextureTypeStr = sgTextureTypeToString(
            SGTextureType::SGTP_FRAMEBUFFER_DEPTH_ATTACHMENT
    );
    std::string fbDepthStencilTextureTypeStr = sgTextureTypeToString(
            SGTextureType::SGTP_FRAMEBUFFER_DEPTH_STENCIL_ATTACHMENT
    );
    std::string fbColorTextureTypeStr = sgTextureTypeToString(
            SGTextureType::SGTP_FRAMEBUFFER_COLOR_ATTACHMENT
    );
    std::string fbRenderTextureTypeStr = sgTextureTypeToString(
            SGTextureType::SGTP_FRAMEBUFFER_RENDER_ATTACHMENT
    );

    for(const auto& block : blocks)
    {
        SGTextureType textureType = block.first;

        if(textureType == SGTextureType::SGTP_FRAMEBUFFER_DEPTH_ATTACHMENT)
        {
            std::uint8_t finalOffset = frameBufferIndex * block.second.m_texturesUnitOffset;

            glActiveTexture(GL_TEXTURE0 + finalOffset);
            glBindTexture(GL_TEXTURE_2D, m_attachments[SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT].m_handler);

            material->getCurrentShader()->useTexture(fbDepthTextureTypeStr + "Samplers[" + std::to_string(frameBufferIndex) + "][0]", finalOffset);

            fbDepthAttachmentsCnt++;
        }
        else if(textureType == SGTextureType::SGTP_FRAMEBUFFER_DEPTH_STENCIL_ATTACHMENT)
        {
            // todo: impl
        }
        else if(textureType == SGTextureType::SGTP_FRAMEBUFFER_COLOR_ATTACHMENT)
        {
            std::uint8_t curColAttachment = 0;

            for(const auto& fbAttachment : m_attachments)
            {
                if(fbAttachment.first >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
                   fbAttachment.first <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31 &&
                   curColAttachment < block.second.m_maximumTextures)
                {
                    glActiveTexture(GL_TEXTURE0 + (frameBufferIndex * block.second.m_texturesUnitOffset + curColAttachment));
                    glBindTexture(GL_TEXTURE_2D, m_attachments[fbAttachment.first].m_handler);

                    material->getCurrentShader()->useTexture(fbColorTextureTypeStr + "Samplers[" +
                    std::to_string(frameBufferIndex) +
                    "]["+
                                                             std::to_string(curColAttachment) + "]",
                                                             block.second.m_texturesUnitOffset + curColAttachment);

                    ++curColAttachment;

                    ++fbColorAttachmentsCnt;
                }
            }
        }
        else if(textureType == SGTextureType::SGTP_FRAMEBUFFER_RENDER_ATTACHMENT)
        {
            // todo: impl
        }
    }

    material->getCurrentShader()->useInteger(fbDepthTextureTypeStr + "_UNIFORM_COUNT", fbDepthAttachmentsCnt);
    material->getCurrentShader()->useInteger(fbDepthStencilTextureTypeStr + "_UNIFORM_COUNT", fbDepthStencilAttachmentsCnt);
    material->getCurrentShader()->useInteger(fbColorTextureTypeStr + "_UNIFORM_COUNT", fbColorAttachmentsCnt);
    material->getCurrentShader()->useInteger(fbRenderTextureTypeStr + "_UNIFORM_COUNT", fbRenderAttachmentsCnt);*/

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::bindAttachment
(const SGFrameBufferAttachmentType& attachmentType, const std::uint8_t& textureBlock)
{
    glActiveTexture(GL_TEXTURE0 + textureBlock);
    glBindTexture(GL_TEXTURE_2D, m_attachments[attachmentType].m_handler);

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::bindAttachmentToRead()
{
    if(m_attachments.find(m_drawAttachmentType) != m_attachments.cend())
    {
        if(m_drawAttachmentType == SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT)
        {
            glReadBuffer(GL_DEPTH_ATTACHMENT);
        }
        else if(m_drawAttachmentType == SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT)
        {
            glReadBuffer(GL_DEPTH_STENCIL_ATTACHMENT);
        }
        else if(m_drawAttachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
                m_drawAttachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
        {
            glReadBuffer(GL_COLOR_ATTACHMENT0 + (m_drawAttachmentType - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
        }
        else if(m_drawAttachmentType == SGFrameBufferAttachmentType::SGG_RENDER_ATTACHMENT)
        {
            // todo: impl
            //glReadBuffer()
        }
    }

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::bindAttachmentToDraw()
{
    if(m_attachments.find(m_drawAttachmentType) != m_attachments.cend())
    {
        if(m_drawAttachmentType == SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT)
        {
            glDrawBuffer(GL_DEPTH_ATTACHMENT);
        }
        else if(m_drawAttachmentType == SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT)
        {
            glDrawBuffer(GL_DEPTH_STENCIL_ATTACHMENT);
        }
        else if(m_drawAttachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
                m_drawAttachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
        {
            glDrawBuffer(GL_COLOR_ATTACHMENT0 + (m_drawAttachmentType - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
        }
        else if(m_drawAttachmentType == SGFrameBufferAttachmentType::SGG_RENDER_ATTACHMENT)
        {
            // todo: impl
            //glDrawBuffer()
        }
    }

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::unbindAttachmentToRead()
{
    glReadBuffer(GL_NONE);

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::unbindAttachmentToDraw()
{
    glDrawBuffer(GL_NONE);

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::bind()
{
    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handler);

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

    return shared_from_this();
}

void Core::Graphics::GL4FrameBuffer::destroy()
{
    glDeleteFramebuffers(1, &m_handler);
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::GL4FrameBuffer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer>
Core::Graphics::GL4FrameBuffer::addAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                              const SGGColorFormat& format,
                                              const SGGColorInternalFormat& internalFormat,
                                              const int& mipLevel,
                                              const int& layer)
{
    if(m_attachments.find(attachmentType) != m_attachments.end())
    {
        SGCF_ERROR("Error when adding an attachment to the framebuffer: "
                   "an attachment with this type already exists.", SG_LOG_GAPI_FILE);

        return shared_from_this();
    }

    // TODO: MAKE VERIFY INTERNAL FORMAT
    // TODO: MAKE VERIFY SIZE TYPE

    if(attachmentType == SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT)
    {
            auto& newAttachment = m_attachments[attachmentType];

        newAttachment.m_format = format;
        newAttachment.m_internalFormat = internalFormat;
        newAttachment.m_mipLevel = mipLevel;
        newAttachment.m_layer = layer;

        glGenTextures(1, &newAttachment.m_handler);
        glBindTexture(GL_TEXTURE_2D, newAttachment.m_handler);

        glTexImage2D(GL_TEXTURE_2D,
                     mipLevel,
                     GLGraphicsTypesCaster::sggInternalFormatToGL(internalFormat),
                     m_width, m_height,
                     0,
                     GL_DEPTH_COMPONENT,
                // todo: make customizable
                     GL_FLOAT,
                     nullptr
        );

        // TODO: make it customizable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

        // todo: make it customizable
        float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D, newAttachment.m_handler, mipLevel
        );
    }
    else if(attachmentType == SGFrameBufferAttachmentType::SGG_DEPTH_STENCIL_ATTACHMENT)
    {
        // todo: make
    }
    else if(attachmentType >= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0 &&
            attachmentType <= SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT31)
    {
        GLFrameBufferAttachment& newAttachment = m_attachments[attachmentType];

        newAttachment.m_format = format;
        newAttachment.m_internalFormat = internalFormat;
        newAttachment.m_mipLevel = mipLevel;
        newAttachment.m_layer = layer;

        glGenTextures(1, &newAttachment.m_handler);
        glBindTexture(GL_TEXTURE_2D, newAttachment.m_handler);

        glTexImage2D(GL_TEXTURE_2D,
                     mipLevel,
                     GLGraphicsTypesCaster::sggInternalFormatToGL(internalFormat),
                     m_width, m_height,
                     0,
                     GLGraphicsTypesCaster::sggFormatToGL(format),
                     GL_UNSIGNED_BYTE,
                     nullptr);

        // TODO: make it customizable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + (attachmentType - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0),
                               GL_TEXTURE_2D, newAttachment.m_handler, mipLevel);
    }
    else if(attachmentType == SGFrameBufferAttachmentType::SGG_RENDER_ATTACHMENT)
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
