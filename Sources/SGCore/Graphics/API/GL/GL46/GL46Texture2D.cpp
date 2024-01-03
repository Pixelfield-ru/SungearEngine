//
// Created by stuka on 28.05.2023.
//

#include "GL46Texture2D.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "../GLGraphicsTypesCaster.h"

#include "SGCore/Logging/Log.h"

SGCore::GL46Texture2D::~GL46Texture2D() noexcept
{
    destroy();
}

// migrate to gl46
void SGCore::GL46Texture2D::create() noexcept
{
    m_glInternalFormat = GLGraphicsTypesCaster::sggInternalFormatToGL(m_internalFormat);
    m_glFormat = GLGraphicsTypesCaster::sggFormatToGL(m_format);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_handler);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTextureParameteri(m_handler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_handler, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureParameteri(m_handler, GL_GENERATE_MIPMAP, GL_TRUE);

    glTextureParameteri(m_handler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_handler, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTextureStorage2D(m_handler, 1, m_glInternalFormat, m_width, m_height);
    glTextureSubImage2D(m_handler,
                        0,
                        0,
                        0,
                        m_width,
                        m_height,
                        m_glFormat,
                        GL_UNSIGNED_BYTE,
                        m_textureData.get());

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif
}

void SGCore::GL46Texture2D::destroy() noexcept
{
    glDeleteTextures(1, &m_handler);
}

void SGCore::GL46Texture2D::bind(const std::uint8_t& textureUnit) noexcept
{
    glBindTextureUnit(textureUnit, m_handler);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, m_handler);
}

SGCore::GL46Texture2D& SGCore::GL46Texture2D::operator=
        (const Ref<ITexture2D>& other)
{

    return *this;
}
