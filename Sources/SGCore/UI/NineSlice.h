//
// Created by stuka on 09.02.2025.
//

#ifndef NINESLICE_H
#define NINESLICE_H

#include "UIElementMesh/UIVertex.h"
#include "TransformTree/UIElementCache.h"

namespace SGCore::UI
{
    struct NineSlice
    {
        template<typename IndexT>
        requires(std::is_integral_v<IndexT>)
        static void generate9SlicedQuad(const std::array<glm::vec2, 4> bordersRadius,
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

            glm::vec2 biggestBordersRadius { };

            biggestBordersRadius.x = bordersRadius[0].x;
            biggestBordersRadius.x = std::max(biggestBordersRadius.x, bordersRadius[1].x);
            biggestBordersRadius.x = std::max(biggestBordersRadius.x, bordersRadius[2].x);
            biggestBordersRadius.x = std::max(biggestBordersRadius.x, bordersRadius[3].x);

            biggestBordersRadius.y = bordersRadius[0].y;
            biggestBordersRadius.y = std::max(biggestBordersRadius.y, bordersRadius[1].y);
            biggestBordersRadius.y = std::max(biggestBordersRadius.y, bordersRadius[2].y);
            biggestBordersRadius.y = std::max(biggestBordersRadius.y, bordersRadius[3].y);

            const glm::vec2 CSliceSize = { 2, 2 };

            const glm::vec2 sideSize = {
                biggestBordersRadius.x + borderWidth,
                biggestBordersRadius.y + borderWidth
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
            
            IndexT currentMaxIndex = 0;

            // ========================================================= generating 5 slice

            outputQuadVertices.push_back({
                .m_position = { CSliceLBPos, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 5
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceLTPos, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 5
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 5
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRBPos, 0.0 },
                .m_uv = { },
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
                .m_uv = { },
                .m_sliceIndex = 2
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceLTPos.x, 0, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 2
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos.x, 0, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 2
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos.x, -sideSize.y, 0.0 },
                .m_uv = { },
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
                .m_uv = { },
                .m_sliceIndex = 8
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceLTPos.x, sideSize.y, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 8
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos.x, sideSize.y, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 8
            });

            outputQuadVertices.push_back({
                .m_position = { CSliceRTPos.x, 0, 0.0 },
                .m_uv = { },
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
                .m_uv = { },
                .m_sliceIndex = 4
            });

            outputQuadVertices.push_back({
                .m_position = { -sideSize.x, CSliceLTPos.y, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 4
            });

            outputQuadVertices.push_back({
                .m_position = { 0, CSliceLTPos.y, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 4
            });

            outputQuadVertices.push_back({
                .m_position = { 0, CSliceLBPos.y, 0.0 },
                .m_uv = { },
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
                .m_uv = { },
                .m_sliceIndex = 6
            });

            outputQuadVertices.push_back({
                .m_position = { 0, CSliceRTPos.y, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 6
            });

            outputQuadVertices.push_back({
                .m_position = { sideSize.x, CSliceRTPos.y, 0.0 },
                .m_uv = { },
                .m_sliceIndex = 6
            });

            outputQuadVertices.push_back({
                .m_position = { sideSize.x, CSliceRBPos.y, 0.0 },
                .m_uv = { },
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

            // ========================================================= generating 1 (left-top) slice

            generateCorner(bordersRadius[0],
                           biggestBordersRadius,
                           cornerSegmentsCount,
                           90,
                           180,
                           1,
                           currentMaxIndex,
                           outputQuadVertices,
                           outputIndices);

            // ========================================================= generating 7 (left-bottom) slice

            generateCorner(bordersRadius[3],
                           biggestBordersRadius,
                           cornerSegmentsCount,
                           0,
                           90,
                           7,
                           currentMaxIndex,
                           outputQuadVertices,
                           outputIndices);

            // ========================================================= generating 3 (right-top) slice

            generateCorner(bordersRadius[1],
                           biggestBordersRadius,
                           cornerSegmentsCount,
                           180,
                           270,
                           3,
                           currentMaxIndex,
                           outputQuadVertices,
                           outputIndices);

            // ========================================================= generating 9 (right-bottom) slice

            generateCorner(bordersRadius[2],
                           biggestBordersRadius,
                           cornerSegmentsCount,
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
        static void generateCorner(const glm::vec2& borderRadius,
                                   const glm::vec2& biggestBorderRadius,
                                   const size_t& segmentsCount,
                                   const float& startAngle,
                                   const float& endAngle,
                                   const std::int32_t& sliceIndex,
                                   IndexT& outputMaxIndex,
                                   std::vector<UIVertex>& outputQuadVertices,
                                   std::vector<IndexT>& outputIndices) noexcept
        {
            // quad indices for this slice index
            // to avoid faces culling
            std::array<IndexT, 6> quadIndices;
            if(sliceIndex == 1 || sliceIndex == 9)
            {
                quadIndices[0] = 0;
                quadIndices[1] = 1;
                quadIndices[2] = 2;
                quadIndices[3] = 0;
                quadIndices[4] = 2;
                quadIndices[5] = 3;
            }
            else if(sliceIndex == 3 || sliceIndex == 7)
            {
                quadIndices[0] = 0;
                quadIndices[1] = 2;
                quadIndices[2] = 1;
                quadIndices[3] = 0;
                quadIndices[4] = 3;
                quadIndices[5] = 2;
            }

            if(borderRadius.x == 0.0f && borderRadius.y == 0.0f)
            {
                std::array<glm::vec3, 4> vertices {
                    glm::vec3 { 0.0, 0.0, 0.0 },
                    glm::vec3 { biggestBorderRadius.x, 0.0f, 0.0 },
                    glm::vec3 { biggestBorderRadius.x, -biggestBorderRadius.y, 0.0 },
                    glm::vec3 { 0.0f, -biggestBorderRadius.y, 0.0f }
                };

                generateCornerQuad(vertices, quadIndices, sliceIndex, outputMaxIndex, outputQuadVertices, outputIndices);

                return;
            }

            const glm::vec2 toBiggestRadiusDif {
                std::max(biggestBorderRadius.x - borderRadius.x, 0.0f),
                std::max(biggestBorderRadius.y - borderRadius.y, 0.0f)
            };

            glm::vec3 toBiggestRadiusOffset = { toBiggestRadiusDif, 0.0f };
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

            float originalRotationVectorY = borderRadius.x;
            if(sliceIndex == 7 || sliceIndex == 3)
            {
                originalRotationVectorY = borderRadius.y;
            }

            glm::vec3 rotatedVertex = glm::vec3(0.0f, originalRotationVectorY, 0.0f);
            glm::vec3 lastRotatedVertex = glm::angleAxis(glm::radians(currentRoundingAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * rotatedVertex;

            const float angleStep = (endAngle - startAngle) / (float) segmentsCount;

            while(currentRoundingAngle < endAngle)
            {
                currentRoundingAngle += angleStep;

                rotatedVertex = glm::vec3(0.0f, originalRotationVectorY, 0.0f);

                currentRoundingAngle = std::min(currentRoundingAngle, endAngle);

                float angleRadians = glm::radians(currentRoundingAngle);

                // angle dif between X axis and new point on ellipsis
                float theta = atan2(rotatedVertex.y / borderRadius.y, rotatedVertex.x / borderRadius.x);

                float newTheta = theta + angleRadians;

                // rotating vector
                float newX = borderRadius.x * cos(newTheta);
                float newY = borderRadius.y * sin(newTheta);

                rotatedVertex.x = newX;
                rotatedVertex.y = newY;

                outputQuadVertices.push_back({
                    .m_position = toBiggestRadiusOffset + lastRotatedVertex,
                    .m_uv = { },
                    .m_sliceIndex = sliceIndex
                });

                outputQuadVertices.push_back({
                    .m_position = toBiggestRadiusOffset,
                    .m_uv = { },
                    .m_sliceIndex = sliceIndex
                });

                outputQuadVertices.push_back({
                    .m_position = toBiggestRadiusOffset + rotatedVertex,
                    .m_uv = { },
                    .m_sliceIndex = sliceIndex
                });

                outputIndices.push_back(outputMaxIndex + 2);
                outputIndices.push_back(outputMaxIndex + 1);
                outputIndices.push_back(outputMaxIndex + 0);

                outputMaxIndex += 3;

                lastRotatedVertex = rotatedVertex;
            }

            // =======================================================================================
            // creating two parallelograms to fill gaps if current borderRadius is lower than biggestBorderRadius

            const glm::vec3 toBiggestRadiusOffsetRaw = { toBiggestRadiusDif, 0.0f };

            if(borderRadius.x < biggestBorderRadius.x)
            {
                std::array<glm::vec3, 4> vertices {
                    glm::vec3 { 0.0, 0.0, 0.0 },
                    glm::vec3 { toBiggestRadiusOffsetRaw.x, -toBiggestRadiusOffsetRaw.y, 0.0f },
                    glm::vec3 { toBiggestRadiusOffsetRaw.x, -biggestBorderRadius.y, 0.0 },
                    glm::vec3 { 0.0f, -biggestBorderRadius.y, 0.0 }
                };

                generateCornerQuad(vertices, quadIndices, sliceIndex, outputMaxIndex, outputQuadVertices, outputIndices);
            }

            if(borderRadius.y < biggestBorderRadius.y)
            {
                std::array<glm::vec3, 4> vertices {
                    glm::vec3 { 0.0, 0.0, 0.0 },
                    glm::vec3 { biggestBorderRadius.x, 0.0f, 0.0 },
                    glm::vec3 { biggestBorderRadius.x, -toBiggestRadiusOffsetRaw.y, 0.0 },
                    glm::vec3 { toBiggestRadiusOffsetRaw.x, -toBiggestRadiusOffsetRaw.y, 0.0f }
                };

                generateCornerQuad(vertices, quadIndices, sliceIndex, outputMaxIndex, outputQuadVertices, outputIndices);
            }

            // =======================================================================================
        }

        template<typename IndexT>
        requires(std::is_integral_v<IndexT>)
        static void generateCornerQuad(std::array<glm::vec3, 4>& verticesPos,
                                       const std::array<IndexT, 6>& quadIndices,
                                       const std::int32_t& sliceIndex,
                                       IndexT& outputMaxIndex,
                                       std::vector<UIVertex>& outputQuadVertices,
                                       std::vector<IndexT>& outputIndices) noexcept
        {
            if(sliceIndex == 1)
            {
                for(auto& v : verticesPos) v.x *= -1.0f;
            }
            else if(sliceIndex == 7)
            {
                for(auto& v : verticesPos) v *= -1.0f;
            }
            else if(sliceIndex == 9)
            {
                for(auto& v : verticesPos) v.y *= -1.0f;
            }

            outputQuadVertices.push_back({
                .m_position = verticesPos[0],
                .m_uv = { },
                .m_sliceIndex = sliceIndex
            });

            outputQuadVertices.push_back({
                .m_position = verticesPos[1],
                .m_uv = { },
                .m_sliceIndex = sliceIndex
            });

            outputQuadVertices.push_back({
                .m_position = verticesPos[2],
                .m_uv = { },
                .m_sliceIndex = sliceIndex
            });

            outputQuadVertices.push_back({
                .m_position = verticesPos[3],
                .m_uv = { },
                .m_sliceIndex = sliceIndex
            });

            for(size_t i = 0; i < quadIndices.size(); ++i)
            {
                outputIndices.push_back(outputMaxIndex + quadIndices[i]);
            }

            outputMaxIndex += 4;
        }
    };
}

#endif //NINESLICE_H
