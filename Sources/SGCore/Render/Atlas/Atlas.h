//
// Created by stuka on 27.04.2025.
//

#ifndef SUNGEARENGINE_ATLAS_H
#define SUNGEARENGINE_ATLAS_H
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Memory/AssetRef.h"

namespace SGCore
{
    struct AtlasUV
    {
        glm::vec2 m_uvMin;
        glm::vec2 m_uvMax;
    };

    struct Atlas
    {
        AssetRef<ITexture2D> m_atlasTexture;
        std::vector<AtlasUV> m_atlasUVs;
    };
}

#endif // SUNGEARENGINE_ATLAS_H
