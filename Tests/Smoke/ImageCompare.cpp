//
// Created by 8bitniksis on 16.08.2026.
//

#include "ImageCompare.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

bool SGSmoke::RGBA8Image::isValid() const noexcept
{
    return m_width > 0 && m_height > 0 &&
           m_pixels.size() == static_cast<std::size_t>(m_width) * m_height * 4;
}

bool SGSmoke::toRGBA8(const SGCore::AttachmentReadback& readback, RGBA8Image& outImage) noexcept
{
    // channel order in the source: index of R, G, B, A within a source pixel (-1 = absent)
    int sourceIndex[4] = { -1, -1, -1, -1 };
    switch(readback.m_format)
    {
        case SGGColorFormat::SGG_R:    sourceIndex[0] = 0; break;
        case SGGColorFormat::SGG_RG:   sourceIndex[0] = 0; sourceIndex[1] = 1; break;
        case SGGColorFormat::SGG_RGB:  sourceIndex[0] = 0; sourceIndex[1] = 1; sourceIndex[2] = 2; break;
        case SGGColorFormat::SGG_BGR:  sourceIndex[0] = 2; sourceIndex[1] = 1; sourceIndex[2] = 0; break;
        case SGGColorFormat::SGG_RGBA: sourceIndex[0] = 0; sourceIndex[1] = 1; sourceIndex[2] = 2; sourceIndex[3] = 3; break;
        case SGGColorFormat::SGG_BGRA: sourceIndex[0] = 2; sourceIndex[1] = 1; sourceIndex[2] = 0; sourceIndex[3] = 3; break;
        default: return false;
    }

    const std::size_t pixelsCount = static_cast<std::size_t>(readback.m_width) * readback.m_height;
    const std::size_t channels = static_cast<std::size_t>(readback.m_channelsCount);

    auto readChannel = [&](std::size_t pixel, int channel) -> std::uint8_t
    {
        const std::size_t index = pixel * channels + static_cast<std::size_t>(channel);
        switch(readback.m_dataType)
        {
            case SGGDataType::SGG_UNSIGNED_BYTE:
                return readback.m_data[index];
            case SGGDataType::SGG_UNSIGNED_SHORT:
            {
                std::uint16_t value { };
                std::memcpy(&value, readback.m_data.data() + index * sizeof(value), sizeof(value));
                return static_cast<std::uint8_t>(value >> 8);
            }
            case SGGDataType::SGG_FLOAT:
            {
                float value { };
                std::memcpy(&value, readback.m_data.data() + index * sizeof(value), sizeof(value));
                return static_cast<std::uint8_t>(std::clamp(value, 0.0f, 1.0f) * 255.0f + 0.5f);
            }
            default:
                return 0;
        }
    };

    if(readback.m_dataType != SGGDataType::SGG_UNSIGNED_BYTE &&
       readback.m_dataType != SGGDataType::SGG_UNSIGNED_SHORT &&
       readback.m_dataType != SGGDataType::SGG_FLOAT)
    {
        return false;
    }

    outImage.m_width = readback.m_width;
    outImage.m_height = readback.m_height;
    outImage.m_pixels.resize(pixelsCount * 4);

    for(std::size_t pixel = 0; pixel < pixelsCount; ++pixel)
    {
        for(int channel = 0; channel < 4; ++channel)
        {
            const int source = sourceIndex[channel];
            outImage.m_pixels[pixel * 4 + channel] =
                source >= 0 ? readChannel(pixel, source) : (channel == 3 ? 255 : 0);
        }
    }

    return outImage.isValid();
}

void SGSmoke::flipVertically(RGBA8Image& image) noexcept
{
    if(!image.isValid()) return;

    const std::size_t rowSize = static_cast<std::size_t>(image.m_width) * 4;
    std::vector<std::uint8_t> rowBuffer(rowSize);

    for(std::int32_t y = 0; y < image.m_height / 2; ++y)
    {
        auto* top = image.m_pixels.data() + static_cast<std::size_t>(y) * rowSize;
        auto* bottom = image.m_pixels.data() + static_cast<std::size_t>(image.m_height - 1 - y) * rowSize;

        std::copy(top, top + rowSize, rowBuffer.data());
        std::copy(bottom, bottom + rowSize, top);
        std::copy(rowBuffer.data(), rowBuffer.data() + rowSize, bottom);
    }
}

bool SGSmoke::writePNG(const std::filesystem::path& path, const RGBA8Image& image) noexcept
{
    if(!image.isValid()) return false;

    const std::string pathStr = path.string();
    return stbi_write_png(pathStr.c_str(), image.m_width, image.m_height, 4,
                          image.m_pixels.data(), image.m_width * 4) != 0;
}

bool SGSmoke::readPNG(const std::filesystem::path& path, RGBA8Image& outImage) noexcept
{
    const std::string pathStr = path.string();

    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_uc* data = stbi_load(pathStr.c_str(), &width, &height, &channels, 4);

    if(!data) return false;

    outImage.m_width = width;
    outImage.m_height = height;
    outImage.m_pixels.assign(data, data + static_cast<std::size_t>(width) * height * 4);

    stbi_image_free(data);

    return outImage.isValid();
}

SGSmoke::CompareResult SGSmoke::compare(const RGBA8Image& actual,
                                        const RGBA8Image& reference,
                                        std::uint8_t channelThreshold) noexcept
{
    CompareResult result;

    if(!actual.isValid() || !reference.isValid() ||
       actual.m_width != reference.m_width || actual.m_height != reference.m_height)
    {
        result.m_sizeMismatch = true;
        result.m_differingPixelsFraction = 1.0;
        return result;
    }

    const std::size_t pixelsCount = static_cast<std::size_t>(actual.m_width) * actual.m_height;
    std::size_t differingPixels = 0;

    for(std::size_t i = 0; i < pixelsCount; ++i)
    {
        bool pixelDiffers = false;

        for(std::size_t channel = 0; channel < 4; ++channel)
        {
            const int difference = std::abs(int(actual.m_pixels[i * 4 + channel]) -
                                            int(reference.m_pixels[i * 4 + channel]));

            result.m_maxChannelDifference = std::max<std::uint8_t>(result.m_maxChannelDifference,
                                                                   static_cast<std::uint8_t>(difference));

            if(difference > channelThreshold) pixelDiffers = true;
        }

        if(pixelDiffers) ++differingPixels;
    }

    result.m_differingPixelsFraction = double(differingPixels) / double(pixelsCount);

    return result;
}

bool SGSmoke::writeDiffPNG(const std::filesystem::path& path,
                           const RGBA8Image& actual,
                           const RGBA8Image& reference) noexcept
{
    if(!actual.isValid() || !reference.isValid() ||
       actual.m_width != reference.m_width || actual.m_height != reference.m_height)
    {
        return false;
    }

    RGBA8Image diff;
    diff.m_width = actual.m_width;
    diff.m_height = actual.m_height;
    diff.m_pixels.resize(actual.m_pixels.size());

    for(std::size_t i = 0; i < actual.m_pixels.size(); i += 4)
    {
        for(std::size_t channel = 0; channel < 3; ++channel)
        {
            diff.m_pixels[i + channel] = static_cast<std::uint8_t>(
                std::abs(int(actual.m_pixels[i + channel]) - int(reference.m_pixels[i + channel])));
        }
        diff.m_pixels[i + 3] = 255;
    }

    return writePNG(path, diff);
}
