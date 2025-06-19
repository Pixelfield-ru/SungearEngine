//
// Created by stuka on 20.06.2025.
//

#ifndef SUNGEARENGINE_ATLAS_H
#define SUNGEARENGINE_ATLAS_H

#include "AtlasRect.h"
#include "SGCore/Graphics/API/ITexture2D.h"

namespace SGCore
{
    struct Atlas
    {
        void findBestRect(const glm::ivec2& textureSize, AtlasRect& outputRect) noexcept;
        void packTexture(const AtlasRect& inRect, const ITexture2D* texture) noexcept;

    private:
        std::vector<AtlasRect> m_atlasFreeRects;
        std::vector<AtlasRect> m_atlasUsedRects;

        glm::ivec2 m_totalSize { };

        static void splitRect(const AtlasRect& rectToSplit, const AtlasRect& innerRect, AtlasRect& biggerSplit, AtlasRect& smallerSplit) noexcept;
    };
}

#endif // SUNGEARENGINE_ATLAS_H
