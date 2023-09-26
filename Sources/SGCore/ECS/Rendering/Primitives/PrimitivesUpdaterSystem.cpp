#include "PrimitivesUpdaterSystem.h"
#include "IPrimitiveComponent.h"
#include "LineComponent.h"
#include "SphereComponent.h"
#include "glm/gtx/rotate_vector.hpp"

void Core::ECS::PrimitivesUpdaterSystem::update(const std::shared_ptr<Scene>& scene,
                                                const std::shared_ptr<Core::ECS::Entity>& entity)
{
    std::list<std::shared_ptr<IPrimitiveComponent>> primitiveComponents = entity->getComponents<IPrimitiveComponent>();

    for(const auto& primitiveComponent : primitiveComponents)
    {
        const auto& materialShader = primitiveComponent->m_mesh->m_material->getCurrentShader();

        if (!materialShader) continue;

        materialShader->bind();

        if(primitiveComponent->m_offset != primitiveComponent->m_lastOffset)
        {
            materialShader->useVectorf(
                    "offset",
                    primitiveComponent->m_offset
                    );

            primitiveComponent->m_lastOffset = primitiveComponent->m_offset;
        }

        if(primitiveComponent->m_color != primitiveComponent->m_lastColor)
        {
            materialShader->useVectorf(
                    "color",
                    primitiveComponent->m_color
            );

            primitiveComponent->m_lastColor = primitiveComponent->m_color;
        }

        auto sphereComponent = std::dynamic_pointer_cast<SphereComponent>(primitiveComponent);

        if(sphereComponent != nullptr)
        {
            if(sphereComponent->m_angleIncrement != sphereComponent->m_lastAngleIncrement ||
            sphereComponent->m_radius != sphereComponent->m_lastRadius)
            {
                // clear
                sphereComponent->m_mesh->m_positions.clear();
                sphereComponent->m_mesh->m_indices.clear();


                // degrees left to iterate through every circle point
                float degLeft = 360.0f;

                // vertices count for every circle in sphere
                int circleVerticesNum = (int) floorf(360.0f / sphereComponent->m_angleIncrement);

                // circle x-offset
                float circlesXOffset = (sphereComponent->m_radius * 2.0f) / ((float) (circleVerticesNum));

                glm::vec3 curPos = {0.0, 0.0, 0.0};

                // creating vertices for every circle in sphere
                // iterating from left end point of sphere to right end point of sphere
                for (float curCircleOffset = -sphereComponent->m_radius;
                     curCircleOffset < sphereComponent->m_radius; curCircleOffset += circlesXOffset)
                {
                    while (degLeft >= sphereComponent->m_angleIncrement)
                    {
                        curPos.x = curPos.y = 0.0;
                        float circleRadius = sqrt(pow(sphereComponent->m_radius, 2.0f) - pow(curCircleOffset, 2.0f));
                        curPos.z = circleRadius;

                        curPos = glm::rotate(curPos, glm::radians(degLeft), glm::vec3 {1.0, 0.0, 0.0});

                        degLeft -= sphereComponent->m_angleIncrement;

                        sphereComponent->m_mesh->m_positions.push_back(curPos.x + curCircleOffset);
                        sphereComponent->m_mesh->m_positions.push_back(curPos.y);
                        sphereComponent->m_mesh->m_positions.push_back(curPos.z);
                    }
                    degLeft = 360.0f;
                }

                // just for iterate trough loops
                size_t i = 0;
                size_t k = 0;

                // pushing first index
                sphereComponent->m_mesh->m_indices.push_back(0);

                // creating the right end of the sphere
                for (i = 0; i < circleVerticesNum; i++)
                {
                    sphereComponent->m_mesh->m_positions.push_back(sphereComponent->m_radius);
                    sphereComponent->m_mesh->m_positions.push_back(0);
                    sphereComponent->m_mesh->m_positions.push_back(0);
                }

                size_t resultIndex = 0;

                // do we need to go in the opposite direction to index the positions of the sphere
                bool goBackwards = false;
                // push indices to x-join vertices
                for(i = 0; i < circleVerticesNum; ++i)
                {
                    if(!goBackwards)
                    {
                        for (k = 0; k <= circleVerticesNum; ++k)
                        {
                            resultIndex = i + k * circleVerticesNum;

                            sphereComponent->m_mesh->m_indices.push_back(resultIndex);
                            sphereComponent->m_mesh->m_indices.push_back(resultIndex);
                        }
                    }
                    else
                    {
                        for (k = circleVerticesNum; k > 0; --k)
                        {
                            resultIndex = i + k * circleVerticesNum;

                            sphereComponent->m_mesh->m_indices.push_back(resultIndex);
                            sphereComponent->m_mesh->m_indices.push_back(resultIndex);
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

                        sphereComponent->m_mesh->m_indices.push_back(resultIndex);
                        sphereComponent->m_mesh->m_indices.push_back(resultIndex);
                    }

                    sphereComponent->m_mesh->m_indices.push_back(i * circleVerticesNum);
                }

                // preparing mesh
                sphereComponent->m_mesh->prepare();

                sphereComponent->m_lastAngleIncrement = sphereComponent->m_angleIncrement;
                sphereComponent->m_lastRadius = sphereComponent->m_radius;
            }
        }

        // todo: make individual update for box, and other primitives
    }

}

void Core::ECS::PrimitivesUpdaterSystem::deltaUpdate(const std::shared_ptr<Scene>& scene,
                                                     const std::shared_ptr<Core::ECS::Entity>& entity,
                                                     const double& deltaTime)
{

}
