//
// Created by stuka on 28.05.2023.
//

#include "GL4Texture2D.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "../GLGraphicsTypesCaster.h"

#include "SGCore/Logging/Log.h"

Core::Graphics::GL4Texture2D::~GL4Texture2D() noexcept
{
    destroy();
}

// migrate to gl46
void Core::Graphics::GL4Texture2D::create(std::weak_ptr<Memory::Assets::Texture2DAsset> asset) noexcept
{
    std::shared_ptr<Core::Memory::Assets::Texture2DAsset> assetSharedPtr = m_texture2DAsset.lock();

    if(assetSharedPtr && m_texture2DAsset.expired())
    {
        assetSharedPtr->removeObserver(this);
    }

    m_texture2DAsset = asset;

    assetSharedPtr = m_texture2DAsset.lock();

    assetSharedPtr->addObserver(this);

    m_glInternalFormat = GLGraphicsTypesCaster::sggInternalFormatToGL(assetSharedPtr->getInternalFormat());
    m_glFormat = GLGraphicsTypesCaster::sggFormatToGL(assetSharedPtr->getFormat());

    glGenTextures(1, &m_handler);
    switch(assetSharedPtr->getType())
    {
        case SGG_DEFAULT_TEXTURE:
        {
            glBindTexture(GL_TEXTURE_2D, m_handler);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexStorage2D(GL_TEXTURE_2D, 1, m_glInternalFormat,
                           assetSharedPtr->getWidth(), assetSharedPtr->getHeight());

            glTexSubImage2D(GL_TEXTURE_2D,
                            0,
                            0,
                            0,
                            assetSharedPtr->getWidth(),
                            assetSharedPtr->getHeight(),
                            m_glFormat,
                            GL_UNSIGNED_BYTE,
                            reinterpret_cast<const void*>(assetSharedPtr->getData().get()));

            break;
        }
        case SGG_CUBE_MAP_TEXTURE:
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_handler);

            //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            /*glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

            int offsetX = 0;
            int offsetY = 0;

            int partWidth = assetSharedPtr->getWidth() / 4;
            int partHeight = assetSharedPtr->getHeight() / 3;
            /*int partWidth = assetSharedPtr->getWidth() / 6;
            int partHeight = assetSharedPtr->getHeight() / 6;*/

            /*glTexStorage2D(GL_TEXTURE_CUBE_MAP,
                           1, m_glInternalFormat,
                           assetSharedPtr->getWidth(), assetSharedPtr->getHeight());*/

            //glPixelStorei(GL_UNPACK_ALIGNMENT, 3);

            auto* currentPtr = assetSharedPtr->getData().get();

            int offsets[6] {
                partWidth * 4,
                (assetSharedPtr->getWidth() * partHeight + partWidth * 0) * 4,
                (assetSharedPtr->getWidth() * partHeight + partWidth * 1) * 4,
                (assetSharedPtr->getWidth() * partHeight + partWidth * 2) * 4,
                (assetSharedPtr->getWidth() * partHeight + partWidth * 3) * 4,
                (assetSharedPtr->getWidth() * partHeight * 2 + partWidth * 1) * 4
            };
            /*int offsets[6] {
                    assetSharedPtr->getHeight() * partWidth * 4,
                    partHeight * 1 * 4,
                    (assetSharedPtr->getHeight() * partWidth + partHeight * 1) * 4,
                    (assetSharedPtr->getHeight() * partWidth + partHeight * 2) * 4,
                    (assetSharedPtr->getHeight() * partWidth * 2 + partHeight * 1) * 4,
                    (assetSharedPtr->getHeight() * partWidth * 3 + partHeight * 1) * 4
            };*/

            for(size_t i = 0; i < 6; i++)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,
                             GL_RGBA8,
                             partWidth,
                             partHeight,
                             0,
                             GL_RGBA,
                             GL_UNSIGNED_BYTE,
                             //reinterpret_cast<const void*>(
                                     currentPtr + offsets[i]
                                    // )
                );

                //currentPtr += 2048 * 4;

                /*glTexSubImage2D(GL_TEXTURE_CUBE_MAP,
                                0,
                                offsetX * partWidth,
                                offsetY * partHeight,
                                partWidth,
                                partHeight,
                                m_glFormat,
                                GL_UNSIGNED_BYTE,
                                reinterpret_cast<const void*>(assetSharedPtr->getData().get()));*/
            }

            //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            break;
        }
    }


    #ifdef SUNGEAR_DEBUG
        GL4Renderer::getInstance()->checkForErrors();
    #endif
}

void Core::Graphics::GL4Texture2D::destroy() noexcept
{
    glDeleteTextures(1, &m_handler);
}

void Core::Graphics::GL4Texture2D::bind(const std::uint8_t& textureUnit) noexcept
{
    //glBindTextureUnit(textureUnit, m_handler);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    switch(m_texture2DAsset.lock()->getType())
    {
        case SGG_DEFAULT_TEXTURE: glBindTexture(GL_TEXTURE_2D, m_handler); break;
        case SGG_CUBE_MAP_TEXTURE: glBindTexture(GL_TEXTURE_CUBE_MAP, m_handler); break;
    }
}

void Core::Graphics::GL4Texture2D::onAssetModified() noexcept
{

}

void Core::Graphics::GL4Texture2D::onAssetPathChanged() noexcept
{

}

void Core::Graphics::GL4Texture2D::onAssetDeleted() noexcept
{

}

void Core::Graphics::GL4Texture2D::onAssetRestored() noexcept
{

}

Core::Graphics::GL4Texture2D& Core::Graphics::GL4Texture2D::operator=
        (const std::shared_ptr<ITexture2D>& other)
{
    create(other->getAsset());

    return *this;
}
