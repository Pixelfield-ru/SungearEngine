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
        static void generate9SlicedQuad(const glm::vec4& borderRadius,
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

            float biggestBorderRadius = borderRadius.x;
            if(biggestBorderRadius < borderRadius.y) biggestBorderRadius = borderRadius.y;
            if(biggestBorderRadius < borderRadius.z) biggestBorderRadius = borderRadius.z;
            if(biggestBorderRadius < borderRadius.w) biggestBorderRadius = borderRadius.w;

            const glm::vec2 CSliceSize = { 2, 2 };

            const glm::vec2 sideSize = {
                biggestBorderRadius + borderWidth,
                biggestBorderRadius + borderWidth
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

            if(sideSize.x == 0.0f && sideSize.y == 0.0f) return;

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

            // 4 vertices
            currentMaxIndex += 4;

            const size_t cornerSegmentsCount = 10;
            const float corner1AngleStep = borderRadius.x / cornerSegmentsCount;
            const float corner2AngleStep = borderRadius.y / cornerSegmentsCount;
            const float corner3AngleStep = borderRadius.z / cornerSegmentsCount;
            const float corner4AngleStep = borderRadius.w / cornerSegmentsCount;

            // ========================================================= generating 1 slice

            generateCorner(borderRadius.x,
                           biggestBorderRadius,
                           corner1AngleStep,
                           90,
                           180,
                           1,
                           currentMaxIndex,
                           outputQuadVertices,
                           outputIndices);

            // ========================================================= generating 3 slice

            generateCorner(borderRadius.y,
                           biggestBorderRadius,
                           corner2AngleStep,
                           180,
                           270,
                           3,
                           currentMaxIndex,
                           outputQuadVertices,
                           outputIndices);

            // ========================================================= generating 7 slice

            generateCorner(borderRadius.z,
                           biggestBorderRadius,
                           corner3AngleStep,
                           360,
                           450,
                           7,
                           currentMaxIndex,
                           outputQuadVertices,
                           outputIndices);

            // ========================================================= generating 9 slice

            generateCorner(borderRadius.w,
                           biggestBorderRadius,
                           corner4AngleStep,
                           270,
                           360,
                           9,
                           currentMaxIndex,
                           outputQuadVertices,
                           outputIndices);
        }

    private:
        template<typename IndexT>
        requires(std::is_integral_v<IndexT>)
        static void generateCorner(const float& borderRadius,
                                   const float& biggestBorderRadius,
                                   const float& angleStep,
                                   const float& startAngle,
                                   const float& endAngle,
                                   const std::int32_t& sliceIndex,
                                   size_t& outputMaxIndex,
                                   std::vector<UIVertex>& outputQuadVertices,
                                   std::vector<IndexT>& outputIndices) noexcept
        {
            if(borderRadius == 0.0f)
            {
                glm::vec3 vertex0 = { 0.0, 0.0f, 0.0f };
                glm::vec3 vertex1 = { 0.0, biggestBorderRadius, 0.0f };
                glm::vec3 vertex2 = { biggestBorderRadius, biggestBorderRadius, 0.0f };
                glm::vec3 vertex3 = { biggestBorderRadius, 0.0f, 0.0f };

                vertex0 = glm::angleAxis(glm::radians(endAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * vertex0;
                vertex1 = glm::angleAxis(glm::radians(endAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * vertex1;
                vertex2 = glm::angleAxis(glm::radians(endAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * vertex2;
                vertex3 = glm::angleAxis(glm::radians(endAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * vertex3;

                outputQuadVertices.push_back({
                    .m_position = vertex0,
                    .m_sliceIndex = sliceIndex
                });

                outputQuadVertices.push_back({
                    .m_position = vertex1,
                    .m_sliceIndex = sliceIndex
                });

                outputQuadVertices.push_back({
                    .m_position = vertex2,
                    .m_sliceIndex = sliceIndex
                });

                outputQuadVertices.push_back({
                    .m_position = vertex3,
                    .m_sliceIndex = sliceIndex
                });

                outputIndices.push_back(outputMaxIndex + 0);
                outputIndices.push_back(outputMaxIndex + 2);
                outputIndices.push_back(outputMaxIndex + 1);

                outputIndices.push_back(outputMaxIndex + 0);
                outputIndices.push_back(outputMaxIndex + 3);
                outputIndices.push_back(outputMaxIndex + 2);

                outputMaxIndex += 4;

                return;
            }

            const float toBiggestRadiusDif = std::max(biggestBorderRadius - borderRadius, 0.0f);
            glm::vec3 toBiggestRadiusOffset = { toBiggestRadiusDif, toBiggestRadiusDif, 0.0f };
            if(sliceIndex == 1 || sliceIndex == 3)
            {
                if(sliceIndex == 1)
                {
                    toBiggestRadiusOffset.x *= -1.0f;
                }

                toBiggestRadiusOffset.y *= -1.0f;
            }
            else if(sliceIndex == 7)
            {
                toBiggestRadiusOffset.x *= -1.0f;
            }

            float currentRoundingAngle = startAngle;

            glm::vec3 rotatedVertex = glm::vec3(0.0f, borderRadius, 0.0f);
            glm::vec3 lastRotatedVertex = glm::angleAxis(glm::radians(currentRoundingAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * rotatedVertex;

            while(currentRoundingAngle < endAngle)
            {
                currentRoundingAngle += angleStep;

                rotatedVertex = glm::vec3(0.0f, borderRadius, 0.0f);

                currentRoundingAngle = std::min(currentRoundingAngle, endAngle);

                glm::quat rotationQuat = glm::angleAxis(glm::radians(currentRoundingAngle), glm::vec3(0.0f, 0.0f, 1.0f));
                rotatedVertex = rotationQuat * rotatedVertex;

                outputQuadVertices.push_back({
                    .m_position = toBiggestRadiusOffset + lastRotatedVertex,
                    .m_sliceIndex = sliceIndex
                });

                outputQuadVertices.push_back({
                    .m_position = toBiggestRadiusOffset,
                    .m_sliceIndex = sliceIndex
                });

                outputQuadVertices.push_back({
                    .m_position = toBiggestRadiusOffset + rotatedVertex,
                    .m_sliceIndex = sliceIndex
                });

                outputIndices.push_back(outputMaxIndex + 2);
                outputIndices.push_back(outputMaxIndex + 1);
                outputIndices.push_back(outputMaxIndex + 0);

                outputMaxIndex += 3;

                lastRotatedVertex = rotatedVertex;
            }
        }
    };
}

#endif //NINESLICE_H
