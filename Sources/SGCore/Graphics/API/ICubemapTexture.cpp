//
// Created by stuka on 29.08.2023.
//

#include "ICubemapTexture.h"

SGCore::Weak<SGCore::CubemapAsset> SGCore::ICubemapTexture::getAsset() noexcept
{
    return m_cubemapAsset;
}
