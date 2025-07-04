//
// Created by stuka on 20.06.2025.
//

#include "Atlas.h"

#include <stb_image_resize2.h>

#include "SGCore/Graphics/API/GPUDeviceInfo.h"
#include "SGCore/Graphics/API/ITexture2D.h"


void SGCore::Atlas::findBestRect(glm::ivec2 textureSize, AtlasRect& outputRect) noexcept
{
    m_atlasRects.push_back({ 0, 0, textureSize.x, textureSize.y });

    const bool isPacked = tryPack(textureSize, outputRect);

    if(!isPacked)
    {
        m_atlasRects.pop_back();
        outputRect.m_position = { -1, -1 };
        outputRect.m_size = { -1, -1 };

        return;
    }

    const auto r = *m_atlasRects.rbegin();

    outputRect = {
        .m_position = { r.x, r.y },
        .m_size = { r.w, r.h },
    };
}

void SGCore::Atlas::packTexture(const AtlasRect& inRect, const ITexture2D* texture) noexcept
{
    if(inRect.m_position.x + inRect.m_size.x > m_maxSideSize || inRect.m_position.y + inRect.m_size.y > m_maxSideSize ||
       inRect.m_position.x < 0 || inRect.m_position.y < 0 ||
       inRect.m_size.x < 0 || inRect.m_size.y < 0)
    {
        LOG_E(SGCORE_TAG,
              "Can not pack texture into atlas! Out of max texture size bounds!\n"
              "\tCurrent atlas size: {}, {}.\n"
              "\tPack position: {}, {}.\n"
              "\tPack size: {}, {}.\n"
              "\tMax texture size: {}, {}.",
              m_currentSideSize, m_currentSideSize,
              inRect.m_position.x, inRect.m_position.y,
              inRect.m_size.x, inRect.m_size.y,
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

    if(m_currentSideSize > m_atlasTexture->getWidth() || m_currentSideSize > m_atlasTexture->getHeight())
    {
        m_atlasTexture->resizeDataBuffer(m_currentSideSize, m_currentSideSize);
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
    m_atlasTexture->subTextureData(externalTextureData.data(), texture->getWidth(), texture->getHeight(), inRect.m_position.x, inRect.m_position.y);
}

SGCore::Ref<SGCore::ITexture2D> SGCore::Atlas::getTexture() const noexcept
{
    return m_atlasTexture;
}

bool SGCore::Atlas::tryPack(glm::ivec2 textureSize, AtlasRect& outputRect) noexcept
{
    bool isSuccessful = true;

    bool isPacked = true;

    auto report_successful = [](rect_type&) {
        return rectpack2D::callback_result::CONTINUE_PACKING;
    };

    auto report_unsuccessful = [&isSuccessful](rect_type&) {
        isSuccessful = false;
        return rectpack2D::callback_result::ABORT_PACKING;
    };

    auto report_result = [&isSuccessful, &textureSize, &outputRect, &isPacked, this](const rectpack2D::rect_wh& result_size) {
        std::cout << "Resultant bin: " << result_size.w << " " << result_size.h << std::endl;

        if(!isSuccessful)
        {
            m_currentSideSize += textureSize.x;

            if(m_currentSideSize > m_maxSideSize)
            {
                isPacked = false;
                return;
            }

            isPacked = tryPack(textureSize, outputRect);
        }
        else
        {
            std::cout << "Packing successful!" << std::endl;
        }
    };

    const auto discardStep = -4;

    const auto result_size = rectpack2D::find_best_packing_dont_sort<spaces_type>(
        m_atlasRects,
        rectpack2D::make_finder_input(
            m_currentSideSize,
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
