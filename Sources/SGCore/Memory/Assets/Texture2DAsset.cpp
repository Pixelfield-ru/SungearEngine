//
// Created by stuka on 25.05.2023.
//

//#include "Texture2DAsset.h"

#include <memory>
#include <filesystem>

#include "Texture2DAsset.h"
#include "SGCore/Main/CoreMain.h"
#include <stb/stb_image.h>

void Core::Memory::Assets::Texture2DDataDeleter::operator()(std::uint8_t* data)
{
    stbi_image_free(data);
}

// ----

Core::Memory::Assets::Texture2DAsset::Texture2DAsset(const SGTextureType& type) noexcept
{
    m_type = type;
}

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::Texture2DAsset::load(const std::string& path)
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

    m_texture2D = std::shared_ptr<Graphics::ITexture2D>(Core::Main::CoreMain::getRenderer().createTexture2D());
    m_texture2D->create(sharedPtr);

    SGC_INFO("Loaded texture. Width: " + std::to_string(m_width) + ", height: " + std::to_string(m_height)
             + ", MB size: " + std::to_string(m_width * m_height * m_channelsInFile / 1024.0 / 1024.0) + ", channels: " +
             std::to_string(m_channelsInFile) + ", path: " + m_path.string());

    return sharedPtr;
}

std::shared_ptr<Core::Graphics::ITexture2D> Core::Memory::Assets::Texture2DAsset::getTexture2D() noexcept
{
    return m_texture2D;
}

SGGColorInternalFormat Core::Memory::Assets::Texture2DAsset::getInternalFormat() const noexcept
{
    return m_internalFormat;
}

SGGColorFormat Core::Memory::Assets::Texture2DAsset::getFormat() const noexcept
{
    return m_format;
}

SGTextureType Core::Memory::Assets::Texture2DAsset::getType() const noexcept
{
    return m_type;
}

int Core::Memory::Assets::Texture2DAsset::getWidth() noexcept
{
    return m_width;
}

int Core::Memory::Assets::Texture2DAsset::getHeight() noexcept
{
    return m_height;
}

std::shared_ptr<std::uint8_t[]> Core::Memory::Assets::Texture2DAsset::getData() noexcept
{
    return m_textureData;
}
