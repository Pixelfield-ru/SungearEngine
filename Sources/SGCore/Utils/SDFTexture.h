//
// Created by stuka on 19.03.2025.
//

#ifndef SGCORESDFTEXTURE_H
#define SGCORESDFTEXTURE_H

#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct SDFTexture
    {
    private:
        static constexpr glm::ivec2 pointInside = { 0, 0 };
        static constexpr glm::ivec2 pointOutside = { 9999, 9999 };

    public:
        Ref<ITexture2D> m_texture;

        void generate(const std::vector<std::uint8_t>& originalImage, std::uint32_t imageWidth, std::uint32_t imageHeight, float maxDist) noexcept
        {
            m_texture = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());

            std::vector<std::uint8_t> sdfBuffer;
            sdfBuffer.resize(imageWidth * imageHeight);

            std::vector<glm::ivec2> grid0;
            grid0.resize(imageWidth * imageHeight);

            std::vector<glm::ivec2> grid1;
            grid1.resize(imageWidth * imageHeight);

            for(int y = 0; y < imageHeight; ++y)
            {
                for (int x = 0; x < imageWidth; ++x)
                {
                    const std::uint8_t pixelColor = originalImage[y * imageWidth + x];

                    // Points inside get marked with a dx/dy of zero.
                    // Points outside get marked with an infinitely large distance.
                    if (pixelColor < 128)
                    {
                        grid0[y * imageWidth + x] = pointInside;
                        grid1[y * imageWidth + x] = pointOutside;
                    }
                    else
                    {
                        grid1[y * imageWidth + x] = pointInside;
                        grid0[y * imageWidth + x] = pointOutside;
                    }
                }
            }

            calculateGridSDF(grid0, imageWidth, imageHeight);
            calculateGridSDF(grid1, imageWidth, imageHeight);

            // final pass
            for(int y = 0; y < imageHeight; ++y)
            {
                for(int x = 0; x < imageWidth; ++x)
                {
                    // Calculate the actual distance from the dx/dy
                    const int dist1 = (int) (std::sqrt((double) lengthSquared(grid0[y * imageWidth + x])));
                    const int dist2 = (int) (std::sqrt((double) lengthSquared(grid1[y * imageWidth + x])));
                    const int dist = dist1 - dist2;

                    // Clamp and scale it, just for display purposes.
                    int c = dist * 3 + 128;
                    if (c < 0) c = 0;
                    if (c > 255) c = 255;

                    sdfBuffer[y * imageWidth + x] = c;
                }
            }

            m_texture->create(sdfBuffer.data(),
                    imageWidth,
                    imageHeight,
                    1,
                    SGGColorInternalFormat::SGG_R8,
                    SGGColorFormat::SGG_R);
        }

    private:
        static std::int32_t lengthSquared(const glm::ivec2& v) noexcept
        {
            return v.x * v.x + v.y * v.y;
        }

        static void calculateGridSDF(std::vector<glm::ivec2>& grid, std::uint32_t gridWidth, std::uint32_t gridHeight) noexcept;

        static float getDistanceToNearestBorder(const std::vector<std::uint8_t>& originalImage, std::uint32_t pixelPosX, std::uint32_t pixelPosY, std::uint32_t imageWidth, std::uint32_t imageHeight, float maxDist) noexcept
        {
            float minDist = maxDist;

            for(int i = 0; i < imageHeight; ++i)
            {
                for(int j = 0; j < imageWidth; ++j)
                {
                    // 0 - background, 255 - contour
                    if(originalImage[i * imageWidth + j] > 128)
                    {
                        const float dist = std::sqrt((pixelPosX - j) * (pixelPosX - j) + (pixelPosY - i) * (pixelPosY - i));
                        minDist = std::min(minDist, dist);
                    }
                }
            }

            return minDist;
        }
    };
}

#endif // SGCORESDFTEXTURE_H
