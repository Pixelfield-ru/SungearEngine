//
// Created by stuka on 20.06.2025.
//

#include "Atlas.h"

void SGCore::Atlas::findBestRect(const glm::ivec2& textureSize, AtlasRect& outputRect) noexcept
{
    // if no textures added
    if(m_atlasUsedRects.empty())
    {
        m_atlasFreeRects.push_back({
            .m_size = { textureSize.x, textureSize.y },
            .m_position = { 0, 0 }
        });

        m_totalSize = { textureSize.x, textureSize.y };
    }

    // if no more free rects
    if(m_atlasFreeRects.empty())
    {
        const auto lastTotalSize = m_totalSize;

        // resizing atlas...
        m_totalSize.x = std::max(textureSize.x, m_totalSize.x);
        m_totalSize.y = std::max(textureSize.y, m_totalSize.y);

        const AtlasRect innerRect {
            .m_size = lastTotalSize
        };

        const AtlasRect currentRect {
            .m_size = lastTotalSize
        };

        AtlasRect smallerSplit { };
        AtlasRect biggerSplit { };
        splitRect(currentRect, innerRect, biggerSplit, smallerSplit);

        if(biggerSplit.area() != 0)
        {
            m_atlasFreeRects.push_back(biggerSplit);
        }
        if(smallerSplit.area() != 0)
        {
            m_atlasFreeRects.push_back(smallerSplit);
        }
    }

    std::int64_t foundRectIdx = m_atlasFreeRects.size() - 1;
    const AtlasRect* bestAtlasRect = &*m_atlasFreeRects.rbegin();

    // while texture not fits in the best atlas rect
    // finding best rect
    while(bestAtlasRect->m_size.x < textureSize.x || bestAtlasRect->m_size.y < textureSize.y)
    {
        const auto* tmpRect = &m_atlasFreeRects[foundRectIdx];

        if(tmpRect->m_size.x < bestAtlasRect->m_size.x || tmpRect->m_size.y < bestAtlasRect->m_size.y)
        {
            bestAtlasRect = tmpRect;
        }

        --foundRectIdx;
        if(foundRectIdx < 0) break;
    }

    // if best rect not matches
    // todo: return error or try to flip texture and try to find best rect again or resize atlas
    if(bestAtlasRect->m_size.x < textureSize.x || bestAtlasRect->m_size.y < textureSize.y)
    {
        return;
    }

    outputRect = *bestAtlasRect;

    m_atlasFreeRects[foundRectIdx] = m_atlasFreeRects.back();
    m_atlasFreeRects.pop_back();

    // ==================================================================================
    // splitting best rect
    AtlasRect smallerSplit { };
    AtlasRect biggerSplit { };
    const AtlasRect textureRect {
        .m_size = { textureSize.x, textureSize.y },
    };
    splitRect(outputRect, textureRect, biggerSplit, smallerSplit);

    if(biggerSplit.area() != 0)
    {
        m_atlasFreeRects.push_back(biggerSplit);
    }
    if(smallerSplit.area() != 0)
    {
        m_atlasFreeRects.push_back(smallerSplit);
    }

    // ==================================================================================

    m_atlasUsedRects.push_back(*bestAtlasRect);
}

void SGCore::Atlas::packTexture(const AtlasRect& inRect, const ITexture2D* texture) noexcept
{

}

void SGCore::Atlas::splitRect(const AtlasRect& rectToSplit, const AtlasRect& innerRect, AtlasRect& biggerSplit, AtlasRect& smallerSplit) noexcept
{
    // splitting using left bottom corner
    smallerSplit.m_size.x = innerRect.m_size.x;
    smallerSplit.m_size.y = rectToSplit.m_size.y - innerRect.m_size.y;
    smallerSplit.m_position.x = rectToSplit.m_position.x;
    smallerSplit.m_position.y = rectToSplit.m_position.y + innerRect.m_size.y;

    biggerSplit.m_size.x = rectToSplit.m_size.x - innerRect.m_size.x;
    biggerSplit.m_size.y = innerRect.m_size.y;
    biggerSplit.m_position.x = rectToSplit.m_position.x + innerRect.m_size.x;
    biggerSplit.m_position.y = rectToSplit.m_position.y;

    if(smallerSplit.area() > biggerSplit.area())
    {
        std::swap(biggerSplit, smallerSplit);
    }
}
