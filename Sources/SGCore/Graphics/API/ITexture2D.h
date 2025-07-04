//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_ITEXTURE2D_H
#define NATIVECORE_ITEXTURE2D_H

#include <SGCore/pch.h>

#include "GraphicsDataTypes.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"

#include <gli/gli.hpp>

#include "TextureStorageMode.h"
#include "SGCore/Utils/Assert.h"

namespace SGCore
{
    class IFrameBuffer;

    struct STBITextureDataDeleter
    {
        void operator()(const std::uint8_t* data);
    };
    
    class ITexture2D : public IAsset, public std::enable_shared_from_this<ITexture2D>
    {
        friend class IFrameBuffer;

        using data_ptr = std::unique_ptr<std::uint8_t[], STBITextureDataDeleter>;

    public:
        friend class AssetManager;

        sg_serde_as_friend()

        sg_implement_type_id(ITexture2D, 1)

        virtual ~ITexture2D() = default;
        
        SGGColorInternalFormat m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
        SGGColorFormat m_format = SGGColorFormat::SGG_RGBA;
        // type of data in buffer (m_internalFormat & m_format). MUST MATCH THE INTERNAL FORMAT AND THE FORMAT.
        SGGDataType m_dataType = SGGDataType::SGG_UNSIGNED_BYTE;
        
        int m_channelsCount = 4;

        int m_mipLevel = 0;
        int m_layer = 0;
        
        bool m_isCompressedFormat = false;

        bool m_isTextureBuffer = false;
        SGGUsage m_textureBufferUsage = SGGUsage::SGG_STATIC;

        bool m_useMultisampling = false;
        std::uint8_t m_multisamplingSamplesCount = 8;

        /// CAN BE USED ONLY IF THIS TEXTURE IS FRAMEBUFFER ATTACHMENT
        BlendingState m_blendingState;
        /// CAN BE USED ONLY IF THIS TEXTURE IS FRAMEBUFFER ATTACHMENT
        glm::vec4 m_clearColor = { 0.0, 0.0, 0.0, 1.0 };

        virtual void create() = 0;

        /**
         *
         * @tparam DataType
         * @param data
         * @param width Width of texture (in pixels).
         * @param height Height of texture (in pixels).
         * @param channelsCount
         * @param internalFormat
         * @param format
         */
        template<typename DataType = std::uint8_t>
        void create(const DataType* data,
                    const size_t& width,
                    const size_t& height,
                    const int& channelsCount,
                    SGGColorInternalFormat internalFormat,
                    SGGColorFormat format)
        {
            const size_t byteSize = width * height * getSGGInternalFormatChannelsSizeInBytes(internalFormat);

            m_width = width;
            m_height = height;
            m_channelsCount = channelsCount;
            m_internalFormat = internalFormat;
            m_format = format;

            m_textureData = data_ptr(new std::uint8_t[byteSize]);
            std::memcpy(m_textureData.get(), reinterpret_cast<const std::uint8_t*>(data), byteSize);

            create();
        }

        /**
         * Moves 'data' to ITexture2D and now ITexture2D owns this data
         * @tparam DataType
         * @param data
         * @param width Width of texture (in pixels).
         * @param height Height of texture (in pixels).
         * @param channelsCount
         * @param internalFormat
         * @param format
         */
        template<typename DataType = std::uint8_t>
        void moveAndCreate(DataType* data,
                           const size_t& width,
                           const size_t& height,
                           const int& channelsCount,
                           SGGColorInternalFormat internalFormat,
                           SGGColorFormat format)
        {
            // size_t byteSize = width * height * sizeof(DataType) * channelsCount;

            m_width = width;
            m_height = height;
            m_channelsCount = channelsCount;
            m_internalFormat = internalFormat;
            m_format = format;

            // TODO: IS IT NOT MEMORY LEAK OR UB???
            m_textureData = data_ptr(reinterpret_cast<std::uint8_t*>(data));

            create();
        }

        virtual void createAsFrameBufferAttachment(const Ref<IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) = 0;

