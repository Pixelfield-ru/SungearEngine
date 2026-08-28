//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#include <cstdint>

#include <sgcore_export.h>

#include "GraphicsDataTypes.h"

namespace SGCore
{
    enum class ChannelKind
    {
        UNKNOWN,
        UNORM,
        SNORM,
        UINT,
        SINT,
        SFLOAT
    };

    /// How an image format stores one texel, channel by channel. Packed, compressed and planar
    /// formats report UNKNOWN: their texels have no channel array to walk.
    struct ChannelLayout
    {
        ChannelKind m_kind = ChannelKind::UNKNOWN;
        std::uint32_t m_channels = 0;
        /// Bytes one channel occupies; 0 together with UNKNOWN.
        std::uint32_t m_channelSize = 0;
        /// True for the BGRA family: channel 0 in memory is blue, not red.
        bool m_reversedChannels = false;

        [[nodiscard]] bool isPlain() const noexcept { return m_kind != ChannelKind::UNKNOWN && m_channelSize != 0; }
    };

    /**
     * Conversion between a CPU pixel buffer (SGGDataType x channels, the way the engine declares its
     * textures) and the channel layout an image actually stores.
     *
     * OpenGL converts on the fly — glTexImage2D narrows the buffer into the internal format and
     * glReadPixels widens it back — while an explicit API copies raw memory in both directions. So
     * both explicit backends have to do this conversion themselves, and they do it here rather than
     * each in its own copy: the two directions have to stay each other's exact inverse, and the
     * engine has textures where the declared data type and the image format genuinely disagree (the
     * SSAO noise hands 32-bit floats to an RGB16F image).
     */
    struct SGCORE_EXPORT PixelConversion
    {
        /// True when the CPU data type is stored exactly the way the image stores a channel.
        [[nodiscard]] static bool typeMatchesLayout(SGGDataType type, const ChannelLayout& layout) noexcept;

        /// One channel of a CPU buffer in the value space GL uploads through: normalized byte
        /// channels come out in [0; 1], float and integer channels keep their magnitude.
        [[nodiscard]] static float decodeSourceChannel(const std::uint8_t* source, SGGDataType type) noexcept;
        /// The same value space, read out of an image channel.
        [[nodiscard]] static float decodeImageChannel(const std::uint8_t* source, const ChannelLayout& layout) noexcept;

        static void encodeImageChannel(std::uint8_t* destination, const ChannelLayout& layout, float value) noexcept;
        static void encodeSourceChannel(std::uint8_t* destination, SGGDataType type, float value) noexcept;

        /// CPU buffer -> image layout (texture upload). Channels the source does not carry (the alpha
        /// of an image widened from RGB) are written as opaque.
        static void encodeToImage(const std::uint8_t* source, std::uint8_t* destination, std::uint64_t pixelCount,
                                  SGGDataType sourceType, std::uint32_t sourceChannels,
                                  const ChannelLayout& layout, std::uint32_t texelSize) noexcept;

        /// Image layout -> CPU buffer (attachment readback).
        static void decodeFromImage(const std::uint8_t* source, std::uint8_t* destination, std::uint64_t pixelCount,
                                    const ChannelLayout& layout, std::uint32_t texelSize,
                                    SGGDataType destinationType, std::uint32_t destinationChannels) noexcept;
    };
}
