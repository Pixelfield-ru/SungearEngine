//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_ITEXTURE2D_H
#define NATIVECORE_ITEXTURE2D_H

#include <iostream>
#include "GraphicsDataTypes.h"
#include "SGCore/Memory/Assets/Texture2DAsset.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class Texture2DAsset;

    class ITexture2D : public IAssetObserver, public std::enable_shared_from_this<ITexture2D>
    {
    public:
        virtual void create(Weak<Texture2DAsset>) { };
        virtual void destroy() = 0;

        virtual void bind(const std::uint8_t& textureUnit) = 0;

        // todo:: WTH BRUH. MUST RETURN LOCKED WEAK
        Weak<Texture2DAsset> getAsset() noexcept;

        virtual ITexture2D& operator=(const Ref<ITexture2D>& other) = 0;
        //virtual operator=(std::shared_ptr<ITexture2D> other);

    protected:
        std::weak_ptr<Texture2DAsset> m_texture2DAsset;
    };
}

#endif //NATIVECORE_ITEXTURE2D_H
