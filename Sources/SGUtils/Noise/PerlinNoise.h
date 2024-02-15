//
// Created by Ilya on 15.02.2024.
//

#ifndef SUNGEARENGINE_PERLINNOISE_H
#define SUNGEARENGINE_PERLINNOISE_H

#include <glm/vec2.hpp>
#include "SGUtils/MathUtils.h"
#include "glm/geometric.hpp"
#include "SGUtils/SingleArrayMatrix.h"

namespace SGCore
{
    struct PerlinNoise
    {
        SingleArrayMatrix<float> m_map;

        void setSeed(const size_t& seed = 0) noexcept
        {
            m_seed = seed;

            std::random_device rd;
            std::mt19937 gen(seed);
            std::uniform_int_distribution<std::mt19937::result_type> dist(std::numeric_limits<unsigned char>::min(),
                                                                          std::numeric_limits<unsigned char>::max());

            for(size_t i = 0; i < s_permutationTableSz; ++i)
            {
                m_permutationTable[i] = dist(gen);
            }
        }

        [[nodiscard]] size_t getSeed() const noexcept
        {
            return m_seed;
        }

        float generate(const glm::vec2& vec) noexcept
        {
            // left, top
            glm::ivec2 ltPoint = { std::floor(vec.x), std::floor(vec.y) };

            // local point in quad
            glm::ivec2 localPoint = { vec.x - ltPoint.x, vec.y - ltPoint.y };

            glm::vec2 tlGradient = getPseudoRandomGradientVector(ltPoint);
            glm::vec2 trGradient = getPseudoRandomGradientVector({ ltPoint.x + 1, ltPoint.y });
            glm::vec2 blGradient = getPseudoRandomGradientVector({ ltPoint.x, ltPoint.y + 1 });
            glm::vec2 brGradient = getPseudoRandomGradientVector(ltPoint + 1);

            glm::vec2 toTL = getPseudoRandomGradientVector(localPoint);
            glm::vec2 toTR = getPseudoRandomGradientVector({ localPoint.x - 1, localPoint.y });
            glm::vec2 toBL = getPseudoRandomGradientVector({ localPoint.x, localPoint.y - 1 });
            glm::vec2 toBR = getPseudoRandomGradientVector(localPoint - 1);

            float tx1 = glm::dot(toTL, tlGradient);
            float tx2 = glm::dot(toTR, trGradient);
            float bx1 = glm::dot(toBL, blGradient);
            float bx2 = glm::dot(toBR, brGradient);

            localPoint = { MathUtils::quinticCurve(localPoint.x), MathUtils::quinticCurve(localPoint.y) };

            float tx = MathUtils::lerp(tx1, tx2, (float) localPoint.x);
            float bx = MathUtils::lerp(bx1, bx2, (float) localPoint.x);
            float tb = MathUtils::lerp(tx, bx, (float) localPoint.y);

            return tb;
        }

        float generateMultiOctave(glm::vec2 pos, size_t octaves, const float& persistence = 0.5f) noexcept
        {
            float amplitude = 1; // сила применения шума к общей картине, будет уменьшаться с "мельчанием" шума
            // как сильно уменьшаться - регулирует persistence
            float max = 0; // необходимо для нормализации результата
            float result = 0; // накопитель результата

            while(octaves-- > 0)
            {
                max += amplitude;
                result += generate(pos) * amplitude;
                amplitude *= persistence;
                pos *= 2; // удваиваем частоту шума (делаем его более мелким) с каждой октавой
            }

            return result / max;
        }

        void generateMap(const glm::ivec2& mapSize) noexcept
        {
            m_map = SingleArrayMatrix<float>(mapSize.x, mapSize.y);
            m_currentMapSize = mapSize;

            for(size_t r = 0; r < mapSize.x; ++r)
            {
                for(size_t c = 0; c < mapSize.y; ++c)
                {
                    m_map.get(r, c) = generate({ r, c });
                }
            }
        }

        void generateMapMultiOctave(const glm::ivec2& mapSize, size_t octaves, const float& persistence = 0.5f) noexcept
        {
            m_map = SingleArrayMatrix<float>(mapSize.x, mapSize.y);
            m_currentMapSize = mapSize;

            for(size_t r = 0; r < mapSize.x; ++r)
            {
                for(size_t c = 0; c < mapSize.y; ++c)
                {
                    m_map.get(r, c) = generateMultiOctave({ r, c }, octaves, persistence);
                }
            }
        }

        glm::vec2 getPseudoRandomGradientVector(const glm::ivec2& pos) noexcept
        {
            auto v = (size_t)(((pos.x * 1836311903) ^ (pos.y * 2971215073) + 4807526976) & 1023);
            v = m_permutationTable[v] & 3;

            switch (v)
            {
                case 0:  return { 1, 0 };
                case 1:  return { -1, 0 };
                case 2:  return { 0, 1 };
                default: return { 0, -1 };
            }
        }

        [[nodiscard]] glm::ivec2 getCurrentMapSize() const noexcept
        {
            return m_currentMapSize;
        }

    private:
        static constexpr const inline size_t s_permutationTableSz = 1024;
        
        glm::ivec2 m_currentMapSize { };

        std::uint8_t m_permutationTable[s_permutationTableSz] { };

        size_t m_seed = 0;
    };
}

#endif //SUNGEARENGINE_PERLINNOISE_H
