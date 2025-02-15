//
// Created by stuka on 09.02.2025.
//

#ifndef NINESLICE_H
#define NINESLICE_H

#include "UIElementMesh/UIVertex.h"

namespace SGCore::UI
{
    struct NineSlice
    {
        template<typename IndexT>
        requires(std::is_integral_v<IndexT>)
        static void generate9SlicedQuad(const float& borderRadius,
                                        const float& borderWidth,
                                        std::vector<UIVertex>& outputQuadVertices,
                                        std::vector<IndexT>& outputIndices) noexcept
        {
            // WARNING!: CENTERAL SLICE MUST HAS VERTICES WITH POSITIONS -1 AND 1 AND MUST BE QUAD

            // LT - LEFT TOP
            // LB - LEFT BOTTOM
            // RT - RIGHT TOP
            // RB - RIGHT BOTTOM
            // C - CENTER

            const glm::vec2 CSliceSize = { 2, 2 };

            const glm::vec2 sideSize = {
                borderRadius + borderWidth,
                borderRadius + borderWidth
            };

            //  1 ---- 2 ---- 3 ---- 4
            //  |  1s  |  2s  |  3s  |
            //  |      |      |      |
            //  5 ---- 6 ---- 7 ---- 8
            //  |  4s  |  5s  |  6s  |
            //  |      |      |      |
            //  9 ---- 10---- 11---- 12
            //  |  7s  |  8s  |  9s  |
            //  |      |      |      |
            //  13---- 14---- 15---- 16
            // 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 - points
            // 1s, 2s, 3s, 4s, 5s, 6s, 7s, 8s, 9s - slices

            // 6 point
            const glm::vec2 CSliceLTPos = { -CSliceSize.x / 2.0f, CSliceSize.y / 2.0f };
            // 7 point
            const glm::vec2 CSliceRTPos = { CSliceSize.x / 2.0f, CSliceSize.y / 2.0f };
            // 10 point
            const glm::vec2 CSliceLBPos = { -CSliceSize.x / 2.0f, -CSliceSize.y / 2.0f };
            // 11 point
            const glm::vec2 CSliceRBPos = { CSliceSize.x / 2.0f, -CSliceSize.y / 2.0f };

            size_t currentMaxIndex = 0;

            // ========================================================= generating 5 slice

            outputQuadVertices.push_back({
                .m_position = { CSliceLBPos, 0.0 },
                .m_sliceIndex = 5
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceLTPos, 0.0 },
                .m_sliceIndex = 5
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos, 0.0 },
                .m_sliceIndex = 5
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRBPos, 0.0 },
                .m_sliceIndex = 5
            });

            outputIndices.push_back(0);
            outputIndices.push_back(2);
            outputIndices.push_back(1);

            outputIndices.push_back(0);
            outputIndices.push_back(3);
            outputIndices.push_back(2);

            currentMaxIndex = 4;

            // ========================================================= generating 2 slice

            outputQuadVertices.push_back({
                .m_position = { CSliceLTPos.x, -sideSize.y, 0.0 },
                .m_sliceIndex = 2
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceLTPos.x, 0, 0.0 },
                .m_sliceIndex = 2
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos.x, 0, 0.0 },
                .m_sliceIndex = 2
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos.x, -sideSize.y, 0.0 },
                .m_sliceIndex = 2
            });

            outputIndices.push_back(currentMaxIndex + 0);
            outputIndices.push_back(currentMaxIndex + 2);
            outputIndices.push_back(currentMaxIndex + 1);

            outputIndices.push_back(currentMaxIndex + 0);
            outputIndices.push_back(currentMaxIndex + 3);
            outputIndices.push_back(currentMaxIndex + 2);

            // 4 vertices
            currentMaxIndex += 4;

            // ========================================================= generating 8 slice

            outputQuadVertices.push_back({
                .m_position = { CSliceLTPos.x, 0, 0.0 },
                .m_sliceIndex = 8
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceLTPos.x, sideSize.y, 0.0 },
                .m_sliceIndex = 8
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos.x, sideSize.y, 0.0 },
                .m_sliceIndex = 8
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos.x, 0, 0.0 },
                .m_sliceIndex = 8
            });

            outputIndices.push_back(currentMaxIndex + 0);
            outputIndices.push_back(currentMaxIndex + 2);
            outputIndices.push_back(currentMaxIndex + 1);

            outputIndices.push_back(currentMaxIndex + 0);
            outputIndices.push_back(currentMaxIndex + 3);
            outputIndices.push_back(currentMaxIndex + 2);

            // 4 vertices
            currentMaxIndex += 4;

            // ========================================================= generating 4 slice

            outputQuadVertices.push_back({
                .m_position = { -sideSize.x, CSliceLBPos.y, 0.0 },
                .m_sliceIndex = 4
            });

            outputQuadVertices.push_back({
                .m_position = { -sideSize.x, CSliceLTPos.y, 0.0 },
                .m_sliceIndex = 4
            });

            outputQuadVertices.push_back({
                .m_position = { 0, CSliceLTPos.y, 0.0 },
                .m_sliceIndex = 4
            });

            outputQuadVertices.push_back({
                .m_position = { 0, CSliceLBPos.y, 0.0 },
                .m_sliceIndex = 4
            });

            outputIndices.push_back(currentMaxIndex + 0);
            outputIndices.push_back(currentMaxIndex + 2);
            outputIndices.push_back(currentMaxIndex + 1);

            outputIndices.push_back(currentMaxIndex + 0);
            outputIndices.push_back(currentMaxIndex + 3);
            outputIndices.push_back(currentMaxIndex + 2);

            // 4 vertices
            currentMaxIndex += 4;

            // ========================================================= generating 6 slice

            outputQuadVertices.push_back({
                .m_position = { 0, CSliceRBPos.y, 0.0 },
                .m_sliceIndex = 6
            });

            outputQuadVertices.push_back({
                .m_position = { 0, CSliceRTPos.y, 0.0 },
                .m_sliceIndex = 6
            });

            outputQuadVertices.push_back({
                .m_position = { sideSize.x, CSliceRTPos.y, 0.0 },
                .m_sliceIndex = 6
            });

            outputQuadVertices.push_back({
                .m_position = { sideSize.x, CSliceRBPos.y, 0.0 },
                .m_sliceIndex = 6
            });

            outputIndices.push_back(currentMaxIndex + 0);
            outputIndices.push_back(currentMaxIndex + 2);
            outputIndices.push_back(currentMaxIndex + 1);

            outputIndices.push_back(currentMaxIndex + 0);
            outputIndices.push_back(currentMaxIndex + 3);
            outputIndices.push_back(currentMaxIndex + 2);
        }
    };
}

#endif //NINESLICE_H
