//
// Created by stuka on 20.06.2025.
//

#include "Atlas.h"

#include <stb_image_resize2.h>

#include "SGCore/Graphics/API/ITexture2D.h"


void SGCore::Atlas::findBestRect(glm::ivec2 textureSize, AtlasRect& outputRect) noexcept
{
    // if no textures added
    if(m_atlasUsedRects.empty())
    {
        m_atlasFreeRects.push_back({
            .m_position = { 0, 0 },
            .m_size = { textureSize.x, textureSize.y }
        });

        m_totalSize = { textureSize.x, textureSize.y };
    }

    int bestScore = std::numeric_limits<int>::max();
    std::int64_t bestRectIndex = -1;

    AtlasRect bestRect { };

    // while texture not fits in the best atlas rect
    // finding best rect
    for(size_t i = 0; i < m_atlasFreeRects.size(); i++)
    {
        const auto& free = m_atlasFreeRects[i];

        // Проверка без поворота
        if(free.m_size.x >= textureSize.x && free.m_size.y >= textureSize.y)
        {
            int score = free.area() - textureSize.x * textureSize.y; // Остаточная площадь

            if(score < bestScore)
            {
                bestScore = score;
                bestRect = free;

                bestRectIndex = i;
            }
        }
    }

    // if best rect not matches
    // todo: maybe add return error or try to flip texture and try to find best rect again
    if(bestScore == std::numeric_limits<int>::max())
    {
        resizeAtlasForTexture(textureSize);

        bestRect = *m_atlasFreeRects.rbegin();
        bestRectIndex = m_atlasFreeRects.size() - 1;
    }

    outputRect = bestRect;

    std::swap(m_atlasFreeRects[bestRectIndex], m_atlasFreeRects.back());
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

    m_atlasUsedRects.push_back(bestRect);
}

