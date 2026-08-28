//
// Created by 8bitniksis on 20.08.2026.
//

#include "PixelConversion.h"

#include <algorithm>
#include <cmath>
#include <cstring>

#include <glm/gtc/packing.hpp>

namespace
{
    std::uint32_t bytesPerChannel(SGGDataType type) noexcept
    {
        const auto size = getSGGDataTypeSizeInBytes(type);
        return size == 0 ? 1 : size;
    }
}

bool SGCore::PixelConversion::typeMatchesLayout(SGGDataType type, const ChannelLayout& layout) noexcept
{
    switch(type)
    {
        case SGGDataType::SGG_FLOAT: return layout.m_kind == ChannelKind::SFLOAT && layout.m_channelSize == 4;
        case SGGDataType::SGG_UNSIGNED_BYTE: return layout.m_kind == ChannelKind::UNORM && layout.m_channelSize == 1;
        case SGGDataType::SGG_BYTE: return layout.m_kind == ChannelKind::SNORM && layout.m_channelSize == 1;
        case SGGDataType::SGG_UNSIGNED_SHORT: return layout.m_kind == ChannelKind::UINT && layout.m_channelSize == 2;
        case SGGDataType::SGG_SHORT: return layout.m_kind == ChannelKind::SINT && layout.m_channelSize == 2;
        case SGGDataType::SGG_UNSIGNED_INT: return layout.m_kind == ChannelKind::UINT && layout.m_channelSize == 4;
        case SGGDataType::SGG_INT: return layout.m_kind == ChannelKind::SINT && layout.m_channelSize == 4;
        default: return false;
    }
}

float SGCore::PixelConversion::decodeSourceChannel(const std::uint8_t* source, SGGDataType type) noexcept
{
    switch(type)
    {
        case SGGDataType::SGG_FLOAT: { float value; std::memcpy(&value, source, sizeof(value)); return value; }
        case SGGDataType::SGG_BYTE: return std::max(static_cast<float>(*reinterpret_cast<const std::int8_t*>(source)) / 127.0f, -1.0f);
        case SGGDataType::SGG_UNSIGNED_SHORT: { std::uint16_t value; std::memcpy(&value, source, sizeof(value)); return static_cast<float>(value); }
        case SGGDataType::SGG_SHORT: { std::int16_t value; std::memcpy(&value, source, sizeof(value)); return static_cast<float>(value); }
        case SGGDataType::SGG_UNSIGNED_INT: { std::uint32_t value; std::memcpy(&value, source, sizeof(value)); return static_cast<float>(value); }
        case SGGDataType::SGG_INT: { std::int32_t value; std::memcpy(&value, source, sizeof(value)); return static_cast<float>(value); }
        default: return static_cast<float>(*source) / 255.0f; // SGG_UNSIGNED_BYTE and anything byte-shaped
    }
}

float SGCore::PixelConversion::decodeImageChannel(const std::uint8_t* source, const ChannelLayout& layout) noexcept
{
    switch(layout.m_kind)
    {
        case ChannelKind::UNORM:
        {
            if(layout.m_channelSize == 1) return static_cast<float>(*source) / 255.0f;
            std::uint16_t value; std::memcpy(&value, source, sizeof(value));
            return static_cast<float>(value) / 65535.0f;
        }
        case ChannelKind::SNORM:
        {
            if(layout.m_channelSize == 1) return std::max(static_cast<float>(*reinterpret_cast<const std::int8_t*>(source)) / 127.0f, -1.0f);
            std::int16_t value; std::memcpy(&value, source, sizeof(value));
            return std::max(static_cast<float>(value) / 32767.0f, -1.0f);
        }
        case ChannelKind::UINT:
        {
            if(layout.m_channelSize == 1) return static_cast<float>(*source);
            if(layout.m_channelSize == 2) { std::uint16_t value; std::memcpy(&value, source, sizeof(value)); return static_cast<float>(value); }
            std::uint32_t value; std::memcpy(&value, source, sizeof(value));
            return static_cast<float>(value);
        }
        case ChannelKind::SINT:
        {
            if(layout.m_channelSize == 1) return static_cast<float>(*reinterpret_cast<const std::int8_t*>(source));
            if(layout.m_channelSize == 2) { std::int16_t value; std::memcpy(&value, source, sizeof(value)); return static_cast<float>(value); }
            std::int32_t value; std::memcpy(&value, source, sizeof(value));
            return static_cast<float>(value);
        }
        case ChannelKind::SFLOAT:
        {
            if(layout.m_channelSize == 2)
            {
                std::uint16_t half; std::memcpy(&half, source, sizeof(half));
                return glm::unpackHalf1x16(half);
            }
            float value; std::memcpy(&value, source, sizeof(value));
            return value;
        }
        default:
            return 0.0f;
    }
}

