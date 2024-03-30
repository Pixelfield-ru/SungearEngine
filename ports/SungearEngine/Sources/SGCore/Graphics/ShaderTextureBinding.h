//
// Created by stuka on 24.12.2023.
//

#ifndef SUNGEARENGINE_TEXTURESBLOCK_H
#define SUNGEARENGINE_TEXTURESBLOCK_H

#include <string>
#include <list>
#include <vector>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"

namespace SGCore
{
    class ITexture2D;
    class ISubPassShader;
    class IMaterial;

    struct ShaderTextureBinding
    {
        std::string m_bindingName;
        Weak<ITexture2D> m_texture;
    };
}

#endif //SUNGEARENGINE_TEXTURESBLOCK_H
