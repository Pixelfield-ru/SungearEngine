//
// Created by stuka on 24.12.2023.
//

#ifndef SUNGEARENGINE_TEXTURESBLOCK_H
#define SUNGEARENGINE_TEXTURESBLOCK_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
// #include "SGCore/Memory/AssetRef.h"
#include "SGCore/Memory/AssetWeakRef.h"

namespace SGCore
{
    class ITexture2D;
    class IShader;
    class IMaterial;

    struct ShaderTextureBinding
    {
        std::string m_bindingName;
        AssetWeakRef<ITexture2D> m_texture;
    };
}

#endif //SUNGEARENGINE_TEXTURESBLOCK_H
