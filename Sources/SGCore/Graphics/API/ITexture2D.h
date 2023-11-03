//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_ITEXTURE2D_H
#define NATIVECORE_ITEXTURE2D_H

#include <iostream>
#include "GraphicsDataTypes.h"
#include "SGCore/Memory/Assets/Texture2DAsset.h"

namespace Core::Memory::Assets
{
    class Texture2DAsset;
}

namespace Core::Graphics
{
    class ITexture2D : public Memory::Assets::IAssetObserver, public std::enable_shared_from_this<ITexture2D>
    {
    public:
        SGTextureType m_textureType = SGTextureType::SGTP_DIFFUSE;

        virtual void create(std::weak_ptr<Memory::Assets::Texture2DAsset>) { };
        virtual void destroy() = 0;

        virtual void bind(const std::uint8_t& textureUnit) = 0;

        std::weak_ptr<Memory::Assets::Texture2DAsset> getAsset() noexcept;

        virtual ITexture2D& operator=(const std::shared_ptr<ITexture2D>& other) = 0;
        //virtual operator=(std::shared_ptr<ITexture2D> other);

    protected:
        std::weak_ptr<Memory::Assets::Texture2DAsset> m_texture2DAsset;
    };
}

#endif //NATIVECORE_ITEXTURE2D_H
