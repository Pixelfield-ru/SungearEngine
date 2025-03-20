//
// Created by stuka on 19.03.2025.
//

#include "SDFTexture.h"

void SGCore::SDFTexture::calculateGridSDF(std::vector<glm::ivec2>& grid, std::uint32_t gridWidth, std::uint32_t gridHeight) noexcept
{
    const auto compareAndSetFunc = [&gridWidth, &gridHeight, &grid](glm::ivec2& point, int x, int y, int offsetX, int offsetY) noexcept {
        const int finalX = x + offsetX;
        const int finalY = y + offsetY;

        glm::ivec2 otherPoint = pointOutside;

        if(finalX >= 0 && finalY >= 0 && finalX < gridWidth && finalY < gridHeight)
        {
            otherPoint = grid[finalY * gridWidth + finalX];
        }

        otherPoint.x += offsetX;
        otherPoint.y += offsetY;

        if(lengthSquared(otherPoint) < lengthSquared(point))
        {
            point = otherPoint;
        }
    };

    // pass 0
    for(int y = 0; y < gridHeight; ++y)
    {
        for(int x = 0; x < gridWidth; ++x)
        {
            auto& point = grid[y * gridWidth + x];

            compareAndSetFunc(point, x, y, -1, 0);
            compareAndSetFunc(point, x, y, 0, -1);
            compareAndSetFunc(point, x, y, -1, -1);
            compareAndSetFunc(point, x, y, 1, -1);
        }

        for(int x = gridWidth - 1; x >= 0; --x)
        {
            auto& point = grid[y * gridWidth + x];

            compareAndSetFunc(point, x, y, 1, 0);
        }
    }

    // pass 1
    for(int y = gridHeight - 1; y >= 0; --y)
    {
        for(int x = gridWidth - 1; x >= 0; --x)
        {
            auto& point = grid[y * gridWidth + x];

            compareAndSetFunc(point, x, y, 1, 0);
            compareAndSetFunc(point, x, y, 0, 1);
            compareAndSetFunc(point, x, y, -1, 1);
            compareAndSetFunc(point, x, y, 1, 1);
        }

        for(int x = 0; x < gridWidth; ++x)
        {
            auto& point = grid[y * gridWidth + x];

            compareAndSetFunc(point, x, y, -1, 0);
        }
    }
}