void SGCore::PixelConversion::encodeImageChannel(std::uint8_t* destination, const ChannelLayout& layout, float value) noexcept
{
    switch(layout.m_kind)
    {
        case ChannelKind::UNORM:
        {
            const float clamped = std::clamp(value, 0.0f, 1.0f);
            if(layout.m_channelSize == 1) { *destination = static_cast<std::uint8_t>(std::lround(clamped * 255.0f)); return; }
            const auto encoded = static_cast<std::uint16_t>(std::lround(clamped * 65535.0f));
            std::memcpy(destination, &encoded, sizeof(encoded));
            return;
        }
        case ChannelKind::SNORM:
        {
            const float clamped = std::clamp(value, -1.0f, 1.0f);
            if(layout.m_channelSize == 1) { const auto e = static_cast<std::int8_t>(std::lround(clamped * 127.0f)); std::memcpy(destination, &e, sizeof(e)); return; }
            const auto encoded = static_cast<std::int16_t>(std::lround(clamped * 32767.0f));
            std::memcpy(destination, &encoded, sizeof(encoded));
            return;
        }
        case ChannelKind::UINT:
        {
            const auto magnitude = static_cast<std::uint32_t>(std::llround(std::max(value, 0.0f)));
            if(layout.m_channelSize == 1) { *destination = static_cast<std::uint8_t>(magnitude); return; }
            if(layout.m_channelSize == 2) { const auto e = static_cast<std::uint16_t>(magnitude); std::memcpy(destination, &e, sizeof(e)); return; }
            std::memcpy(destination, &magnitude, sizeof(magnitude));
            return;
        }
        case ChannelKind::SINT:
        {
            const auto magnitude = static_cast<std::int32_t>(std::llround(value));
            if(layout.m_channelSize == 1) { const auto e = static_cast<std::int8_t>(magnitude); std::memcpy(destination, &e, sizeof(e)); return; }
            if(layout.m_channelSize == 2) { const auto e = static_cast<std::int16_t>(magnitude); std::memcpy(destination, &e, sizeof(e)); return; }
            std::memcpy(destination, &magnitude, sizeof(magnitude));
            return;
        }
        case ChannelKind::SFLOAT:
        {
            if(layout.m_channelSize == 2) { const std::uint16_t half = glm::packHalf1x16(value); std::memcpy(destination, &half, sizeof(half)); return; }
            std::memcpy(destination, &value, sizeof(value));
            return;
        }
        default:
            return;
    }
}

void SGCore::PixelConversion::encodeSourceChannel(std::uint8_t* destination, SGGDataType type, float value) noexcept
{
    switch(type)
    {
        case SGGDataType::SGG_FLOAT:
            std::memcpy(destination, &value, sizeof(value));
            break;
        case SGGDataType::SGG_UNSIGNED_BYTE:
        case SGGDataType::SGG_BOOL:
            *destination = static_cast<std::uint8_t>(std::lround(std::clamp(value, 0.0f, 1.0f) * 255.0f));
            break;
        case SGGDataType::SGG_BYTE:
        {
            const auto encoded = static_cast<std::int8_t>(std::lround(std::clamp(value, -1.0f, 1.0f) * 127.0f));
            std::memcpy(destination, &encoded, sizeof(encoded));
            break;
        }
        case SGGDataType::SGG_UNSIGNED_SHORT:
        {
            const auto encoded = static_cast<std::uint16_t>(std::lround(std::max(value, 0.0f)));
            std::memcpy(destination, &encoded, sizeof(encoded));
            break;
        }
        case SGGDataType::SGG_SHORT:
        {
            const auto encoded = static_cast<std::int16_t>(std::lround(value));
            std::memcpy(destination, &encoded, sizeof(encoded));
            break;
        }
        case SGGDataType::SGG_UNSIGNED_INT:
        {
            const auto encoded = static_cast<std::uint32_t>(std::llround(std::max(value, 0.0f)));
            std::memcpy(destination, &encoded, sizeof(encoded));
            break;
        }
        case SGGDataType::SGG_INT:
        {
            const auto encoded = static_cast<std::int32_t>(std::llround(value));
            std::memcpy(destination, &encoded, sizeof(encoded));
            break;
        }
        default:
            break;
    }
}