void SGCore::Atlas::packTexture(const AtlasRect& inRect, const ITexture2D* texture) noexcept
{
    if(!m_atlasTexture)
    {
        m_atlasTexture = SGCore::Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
        m_atlasTexture->m_format = SGGColorFormat::SGG_RGBA;
        m_atlasTexture->m_internalFormat = SGGColorInternalFormat::SGG_RGBA32_FLOAT;
        m_atlasTexture->m_channelsCount = 4;
        m_atlasTexture->m_dataType = SGGDataType::SGG_FLOAT;
        m_atlasTexture->create();
    }

    if(m_totalSize.x > m_atlasTexture->getWidth() || m_totalSize.y > m_atlasTexture->getHeight())
    {
        m_atlasTexture->resizeDataBuffer(m_totalSize.x, m_totalSize.y);
    }

    // ===================================================================================================================
    // ===================================================================================================================
    // ===================================================================================================================

    const std::uint8_t externalTextureChannelsSize = getSGGInternalFormatChannelsSizeInBytes(texture->m_internalFormat);
    const std::uint8_t atlasTextureChannelsSize = getSGGInternalFormatChannelsSizeInBytes(m_atlasTexture->m_internalFormat);

    // adjusting the buffer internal format of input texture to internal format of atlas
    /*std::vector<std::uint8_t> externalTextureData;
    externalTextureData.resize(texture->getWidth() * texture->getHeight() * atlasTextureChannelsSize, 0);*/

    Scope<std::uint8_t[]> externalTextureData = MakeScope<std::uint8_t[]>(texture->getWidth() * texture->getHeight() * atlasTextureChannelsSize);
    std::cout << "externalTextureData.size(): " << (texture->getWidth() * texture->getHeight() * atlasTextureChannelsSize) << ", texture size: " << (texture->getWidth() * texture->getHeight() * externalTextureChannelsSize) << std::endl;
    convertTextureFormatToRGBA32INT(texture->getData().get(), externalTextureData.get(), texture->getWidth() * texture->getHeight(), getSGGEveryChannelSizeInBits(texture->m_internalFormat), texture->m_dataType);

    /*for(size_t p = 0; p < externalTextureData.size() / 16; ++p)
    {
        std::cout << "pixel " << p << " color: "
        << *(reinterpret_cast<float*>(externalTextureData.data()) + p * 4) << ", "
        << *(reinterpret_cast<float*>(externalTextureData.data()) + 1 + p * 4) << ", "
        << *(reinterpret_cast<float*>(externalTextureData.data()) + 2 + p * 4) << ", "
        << *(reinterpret_cast<float*>(externalTextureData.data()) + 3 + p * 4) << ", "
        << std::endl;
    }*/

    /*if(inRect.m_size.x < texture->getWidth() || inRect.m_size.y < texture->getHeight())
    {
        // resizing data buffer
        std::vector<std::uint8_t> resizedDataBuffer;
        resizedDataBuffer.reserve(inRect.m_size.x * inRect.m_size.y * atlasTextureChannelsSize);
        stbir_resize_uint8_linear(externalTextureData.data(), texture->getWidth(), texture->getHeight(), 0,
                                  resizedDataBuffer.data(), inRect.m_size.x, inRect.m_size.y, 0, STBIR_RGBA);

        externalTextureData = std::move(resizedDataBuffer);
    }*/

    m_atlasTexture->bind(0);
    m_atlasTexture->subTextureData(externalTextureData.get(), texture->getWidth(), texture->getHeight(), inRect.m_position.x, inRect.m_position.y);
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

void SGCore::Atlas::convertTextureFormatToRGBA32INT(const std::uint8_t* srcBuffer,
                                                    std::uint8_t* dstBuffer,
                                                    size_t pixelsCount,
                                                    const std::vector<std::uint8_t>& srcChannelsBits,
                                                    SGGDataType srcBufferDataType) noexcept
{
    std::uint8_t srcBitsPerPixel = 0;
    const std::uint8_t channelsCount = srcChannelsBits.size();

    for(const auto c : srcChannelsBits) srcBitsPerPixel += c;

    static const auto getMaxValue = [](std::int64_t bitsCnt) noexcept
    {
        std::uint64_t v = -1;
        v <<= 64 - bitsCnt;
        v >>= 64 - bitsCnt;

        return v;
    };

    static const auto getBits = []<std::integral T>(T value, std::uint8_t sizeInBits, std::uint8_t bitsCnt, std::uint8_t offset) noexcept
    {
        value <<= offset;
        value >>= sizeInBits - bitsCnt;

        return value;
    };

    static const auto placeChannelFunc = [srcBufferDataType](std::uint8_t* dst, SGGChannelType channelType, std::uint8_t channelBitSize, std::uint8_t channelBitOffset, const std::uint8_t* src) {
        std::uint8_t dstBufferOffset = 0;
        std::uint8_t srcBufferByteOffset = 0;
        std::uint8_t srcBufferBitOffset = 0;
        const std::uint8_t channelByteOffset = (std::uint8_t) std::ceil(channelBitOffset / 8.0f);
        const float channelMaxValue = (float) getMaxValue(channelBitSize);

        switch(channelType)
        {
            case SGGChannelType::SGG_R:
            {
                dstBufferOffset = 0;

                break;
            }
            case SGGChannelType::SGG_G:
            {
                dstBufferOffset = 4;

                break;
            }
            case SGGChannelType::SGG_B:
            {
                dstBufferOffset = 8;

                break;
            }
            case SGGChannelType::SGG_A:
            {
                dstBufferOffset = 12;

                break;
            }
        }

        float col = 0.0f;
        if(srcBufferDataType == SGGDataType::SGG_UNSIGNED_BYTE)
        {
            col = getBits(*(src + channelByteOffset), 8, channelBitSize, 0) / channelMaxValue;
        }

        reinterpret_cast<float&>(*(dst + dstBufferOffset)) = col;
    };

    size_t dstBufferOffset = 0;
    size_t srcBufferOffset = 0;

    for(size_t i = 0; i < pixelsCount; ++i)
    {
        if(channelsCount >= 1)
        {
            placeChannelFunc(dstBuffer + dstBufferOffset, SGGChannelType::SGG_R, srcChannelsBits[0], 0, srcBuffer + srcBufferOffset);
        }
        if(channelsCount >= 2)
        {
            placeChannelFunc(dstBuffer + dstBufferOffset, SGGChannelType::SGG_G, srcChannelsBits[1], srcChannelsBits[0], srcBuffer + srcBufferOffset);
        }
        if(channelsCount >= 3)
        {
            placeChannelFunc(dstBuffer + dstBufferOffset, SGGChannelType::SGG_B, srcChannelsBits[2], srcChannelsBits[0] + srcChannelsBits[1], srcBuffer + srcBufferOffset);
        }
        if(channelsCount >= 4)
        {
            placeChannelFunc(dstBuffer + dstBufferOffset, SGGChannelType::SGG_A, srcChannelsBits[3], srcChannelsBits[0] + srcChannelsBits[1] + srcChannelsBits[2], srcBuffer + srcBufferOffset);
        }
        else
        {
            const float a = 1.0f;
            std::memcpy(dstBuffer + dstBufferOffset + 12, &a, 4);
        }

        dstBufferOffset += 16;
        srcBufferOffset += (std::uint8_t) std::ceil(srcBitsPerPixel / 8.0f);
    }

    std::cout << "dstBufferOffset: " << dstBufferOffset << ", srcBufferOffset: " << srcBufferOffset << std::endl;
}

void SGCore::Atlas::resizeAtlasForTexture(glm::ivec2 textureSize) noexcept
{
    const auto lastTotalSize = m_totalSize;

    const auto sizesDif = glm::max(textureSize - lastTotalSize, glm::ivec2(0, 0));

    if(sizesDif.x >= sizesDif.y)
    {
        // placing new free rect on right
        m_totalSize += glm::ivec2(textureSize.x, sizesDif.y);

        const AtlasRect newFreeRect {
            .m_position = { lastTotalSize.x, 0 },
            .m_size = { textureSize.x, m_totalSize.y }
        };

        m_atlasFreeRects.push_back(newFreeRect);
    }
    else if(sizesDif.y > sizesDif.x)
    {
        // placing new free rect on top
        m_totalSize += glm::ivec2(sizesDif.x, textureSize.y);

        const AtlasRect newFreeRect {
            .m_position = { 0, lastTotalSize.y },
            .m_size = { m_totalSize.x, textureSize.y }
        };

        m_atlasFreeRects.push_back(newFreeRect);
    }
}
