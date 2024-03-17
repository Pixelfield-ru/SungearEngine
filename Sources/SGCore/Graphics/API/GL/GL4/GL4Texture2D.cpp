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
    if(m_isTextureBuffer)
    {
        glGenBuffers(1, &m_textureBufferHandler);
        glBindBuffer(GL_TEXTURE_BUFFER, m_textureBufferHandler);
        glBufferData(GL_TEXTURE_BUFFER, m_width * m_height * getSGGDataTypeSizeInBytes(m_dataType), m_textureData.get(),
                     GLGraphicsTypesCaster::sggBufferUsageToGL(m_textureBufferUsage));
        
        glGenTextures(1, &m_textureHandler);
        glBindTexture(GL_TEXTURE_BUFFER, m_textureHandler);
        glTexBuffer(GL_TEXTURE_BUFFER, GLGraphicsTypesCaster::sggInternalFormatToGL(m_internalFormat), m_textureBufferHandler);
        
        glBindTexture(GL_TEXTURE_BUFFER, 0);
        glBindBuffer(GL_TEXTURE_BUFFER, 0);
    }
    else
    {
        glGenTextures(1, &m_textureHandler);

        glBindTexture(GL_TEXTURE_2D, m_textureHandler);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        if(m_isCompressedFormat)
        {
            glCompressedTexImage2D(GL_TEXTURE_2D,
                                   0,
                                   GLGraphicsTypesCaster::sggInternalFormatToGL(m_internalFormat),
                                   m_width,
                                   m_height,
                                   0,
                                   m_pixelSize,
                                   m_textureData.get());
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GLGraphicsTypesCaster::sggInternalFormatToGL(m_internalFormat),
                         m_width,
                         m_height,
                         0,
                         GLGraphicsTypesCaster::sggFormatToGL(m_format),
                         GLGraphicsTypesCaster::sggDataTypeToGL(m_dataType),
                         m_textureData.get());
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);

        if(DeviceGLInfo::getSupportingExtensions().contains(SG_STRINGIFY(GL_EXT_texture_filter_anisotropic)))
        {
            float amount = std::min(4.0f, DeviceGLInfo::getMaxTextureMaxAnisotropy());

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
        }
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
        glGenTextures(1, &m_textureHandler);
        glBindTexture(GL_TEXTURE_2D, m_textureHandler);

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
                               GL_TEXTURE_2D, m_textureHandler, m_mipLevel
        );
    }
    else if(isDepthStencilAttachment(attachmentType))
    {

        glGenTextures(1, &m_textureHandler);
        glBindTexture(GL_TEXTURE_2D, m_textureHandler);

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
                               GL_TEXTURE_2D, m_textureHandler, m_mipLevel);
    }
    else if(isColorAttachment(attachmentType))
    {
        GLenum glType = !m_useMultisampling ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;

        glGenTextures(1, &m_textureHandler);
        glBindTexture(glType, m_textureHandler);

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
                               glType, m_textureHandler, m_mipLevel);
    }
    else if(isRenderAttachment(attachmentType))
    {
        // todo: make
    }
}

void SGCore::GL4Texture2D::subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept
{
    glBufferSubData(GL_TEXTURE_BUFFER, bytesOffset, bytesCount, m_textureData.get());
}

void SGCore::GL4Texture2D::subTextureDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept
{
    /*const size_t bytesWidth = m_width * getSGGDataTypeSizeInBytes(m_dataType);
    
    const size_t y = bytesOffset % bytesWidth;
    const size_t x = bytesOffset - y * bytesWidth;
    
    glBindTexture(GL_TEXTURE_2D, m_textureHandler);
    glTextureSubImage2D()*/
}

void SGCore::GL4Texture2D::destroy() noexcept
{
    glDeleteTextures(1, &m_textureHandler);
    glDeleteBuffers(1, &m_textureBufferHandler);
}

void SGCore::GL4Texture2D::bind(const std::uint8_t& textureUnit) noexcept
{
    if(!m_isTextureBuffer)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_textureHandler);
    }
    else
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_BUFFER, m_textureHandler);
        glBindBuffer(GL_TEXTURE_BUFFER, m_textureBufferHandler);
    }
}

SGCore::GL4Texture2D& SGCore::GL4Texture2D::operator=
        (const Ref<ITexture2D>& other)
{
    return *this;
}
