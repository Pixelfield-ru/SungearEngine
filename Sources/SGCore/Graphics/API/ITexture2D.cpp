#include "ITexture2D.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"

#define DDSKTX_IMPLEMENT

#include "SGCore/Utils/DDSLoader/dds-ktx.h"
#include "SGCore/Utils/FileUtils.h"

#include <stb_image.h>
#include <stb_image_resize2.h>
#include <SGCore/Logger/Logger.h>

void SGCore::STBITextureDataDeleter::operator()(const std::uint8_t* data)
{
    std::printf("deleted texture by address: %ul\n", data);
    delete[] data;
    // stbi_image_free((void*) data);
}

// ----------------------------------

void SGCore::ITexture2D::doLoad(const std::filesystem::path& path)
{
    int channelsDesired = 0;
    
    if(!std::filesystem::exists(path))
    {
        LOG_E(SGCORE_TAG,
              "Error while loading texture: texture by path {} does not exist.", Utils::toUTF8(path.u16string()));
        return;
    }
    
    // m_texture2D = Ref<ITexture2D>(CoreMain::getRenderer().createTexture2D())->addToGlobalStorage();

    const auto& ext = m_path.extension();
    
    if(ext == ".png" || ext == ".jpg" || ext == ".jpeg")
    {
        m_textureData = Ref<std::uint8_t[]>(
                stbi_load(m_path.string().data(),
                          &m_width, &m_height,
                          &m_channelsCount, channelsDesired),
                STBITextureDataDeleter {});
        
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
    }
}

void SGCore::ITexture2D::doLazyLoad()
{
    create();
    
    addToGlobalStorage();

    LOG_I(SGCORE_TAG,
          "Loaded texture (in lazy load). Width: {}, height: {}, MB size: {}, channels: {}, path: {}",
          m_width,
          m_height,
          m_width * m_height * m_channelsCount / 1024.0 / 1024.0,
          m_channelsCount,
          m_path.string());
}

void SGCore::ITexture2D::addToGlobalStorage() noexcept
{
    GPUObjectsStorage::addTexture(shared_from_this());
}

SGCore::Ref<std::uint8_t[]> SGCore::ITexture2D::getData() noexcept
{
    return m_textureData;
}

void SGCore::ITexture2D::resize(std::int32_t newWidth, std::int32_t newHeight, bool noDataResize) noexcept
{
    Ref<std::uint8_t[]> newData = Ref<std::uint8_t[]>(new std::uint8_t[newWidth * newHeight * m_channelsCount],
                                                      STBITextureDataDeleter {});
    
    if(!noDataResize)
    {
        // stbir_resize()
        
        if(m_channelsCount == 4)
        {
            stbir_resize_uint8_linear(m_textureData.get(), m_width, m_height, 0, newData.get(), newWidth, newHeight, 0, STBIR_RGBA);
        }
        else if(m_channelsCount == 3)
        {
            stbir_resize_uint8_linear(m_textureData.get(), m_width, m_height, 0, newData.get(), newWidth, newHeight, 0, STBIR_RGB);
        }
        
        m_textureData = newData;
    }
    
    m_width = newWidth;
    m_height = newHeight;
    
    if(!noDataResize)
    {
        create();
    }
}

std::int32_t SGCore::ITexture2D::getWidth() const noexcept
{
    return m_width;
}

std::int32_t SGCore::ITexture2D::getHeight() const noexcept
{
    return m_height;
}
