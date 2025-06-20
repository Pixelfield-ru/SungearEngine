#include "ITexture2D.h"

#define DDSKTX_IMPLEMENT

#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Memory/AssetManager.h"

#include <SGCore/Logger/Logger.h>
#include <stb_image.h>
#include <stb_image_resize2.h>

void SGCore::STBITextureDataDeleter::operator()(const std::uint8_t* data)
{
    std::printf("deleted texture by address: %ul\n", data);
    delete[] data;
    // stbi_image_free((void*) data);
}

// ----------------------------------

void SGCore::ITexture2D::doLoad(const InterpolatedPath& path)
{
    int channelsDesired = 0;

    // m_texture2D = Ref<ITexture2D>(CoreMain::getRenderer().createTexture2D())->addToGlobalStorage();

    const auto ext = getPath().resolved().extension();
    
    if(ext == ".png" || ext == ".jpg" || ext == ".jpeg")
    {
        /*stbi_load_from_memory(m_textureData.get(), readBytes, &m_width, &m_height,
                              &m_channelsCount, channelsDesired);*/
        m_textureData = Ref<std::uint8_t[]>(
                stbi_load(getPath().resolved().string().data(),
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
          m_width * m_height * m_channelsCount / 1024.0 / 1024.0,
          m_channelsCount,
          getPath().resolved().string());
}

SGCore::Ref<std::uint8_t[]> SGCore::ITexture2D::getData() noexcept
{
    return m_textureData;
}

SGCore::Ref<const unsigned char[]> SGCore::ITexture2D::getData() const noexcept
{
    return m_textureData;
}

void SGCore::ITexture2D::resize(std::int32_t newWidth, std::int32_t newHeight, bool noDataResize) noexcept
{
    // TODO: почему я не умножаю на размер одного канала???

    if(!noDataResize)
    {
        Ref<std::uint8_t[]> newData = Ref<std::uint8_t[]>(new std::uint8_t[newWidth * newHeight * m_channelsCount],
                                                              STBITextureDataDeleter {});

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

void SGCore::ITexture2D::resizeDataBuffer(std::int32_t newWidth, std::int32_t newHeight) noexcept
{
    Ref<std::uint8_t[]> newData = Ref<std::uint8_t[]>(new std::uint8_t[newWidth * newHeight * m_channelsCount],
                                                              STBITextureDataDeleter {});

    m_textureData = newData;

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

SGGDataType SGCore::ITexture2D::getDataType() const noexcept
{
    return m_dataType;
}

void SGCore::ITexture2D::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    auto textureData = parentAssetManager->getPackage().readBytes(m_textureDataOffsetInPackage, m_textureDataSizeInPackage);

    m_textureData = Ref<std::uint8_t[]>(reinterpret_cast<std::uint8_t*>(textureData));
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
