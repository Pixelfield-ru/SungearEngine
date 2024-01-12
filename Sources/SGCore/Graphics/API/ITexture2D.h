//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_ITEXTURE2D_H
#define NATIVECORE_ITEXTURE2D_H

#include <iostream>

#include "GraphicsDataTypes.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Main/CoreMain.h"

namespace SGCore
{
    class TextureAsset;

    struct TextureDataDeleter
    {
        void operator()(std::uint8_t* data);
    };

    class ITexture2D : public IAsset, public UniqueNameWrapper, public std::enable_shared_from_this<ITexture2D>
    {
        friend class IFrameBuffer;
        friend class TextureAsset;
        friend class AssetManager;

    public:
        SGGColorInternalFormat m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;
        SGGColorFormat m_format = SGGColorFormat::SGG_RGBA;

        int m_width = 0;
        int m_height = 0;
        int m_channelsCount = 0;

        int m_mipLevel = 0;
        int m_layer = 0;

        bool m_useMultisampling = false;
        std::uint8_t m_multisamplingSamplesCount = 8;

        [[nodiscard]] Ref<IAsset> load(const std::string& path) override;

        virtual void create() = 0;
        virtual void createAsFrameBufferAttachment(const Ref<IFrameBuffer>& parentFrameBuffer, SGFrameBufferAttachmentType attachmentType) = 0;

        virtual void destroy() = 0;

        virtual void bind(const std::uint8_t& textureUnit) = 0;

        Ref<ITexture2D> addToGlobalStorage() noexcept;

        virtual ITexture2D& operator=(const Ref<ITexture2D>& other) = 0;
        //virtual operator=(std::shared_ptr<ITexture2D> other);

        [[nodiscard]] Ref<std::uint8_t[]> getData() noexcept;

    protected:
        Ref<std::uint8_t[]> m_textureData;

    private:
        template<typename InstanceT>
        requires(std::is_same_v<ITexture2D, InstanceT>)
        static Ref<InstanceT> createRefInstance() noexcept
        {
            return Ref<InstanceT>(CoreMain::getRenderer().createTexture2D());
        }
    };
}

#endif //NATIVECORE_ITEXTURE2D_H
