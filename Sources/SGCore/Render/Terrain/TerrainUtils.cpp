//
// Created by stuka on 06.12.2025.
//

#include "TerrainUtils.h"

#include "SGCore/ImportedScenesArch/IMeshData.h"

void SGCore::TerrainUtils::calculateVerticesUsingDisplacementMap(const Terrain::reg_t& terrain,
                                                                 const ITexture2D* map,
                                                                 float stepSize,
                                                                 std::vector<Vertex>& outVertices,
                                                                 std::vector<std::uint32_t>& outIndices) noexcept
{
    const glm::i64vec2 terrainSize {
        terrain.getSize().x * terrain.getPatchSize(),
        terrain.getSize().y * terrain.getPatchSize()
    };

    const size_t verticesCount = (terrainSize.x * terrainSize.y) / stepSize * 4;
    outVertices.reserve(verticesCount);
    outIndices.reserve(verticesCount);

    const glm::vec2 pixelSize {
        (float) terrainSize.x / (float) map->getWidth(),
        (float) terrainSize.y / (float) map->getHeight()
    };

    const glm::vec2 step { stepSize / pixelSize.x, stepSize / pixelSize.y };

    glm::vec2 samplePos { 0, 0 };

    size_t currentIdx = 0;

    for(float z = 0; z < terrainSize.x; z += stepSize)
    {
        for(float x = 0; x < terrainSize.y; x += stepSize)
        {
            const float v0Height = map->sampleRAM<float>({
                samplePos.x,
                samplePos.y
            }).r * terrain.m_heightScale;

            const float v1Height = map->sampleRAM<float>({
                samplePos.x,
                std::min(std::int32_t(samplePos.y + step.y), map->getHeight() - 1)
            }).r * terrain.m_heightScale;

            const float v2Height = map->sampleRAM<float>({
                std::min(std::int32_t(samplePos.x + step.x), map->getWidth() - 1),
                samplePos.y
            }).r * terrain.m_heightScale;

            const float v3Height = map->sampleRAM<float>({
                std::min(std::int32_t(samplePos.x + step.x), map->getWidth() - 1),
                std::min(std::int32_t(samplePos.y + step.y), map->getHeight() - 1)
            }).r * terrain.m_heightScale;

            glm::vec3 v0(x, v0Height, z);
            glm::vec3 v1(x, v1Height, z + stepSize);
            glm::vec3 v2(x + stepSize, v2Height, z);
            glm::vec3 v3(x + stepSize, v3Height, z + stepSize);

            /*glm::vec3 v0(x, v0Height, z);
            glm::vec3 v1((x + stepSize), v2Height, z);
            glm::vec3 v2((x + stepSize), v3Height, (z + stepSize));
            glm::vec3 v3(x, v1Height, (z + stepSize));*/

            v0.x = (v0.x - terrain.getPatchSize() / 2.0f);
            v0.z = (v0.z - terrain.getPatchSize() / 2.0f);

            v1.x = (v1.x - terrain.getPatchSize() / 2.0f);
            v1.z = (v1.z - terrain.getPatchSize() / 2.0f);

            v2.x = (v2.x - terrain.getPatchSize() / 2.0f);
            v2.z = (v2.z - terrain.getPatchSize() / 2.0f);

            v3.x = (v3.x - terrain.getPatchSize() / 2.0f);
            v3.z = (v3.z - terrain.getPatchSize() / 2.0f);

            // todo: make uv
            outVertices.push_back({
                .m_position = v0
            });
            outVertices.push_back({
                .m_position = v1
            });
            outVertices.push_back({
                .m_position = v2
            });
            outVertices.push_back({
                .m_position = v3
            });

            outIndices.push_back(currentIdx + 0);
            outIndices.push_back(currentIdx + 1);
            outIndices.push_back(currentIdx + 2);
            outIndices.push_back(currentIdx + 3);

            samplePos.x += step.x;

            currentIdx += 4;
        }

        samplePos.x = 0.0f;
        samplePos.y += step.y;
    }
}
