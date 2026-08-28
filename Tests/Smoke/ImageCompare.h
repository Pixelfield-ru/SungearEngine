//
// Created by 8bitniksis on 16.08.2026.
//

#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

#include "SGCore/Graphics/API/IFrameBuffer.h"

namespace SGSmoke
{
    struct RGBA8Image
    {
        std::int32_t m_width { };
        std::int32_t m_height { };
        std::vector<std::uint8_t> m_pixels;

        [[nodiscard]] bool isValid() const noexcept;
    };

    struct CompareResult
    {
        bool m_sizeMismatch { };
        /// Fraction of pixels (0..1) that differ from the reference by more than the channel threshold.
        double m_differingPixelsFraction { };
        std::uint8_t m_maxChannelDifference { };
    };

    /**
     * Converts a raw attachment readback to RGBA8 for PNG output. Supports normalized color
     * formats (R, RG, RGB, BGR, RGBA, BGRA) with UNSIGNED_BYTE, UNSIGNED_SHORT or FLOAT data
     * (floats are clamped to [0;1]); missing channels are filled with 0, missing alpha with 255.
     * @return false for integer / depth / unsupported layouts.
     */
    [[nodiscard]] bool toRGBA8(const SGCore::AttachmentReadback& readback, RGBA8Image& outImage) noexcept;

    /// Flips rows in place: graphics APIs with bottom-left origin return the frame upside down.
    void flipVertically(RGBA8Image& image) noexcept;

    [[nodiscard]] bool writePNG(const std::filesystem::path& path, const RGBA8Image& image) noexcept;
    [[nodiscard]] bool readPNG(const std::filesystem::path& path, RGBA8Image& outImage) noexcept;

    /// A pixel counts as different if any channel differs by more than \p channelThreshold.
    [[nodiscard]] CompareResult compare(const RGBA8Image& actual,
                                        const RGBA8Image& reference,
                                        std::uint8_t channelThreshold) noexcept;

    /// Writes |actual - reference| per channel (white = different) to help locate regressions.
    [[nodiscard]] bool writeDiffPNG(const std::filesystem::path& path,
                                    const RGBA8Image& actual,
                                    const RGBA8Image& reference) noexcept;
}
