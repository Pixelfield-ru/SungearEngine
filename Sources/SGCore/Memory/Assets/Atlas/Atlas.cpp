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
            .m_size = { textureSize.x, textureSize.y },
            .m_position = { 0, 0 }
        });

        m_totalSize = { textureSize.x, textureSize.y };
    }

    // if no more free rects
    if(m_atlasFreeRects.empty())
    {
        resizeAtlasForTexture(textureSize);
        /*const auto lastTotalSize = m_totalSize;

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
        }*/
    }

    std::int64_t foundRectIdx = m_atlasFreeRects.size() - 1;
    const AtlasRect* bestAtlasRect = &*m_atlasFreeRects.rbegin();

    // while texture not fits in the best atlas rect
    // finding best rect
    for(size_t i = m_atlasFreeRects.size() - 1; i != 0; --i)
    {
        const auto* tmpRect = &m_atlasFreeRects[i];

        /*if(tmpRect->m_size.x < textureSize.x || tmpRect->m_size.y < textureSize.y)
        {
            break;
        }*/

        if(tmpRect->m_size.x < bestAtlasRect->m_size.x || tmpRect->m_size.y < bestAtlasRect->m_size.y)
        {
            bestAtlasRect = tmpRect;
            foundRectIdx = i;
        }

        if(foundRectIdx < 0) break;
    }

    // if best rect not matches
    // todo: maybe add return error or try to flip texture and try to find best rect again
    if(bestAtlasRect->m_size.x < textureSize.x || bestAtlasRect->m_size.y < textureSize.y)
    {
        resizeAtlasForTexture(textureSize);

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
    convertTextureFormatToRGBA32INT(texture->getData().get(), externalTextureData.get(), texture->getWidth() * texture->getHeight(), getSGGEveryChannelSizeInBits(texture->m_internalFormat));

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
                                                    const std::vector<std::uint8_t>& srcChannelsBits) noexcept
{
    std::uint8_t srcBitsPerPixel = 0;
    const std::uint8_t channelsCount = srcChannelsBits.size();

    for(const auto c : srcChannelsBits) srcBitsPerPixel += c;

    static const auto insertBitsInByte = [](std::uint8_t& dstByte, std::uint8_t srcByte, std::uint8_t bitsCount, std::uint8_t bitsSrcPos, std::uint8_t bitsDestPos)
    {
        const std::uint8_t rightBitsCnt = 8 - (bitsSrcPos + bitsCount);
        const std::uint8_t leftBitsCnt = bitsSrcPos;
        const std::uint8_t last = dstByte;
        dstByte |= srcByte;
        dstByte >>= rightBitsCnt;
        dstByte <<= rightBitsCnt;
        dstByte <<= leftBitsCnt;
        dstByte >>= leftBitsCnt;

        dstByte <<= bitsSrcPos;
        dstByte >>= bitsDestPos;

        dstByte |= last;
    };

    static const auto placeChannelFunc = [](std::uint8_t* dstBufferOffset, SGGChannelType channelType, std::uint8_t channelBitSize, const std::uint8_t* srcBufferOffset) {
        const std::uint8_t bytesPerChannel = std::ceil(channelBitSize / 8.0f);
        switch(channelType)
        {
            case SGGChannelType::SGG_R:
            {
                for(std::uint8_t i = 0; i < bytesPerChannel; ++i)
                {
                    if(i == bytesPerChannel - 1)
                    {
                        insertBitsInByte(dstBufferOffset[i], srcBufferOffset[i], channelBitSize - i * 8, 0, 0);
                    }
                    else insertBitsInByte(dstBufferOffset[i], srcBufferOffset[i], 8, 0, 0);


                }

                // *reinterpret_cast<float*>(dstBufferOffset) = *srcBufferOffset / 255.0f * std::numeric_limits<int>::max();
                // *reinterpret_cast<float*>(dstBufferOffset) = *srcBufferOffset;
                break;
            }
            case SGGChannelType::SGG_G:
            {
                for(std::uint8_t i = 0; i < bytesPerChannel; ++i)
                {
                    if(i == bytesPerChannel - 1)
                    {
                        insertBitsInByte(dstBufferOffset[i + 4], srcBufferOffset[i], channelBitSize - i * 8, 0, 0);
                    }
                    else insertBitsInByte(dstBufferOffset[i + 4], srcBufferOffset[i], 8, 0, 0);
                }

                // std::memcpy(dstBufferOffset + 1, srcBufferOffset, 1);

                // *reinterpret_cast<float*>(dstBufferOffset + 4) = *srcBufferOffset / 255.0f * std::numeric_limits<int>::max();
                // *reinterpret_cast<float*>(dstBufferOffset + 4) = *srcBufferOffset;
                break;
            }
            case SGGChannelType::SGG_B:
            {
                for(std::uint8_t i = 0; i < bytesPerChannel; ++i)
                {
                    if(i == bytesPerChannel - 1)
                    {
                        insertBitsInByte(dstBufferOffset[i + 8], srcBufferOffset[i], channelBitSize - i * 8, 0, 0);
                    }
                    else insertBitsInByte(dstBufferOffset[i + 8], srcBufferOffset[i], 8, 0, 0);
                }

                // *reinterpret_cast<float*>(dstBufferOffset + 8) = *srcBufferOffset / 255.0f * std::numeric_limits<int>::max();
                // *reinterpret_cast<float*>(dstBufferOffset + 8) = *srcBufferOffset;
                break;
            }
            case SGGChannelType::SGG_A:
            {
                for(std::uint8_t i = 0; i < bytesPerChannel; ++i)
                {
                    if(i == bytesPerChannel - 1)
                    {
                        insertBitsInByte(dstBufferOffset[i + 12], srcBufferOffset[i], channelBitSize - i * 8, 0, 0);
                    }
                    else insertBitsInByte(dstBufferOffset[i + 12], srcBufferOffset[i], 8, 0, 0);
                }

                // *reinterpret_cast<float*>(dstBufferOffset + 12) = *srcBufferOffset / 255.0f * std::numeric_limits<int>::max();
                // *reinterpret_cast<float*>(dstBufferOffset + 12) = *srcBufferOffset;
                break;
            }
        }
    };

    size_t dstBufferOffset = 0;
    size_t srcBufferOffset = 0;

    for(size_t i = 0; i < pixelsCount; ++i)
    {
        /*reinterpret_cast<float&>(*(dstBuffer + dstBufferOffset)) = std::numeric_limits<float>::max();
        reinterpret_cast<float&>(*(dstBuffer + dstBufferOffset + 4)) = std::numeric_limits<float>::max();
        reinterpret_cast<float&>(*(dstBuffer + dstBufferOffset + 8)) = std::numeric_limits<float>::max();
        reinterpret_cast<float&>(*(dstBuffer + dstBufferOffset + 12)) = std::numeric_limits<float>::max();*/

        /*// const float a = std::numeric_limits<float>::max();
        const float a = 1.0f;
        std::memcpy(dstBuffer + dstBufferOffset, &a, 4);
        // const float b = std::numeric_limits<float>::max();
        const float b = 1.0f;
        std::memcpy(dstBuffer + dstBufferOffset + 4, &b, 4);
        // const float c = std::numeric_limits<float>::max();
        const float c = 1.0f;
        std::memcpy(dstBuffer + dstBufferOffset + 8, &c, 4);*/
        // const float d = std::numeric_limits<float>::max();
        /*const float d = 1.0f;
        std::memcpy(dstBuffer + dstBufferOffset + 12, &d, 4);*/

        if(channelsCount >= 1)
        {
            placeChannelFunc(dstBuffer + dstBufferOffset, SGGChannelType::SGG_R, srcChannelsBits[0], srcBuffer + srcBufferOffset);
        }
        if(channelsCount >= 2)
        {
            placeChannelFunc(dstBuffer + dstBufferOffset, SGGChannelType::SGG_G, srcChannelsBits[1], srcBuffer + srcBufferOffset + (std::uint8_t) std::ceil(srcChannelsBits[0] / 8.0f));
        }
        if(channelsCount >= 3)
        {
            placeChannelFunc(dstBuffer + dstBufferOffset, SGGChannelType::SGG_B, srcChannelsBits[2], srcBuffer + srcBufferOffset + (std::uint8_t) std::ceil(srcChannelsBits[0] / 8.0f) + (std::uint8_t) std::ceil(srcChannelsBits[1] / 8.0f));
        }
        if(channelsCount >= 4)
        {
            placeChannelFunc(dstBuffer + dstBufferOffset, SGGChannelType::SGG_A, srcChannelsBits[3], srcBuffer + srcBufferOffset + (std::uint8_t) std::ceil(srcChannelsBits[0] / 8.0f) + (std::uint8_t) std::ceil(srcChannelsBits[1] / 8.0f) + (std::uint8_t) std::ceil(srcChannelsBits[2] / 8.0f));
        }
        else
        {
            const float a = std::numeric_limits<float>::max();
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
}
