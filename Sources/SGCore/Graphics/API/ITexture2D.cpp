#include "ITexture2D.h"

#define DDSKTX_IMPLEMENT

#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Memory/AssetManager.h"

#include <SGCore/Logger/Logger.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_resize2.h>

void SGCore::STBITextureDataDeleter::operator()(const std::uint8_t* data)
{
#ifdef SUNGEAR_DEBUG
    std::printf("deleted texture by address: %ul\n", data);
#endif

    stbi_image_free((void*) data);
}

// ----------------------------------

void SGCore::ITexture2D::doLoad(const InterpolatedPath& path)
{
    int channelsDesired = 0;

    // m_texture2D = Ref<ITexture2D>(CoreMain::getRenderer().createTexture2D())->addToGlobalStorage();

    const auto ext = getPath().resolved().extension();
    
    if(ext == ".png" || ext == ".jpg" || ext == ".jpeg")
    {
        m_dataType = SGGDataType::SGG_UNSIGNED_BYTE;

        /*stbi_load_from_memory(m_textureData.get(), readBytes, &m_width, &m_height,
                              &m_channelsCount, channelsDesired);*/
        m_textureData = data_ptr(
            stbi_load(getPath().resolved().string().data(),
                      &m_width, &m_height,
                      &m_channelsCount, channelsDesired));
        
        if(m_channelsCount == 4)
        {
            m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
            m_format = SGGColorFormat::SGG_RGBA;
        }
        else if(m_channelsCount == 3)
        {
            m_internalFormat = SGGColorInternalFormat::SGG_RGB8;
            m_format = SGGColorFormat::SGG_RGB;

            std::cout << "three channel tex" << std::endl;
        }
        else if(m_channelsCount == 2)
        {
            m_internalFormat = SGGColorInternalFormat::SGG_RG8;
            m_format = SGGColorFormat::SGG_RG;

            std::cout << "two channel tex" << std::endl;
        }
        else if(m_channelsCount == 1)
        {
            m_internalFormat = SGGColorInternalFormat::SGG_R8;
            m_format = SGGColorFormat::SGG_R;

            std::cout << "one channel tex" << std::endl;
        }
    }
    else if(ext == ".hdr")
    {
        m_dataType = SGGDataType::SGG_FLOAT;

        m_textureData = data_ptr(
            reinterpret_cast<std::uint8_t*>(stbi_loadf(getPath().resolved().string().data(),
                                                       &m_width, &m_height,
                                                       &m_channelsCount, channelsDesired)));

        if(m_channelsCount == 4)
        {
            m_internalFormat = SGGColorInternalFormat::SGG_RGBA32_FLOAT;
            m_format = SGGColorFormat::SGG_RGBA;
        }
        else if(m_channelsCount == 3)
        {
            m_internalFormat = SGGColorInternalFormat::SGG_RGB32_FLOAT;
            m_format = SGGColorFormat::SGG_RGB;

            std::cout << "three channel FLOAT tex" << std::endl;
        }
        else if(m_channelsCount == 2)
        {
            m_internalFormat = SGGColorInternalFormat::SGG_RG32_FLOAT;
            m_format = SGGColorFormat::SGG_RG;

            std::cout << "two channel FLOAT tex" << std::endl;
        }
        else if(m_channelsCount == 1)
        {
            m_internalFormat = SGGColorInternalFormat::SGG_R32_FLOAT;
            m_format = SGGColorFormat::SGG_R;

            std::cout << "one channel FLOAT tex" << std::endl;
        }
    }
    else if(ext == ".dds")
    {
        const std::string resolvedUTF8Path = Utils::toUTF8(getPath().resolved().u16string());

        m_gliTexture = gli::load(resolvedUTF8Path);
        if(m_gliTexture.empty())
        {
            LOG_E(SGCORE_TAG, "Failed to load .DDS texture by path '{}'.", resolvedUTF8Path);
            return;
        }

        m_isCompressedFormat = true;
    }
}

void SGCore::ITexture2D::doLazyLoad()
{
    create();

    LOG_I(SGCORE_TAG,
          "Loaded texture (in lazy load). Width: {}, height: {}, MB size: {}, channels: {}, path: {}",
          m_width,
          m_height,
          size_t(m_width * m_height) * getSGGInternalFormatChannelsSizeInBytes(m_internalFormat) / 1024.0 / 1024.0,
          m_channelsCount,
          getPath().resolved().string());
}

void SGCore::ITexture2D::destroyOnRAM() noexcept
{
    delete[] m_textureData.release();
}

std::uint8_t* SGCore::ITexture2D::getData() noexcept
{
    return m_textureData.get();
}

const std::uint8_t* SGCore::ITexture2D::getData() const noexcept
{
    return m_textureData.get();
}

void SGCore::ITexture2D::resize(std::int32_t newWidth, std::int32_t newHeight, bool noDataResize) noexcept
{
    if(!noDataResize)
    {
        const size_t newSize = size_t(newWidth * newHeight) * getSGGInternalFormatChannelsSizeInBytes(m_internalFormat);
        data_ptr newData = data_ptr(static_cast<std::uint8_t*>(malloc(newSize)));

        // stbir_resize()
        
        if(m_channelsCount == 4)
        {
            stbir_resize_uint8_linear(m_textureData.get(), m_width, m_height, 0, newData.get(), newWidth, newHeight, 0, STBIR_RGBA);
        }
        else if(m_channelsCount == 3)
        {
            stbir_resize_uint8_linear(m_textureData.get(), m_width, m_height, 0, newData.get(), newWidth, newHeight, 0, STBIR_RGB);
        }
        
        m_textureData = std::move(newData);
    }
    
    m_width = newWidth;
    m_height = newHeight;
    
    if(!noDataResize)
    {
        create();
    }
}

void SGCore::ITexture2D::resizeDataBuffer(std::int32_t newWidth, std::int32_t newHeight, bool saveData) noexcept
{
    if(!saveData)
    {
        m_textureData = nullptr;
    }

    const std::uint8_t dataChannelsSize = getSGGInternalFormatChannelsSizeInBytes(m_internalFormat);

    const size_t newSize = size_t(newWidth * newHeight) * dataChannelsSize;
    data_ptr newData = data_ptr(static_cast<std::uint8_t*>(malloc(newSize)));

    // std::memset(newData.get(), 0, newSize);

    if(saveData)
    {
        for(size_t y = 0; y < m_height; ++y)
        {
            std::memcpy(newData.get() + size_t(y * newWidth) * dataChannelsSize,
                    m_textureData.get() + size_t(y * m_width) * dataChannelsSize,
                    m_width * dataChannelsSize);
        }
    }

    m_textureData = std::move(newData);

    m_width = newWidth;
    m_height = newHeight;

    create();
}

std::int32_t SGCore::ITexture2D::getWidth() const noexcept
{
    return m_width;
}

std::int32_t SGCore::ITexture2D::getHeight() const noexcept
{
    return m_height;
}

void SGCore::ITexture2D::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    auto textureData = parentAssetManager->getPackage().readBytes(m_textureDataOffsetInPackage, m_textureDataSizeInPackage);

    m_textureData = data_ptr(reinterpret_cast<std::uint8_t*>(textureData));
}

SGFrameBufferAttachmentType SGCore::ITexture2D::getFrameBufferAttachmentType() const noexcept
{
    return m_frameBufferAttachmentType;
}

void SGCore::ITexture2D::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                          SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    doLoad(getPath());
}
