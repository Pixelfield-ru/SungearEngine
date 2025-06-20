//
// Created by stuka on 20.06.2025.
//

#include "Atlas.h"

#include <stb_image_resize2.h>

void SGCore::Atlas::findBestRect(glm::ivec2 textureSize, AtlasRect& outputRect) noexcept
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
    for(size_t i = 0; i < m_atlasFreeRects.size(); i++)
    {
        const auto* tmpRect = &m_atlasFreeRects[foundRectIdx];

        /*if(tmpRect->m_size.x < textureSize.x || tmpRect->m_size.y < textureSize.y)
        {
            break;
        }*/

        if(tmpRect->m_size.x < bestAtlasRect->m_size.x || tmpRect->m_size.y < bestAtlasRect->m_size.y)
        {
            bestAtlasRect = tmpRect;
        }

        --foundRectIdx;
        if(foundRectIdx < 0) break;
    }

    // if best rect not matches
    // todo: maybe add return error or try to flip texture and try to find best rect again
    if(bestAtlasRect->m_size.x < textureSize.x || bestAtlasRect->m_size.y < textureSize.y)
    {
        const auto lastTotalSize = m_totalSize;

        const auto sizesDif = glm::max(textureSize - lastTotalSize, glm::ivec2(0, 0));

        if(sizesDif.x >= sizesDif.y)
        {
            // placing new free rect on right
            m_totalSize += glm::ivec2(textureSize.x, sizesDif.y);

            const AtlasRect newFreeRect {
                .m_size = { textureSize.x, m_totalSize.y },
                .m_position = { lastTotalSize.x, 0 }
            };

            m_atlasFreeRects.push_back(newFreeRect);
        }
        else if(sizesDif.y > sizesDif.x)
        {
            // placing new free rect on top
            m_totalSize += glm::ivec2(sizesDif.x, textureSize.y);

            const AtlasRect newFreeRect {
                .m_size = { m_totalSize.x, textureSize.y },
                .m_position = { 0, lastTotalSize.y }
            };

            m_atlasFreeRects.push_back(newFreeRect);
        }

        bestAtlasRect = &*m_atlasFreeRects.rbegin();
        foundRectIdx = m_atlasFreeRects.size() - 1;
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
    if(!m_atlasTexture)
    {
        m_atlasTexture = SGCore::Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
        /*m_atlasTexture->m_format = SGGColorFormat::SGG_RGBA;
        m_atlasTexture->m_internalFormat = SGGColorInternalFormat::SGG_RGBA8;*/
        m_atlasTexture->m_channelsCount = 4;
        m_atlasTexture->create();
    }

    if(m_totalSize.x > m_atlasTexture->getWidth() || m_totalSize.y > m_atlasTexture->getHeight())
    {
        m_atlasTexture->resizeDataBuffer(m_totalSize.x, m_totalSize.y);
    }

    // ===================================================================================================================
    // ===================================================================================================================
    // ===================================================================================================================

    const std::uint8_t externalTextureChannelSize = getSGGInternalFormatChannelSizeInBytes(texture->m_internalFormat);

    // adjusting the buffer internal format of input texture to internal format of atlas
    std::vector<std::uint8_t> externalTextureData;
    externalTextureData.reserve(texture->getWidth() * texture->getHeight() * texture->m_channelsCount * externalTextureChannelSize);
    for(std::int32_t y = 0; y < texture->getHeight(); ++y)
    {
        for(std::int32_t x = 0; x < texture->getWidth(); ++x)
        {
            const std::int32_t posInExternalTexture = (y * texture->getWidth() + x) * 4 * externalTextureChannelSize;
            // todo: externalTextureData
        }
    }

    if(inRect.m_size.x != texture->getWidth() || inRect.m_size.y != texture->getHeight())
    {
        // resizing data buffer
        std::vector<std::uint8_t> resizedDataBuffer;
        resizedDataBuffer.reserve(inRect.m_size.x * inRect.m_size.y * texture->m_channelsCount * externalTextureChannelSize);
        stbir_resize_uint8_linear(externalTextureData.data(), texture->getWidth(), texture->getHeight(), 0,
                                  resizedDataBuffer.data(), inRect.m_size.x, inRect.m_size.y, 0, STBIR_RGBA);

        externalTextureData = std::move(resizedDataBuffer);
    }

    m_atlasTexture->bind(0);
    m_atlasTexture->subTextureData(externalTextureData.data(), externalTextureChannelSize, inRect.m_size.x, inRect.m_size.y, inRect.m_position.x, inRect.m_position.y);
}

SGCore::Ref<SGCore::ITexture2D> SGCore::Atlas::getTexture() const noexcept
{
    return m_atlasTexture;
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
