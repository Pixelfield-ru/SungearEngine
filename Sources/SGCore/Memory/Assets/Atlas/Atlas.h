//
// Created by stuka on 20.06.2025.
//

#ifndef SUNGEARENGINE_ATLAS_H
#define SUNGEARENGINE_ATLAS_H

#include "AtlasRect.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class ITexture2D;

    struct Atlas
    {
        void findBestRect(glm::ivec2 textureSize, AtlasRect& outputRect) noexcept;
        void packTexture(const AtlasRect& inRect, const ITexture2D* texture) noexcept;

        [[nodiscard]] Ref<ITexture2D> getTexture() const noexcept;

    private:
        std::vector<AtlasRect> m_atlasFreeRects;
        std::vector<AtlasRect> m_atlasUsedRects;

        glm::ivec2 m_totalSize { };

        Ref<ITexture2D> m_atlasTexture;

        static void splitRect(const AtlasRect& rectToSplit, const AtlasRect& innerRect, AtlasRect& biggerSplit, AtlasRect& smallerSplit) noexcept;

        static void convertTextureFormatToRGBA32INT(const std::uint8_t* srcBuffer, std::uint8_t* dstBuffer, size_t pixelsCount, const std::vector<std::uint8_t>& srcChannelsBits, SGGDataType srcBufferDataType) noexcept;

        void resizeAtlasForTexture(glm::ivec2 textureSize) noexcept;
    };
}

#endif // SUNGEARENGINE_ATLAS_H
