#include "ITexture2D.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"
#include "spdlog/spdlog.h"

#include <stb/stb_image.h>

void SGCore::STBITextureDataDeleter::operator()(void* data)
{
    stbi_image_free(data);
}

void SGCore::VoidDataDeleter::operator()(void* data)
{
    free(data);
}

// ----------------------------------

void SGCore::ITexture2D::load(const std::string& path)
{
    int channelsDesired = 0;

    // m_texture2D = Ref<ITexture2D>(CoreMain::getRenderer().createTexture2D())->addToGlobalStorage();

    m_path = path;

    m_textureData = Ref<std::uint8_t[]>(
            stbi_load(m_path.string().data(),
                      &m_width, &m_height,
                      &m_channelsCount, channelsDesired),
            STBITextureDataDeleter { });

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

    addToGlobalStorage();
    
    spdlog::info("Loaded texture. Width: {0}, height: {1}, MB size: {2}, channels: {3}, path: {4}",
                 m_width,
                 m_height,
                 m_width * m_height * m_channelsCount / 1024.0 / 1024.0,
                 m_channelsCount,
                 m_path.string());
}

void SGCore::ITexture2D::create
        (const std::uint8_t* data,
         const size_t& width,
         const size_t& height,
         const int& channelsCount,
         SGGColorInternalFormat internalFormat,
         SGGColorFormat format)
{
    size_t byteSize = width * height;
    
    m_width = width;
    m_height = height;
    m_channelsCount = channelsCount;
    m_internalFormat = internalFormat;
    m_format = format;
    
    m_textureData = Ref<std::uint8_t[]>(new std::uint8_t[byteSize]);
    
    std::memcpy(m_textureData.get(), data, byteSize);
    
    create();
    
    addToGlobalStorage();
}

void SGCore::ITexture2D::addToGlobalStorage() noexcept
{
    GPUObjectsStorage::addTexture(shared_from_this());
}

SGCore::Ref<std::uint8_t[]> SGCore::ITexture2D::getData() noexcept
{
    return m_textureData;
}

SGCore::ITexture2D::~ITexture2D()
{

}
