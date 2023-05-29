//
// Created by stuka on 25.05.2023.
//

//#include "Texture2DAsset.h"

#include <stb/stb_image.h>
#include <memory>

#include "Texture2DAsset.h"
#include "SGCore/Main/Core.h"

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::Texture2DAsset::load(const std::string_view& path)
{
    int channelsDesired = 0;

    m_textureData = std::shared_ptr<std::uint8_t[]>(stbi_load(path.data(), &m_width, &m_height, &m_channelsInFile, channelsDesired));

    m_texture2D = std::shared_ptr<Graphics::API::ITexture2D>(Core::Main::Core::getRenderer().createTexture2D());

    if(m_channelsInFile == 4)
    {
        m_internalFormat = SGGInternalFormat::SGG_RGBA8;
        m_format = SGGFormat::SGG_RGBA;
    }
    else if(m_channelsInFile == 3)
    {
        m_internalFormat = SGGInternalFormat::SGG_RGB8;
        m_format = SGGFormat::SGG_RGB;
    }

    std::shared_ptr<Texture2DAsset> sharedPtr = shared_from_this();
    m_texture2D->create(sharedPtr);

    SGC_INFO("Loaded texture. Width: " + std::to_string(m_width) + ", height: " + std::to_string(m_height)
             + ", byte size: " + std::to_string(m_width * m_height) + ", channels: " + std::to_string(m_channelsInFile));

    return sharedPtr;
}

std::shared_ptr<Core::Graphics::API::ITexture2D> Core::Memory::Assets::Texture2DAsset::getTexture2D() noexcept
{
    return m_texture2D;
}

SGGInternalFormat Core::Memory::Assets::Texture2DAsset::getInternalFormat() noexcept
{
    return m_internalFormat;
}

SGGFormat Core::Memory::Assets::Texture2DAsset::getFormat() noexcept
{
    return m_format;
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

