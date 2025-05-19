//
// Created by stuka on 06.05.2025.
//

#include "Terrain.h"

#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/Mesh.h"

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

void SGCore::Terrain::generatePhysicalMesh(Terrain::reg_t& terrain, const Mesh& terrainMesh, std::int32_t stepSize) noexcept
{
    // get first displacement tex
    const auto displacementTex = terrainMesh.m_base.getMaterial()->getTexture(SGTextureType::SGTT_DISPLACEMENT, 0);
    if(!displacementTex) return;

    const auto meshData = terrainMesh.m_base.getMeshData();

    terrainMesh.m_base.getMeshData()->m_physicalMesh = MakeRef<btTriangleMesh>();

    const glm::i64vec2 terrainSize {
        terrain.getSize().x * terrain.getPatchSize(),
        terrain.getSize().y * terrain.getPatchSize()
    };

    const glm::vec2 pixelSize {
        (float) terrainSize.x / (float) displacementTex->getWidth(),
        (float) terrainSize.y / (float) displacementTex->getHeight()
    };

    const glm::vec2 step { stepSize / pixelSize.x, stepSize / pixelSize.y };

    glm::vec2 samplePos { 0, 0 };

    for(int z = 0; z < terrainSize.x; z += stepSize)
    {
        for(int x = 0; x < terrainSize.y; x += stepSize)
        {
            const float v0Height = displacementTex->sampleRAM<float>({ samplePos.x, samplePos.y }).r;

            const float v1Height = displacementTex->sampleRAM<float>({
                samplePos.x,
                std::min(std::int32_t(samplePos.y + step.y), displacementTex->getHeight() - 1)
            }).r;

            const float v2Height = displacementTex->sampleRAM<float>({
                std::min(std::int32_t(samplePos.x + step.x), displacementTex->getWidth() - 1),
                samplePos.y
            }).r;

            const float v3Height = displacementTex->sampleRAM<float>({
                std::min(std::int32_t(samplePos.x + step.x), displacementTex->getWidth() - 1),
                std::min(std::int32_t(samplePos.y + step.y), displacementTex->getHeight() - 1)
            }).r;

            btVector3 v0(x, v0Height * terrain.m_heightScale, z);
            btVector3 v1((x + stepSize), v2Height * terrain.m_heightScale, z);
            btVector3 v2(x, v1Height * terrain.m_heightScale, (z + stepSize));
            btVector3 v3((x + stepSize), v3Height * terrain.m_heightScale, (z + stepSize));

            v0.setX(v0.getX() - terrain.m_patchSize / 2.0f);
            v0.setZ(v0.getZ() - terrain.m_patchSize / 2.0f);

            v1.setX(v1.getX() - terrain.m_patchSize / 2.0f);
            v1.setZ(v1.getZ() - terrain.m_patchSize / 2.0f);

            v2.setX(v2.getX() - terrain.m_patchSize / 2.0f);
            v2.setZ(v2.getZ() - terrain.m_patchSize / 2.0f);

            v3.setX(v3.getX() - terrain.m_patchSize / 2.0f);
            v3.setZ(v3.getZ() - terrain.m_patchSize / 2.0f);

            meshData->m_physicalMesh->addTriangle(v0, v2, v1);

            meshData->m_physicalMesh->addTriangle(v1, v2, v3);

            samplePos.x += step.x;
        }

        samplePos.x = 0.0f;
        samplePos.y += step.y;
    }

    std::cout << "Generated physical mesh for terrain with " << meshData->m_physicalMesh->getNumTriangles() << " triangles!" << std::endl;
}
