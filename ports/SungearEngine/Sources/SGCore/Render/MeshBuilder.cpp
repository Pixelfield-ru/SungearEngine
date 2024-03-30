//
// Created by stuka on 02.02.2024.
//

#include "MeshBuilder.h"
#include "glm/gtx/rotate_vector.hpp"

void SGCore::MeshBuilder::buildLine(SGCore::MeshBase& meshBase, const glm::vec3& start, const glm::vec3& end) noexcept
{

}

void SGCore::MeshBuilder::buildBox3D(SGCore::MeshBase& meshBase, const glm::vec3& size) noexcept
{
    // building a cube ------------------------------

    meshBase.m_meshData->m_positions.clear();

    meshBase.m_meshData->m_positions.push_back(-size.x / 2.0f);
    meshBase.m_meshData->m_positions.push_back(-size.y / 2.0f);
    meshBase.m_meshData->m_positions.push_back(size.z / 2.0f);

    meshBase.m_meshData->m_positions.push_back(-size.x / 2.0f);
    meshBase.m_meshData->m_positions.push_back(-size.y / 2.0f);
    meshBase.m_meshData->m_positions.push_back(-size.z / 2.0f);

    meshBase.m_meshData->m_positions.push_back(-size.x / 2.0f);
    meshBase.m_meshData->m_positions.push_back(size.y / 2.0f);
    meshBase.m_meshData->m_positions.push_back(-size.z / 2.0f);

    meshBase.m_meshData->m_positions.push_back(-size.x / 2.0f);
    meshBase.m_meshData->m_positions.push_back(size.y / 2.0f);
    meshBase.m_meshData->m_positions.push_back(size.z / 2.0f);



    meshBase.m_meshData->m_positions.push_back(size.x / 2.0f);
    meshBase.m_meshData->m_positions.push_back(-size.y / 2.0f);
    meshBase.m_meshData->m_positions.push_back(size.z / 2.0f);

    meshBase.m_meshData->m_positions.push_back(size.x / 2.0f);
    meshBase.m_meshData->m_positions.push_back(-size.y / 2.0f);
    meshBase.m_meshData->m_positions.push_back(-size.z / 2.0f);

    meshBase.m_meshData->m_positions.push_back(size.x / 2.0f);
    meshBase.m_meshData->m_positions.push_back(size.y / 2.0f);
    meshBase.m_meshData->m_positions.push_back(-size.z / 2.0f);

    meshBase.m_meshData->m_positions.push_back(size.x / 2.0f);
    meshBase.m_meshData->m_positions.push_back(size.y / 2.0f);
    meshBase.m_meshData->m_positions.push_back(size.z / 2.0f);

    // --------------------------------

    meshBase.m_meshData->m_indices.clear();

    meshBase.m_meshData->m_indices.push_back(0);
    meshBase.m_meshData->m_indices.push_back(1);

    meshBase.m_meshData->m_indices.push_back(1);
    meshBase.m_meshData->m_indices.push_back(2);

    meshBase.m_meshData->m_indices.push_back(2);
    meshBase.m_meshData->m_indices.push_back(3);

    meshBase.m_meshData->m_indices.push_back(3);
    meshBase.m_meshData->m_indices.push_back(0);



    meshBase.m_meshData->m_indices.push_back(4);
    meshBase.m_meshData->m_indices.push_back(5);

    meshBase.m_meshData->m_indices.push_back(5);
    meshBase.m_meshData->m_indices.push_back(6);

    meshBase.m_meshData->m_indices.push_back(6);
    meshBase.m_meshData->m_indices.push_back(7);

    meshBase.m_meshData->m_indices.push_back(7);
    meshBase.m_meshData->m_indices.push_back(4);



    meshBase.m_meshData->m_indices.push_back(0);
    meshBase.m_meshData->m_indices.push_back(4);

    meshBase.m_meshData->m_indices.push_back(3);
    meshBase.m_meshData->m_indices.push_back(7);

    meshBase.m_meshData->m_indices.push_back(2);
    meshBase.m_meshData->m_indices.push_back(6);

    meshBase.m_meshData->m_indices.push_back(1);
    meshBase.m_meshData->m_indices.push_back(5);

    meshBase.m_meshData->prepare();
}

void SGCore::MeshBuilder::buildSphereVariant1(SGCore::MeshBase& meshBase, const float& radius,
                                              const float& angleIncrement) noexcept
{
    // clear
    meshBase.m_meshData->m_positions.clear();
    meshBase.m_meshData->m_indices.clear();


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

            meshBase.m_meshData->m_positions.push_back(curPos.x + curCircleOffset);
            meshBase.m_meshData->m_positions.push_back(curPos.y);
            meshBase.m_meshData->m_positions.push_back(curPos.z);
        }
        degLeft = 360.0f;
    }

    // just for iterate trough loops
    size_t i = 0;
    size_t k = 0;

    // pushing first index
    meshBase.m_meshData->m_indices.push_back(0);

    // creating the right end of the sphere
    for(i = 0; i < circleVerticesNum; i++)
    {
        meshBase.m_meshData->m_positions.push_back(radius);
        meshBase.m_meshData->m_positions.push_back(0);
        meshBase.m_meshData->m_positions.push_back(0);
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

                meshBase.m_meshData->m_indices.push_back(resultIndex);
                meshBase.m_meshData->m_indices.push_back(resultIndex);
            }
        }
        else
        {
            for(k = circleVerticesNum; k > 0; --k)
            {
                resultIndex = i + k * circleVerticesNum;

                meshBase.m_meshData->m_indices.push_back(resultIndex);
                meshBase.m_meshData->m_indices.push_back(resultIndex);
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

            meshBase.m_meshData->m_indices.push_back(resultIndex);
            meshBase.m_meshData->m_indices.push_back(resultIndex);
        }

        meshBase.m_meshData->m_indices.push_back(i * circleVerticesNum);
    }

    // preparing mesh
    meshBase.m_meshData->prepare();
}
