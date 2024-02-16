//
// Created by stuka on 28.05.2023.
//

#include "GL4Texture2D.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "../GLGraphicsTypesCaster.h"
#include "SGCore/Graphics/API/GL/DeviceGLInfo.h"

SGCore::GL4Texture2D::~GL4Texture2D() noexcept
{
    destroy();
}

// migrate to gl46
void SGCore::GL4Texture2D::create() noexcept
{
    glGenTextures(1, &m_handler);

    glBindTexture(GL_TEXTURE_2D, m_handler);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GLGraphicsTypesCaster::sggInternalFormatToGL(m_internalFormat),
                 m_width,
                 m_height,
                 0,
                 GLGraphicsTypesCaster::sggFormatToGL(m_format),
                 GL_UNSIGNED_BYTE,
                 m_textureData.get());
    
    // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
    
    if(DeviceGLInfo::getSupportingExtensions().contains(SG_STRINGIFY(GL_EXT_texture_filter_anisotropic)))
    {
        float amount = std::min(4.0f, DeviceGLInfo::getMaxTextureMaxAnisotropy());
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
    }
    
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    #ifdef SUNGEAR_DEBUG
        GL4Renderer::getInstance()->checkForErrors();
    #endif
}

void SGCore::GL4Texture2D::createAsFrameBufferAttachment(const SGCore::Ref<SGCore::IFrameBuffer>& parentFrameBuffer,
                                                         SGFrameBufferAttachmentType attachmentType)
{
    // TODO: MAKE VERIFY INTERNAL FORMAT
    // TODO: MAKE VERIFY SIZE TYPE

    if(isDepthAttachment(attachmentType))
    {
        glGenTextures(1, &m_handler);
        glBindTexture(GL_TEXTURE_2D, m_handler);

        glTexImage2D(GL_TEXTURE_2D,
                     m_mipLevel,
                     GLGraphicsTypesCaster::sggInternalFormatToGL(m_internalFormat),
                     m_width, m_height,
                     0,
                     GLGraphicsTypesCaster::sggFormatToGL(m_format),
                // todo: make customizable
                     GL_FLOAT,
                     nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D, m_handler, m_mipLevel
        );
    }
    else if(isDepthStencilAttachment(attachmentType))
    {

        glGenTextures(1, &m_handler);
        glBindTexture(GL_TEXTURE_2D, m_handler);

        glTexImage2D(GL_TEXTURE_2D,
                     m_mipLevel,
                     GLGraphicsTypesCaster::sggInternalFormatToGL(m_internalFormat),
                     m_width, m_height,
                     0,
                     GLGraphicsTypesCaster::sggFormatToGL(m_format),
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
                               GL_TEXTURE_2D, m_handler, m_mipLevel);
    }
    else if(isColorAttachment(attachmentType))
    {
        GLenum glType = !m_useMultisampling ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;

        glGenTextures(1, &m_handler);
        glBindTexture(glType, m_handler);

        if(!m_useMultisampling)
        {
            glTexImage2D(glType,
                         m_mipLevel,
                         GLGraphicsTypesCaster::sggInternalFormatToGL(m_internalFormat),
                         m_width, m_height,
                         0,
                         GLGraphicsTypesCaster::sggFormatToGL(m_format),
                         GL_FLOAT,
                         nullptr
            );
        }
        else
        {
            glTexImage2DMultisample(glType,
                                    m_multisamplingSamplesCount,
                                    GLGraphicsTypesCaster::sggFormatToGL(m_format),
                                    m_width, m_height,
                                    GL_FALSE
            );
        }

        // TODO: make it customizable
        glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + (attachmentType - SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0),
                               glType, m_handler, m_mipLevel);
    }
    else if(isRenderAttachment(attachmentType))
    {
        // todo: make
    }
}

void SGCore::GL4Texture2D::destroy() noexcept
{
    glDeleteTextures(1, &m_handler);
}

void SGCore::GL4Texture2D::bind(const std::uint8_t& textureUnit) noexcept
{
    //glBindTextureUnit(textureUnit, m_handler);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_handler);
}

SGCore::GL4Texture2D& SGCore::GL4Texture2D::operator=
        (const Ref<ITexture2D>& other)
{
    return *this;
}
