//
// Created by stuka on 28.05.2023.
//

#include "GL4Texture2D.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "../GLGraphicsTypesCaster.h"
#include "SGCore/Graphics/API/GL/DeviceGLInfo.h"
#include "SGCore/Threading/ThreadsManager.h"

SGCore::GL4Texture2D::~GL4Texture2D() noexcept
{
    auto deleteTask = MakeRef<Threading::Task>();
    deleteTask->setOnExecuteCallback([texHandler = this->m_textureHandler, bufHandler = this->m_textureBufferHandler] {
        glDeleteTextures(1, &texHandler);
        glDeleteBuffers(1, &bufHandler);
    });
    
    Threading::ThreadsManager::getMainThread()->addTask(deleteTask);
}

// migrate to gl46
void SGCore::GL4Texture2D::create() noexcept
{
    destroy();
    
    if(m_isTextureBuffer)
    {
        glGenBuffers(1, &m_textureBufferHandler);
        glBindBuffer(GL_TEXTURE_BUFFER, m_textureBufferHandler);
        glBufferData(GL_TEXTURE_BUFFER, m_width * m_height * getSGGInternalFormatChannelsSizeInBytes(m_internalFormat), m_textureData.get(),
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
            // dds or ktx
            if(!m_gliTexture.empty())
            {
                gli::gl GL(gli::gl::PROFILE_GL33);
                const gli::gl::format gliFormat = GL.translate(m_gliTexture.format(), m_gliTexture.swizzles());
                const GLenum target = GL.translate(m_gliTexture.target());
                const auto textureExtent = m_gliTexture.extent();

                {
                    const std::string assertMsg = fmt::format(
                        "Can not create texture with compressed format and target not equals to GL_TEXTURE_2D. "
                        "Texture by path: '{}', texture target: GLenum '{}'.",
                        Utils::toUTF8(getPath().resolved().u16string()),
                        target
                    );

                    SG_ASSERT(gli::is_compressed(m_gliTexture.format()) && target == GL_TEXTURE_2D, assertMsg.c_str());
                }


                glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
                glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(m_gliTexture.levels() - 1));
                glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, &gliFormat.Swizzles[0]);
                glTexStorage2D(target, static_cast<GLint>(m_gliTexture.levels()), gliFormat.Internal, textureExtent.x, textureExtent.y);

                // m_internalFormat = gliFormat.Internal;
                m_internalFormat = GLGraphicsTypesCaster::oglInternalFormatToSG(gliFormat.Internal);
                m_format = GLGraphicsTypesCaster::oglFormatToSG(gliFormat.External);
                m_width = textureExtent.x;
                m_height = textureExtent.x;
                m_channelsCount = getSGGFormatChannelsCount(m_format);

                for(std::size_t level = 0; level < m_gliTexture.levels(); ++level)
                {
                    glm::tvec3<GLsizei> Extent(m_gliTexture.extent(level));
                    glCompressedTexSubImage2D(
                        target, static_cast<GLint>(level), 0, 0, Extent.x, Extent.y,
                        gliFormat.Internal, static_cast<GLsizei>(m_gliTexture.size(level)), m_gliTexture.data(0, 0, level));
                }
            }
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
                         GL_UNSIGNED_BYTE,
                         m_textureData.get());
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/
        // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);

        if(DeviceGLInfo::getSupportingExtensions().contains(SG_STRINGIFY(GL_EXT_texture_filter_anisotropic)))
        {
            float amount = std::min(4.0f, DeviceGLInfo::getMaxTextureMaxAnisotropy());

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, amount);
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
    destroy();

    m_frameBufferAttachmentType = attachmentType;
    
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
        /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);*/

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
                               GL_COLOR_ATTACHMENT0 + (std::to_underlying(attachmentType) -
                               std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0)),
                               glType,
                               m_textureHandler,
                               m_mipLevel);

        m_blendingState.m_forAttachment = std::to_underlying(attachmentType) -
                                          std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0);
    }
    else if(isRenderAttachment(attachmentType))
    {
        // todo: make
    }
}

void SGCore::GL4Texture2D::subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept
{
    glBufferSubData(GL_TEXTURE_BUFFER, bytesOffset, bytesCount, m_textureData.get() + bytesOffset);
}

void SGCore::GL4Texture2D::subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY) noexcept
{
    glTexSubImage2D(
        GL_TEXTURE_2D, // target
        0, // mimpmap level todo: make level choose
        areaOffsetX, areaOffsetY, // x and y offsets
        areaWidth, areaHeight, // subdata area size
        GLGraphicsTypesCaster::sggFormatToGL(m_format), // pixels format
        GL_UNSIGNED_BYTE,
        data // new data with offsets
    );
}

void SGCore::GL4Texture2D::destroy() noexcept
{
    glDeleteTextures(1, &m_textureHandler);
    glDeleteBuffers(1, &m_textureBufferHandler);
}

void SGCore::GL4Texture2D::bind(const std::uint8_t& textureUnit) const noexcept
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

void* SGCore::GL4Texture2D::getTextureNativeHandler() const noexcept
{
    return (void*) (intptr_t) m_textureHandler;
}

void* SGCore::GL4Texture2D::getTextureBufferNativeHandler() const noexcept
{
    return (void*) (intptr_t) m_textureBufferHandler;
}
