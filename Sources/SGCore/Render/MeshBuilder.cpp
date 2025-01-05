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

    auto meshData = AssetManager::getInstance()->createAsset<IMeshData>();
    meshBase.setMeshData(meshData);

    meshData->m_vertices.clear();

    meshData->m_vertices.push_back({ .m_position = { -size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f }});

    meshData->m_vertices.push_back({ .m_position = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f }});

    meshData->m_vertices.push_back({ .m_position = { -size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f }});

    meshData->m_vertices.push_back({ .m_position = { -size.x / 2.0f, size.y / 2.0f, size.z / 2.0f }});



    meshData->m_vertices.push_back({ .m_position = { size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f }});

    meshData->m_vertices.push_back({ .m_position = { size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f }});

    meshData->m_vertices.push_back({ .m_position = { size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f }});

    meshData->m_vertices.push_back({ .m_position = { size.x / 2.0f, size.y / 2.0f, size.z / 2.0f }});

    // --------------------------------

    meshData->m_indices.clear();

    meshData->m_indices.push_back(0);
    meshData->m_indices.push_back(1);

    meshData->m_indices.push_back(1);
    meshData->m_indices.push_back(2);

    meshData->m_indices.push_back(2);
    meshData->m_indices.push_back(3);

    meshData->m_indices.push_back(3);
    meshData->m_indices.push_back(0);



    meshData->m_indices.push_back(4);
    meshData->m_indices.push_back(5);

    meshData->m_indices.push_back(5);
    meshData->m_indices.push_back(6);

    meshData->m_indices.push_back(6);
    meshData->m_indices.push_back(7);

    meshData->m_indices.push_back(7);
    meshData->m_indices.push_back(4);



    meshData->m_indices.push_back(0);
    meshData->m_indices.push_back(4);

    meshData->m_indices.push_back(3);
    meshData->m_indices.push_back(7);

    meshData->m_indices.push_back(2);
    meshData->m_indices.push_back(6);

    meshData->m_indices.push_back(1);
    meshData->m_indices.push_back(5);

    meshData->prepare();
}

void SGCore::MeshBuilder::buildSphereVariant1(SGCore::MeshBase& meshBase, const float& radius,
                                              const float& angleIncrement) noexcept
{
    auto meshData = AssetManager::getInstance()->createAsset<IMeshData>();
    meshBase.setMeshData(meshData);

    // clear
    meshData->m_vertices.clear();
    meshData->m_indices.clear();


    // degrees left to iterate through every circle point
    float degLeft = 360.0f;

    // vertices count for every circle in sphere
    int circleVerticesNum = (int) floorf(360.0f / angleIncrement);

    // circle x-offset
    float circlesXOffset = (radius * 2.0f) / ((float) (circleVerticesNum));

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
