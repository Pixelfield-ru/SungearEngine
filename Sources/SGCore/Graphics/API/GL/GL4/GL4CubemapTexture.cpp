//
// Created by stuka on 29.08.2023.
//

#include "GL4CubemapTexture.h"
#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"

// todo: impl 
void SGCore::GL4CubemapTexture::create(std::weak_ptr<CubemapAsset> cubemapAsset)
{
    auto thisWeak = weak_from_this();

    auto lockedCubemap = m_cubemapAsset.lock();

    if(lockedCubemap)
    {
        lockedCubemap->removeObserver(thisWeak.lock());
    }

    m_cubemapAsset = cubemapAsset;

    lockedCubemap = m_cubemapAsset.lock();

    if(lockedCubemap)
    {
        lockedCubemap->addObserver(thisWeak.lock());
    }

    const auto& cubemapParts = lockedCubemap->getParts();

    glGenTextures(1, &m_handler);

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handler);

    std::uint8_t currentPartId = 0;
    for(const auto& part : cubemapParts)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + currentPartId,
                     0,
                     GLGraphicsTypesCaster::sggInternalFormatToGL(part->getInternalFormat()),
                     part->getWidth(),
                     part->getHeight(),
                     0,
                     GLGraphicsTypesCaster::sggFormatToGL(part->getFormat()),
                     GL_UNSIGNED_BYTE,
                     part->getData().get()
        );

        currentPartId++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SGCore::GL4CubemapTexture::destroy()
{
    glDeleteTextures(1, &m_handler);
}

void SGCore::GL4CubemapTexture::bind(const uint8_t& textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handler);
}

SGCore::GL4CubemapTexture&
SGCore::GL4CubemapTexture::operator=(const std::shared_ptr<ITexture2D>& other)
{
    return *this;
}
