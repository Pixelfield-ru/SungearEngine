//
// Created by stuka on 04.12.2025.
//

#include "NavGrid3D.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Math/MathPrimitivesUtils.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/ImportedScenesArch/Vertex.h"

void SGCore::Navigation::NavGrid3D::build(const std::vector<Vertex>& meshVertices,
                                          const std::vector<std::uint32_t>& meshIndices,
                                          std::uint8_t verticesPerPrimitiveCount,
                                          const AABB<>& meshAABB,
                                          const ECS::registry_t& sceneRegistry) noexcept
{
    m_nodes.clear();

    for(float x = meshAABB.m_min.x; x <= meshAABB.m_max.x; x += m_cellSize)
    {
        for(float z = meshAABB.m_min.z; z <= meshAABB.m_max.z; z += m_cellSize)
        {
            const auto y = getHeightAt(meshVertices, meshIndices, { x, z }, verticesPerPrimitiveCount);

            if(!y) continue;

            //std::cout << "node pos: " << glm::to_string(glm::vec3 { x, 0, z }) << std::endl;

            m_nodes.push_back({
                .m_position = { x, *y, z },
                .m_size = m_cellSize
            });
        }
    }
}

void SGCore::Navigation::NavGrid3D::build(const ITexture2D* displacementMap,
                                          float displacementHeightScale,
                                          const AABB<>& meshAABB,
                                          const ECS::registry_t& sceneRegistry) noexcept
{
    m_nodes.clear();

    const glm::vec2 floorSize {
        std::abs(meshAABB.m_max.x - meshAABB.m_min.x),
        std::abs(meshAABB.m_max.z - meshAABB.m_min.z)
    };

    const glm::vec2 pixelSize {
        (float) floorSize.x / (float) displacementMap->getWidth(),
        (float) floorSize.y / (float) displacementMap->getHeight()
    };

    const glm::vec2 step { m_cellSize / pixelSize.x, m_cellSize / pixelSize.y };

    glm::vec2 samplePos { 0, 0 };

    for(float z = meshAABB.m_min.z; z <= meshAABB.m_max.z; z += m_cellSize)
    {
        for(float x = meshAABB.m_min.x; x <= meshAABB.m_max.x; x += m_cellSize)
        {
            if(samplePos.x >= displacementMap->getWidth() || samplePos.y >= displacementMap->getHeight()) continue;

            const auto y = displacementMap->sampleRAM<float>(samplePos).r * displacementHeightScale;

            m_nodes.push_back({
                .m_position = { x, y, z },
                .m_size = m_cellSize
            });

            samplePos.x += step.x;
        }

        samplePos.x = 0.0f;
        samplePos.y += step.y;
    }
}

void SGCore::Navigation::NavGrid3D::applyModelMatrix(const glm::mat4& modelMatrix) noexcept
{
    for(auto& node : m_nodes)
    {
        node.m_position = modelMatrix * glm::vec4(node.m_position, 1.0f);
    }
}

std::optional<float> SGCore::Navigation::NavGrid3D::getHeightAt(const std::vector<Vertex>& meshVertices,
                                                                const std::vector<std::uint32_t>& meshIndices,
                                                                const glm::vec2& position,
                                                                std::uint8_t verticesPerPrimitiveCount) noexcept
{
    const auto rayStart = glm::vec3 { position.x, 1000.0f, position.y};
    const glm::vec3 rayDir { 0, -1, 0 };

    auto minDistance = std::numeric_limits<float>::max();
    float height {};

    for(size_t i = 0; i < meshIndices.size(); i += verticesPerPrimitiveCount)
    {
        const auto v0 = meshVertices[meshIndices[i]].m_position;
        const auto v1 = meshVertices[meshIndices[i + 1]].m_position;
        const auto v2 = meshVertices[meshIndices[i + 2]].m_position;
        glm::vec3 v4{};
        if(verticesPerPrimitiveCount == 4)
        {
            v4 = meshVertices[meshIndices[i + 3]].m_position;
        }

        MathPrimitivesUtils::RayIntersectionInfo<> intersectionInfo;
        if(verticesPerPrimitiveCount == 3)
        {
            MathPrimitivesUtils::rayTriangleIntersection(rayStart, rayDir, v0, v1, v2, intersectionInfo);
        }
        else if(verticesPerPrimitiveCount == 4)
        {
            MathPrimitivesUtils::rayQuadIntersection(rayStart, rayDir, v0, v1, v2,  v4, intersectionInfo);
        }
        if(!intersectionInfo.m_isIntersected) continue;

        if(intersectionInfo.m_hitDistance < minDistance)
        {
            minDistance = intersectionInfo.m_hitDistance;
            height = rayStart.y - intersectionInfo.m_hitDistance; // y = startY - distance
        }
    }

    return (minDistance < std::numeric_limits<float>::max()) ? height : (std::optional<float>) std::nullopt;
}