        /**
         * PLEASE NOTE THAT INTERNAL FORMAT OF data BUFFER MUST BE EQUAL TO INTERNAL FORMAT OF THIS TEXTURE.
         * @param data Buffer with new data.
         * @param elementsCount Count of elements in data buffer. Not in bytes!
         * @param elementsOffset Offset in data buffer. Not in bytes!
         */
        void subTextureBufferData(const std::uint8_t* data, const size_t& elementsCount, const size_t& elementsOffset)
        {
            const int dataChannelsSize = getSGGInternalFormatChannelsSizeInBytes(m_internalFormat);

            if(m_frameBufferAttachmentType != SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT || !m_isTextureBuffer) return;

            // we are using size of data type to check if out of bounds. NOT SIZE OF CHANNEL
            if((elementsOffset + elementsCount) * dataChannelsSize > m_width * m_height * dataChannelsSize)
            {
                SG_ASSERT(false,
                          fmt::format("Can not do subTextureBufferData(...): out of bounds of texture size!\n"
                              "Texture path: '{}'\n"
                              "Texture alias: '{}'\n"
                              "Texture size: {}x{}\n"
                              "Offset to subdata: {}\n"
                              "Elements count to subdata: {}",
                              Utils::toUTF8(getPath().resolved().u16string()),
                              getAlias(),
                              m_width,
                              m_height,
                              elementsOffset,
                              elementsCount).c_str());

                return;
            }

            std::memcpy(m_textureData.get() + size_t(elementsOffset) * dataChannelsSize, data, size_t(elementsCount) * dataChannelsSize);
            
            subTextureBufferDataOnGAPISide(elementsCount * dataChannelsSize, elementsOffset * dataChannelsSize);
        }

        /**
         * PLEASE NOTE THAT INTERNAL FORMAT OF data BUFFER MUST BE EQUAL TO INTERNAL FORMAT OF THIS TEXTURE.
         * @tparam DataType
         * @param data Buffer with new data.
         * @param areaWidth Width of area to subdata. Not in bytes!
         * @param areaHeight Height of area to subdata. Not in bytes!
         * @param areaOffsetX X offset of area to subdata. Not in bytes!
         * @param areaOffsetY Y offset of area to subdata. Not in bytes!
         */
        void subTextureData(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY)
        {
            const int dataChannelsSize = getSGGInternalFormatChannelsSizeInBytes(m_internalFormat);

            if(m_frameBufferAttachmentType != SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT || m_isTextureBuffer) return;

            if(size_t(areaOffsetX * areaOffsetY + areaWidth * areaHeight) * dataChannelsSize > size_t(m_width * m_height) * dataChannelsSize)
            {
                SG_ASSERT(false,
                          fmt::format("Can not do subTextureBufferData(...): out of bounds of texture size (by bytes)!\n"
                              "Texture path: '{}'\n"
                              "Texture alias: '{}'\n"
                              "Texture size: width: {}, height: {}\n"
                              "Area offset to subdata: x: {}, y: {}\n"
                              "Area size to subdata: width: {}, height: {}",
                              Utils::toUTF8(getPath().resolved().u16string()),
                              getAlias(),
                              m_width,
                              m_height,
                              areaOffsetX,
                              areaOffsetY,
                              areaWidth,
                              areaHeight).c_str());

                return;
            }

            for(size_t y = 0; y < areaHeight; y++)
            {
                std::memcpy(m_textureData.get() + size_t(areaOffsetX + (areaOffsetY + y) * m_width) * dataChannelsSize,
                        data + size_t(y * areaWidth) * dataChannelsSize,
                        areaWidth * dataChannelsSize);
            }

            /*std::memcpy(m_textureData.get() + (areaOffsetX + areaOffsetY * m_width) * dataChannelsSize,
                        data,
                        areaWidth * areaHeight * dataChannelsSize);*/

            subTextureDataOnGAPISide(data, areaWidth, areaHeight, areaOffsetX, areaOffsetY);
        }
        
        virtual void destroy() = 0;

        virtual void bind(const std::uint8_t& textureUnit) const = 0;

        virtual ITexture2D& operator=(const Ref<ITexture2D>& other) = 0;
        //virtual operator=(std::shared_ptr<ITexture2D> other);
        
