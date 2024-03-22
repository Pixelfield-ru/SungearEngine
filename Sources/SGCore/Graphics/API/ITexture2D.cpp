#include <spdlog/spdlog.h>
#include <stb_image.h>

#include "ITexture2D.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"

#define DDSKTX_IMPLEMENT

#include "SGUtils/DDSLoader/dds-ktx.h"
#include "SGUtils/FileUtils.h"

void SGCore::STBITextureDataDeleter::operator()(void* data)
{
    stbi_image_free(data);
}

// ----------------------------------

void SGCore::ITexture2D::load(const std::string& path)
{
    int channelsDesired = 0;
    
    m_path = path;
    
    if(!std::filesystem::exists(path))
    {
        spdlog::error("Error while loading texture: texture by path {0} does not exist.", path);
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
    else if(ext == ".dds")
    {
        size_t bufSize;
        void* buffer = SGUtils::FileUtils::readBytes(path, bufSize);
        ddsktx_texture_info textureInfo;
        
        ddsktx_error error;
        
        ddsktx__parse_dds(&textureInfo, buffer, bufSize, &error);
        
        std::string errMsgStr = error.msg;
        
        if(errMsgStr != "")
        {
            spdlog::error("Error while loading .dds texture by path {0}: {1}", path, errMsgStr);
            return;
        }
        
        m_textureData = Ref<std::uint8_t[]>((std::uint8_t*) buffer);
        
        m_width = textureInfo.width;
        m_height = textureInfo.height;
        
        m_isCompressedFormat = ddsktx_format_compressed(textureInfo.format);
        
        std::cout << "path: " << path << ", format: " << textureInfo.format << std::endl;
        
        // todo: all formats
        switch(textureInfo.format)
        {
            case DDSKTX_FORMAT_BC1:
                break;
            case DDSKTX_FORMAT_BC2:
                break;
            case DDSKTX_FORMAT_BC3:
                break;
            case DDSKTX_FORMAT_BC4:
                break;
            case DDSKTX_FORMAT_BC5:
                break;
            case DDSKTX_FORMAT_BC6H:
                break;
            case DDSKTX_FORMAT_BC7:
                break;
            case DDSKTX_FORMAT_ETC1:
                break;
            case DDSKTX_FORMAT_ETC2:
                break;
            case DDSKTX_FORMAT_ETC2A:
                break;
            case DDSKTX_FORMAT_ETC2A1:
                break;
            case DDSKTX_FORMAT_PTC12:
                break;
            case DDSKTX_FORMAT_PTC14:
                break;
            case DDSKTX_FORMAT_PTC12A:
                break;
            case DDSKTX_FORMAT_PTC14A:
                break;
            case DDSKTX_FORMAT_PTC22:
                break;
            case DDSKTX_FORMAT_PTC24:
                break;
            case DDSKTX_FORMAT_ATC:
                break;
            case DDSKTX_FORMAT_ATCE:
                break;
            case DDSKTX_FORMAT_ATCI:
                break;
            case DDSKTX_FORMAT_ASTC4x4:
                break;
            case DDSKTX_FORMAT_ASTC5x5:
                break;
            case DDSKTX_FORMAT_ASTC6x6:
                break;
            case DDSKTX_FORMAT_ASTC8x5:
                break;
            case DDSKTX_FORMAT_ASTC8x6:
                break;
            case DDSKTX_FORMAT_ASTC10x5:
                break;
            case _DDSKTX_FORMAT_COMPRESSED:
                break;
            case DDSKTX_FORMAT_A8:
                break;
            case DDSKTX_FORMAT_R8:
                m_channelsCount = 1;
                m_internalFormat = SGGColorInternalFormat::SGG_R8;
                m_format = SGGColorFormat::SGG_R;
                break;
            case DDSKTX_FORMAT_RGBA8:
                m_channelsCount = 4;
                m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
                m_format = SGGColorFormat::SGG_RGBA;
                break;
            case DDSKTX_FORMAT_RGBA8S:
                break;
            case DDSKTX_FORMAT_RG16:
                m_channelsCount = 2;
                m_internalFormat = SGGColorInternalFormat::SGG_RG16;
                m_format = SGGColorFormat::SGG_RG;
                break;
            case DDSKTX_FORMAT_RGB8:
                m_channelsCount = 3;
                m_internalFormat = SGGColorInternalFormat::SGG_RGB8;
                m_format = SGGColorFormat::SGG_RGB;
                break;
            case DDSKTX_FORMAT_R16:
                m_channelsCount = 1;
                m_internalFormat = SGGColorInternalFormat::SGG_R16;
                m_format = SGGColorFormat::SGG_R;
                break;
            case DDSKTX_FORMAT_R32F:
                m_channelsCount = 1;
                m_internalFormat = SGGColorInternalFormat::SGG_R32_FLOAT;
                m_format = SGGColorFormat::SGG_R;
                break;
            case DDSKTX_FORMAT_R16F:
                m_channelsCount = 1;
                m_internalFormat = SGGColorInternalFormat::SGG_R16_FLOAT;
                m_format = SGGColorFormat::SGG_R;
                break;
            case DDSKTX_FORMAT_RG16F:
                m_channelsCount = 2;
                m_internalFormat = SGGColorInternalFormat::SGG_RG16_FLOAT;
                m_format = SGGColorFormat::SGG_RG;
                break;
            case DDSKTX_FORMAT_RG16S:
                break;
            case DDSKTX_FORMAT_RGBA16F:
                m_channelsCount = 4;
                m_internalFormat = SGGColorInternalFormat::SGG_RGBA16_FLOAT;
                m_format = SGGColorFormat::SGG_RGBA;
                break;
            case DDSKTX_FORMAT_RGBA16:
                m_channelsCount = 4;
                m_internalFormat = SGGColorInternalFormat::SGG_RGBA16;
                m_format = SGGColorFormat::SGG_RGBA;
                break;
            case DDSKTX_FORMAT_BGRA8:
                break;
            case DDSKTX_FORMAT_RGB10A2:
                break;
            case DDSKTX_FORMAT_RG11B10F:
                break;
            case DDSKTX_FORMAT_RG8:
                m_channelsCount = 2;
                m_internalFormat = SGGColorInternalFormat::SGG_RG8;
                m_format = SGGColorFormat::SGG_RG;
                break;
            case DDSKTX_FORMAT_RG8S:
                break;
            case _DDSKTX_FORMAT_COUNT:
                break;
        }
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
