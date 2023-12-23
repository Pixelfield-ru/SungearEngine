//
// Created by stuka on 25.05.2023.
//

//#include "Texture2DAsset.h"

#include <memory>
#include <filesystem>

#include "Texture2DAsset.h"
#include "SGCore/Main/CoreMain.h"
#include <stb/stb_image.h>

void SGCore::Texture2DDataDeleter::operator()(std::uint8_t* data)
{
    stbi_image_free(data);
}

// ----

std::shared_ptr<SGCore::IAsset> SGCore::Texture2DAsset::load(const std::string& path)
{
    int channelsDesired = 0;

    m_path = path;

    m_textureData = std::shared_ptr<std::uint8_t[]>(
            stbi_load(m_path.string().data(),
                      &m_width, &m_height,
                      &m_channelsInFile, channelsDesired),
                      Texture2DDataDeleter { });

    if(m_channelsInFile == 4)
    {
        m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
        m_format = SGGColorFormat::SGG_RGBA;
    }
    else if(m_channelsInFile == 3)
    {
        m_internalFormat = SGGColorInternalFormat::SGG_RGB8;
        m_format = SGGColorFormat::SGG_RGB;
    }

    auto sharedPtr = shared_from_this();

    m_texture2D = Ref<ITexture2D>(CoreMain::getRenderer().createTexture2D())->addToGlobalStorage();
    m_texture2D->create(sharedPtr);

    SGC_INFO("Loaded texture. Width: " + std::to_string(m_width) + ", height: " + std::to_string(m_height)
             + ", MB size: " + std::to_string(m_width * m_height * m_channelsInFile / 1024.0 / 1024.0) + ", channels: " +
             std::to_string(m_channelsInFile) + ", path: " + m_path.string());

    return sharedPtr;
}

std::shared_ptr<SGCore::ITexture2D> SGCore::Texture2DAsset::getTexture2D() noexcept
{
    return m_texture2D;
}

SGGColorInternalFormat SGCore::Texture2DAsset::getInternalFormat() const noexcept
{
    return m_internalFormat;
}

SGGColorFormat SGCore::Texture2DAsset::getFormat() const noexcept
{
    return m_format;
}

int SGCore::Texture2DAsset::getWidth() noexcept
{
    return m_width;
}

int SGCore::Texture2DAsset::getHeight() noexcept
{
    return m_height;
}

std::shared_ptr<std::uint8_t[]> SGCore::Texture2DAsset::getData() noexcept
{
    return m_textureData;
}
