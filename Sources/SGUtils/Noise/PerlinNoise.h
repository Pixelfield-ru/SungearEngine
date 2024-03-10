//
// Created by Ilya on 15.02.2024.
//

#ifndef SUNGEARENGINE_PERLINNOISE_H
#define SUNGEARENGINE_PERLINNOISE_H

#include <glm/vec2.hpp>
#include "SGUtils/Math/MathUtils.h"
#include "glm/geometric.hpp"
#include "SGUtils/SingleArrayMatrix.h"
#include <iostream>

namespace SGCore
{
    struct PerlinNoise
    {
        SingleArrayMatrix<float> m_map;
        SingleArrayMatrix<float> m_seeds;
        
        void generate(const glm::ivec2& mapSize, int octavesCount, float bias)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution dist(std::numeric_limits<float>::min(),
                                                                          std::numeric_limits<float>::max());
            
            m_map = SingleArrayMatrix<float>(mapSize.x, mapSize.y);
            m_seeds = SingleArrayMatrix<float>(mapSize.x, mapSize.y);
            
            for(int i = 0; i < mapSize.x * mapSize.y; ++i)
            {
                m_seeds.data()[i] = dist(gen) / std::numeric_limits<float>::max();
            }
            
            // Used 1D Perlin Noise
            for (int x = 0; x < mapSize.x; x++)
            {
                for(int y = 0; y < mapSize.y; y++)
                {
                    float fNoise = 0.0f;
                    float fScaleAcc = 0.0f;
                    float fScale = 1.0f;
                    
                    for(int o = 0; o < octavesCount; o++)
                    {
                        int nPitch = mapSize.x >> o;
                        if(nPitch == 0) continue;
                        int nSampleX1 = (x / nPitch) * nPitch;
                        int nSampleY1 = (y / nPitch) * nPitch;
                        
                        int nSampleX2 = (nSampleX1 + nPitch) % mapSize.x;
                        int nSampleY2 = (nSampleY1 + nPitch) % mapSize.x;
                        
                        float fBlendX = (float) (x - nSampleX1) / (float) nPitch;
                        float fBlendY = (float) (y - nSampleY1) / (float) nPitch;
                        
                        float fSampleT = (1.0f - fBlendX) * m_seeds.data()[nSampleY1 * mapSize.x + nSampleX1] +
                                         fBlendX * m_seeds.data()[nSampleY1 * mapSize.x + nSampleX2];
                        float fSampleB = (1.0f - fBlendX) * m_seeds.data()[nSampleY2 * mapSize.x + nSampleX1] +
                                         fBlendX * m_seeds.data()[nSampleY2 * mapSize.x + nSampleX2];
                        
                        fScaleAcc += fScale;
                        fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
                        fScale = fScale / bias;
                    }
                    
                    // Scale to seed range
                    m_map.data()[y * mapSize.x + x] = fNoise / fScaleAcc;
                }
            }
        }

        [[nodiscard]] glm::ivec2 getCurrentMapSize() const noexcept
        {
            return { m_map.getRowsCount(), m_map.getColumnsCount() };
        }
        
        size_t m_seed = 0;
    };
}

#endif //SUNGEARENGINE_PERLINNOISE_H
