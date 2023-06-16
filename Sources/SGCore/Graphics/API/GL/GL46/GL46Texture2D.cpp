//
// Created by stuka on 28.05.2023.
//

#include "GL46Texture2D.h"
#include "GL46Renderer.h"
#include "../GLGraphicsTypesCaster.h"

#include "SGCore/Logging/Log.h"

Core::Graphics::API::GL::GL46::GL46Texture2D::~GL46Texture2D() noexcept
{
    destroy();
}

// migrate to gl46
void Core::Graphics::API::GL::GL46::GL46Texture2D::create(std::weak_ptr<Memory::Assets::Texture2DAsset> asset) noexcept
{
    std::shared_ptr<Core::Memory::Assets::Texture2DAsset> originalSharedPtr = m_texture2DAsset.lock();
    if(originalSharedPtr && m_texture2DAsset.expired())
    {
        originalSharedPtr->removeObserver(this);
    }

    m_texture2DAsset = asset;

    originalSharedPtr = m_texture2DAsset.lock();

    originalSharedPtr->addObserver(this);

    m_glInternalFormat = GLGraphicsTypesCaster::sggInternalFormatToGL(originalSharedPtr->getInternalFormat());
    m_glFormat = GLGraphicsTypesCaster::sggFormatToGL(originalSharedPtr->getFormat());

    glCreateTextures(GL_TEXTURE_2D, 1, &m_handler);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTextureParameteri(m_handler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_handler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_2D);

    //glTextureParameteri(m_handler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTextureParameteri(m_handler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_handler, GL_GENERATE_MIPMAP, GL_TRUE);

    glTextureParameteri(m_handler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_handler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(m_handler, 1, m_glInternalFormat, originalSharedPtr->getWidth(), originalSharedPtr->getHeight());
    glTextureSubImage2D(m_handler,
                        0,
                        0,
                        0,
                        originalSharedPtr->getWidth(),
                        originalSharedPtr->getHeight(),
                        m_glFormat,
                        GL_UNSIGNED_BYTE,
                        reinterpret_cast<const void*>(originalSharedPtr->getData().get()));

    #ifdef SUNGEAR_DEBUG
    GL46Renderer::getInstance()->checkForErrors();
    #endif
}

void Core::Graphics::API::GL::GL46::GL46Texture2D::destroy() noexcept
{
    glDeleteTextures(1, &m_handler);
}

void Core::Graphics::API::GL::GL46::GL46Texture2D::bind() noexcept
{
    glBindTextureUnit(m_unit, m_handler);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, m_handler);
}

void Core::Graphics::API::GL::GL46::GL46Texture2D::onAssetModified() noexcept
{

}

void Core::Graphics::API::GL::GL46::GL46Texture2D::onAssetPathChanged() noexcept
{

}

void Core::Graphics::API::GL::GL46::GL46Texture2D::onAssetDeleted() noexcept
{

}

void Core::Graphics::API::GL::GL46::GL46Texture2D::onAssetRestored() noexcept
{

}

std::shared_ptr<Core::Graphics::API::ITexture2D> Core::Graphics::API::GL::GL46::GL46Texture2D::operator=
        (const std::shared_ptr<ITexture2D>& other)
{
    create(other->getAsset());
    //m_unit = other->getUnit();

    return shared_from_this();
}
