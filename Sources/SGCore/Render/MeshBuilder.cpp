//
// Created by stuka on 02.02.2024.
//

#include "MeshBuilder.h"
#include "SGCore/Memory/AssetManager.h"
#include <glm/gtx/rotate_vector.hpp>

void SGCore::MeshBuilder::buildLine(SGCore::MeshBase& meshBase, const glm::vec3& start, const glm::vec3& end) noexcept
{

}

void SGCore::MeshBuilder::buildBox3D(SGCore::MeshBase& meshBase, const glm::vec3& size) noexcept
{
    // building a cube ------------------------------

    // auto meshData = meshBase.getMeshData();

    auto meshData = AssetManager::getInstance()->createAndAddAsset<IMeshData>();
    meshBase.setMeshData(meshData);

    meshData->m_vertices.clear();
    meshData->m_vertices.resize(8);

    auto addVertex = [&](const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& uv) {
        meshData->m_vertices.push_back({ .m_position = pos, .m_uv = { uv.x, uv.y, 0.0f }, .m_normal = normal });
    };

    auto addQuad = [&](const glm::vec3& normal,
                       const glm::vec3& v0, const glm::vec3& v1,
                       const glm::vec3& v2, const glm::vec3& v3,
                       const glm::vec2& uv0, const glm::vec2& uv1,
                       const glm::vec2& uv2, const glm::vec2& uv3) {
        const auto base = meshData->m_vertices.size();

        addVertex(v0, normal, uv0);
        addVertex(v1, normal, uv1);
        addVertex(v2, normal, uv2);
        addVertex(v3, normal, uv3);

        // CW
        meshData->m_indices.push_back(base + 0);
        meshData->m_indices.push_back(base + 2);
        meshData->m_indices.push_back(base + 1);

        meshData->m_indices.push_back(base + 0);
        meshData->m_indices.push_back(base + 3);
        meshData->m_indices.push_back(base + 2);
    };

    const auto h = size / 2.0f;

    // left
    addQuad(
        { -1, 0, 0 },
        { -h.x, -h.y,  h.z },
        { -h.x, -h.y, -h.z },
        { -h.x,  h.y, -h.z },
        { -h.x,  h.y,  h.z },
        { 1, 1 }, { 0, 1 }, { 0, 0 }, { 1, 0 }
    );

    // right
    addQuad(
        { 1, 0, 0 },
        { h.x, -h.y, -h.z },
        { h.x, -h.y, h.z },
        { h.x, h.y, h.z },
        { h.x, h.y, -h.z },
        { 1, 1 }, { 0, 1 }, { 0, 0 }, { 1, 0 }
    );

    // down
    addQuad(
        { 0, -1, 0 },
        {-h.x, -h.y,  h.z},
        { h.x, -h.y,  h.z},
        { h.x, -h.y, -h.z},
        {-h.x, -h.y, -h.z},
        { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }
    );

    // up
    addQuad(
        { 0, 1, 0 },
        { h.x, h.y, h.z },
        { -h.x, h.y, h.z },
        { -h.x, h.y, -h.z },
        { h.x, h.y, -h.z },
        { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }
    );

    // forward
    addQuad(
        { 0, 0, 1 },
        { h.x, -h.y, h.z },
        { -h.x, -h.y, h.z },
        { -h.x, h.y, h.z },
        { h.x, h.y, h.z },
        { 1, 1 }, { 0, 1 }, { 0, 0 }, { 1, 0 }
    );

    // backward
    addQuad(
        { 0, 0, -1 },
        { -h.x, -h.y, -h.z },
        { h.x, -h.y, -h.z },
        { h.x, h.y, -h.z },
        { -h.x, h.y, -h.z },
        { 1, 1 }, { 0, 1 }, { 0, 0 }, { 1, 0 }
    );

    meshData->prepare();
}

void SGCore::MeshBuilder::buildSphereVariant1(SGCore::MeshBase& meshBase, const float& radius,
                                              const float& angleIncrement) noexcept
{
    auto meshData = AssetManager::getInstance()->createAndAddAsset<IMeshData>();
    meshBase.setMeshData(meshData);

    // clear
    meshData->m_vertices.clear();
    meshData->m_indices.clear();


    // degrees left to iterate through every circle point
    float degLeft = 360.0f;

    // vertices count for every circle in sphere
    const int circleVerticesNum = (int) floorf(360.0f / angleIncrement);

    // circle x-offset
    const float circlesXOffset = (radius * 2.0f) / ((float) (circleVerticesNum));

    glm::vec3 curPos = {0.0, 0.0, 0.0};

    // creating vertices for every circle in sphere
    // iterating from left end point of sphere to right end point of sphere
    for(float curCircleOffset = radius;
        curCircleOffset < radius; curCircleOffset += circlesXOffset)
    {
        while(degLeft >= angleIncrement)
        {
            curPos.x = curPos.y = 0.0;
            float circleRadius = sqrt(
                    pow(radius, 2.0f) - pow(curCircleOffset, 2.0f));
            curPos.z = circleRadius;

            curPos = glm::rotate(curPos, glm::radians(degLeft), glm::vec3 {1.0, 0.0, 0.0});

            degLeft -= angleIncrement;

            meshData->m_vertices.push_back({ .m_position = { curPos.x + curCircleOffset, curPos.y, curPos.z }});
        }
        degLeft = 360.0f;
    }

    // just for iterate trough loops
    size_t i = 0;
    size_t k = 0;

    // pushing first index
    meshData->m_indices.push_back(0);

    // creating the right end of the sphere
    for(i = 0; i < circleVerticesNum; i++)
    {
        meshData->m_vertices.push_back({ .m_position = { radius, 0, 0 }});
    }

    size_t resultIndex = 0;

    // do we need to go in the opposite direction to index the positions of the sphere
    bool goBackwards = false;
    // push indices to x-join vertices
    for(i = 0; i < circleVerticesNum; ++i)
    {
        if(!goBackwards)
        {
            for(k = 0; k <= circleVerticesNum; ++k)
            {
                resultIndex = i + k * circleVerticesNum;

                meshData->m_indices.push_back(resultIndex);
                meshData->m_indices.push_back(resultIndex);
            }
        }
        else
        {
            for(k = circleVerticesNum; k > 0; --k)
            {
                resultIndex = i + k * circleVerticesNum;

                meshData->m_indices.push_back(resultIndex);
                meshData->m_indices.push_back(resultIndex);
            }
        }

        goBackwards = !goBackwards;
    }

    // push indices to y-join vertices
    for(i = 0; i <= circleVerticesNum; ++i)
    {
        for(k = 0; k < circleVerticesNum; ++k)
        {
            resultIndex = k + i * circleVerticesNum;

            meshData->m_indices.push_back(resultIndex);
            meshData->m_indices.push_back(resultIndex);
        }

        meshData->m_indices.push_back(i * circleVerticesNum);
    }

    // preparing mesh
    meshData->prepare();
}