        virtual void* getTextureNativeHandler() const noexcept = 0;
        virtual void* getTextureBufferNativeHandler() const noexcept = 0;

        [[nodiscard]] std::uint8_t* getData() noexcept;
        [[nodiscard]] const std::uint8_t* getData() const noexcept;

        /**
         * Stretches/compresses the texture to the desired size.
         * @param newWidth New width of texture (in pixels).
         * @param newHeight New height of texture (in pixels).
         * @param noDataResize Is it necessary to stretch/compress the texture to the required size? If the value is false, the width and height of the texture will be changed, but the data will not.
         */
        void resize(std::int32_t newWidth, std::int32_t newHeight, bool noDataResize = false) noexcept;

        /**
         * Resizes data buffer and width and height of texture but not stretches/compresses texture to the desired size.
         * @param newWidth New width of data buffer.
         * @param newHeight New height of data buffer.
         * @param saveData Is data of texture needs to be saved.
         */
        void resizeDataBuffer(std::int32_t newWidth, std::int32_t newHeight, bool saveData = true) noexcept;

        template<typename DataType>
        [[nodiscard]] glm::vec<4, DataType> sampleRAM(const glm::ivec2& inPosition) const noexcept
        {
            const float texDataTypeSize = getSGGInternalFormatChannelsSizeInBytes(m_internalFormat) / m_channelsCount;
            const std::uint16_t perPixelOffset = getSGGInternalFormatChannelsSizeInBytes(m_internalFormat);

            const size_t finalPosition = (size_t(inPosition.y * m_width) + inPosition.x) * perPixelOffset;

            glm::vec<4, DataType> result { };

            auto* data = m_textureData.get();

            if(m_channelsCount >= 1) result.r = *reinterpret_cast<DataType*>(&data[finalPosition]);
            if(m_channelsCount >= 2) result.g = *reinterpret_cast<DataType*>(&data[finalPosition + std::int32_t(1 * texDataTypeSize)]);
            if(m_channelsCount >= 3) result.b = *reinterpret_cast<DataType*>(&data[finalPosition + std::int32_t(2 * texDataTypeSize)]);
            if(m_channelsCount == 4) result.w = *reinterpret_cast<DataType*>(&data[finalPosition + std::int32_t(3 * texDataTypeSize)]);

            return result;
        }

        [[nodiscard]] std::int32_t getWidth() const noexcept;
        [[nodiscard]] std::int32_t getHeight() const noexcept;

        /**
         * @return What type of framebuffer attachment is this texture.\n
         * Returns \p SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT if this texture is not framebuffer attachment.
         */
        [[nodiscard]] SGFrameBufferAttachmentType getFrameBufferAttachmentType() const noexcept;
        
    protected:
        // USED ONLY FOR DDS AND KTX. an extra half a kilobyte (((
        gli::texture m_gliTexture;

        std::int32_t m_width = 0;
        std::int32_t m_height = 0;
        
        size_t m_pixelSize = 0;

        data_ptr m_textureData = nullptr;

        // USED ONLY IF THIS TEXTURE IS FRAME BUFFER ATTACHMENT
        SGFrameBufferAttachmentType m_frameBufferAttachmentType = SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT;

        virtual void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) { }

        /**
         * Internal subdata. GAPI specific.
         * @param areaWidth Width of area to subdata. Not in bytes!
         * @param areaHeight Height of area to subdata. Not in bytes!
         * @param areaOffsetX X offset of area to subdata. Not in bytes!
         * @param areaOffsetY Y offset of area to subdata. Not in bytes!
         * @param dataChannelsSize Size of channels in bytes.
         */
        virtual void subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY) = 0;

    private:
        std::streamsize m_textureDataOffsetInPackage = 0;
        std::streamsize m_textureDataSizeInPackage = 0;

        void doLoad(const InterpolatedPath& path) override;
        void doLazyLoad() override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        template<typename... AssetCtorArgs>
        static Ref<ITexture2D> createRefInstance(AssetCtorArgs&&... assetCtorArgs) noexcept
        {
            auto tex = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D(std::forward<AssetCtorArgs>(assetCtorArgs)...));

            return tex;
        }
    };
}

#endif //NATIVECORE_ITEXTURE2D_H
