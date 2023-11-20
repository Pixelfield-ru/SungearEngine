//
// Created by stuka on 29.08.2023.
//

#ifndef SUNGEARENGINE_ICUBEMAPTEXTURE_H
#define SUNGEARENGINE_ICUBEMAPTEXTURE_H

#include "ITexture2D.h"

#include "SGCore/Memory/Assets/CubemapAsset.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class CubemapAsset;

    class ICubemapTexture : public ITexture2D
    {
    protected:
        Weak<CubemapAsset> m_cubemapAsset;

    public:
        virtual void create(Weak<CubemapAsset> cubemapAsset) = 0;

        Weak<CubemapAsset> getAsset() noexcept;
    };
}

#endif //SUNGEARENGINE_ICUBEMAPTEXTURE_H
