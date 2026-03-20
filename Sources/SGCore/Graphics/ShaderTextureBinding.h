//
// Created by stuka on 24.12.2023.
//

#pragma once

#include "SGCore/Memory/AssetWeakRef.h"

namespace SGCore
{
    class ITexture2D;
    class IShader;
    class IMaterial;

    struct SGCORE_EXPORT ShaderTextureBinding
    {
        std::string m_bindingName;
        AssetWeakRef<ITexture2D> m_texture;
    };
}
