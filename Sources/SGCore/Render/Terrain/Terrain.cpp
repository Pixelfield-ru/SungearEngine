//
// Created by stuka on 06.05.2025.
//

#include "Terrain.h"

#include "SGCore/ImportedScenesArch/IMeshData.h"

void SGCore::Terrain::generate(Terrain::reg_t& terrain,
                               const AssetRef<IMeshData>& terrainMeshData,
                               std::int64_t patchesCountX,
                               std::int64_t patchesCountY,
                               std::int64_t patchSize) noexcept
{
    terrainMeshData->destroy();

    size_t currentIdx = 0;

    glm::vec2 curPos = { 0.0, 0.0 };

    terrain.m_uvScale = { patchSize, patchSize };
    terrain.m_size = { patchesCountX, patchesCountY };
    terrain.m_patchSize = patchSize;

    for(int y = 0; y < patchesCountY; ++y)
    {
        for(int x = 0; x < patchesCountX; ++x)
        {
            terrainMeshData->m_vertices.push_back({
                .m_position = glm::vec3 { -1, 0, -1.0f } * patchSize / 2.0f + glm::vec3(curPos.x, 0.0, curPos.y),
                .m_uv = { x / (float) patchesCountX, y / (float) patchesCountY, 0.0 }
            });

            terrainMeshData->m_vertices.push_back({
                .m_position = glm::vec3 { -1, 0, 1.0f } * patchSize / 2.0f + glm::vec3(curPos.x, 0.0, curPos.y),
                .m_uv = { x / (float) patchesCountX, (y + 1) / (float) patchesCountY, 0.0f }
            });

            terrainMeshData->m_vertices.push_back({
                .m_position = glm::vec3 { 1, 0, 1.0f } * patchSize / 2.0f + glm::vec3(curPos.x, 0.0, curPos.y),
                .m_uv = { (x + 1) / (float) patchesCountX, (y + 1) / (float) patchesCountY, 0.0f }
            });

            terrainMeshData->m_vertices.push_back({
                .m_position = glm::vec3 { 1, 0, -1.0f } * patchSize / 2.0f + glm::vec3(curPos.x, 0.0, curPos.y),
                .m_uv = { (x + 1) / (float) patchesCountX, y / (float) patchesCountY, 0.0f }
            });

            terrainMeshData->m_indices.push_back(currentIdx + 0);
            terrainMeshData->m_indices.push_back(currentIdx + 3);
            terrainMeshData->m_indices.push_back(currentIdx + 2);
            terrainMeshData->m_indices.push_back(currentIdx + 1);

            currentIdx += 4;

            curPos.x += patchSize;
        }

        curPos.x = 0;
        curPos.y += patchSize;
    }

    terrainMeshData->prepare();
}
