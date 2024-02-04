#include "ITexture2D.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"
#include "spdlog/spdlog.h"

#include <stb/stb_image.h>

void SGCore::TextureDataDeleter::operator()(std::uint8_t* data)
{
    stbi_image_free(data);
}

// ----------------------------------

SGCore::Ref<SGCore::IAsset> SGCore::ITexture2D::load(const std::string& path)
{
    int channelsDesired = 0;

    // m_texture2D = Ref<ITexture2D>(CoreMain::getRenderer().createTexture2D())->addToGlobalStorage();

    m_path = path;

    m_textureData = std::shared_ptr<std::uint8_t[]>(
            stbi_load(m_path.string().data(),
                      &m_width, &m_height,
                      &m_channelsCount, channelsDesired),
            TextureDataDeleter { });

    if(m_channelsCount == 4)
    {
        m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
        m_format = SGGColorFormat::SGG_RGBA;
    }
    else if(m_channelsCount == 3)
    {
        m_internalFormat = SGGColorInternalFormat::SGG_RGB8;
        m_format = SGGColorFormat::SGG_RGB;
    }

    create();
    // create(sharedPtr);

    spdlog::info("Loaded texture. Width: {0}, height: {1}, MB size: {2}, channels: {3}, path: {4}",
                 m_width,
                 m_height,
                 m_width * m_height * m_channelsCount / 1024.0 / 1024.0,
                 m_channelsCount,
                 m_path.string());

    return shared_from_this();
}

void SGCore::ITexture2D::addToGlobalStorage() noexcept
{
    GPUObjectsStorage::addTexture(shared_from_this());
}

SGCore::Ref<uint8_t[]> SGCore::ITexture2D::getData() noexcept
{
    return m_textureData;
}
