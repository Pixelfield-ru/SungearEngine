//
// Created by stuka on 29.08.2023.
//

#ifndef SUNGEARENGINE_ICUBEMAPTEXTURE_H
#define SUNGEARENGINE_ICUBEMAPTEXTURE_H

#include "ITexture2D.h"

#include "SGCore/Memory/Assets/CubemapAsset.h"

namespace SGCore
{
    class CubemapAsset;

    class ICubemapTexture : public ITexture2D
    {
    protected:
        std::weak_ptr<CubemapAsset> m_cubemapAsset;

    public:
        virtual void create(std::weak_ptr<CubemapAsset> cubemapAsset) = 0;

        std::weak_ptr<CubemapAsset> getAsset() noexcept;
    };
}

#endif //SUNGEARENGINE_ICUBEMAPTEXTURE_H