void SGCore::PixelConversion::encodeToImage(const std::uint8_t* source, std::uint8_t* destination, std::uint64_t pixelCount,
                                            SGGDataType sourceType, std::uint32_t sourceChannels,
                                            const ChannelLayout& layout, std::uint32_t texelSize) noexcept
{
    if(!source || !destination || pixelCount == 0 || !layout.isPlain() || texelSize == 0) return;

    const std::uint32_t channelSize = bytesPerChannel(sourceType);
    const std::uint32_t channels = sourceChannels == 0 ? 1 : sourceChannels;
    const std::uint64_t sourcePixel = static_cast<std::uint64_t>(channels) * channelSize;
    const bool exact = typeMatchesLayout(sourceType, layout) && !layout.m_reversedChannels;

    // identical layouts: the fast path the 8-bit textures of the engine take
    if(exact && layout.m_channels == channels && texelSize == sourcePixel)
    {
        std::memcpy(destination, source, pixelCount * sourcePixel);
        return;
    }

    if(exact && layout.m_channels > channels && texelSize == static_cast<std::uint64_t>(layout.m_channels) * channelSize)
    {
        // only the channel count differs (an image widened from RGB): copy what there is and make
        // the missing channels opaque. Written through the encoder rather than filled with 0xFF, so
        // that an opaque alpha stays 1.0 for a float image too.
        for(std::uint64_t i = 0; i < pixelCount; ++i)
        {
            std::uint8_t* pixel = destination + i * texelSize;
            std::memcpy(pixel, source + i * sourcePixel, sourcePixel);
            for(std::uint32_t channel = channels; channel < layout.m_channels; ++channel)
            {
                encodeImageChannel(pixel + static_cast<std::uint64_t>(channel) * layout.m_channelSize, layout, 1.0f);
            }
        }
        return;
    }

    const std::uint32_t channelsToConvert = std::min(channels, layout.m_channels);
    for(std::uint64_t i = 0; i < pixelCount; ++i)
    {
        for(std::uint32_t channel = 0; channel < layout.m_channels; ++channel)
        {
            const std::uint32_t target = layout.m_reversedChannels && channel < 3 ? 2 - channel : channel;
            std::uint8_t* out = destination + i * texelSize + static_cast<std::uint64_t>(target) * layout.m_channelSize;

            // channels the source does not carry (the alpha of a widened RGB image) read as opaque
            const float value = channel < channelsToConvert
                                    ? decodeSourceChannel(source + i * sourcePixel + static_cast<std::uint64_t>(channel) * channelSize, sourceType)
                                    : 1.0f;
            encodeImageChannel(out, layout, value);
        }
    }
}

void SGCore::PixelConversion::decodeFromImage(const std::uint8_t* source, std::uint8_t* destination, std::uint64_t pixelCount,
                                              const ChannelLayout& layout, std::uint32_t texelSize,
                                              SGGDataType destinationType, std::uint32_t destinationChannels) noexcept
{
    if(!source || !destination || pixelCount == 0 || !layout.isPlain() || texelSize == 0) return;

    const std::uint32_t channelSize = bytesPerChannel(destinationType);
    const std::uint32_t channels = destinationChannels == 0 ? 1 : destinationChannels;
    const std::uint64_t destinationPixel = static_cast<std::uint64_t>(channels) * channelSize;
    const bool exact = typeMatchesLayout(destinationType, layout) && !layout.m_reversedChannels;

    if(exact && texelSize == destinationPixel)
    {
        std::memcpy(destination, source, pixelCount * destinationPixel);
        return;
    }
    if(exact)
    {
        // only the channel count differs: a widened RGB image is narrowed back
        const std::uint64_t copyBytes = std::min<std::uint64_t>(texelSize, destinationPixel);
        for(std::uint64_t i = 0; i < pixelCount; ++i)
        {
            std::memcpy(destination + i * destinationPixel, source + i * texelSize, copyBytes);
        }
        return;
    }

    const std::uint32_t channelsToConvert = std::min(layout.m_channels, channels);
    for(std::uint64_t i = 0; i < pixelCount; ++i)
    {
        for(std::uint32_t channel = 0; channel < channelsToConvert; ++channel)
        {
            const std::uint32_t sourceChannel = layout.m_reversedChannels && channel < 3 ? 2 - channel : channel;
            const float value = decodeImageChannel(source + i * texelSize + static_cast<std::uint64_t>(sourceChannel) * layout.m_channelSize, layout);
            encodeSourceChannel(destination + i * destinationPixel + static_cast<std::uint64_t>(channel) * channelSize, destinationType, value);
        }
    }
}
