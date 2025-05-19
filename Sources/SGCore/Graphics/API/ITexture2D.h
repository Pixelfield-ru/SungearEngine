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

    public:
        friend class AssetManager;

        sg_serde_as_friend()

        sg_implement_type_id(ITexture2D, 1)

        virtual ~ITexture2D() = default;
        
        SGGColorInternalFormat m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
        SGGColorFormat m_format = SGGColorFormat::SGG_RGBA;
        
        int m_channelsCount = 1;

        int m_mipLevel = 0;
        int m_layer = 0;
        
        bool m_isCompressedFormat = false;

        SGGDataType m_dataType = SGGDataType::SGG_UNSIGNED_BYTE;

        bool m_isTextureBuffer = false;
        SGGUsage m_textureBufferUsage = SGGUsage::SGG_STATIC;

        bool m_useMultisampling = false;
        std::uint8_t m_multisamplingSamplesCount = 8;

        /// CAN BE USED ONLY IF THIS TEXTURE IS FRAMEBUFFER ATTACHMENT
        BlendingState m_blendingState;
        /// CAN BE USED ONLY IF THIS TEXTURE IS FRAMEBUFFER ATTACHMENT
        glm::vec4 m_clearColor = { 0.0, 0.0, 0.0, 1.0 };

        virtual void create() = 0;

        template<typename DataType = std::uint8_t>
        requires(std::is_scalar_v<DataType>)
        void create(const DataType* data,
                    const size_t& width,
                    const size_t& height,
                    const int& channelsCount,
                    SGGColorInternalFormat internalFormat,
                    SGGColorFormat format)
        {
            size_t byteSize = width * height * sizeof(DataType) * channelsCount;

            m_width = width;
            m_height = height;
            m_channelsCount = channelsCount;
            m_internalFormat = internalFormat;
            m_format = format;
            m_dataType = getSGDataTypeFromCPPType<DataType>();

            m_textureData = Ref<std::uint8_t[]>(new std::uint8_t[byteSize]);
            std::memcpy(m_textureData.get(), data, byteSize);

            create();
        }

        /// Moves 'data' to ITexture2D and now ITexture2D owns this data
        template<typename DataType = std::uint8_t>
        requires(std::is_scalar_v<DataType>)
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
            m_dataType = getSGDataTypeFromCPPType<DataType>();

            // TODO: IS IT NOT MEMORY LEAK OR UB???
            m_textureData = Ref<std::uint8_t[]>(data);

            create();
        }

        virtual void createAsFrameBufferAttachment(const Ref<IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) = 0;
        
        template<typename DataType = std::uint8_t>
        requires(std::is_scalar_v<DataType>)
        void subTextureBufferData(const DataType* data, const size_t& elementsCount, const size_t& elementsOffset)
        {
            static constexpr int dataTypeSize = sizeof(DataType);

            if(m_frameBufferAttachmentType != SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT || !m_isTextureBuffer) return;

            if((elementsOffset + elementsCount) * dataTypeSize > m_width * m_height * getSGGDataTypeSizeInBytes(m_dataType))
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

            std::memcpy(m_textureData.get() + elementsOffset * dataTypeSize, data, elementsCount * dataTypeSize);
            
            subTextureBufferDataOnGAPISide(elementsCount * dataTypeSize, elementsOffset * dataTypeSize);
        }
        
        // todo:
        template<typename DataType = std::uint8_t>
        requires(std::is_scalar_v<DataType>)
        void subTextureData(const DataType* data, std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY)
        {
            static constexpr int dataTypeSize = sizeof(DataType);

            if(m_frameBufferAttachmentType != SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT || m_isTextureBuffer) return;

            if((areaOffsetX * areaOffsetY + areaWidth * areaHeight) * dataTypeSize > m_width * m_height * getSGGDataTypeSizeInBytes(m_dataType))
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

            std::memcpy(m_textureData.get() + (areaOffsetX + areaOffsetY * m_width) * dataTypeSize,
                        data,
                        areaWidth * areaHeight * dataTypeSize);
            
            subTextureDataOnGAPISide(areaWidth, areaHeight, areaOffsetX, areaOffsetY, dataTypeSize);
        }
        
        virtual void destroy() = 0;

        virtual void bind(const std::uint8_t& textureUnit) const = 0;

        virtual ITexture2D& operator=(const Ref<ITexture2D>& other) = 0;
        //virtual operator=(std::shared_ptr<ITexture2D> other);
        
        virtual void* getTextureNativeHandler() const noexcept = 0;
        virtual void* getTextureBufferNativeHandler() const noexcept = 0;

        [[nodiscard]] Ref<std::uint8_t[]> getData() noexcept;
        
        // TODO: do documentation
        void resize(std::int32_t newWidth, std::int32_t newHeight, bool noDataResize = false) noexcept;

        template<typename DataType>
        [[nodiscard]] glm::vec<4, DataType> sampleRAM(const glm::ivec2& inPosition) const noexcept
        {
            const std::uint16_t texDataTypeSize = getSGGDataTypeSizeInBytes(m_dataType);
            const std::uint16_t perPixelOffset = texDataTypeSize * m_channelsCount;

            const std::int32_t finalPosition = (inPosition.y * m_width + inPosition.x) * perPixelOffset;

            glm::vec<4, DataType> result { };

            auto* data = m_textureData.get();

            if(m_channelsCount >= 1) result.r = *reinterpret_cast<DataType*>(&data[finalPosition]);
            if(m_channelsCount >= 2) result.g = *reinterpret_cast<DataType*>(&data[finalPosition + 1 * texDataTypeSize]);
            if(m_channelsCount >= 3) result.b = *reinterpret_cast<DataType*>(&data[finalPosition + 2 * texDataTypeSize]);
            if(m_channelsCount == 4) result.w = *reinterpret_cast<DataType*>(&data[finalPosition + 3 * texDataTypeSize]);

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

        Ref<std::uint8_t[]> m_textureData = nullptr;

        // USED ONLY IF THIS TEXTURE IS FRAME BUFFER ATTACHMENT
        SGFrameBufferAttachmentType m_frameBufferAttachmentType = SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT;

        virtual void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) { }
        virtual void subTextureDataOnGAPISide(std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY, int dataTypeSize) = 0;

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
