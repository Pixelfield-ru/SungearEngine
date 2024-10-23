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
#include "SGCore/Graphics/GPUObject.h"

namespace SGCore
{
    class TextureAsset;
    class IFrameBuffer;

    struct STBITextureDataDeleter
    {
        void operator()(const std::uint8_t* data);
    };
    
    class ITexture2D : public IAsset, public std::enable_shared_from_this<ITexture2D>, public GPUObject
    {
        friend class IFrameBuffer;
        friend class TextureAsset;
        friend class AssetManager;

    public:
        sg_serde_as_friend()

        static inline size_t asset_type_id = StaticTypeID<ITexture2D>::setID(1);

        virtual ~ITexture2D() = default;
        
        ITexture2D() = default;
        
        SGGColorInternalFormat m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
        SGGColorFormat m_format = SGGColorFormat::SGG_RGBA;
        
        int m_channelsCount = 1;

        int m_mipLevel = 0;
        int m_layer = 0;
        
        bool m_isCompressedFormat = false;

        SGGDataType m_dataType = SGG_UNSIGNED_BYTE;

        bool m_isTextureBuffer = false;
        SGGUsage m_textureBufferUsage = SGGUsage::SGG_STATIC;

        bool m_useMultisampling = false;
        std::uint8_t m_multisamplingSamplesCount = 8;

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

            addToGlobalStorage();
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

            addToGlobalStorage();
        }

        virtual void createAsFrameBufferAttachment(const Ref<IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) = 0;
        
        template<typename DataType = std::uint8_t>
        requires(std::is_scalar_v<DataType>)
        void subTextureBufferData(const DataType* data, const size_t& elementsCount, const size_t& elementsOffset)
        {
            std::memcpy(m_textureData.get() + elementsOffset * sizeof(DataType), data, elementsCount * sizeof(DataType));
            
            subTextureBufferDataOnGAPISide(elementsCount * sizeof(DataType), elementsOffset * sizeof(DataType));
        }
        
        // todo:
        template<typename DataType = std::uint8_t>
        requires(std::is_scalar_v<DataType>)
        void subTextureData(const DataType* data, const size_t& elementsCount, const size_t& elementsOffset)
        {
            /*std::memcpy(m_textureData.get() + elementsOffset * sizeof(DataType), data, elementsCount * sizeof(DataType));
            
            subTextureDataOnGAPISide(elementsCount * sizeof(DataType), elementsOffset * sizeof(DataType));*/
        }
        
        virtual void destroy() = 0;

        virtual void bind(const std::uint8_t& textureUnit) = 0;

        void addToGlobalStorage() noexcept final;

        virtual ITexture2D& operator=(const Ref<ITexture2D>& other) = 0;
        //virtual operator=(std::shared_ptr<ITexture2D> other);
        
        virtual void* getTextureNativeHandler() const noexcept = 0;
        virtual void* getTextureBufferNativeHandler() const noexcept = 0;

        [[nodiscard]] Ref<std::uint8_t[]> getData() noexcept;
        
        // TODO: do documentation
        void resize(std::int32_t newWidth, std::int32_t newHeight, bool noDataResize = false) noexcept;

        [[nodiscard]] std::int32_t getWidth() const noexcept;
        [[nodiscard]] std::int32_t getHeight() const noexcept;
        
    protected:
        std::int32_t m_width = 0;
        std::int32_t m_height = 0;
        
        size_t m_pixelSize = 0;

        void doLoad(const std::filesystem::path& path) override;
        void doLazyLoad() override;
        
        virtual void subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) { }
        virtual void subTextureDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) = 0;
        
        Ref<std::uint8_t[]> m_textureData = nullptr;

    private:
        template<typename InstanceT, typename... AssetCtorArgs>
        requires(std::is_same_v<ITexture2D, InstanceT>)
        static Ref<InstanceT> createRefInstance(AssetCtorArgs&&... assetCtorArgs) noexcept
        {
            auto tex = Ref<InstanceT>(CoreMain::getRenderer()->createTexture2D());

            tex->addToGlobalStorage(std::forward<AssetCtorArgs>(assetCtorArgs)...);

            return tex;
        }
    };
}

#endif //NATIVECORE_ITEXTURE2D_H
