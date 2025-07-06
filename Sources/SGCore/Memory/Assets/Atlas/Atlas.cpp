//
// Created by stuka on 20.06.2025.
//

#include "Atlas.h"

#include <stb_image_resize2.h>

#include "SGCore/Graphics/API/GPUDeviceInfo.h"
#include "SGCore/Graphics/API/ITexture2D.h"


void SGCore::Atlas::findBestRect(glm::ivec2 textureSize, rectpack2D::rect_xywh& outputRect, size_t rectHash) noexcept
{
    if(m_atlasRectsMap.contains(rectHash))
    {
        outputRect = m_atlasRects[m_atlasRectsMap[rectHash]];

        return;
    }

    const auto lastRects = m_atlasRects;
    const auto lastRectsCount = m_atlasRects.size();

    m_atlasRects.push_back({ 0, 0, textureSize.x, textureSize.y });

    const bool isPacked = tryPackLastInsertedRect();

    for(size_t i = 0; i < lastRectsCount; ++i)
    {
        const auto& lastRect = lastRects[i];
        const auto& currentRect = m_atlasRects[i];

        if(lastRect.x != currentRect.x || lastRect.y != currentRect.y || lastRect.w != currentRect.w || lastRect.h != currentRect.h)
        {
            std::cout << fmt::format(
                "Discrepancy between the rects! Last rect: pos: {}, {}, size: {}, {}. Current rect: pos: {}, {}, size: {}, {}.",
                lastRect.x, lastRect.y, lastRect.w, lastRect.h, currentRect.x, currentRect.y, currentRect.w, currentRect.h) << std::endl;
        }
    }

    if(!isPacked)
    {
        m_atlasRects.pop_back();
        outputRect.x = -1;
        outputRect.y = -1;
        outputRect.w = -1;
        outputRect.h = -1;

        return;
    }

    const auto r = *m_atlasRects.rbegin();

    outputRect = rectpack2D::rect_xywh(r.x, r.y, r.w, r.h);

    m_atlasRectsMap[rectHash] = m_atlasRects.size() - 1;
}

void SGCore::Atlas::packTexture(const rectpack2D::rect_xywh& inRect, const ITexture2D* texture) noexcept
{
    if(inRect.x + inRect.w > m_maxSideSize || inRect.y + inRect.h > m_maxSideSize ||
       inRect.x < 0 || inRect.y < 0 ||
       inRect.w < 0 || inRect.h < 0)
    {
        LOG_E(SGCORE_TAG,
              "Can not pack texture into atlas! Out of max texture size bounds!\n"
              "\tCurrent atlas size: {}, {}.\n"
              "\tPack position: {}, {}.\n"
              "\tPack size: {}, {}.\n"
              "\tMax texture size: {}, {}.",
              m_atlasSize.x, m_atlasSize.y,
              inRect.x, inRect.y,
              inRect.w, inRect.h,
              m_maxSideSize, m_maxSideSize);

        return;
    }

    if(!m_atlasTexture)
    {
        m_atlasTexture = SGCore::Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
        m_atlasTexture->m_format = SGGColorFormat::SGG_RGBA;
        m_atlasTexture->m_internalFormat = SGGColorInternalFormat::SGG_RGBA32_FLOAT;
        m_atlasTexture->m_channelsCount = 4;
        m_atlasTexture->m_dataType = SGGDataType::SGG_FLOAT;
        m_atlasTexture->create();
    }

    if(m_atlasSize.x > m_atlasTexture->getWidth() || m_atlasSize.y > m_atlasTexture->getHeight())
    {
        m_atlasTexture->resizeDataBuffer(m_atlasSize.x, m_atlasSize.y);
    }

    // ===================================================================================================================
    // ===================================================================================================================
    // ===================================================================================================================

    const std::uint8_t externalTextureChannelsSize = getSGGInternalFormatChannelsSizeInBytes(texture->m_internalFormat);
    const std::uint8_t atlasTextureChannelsSize = getSGGInternalFormatChannelsSizeInBytes(m_atlasTexture->m_internalFormat);

    // adjusting the buffer internal format of input texture to internal format of atlas
    std::vector<std::uint8_t> externalTextureData;
    externalTextureData.resize(texture->getWidth() * texture->getHeight() * atlasTextureChannelsSize, 0);
    convertTextureFormatToRGBA32INT(texture->getData(), externalTextureData.data(), texture->getWidth() * texture->getHeight(), getSGGEveryChannelSizeInBits(texture->m_internalFormat), texture->m_dataType);

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
    m_atlasTexture->subTextureData(externalTextureData.data(), inRect.w, inRect.h, inRect.x, inRect.y);
}

const rectpack2D::rect_xywh* SGCore::Atlas::getRectByHash(size_t hash) const noexcept
{
    const auto it = m_atlasRectsMap.find(hash);
    return it != m_atlasRectsMap.end() ? &m_atlasRects[it->second] : nullptr;
}

SGCore::Ref<SGCore::ITexture2D> SGCore::Atlas::getTexture() const noexcept
{
    return m_atlasTexture;
}

bool SGCore::Atlas::tryPackLastInsertedRect() noexcept
{
    bool isPacked = true;

    auto report_successful = [](rect_type&) {
        return rectpack2D::callback_result::CONTINUE_PACKING;
    };

    auto report_unsuccessful = [&isPacked](rect_type& r) {
        isPacked = false;

        std::cout << "Unsuccessful texture packing! With rect: pos: " << r.x << ", " << r.y << ", size: " << r.w << ", " << r.h << ". Continuing..." << std::endl;

        return rectpack2D::callback_result::CONTINUE_PACKING;
    };

    auto report_result = [this](const rectpack2D::rect_wh& result_size) {
        m_atlasSize.x = result_size.w;
        m_atlasSize.y = result_size.h;

        std::cout << "Packing successful! Resultant bin: " << m_atlasSize.x << ", " << m_atlasSize.y << std::endl;
    };

    const auto discardStep = 1;

    const auto result_size = rectpack2D::find_best_packing_dont_sort<spaces_type>(
        m_atlasRects,
        rectpack2D::make_finder_input(
            m_maxSideSize,
            discardStep,
            report_successful,
            report_unsuccessful,
            rectpack2D::flipping_option::DISABLED
        )
    );

    report_result(result_size);

    return isPacked;
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
}
