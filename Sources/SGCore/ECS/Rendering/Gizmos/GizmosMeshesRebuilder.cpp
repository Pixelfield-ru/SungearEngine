#include "GizmosMeshesRebuilder.h"
#include "IGizmo.h"
#include "LineGizmo.h"
#include "SphereGizmo.h"
#include "glm/gtx/rotate_vector.hpp"
#include "BoxGizmo.h"
#include "SGCore/Graphics/Defines.h"
#include "SGCore/ECS/ECSWorld.h"

// todo: make optimization

SGCore::GizmosMeshesRebuilder::GizmosMeshesRebuilder()
{
    m_componentsCollector.configureCachingFunction<IGizmo, Transform>();
}

void SGCore::GizmosMeshesRebuilder::fixedUpdate(const Ref<Scene>& scene)
{
    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsCollector.m_cachedEntities, layer, cachedEntity)
        auto transformComponent = cachedEntity.getComponent<Transform>();
        if(!transformComponent) continue;
        auto gizmoComponents = cachedEntity.getComponents<IGizmo>();

        for(const auto& gizmoComponent : gizmoComponents)
        {
            // if follow all
            if(gizmoComponent->m_followEntityTRS.x &&
               gizmoComponent->m_followEntityTRS.y &&
               gizmoComponent->m_followEntityTRS.z)
            {
                gizmoComponent->m_modelMatrix = transformComponent->m_modelMatrix;
            }
            else
            {
                gizmoComponent->m_modelMatrix = glm::identity<glm::mat4>();

                // if follow translation
                if(gizmoComponent->m_followEntityTRS.x)
                {
                    gizmoComponent->m_modelMatrix *= transformComponent->m_translationMatrix;
                }
                // if follow rotation
                if(gizmoComponent->m_followEntityTRS.y)
                {
                    gizmoComponent->m_modelMatrix *= transformComponent->m_rotationMatrix;
                }
                // if follow scale
                if(gizmoComponent->m_followEntityTRS.z)
                {
                    gizmoComponent->m_modelMatrix *= transformComponent->m_scaleMatrix;
                }
            }

            // ------------------- individual for every primitive component

            auto sphereComponent = std::dynamic_pointer_cast<SphereGizmo>(gizmoComponent);

            if(sphereComponent != nullptr)
            {
                if(sphereComponent->m_angleIncrement != sphereComponent->m_lastAngleIncrement ||
                   sphereComponent->m_radius != sphereComponent->m_lastRadius)
                {
                    // clear
                    sphereComponent->m_meshData->m_positions.clear();
                    sphereComponent->m_meshData->m_indices.clear();


                    // degrees left to iterate through every circle point
                    float degLeft = 360.0f;

                    // vertices count for every circle in sphere
                    int circleVerticesNum = (int) floorf(360.0f / sphereComponent->m_angleIncrement);

                    // circle x-offset
                    float circlesXOffset = (sphereComponent->m_radius * 2.0f) / ((float) (circleVerticesNum));

                    glm::vec3 curPos = {0.0, 0.0, 0.0};

                    // creating vertices for every circle in sphere
                    // iterating from left end point of sphere to right end point of sphere
                    for(float curCircleOffset = -sphereComponent->m_radius;
                        curCircleOffset < sphereComponent->m_radius; curCircleOffset += circlesXOffset)
                    {
                        while(degLeft >= sphereComponent->m_angleIncrement)
                        {
                            curPos.x = curPos.y = 0.0;
                            float circleRadius = sqrt(
                                    pow(sphereComponent->m_radius, 2.0f) - pow(curCircleOffset, 2.0f));
                            curPos.z = circleRadius;

                            curPos = glm::rotate(curPos, glm::radians(degLeft), glm::vec3 {1.0, 0.0, 0.0});

                            degLeft -= sphereComponent->m_angleIncrement;

                            sphereComponent->m_meshData->m_positions.push_back(curPos.x + curCircleOffset);
                            sphereComponent->m_meshData->m_positions.push_back(curPos.y);
                            sphereComponent->m_meshData->m_positions.push_back(curPos.z);
                        }
                        degLeft = 360.0f;
                    }

                    // just for iterate trough loops
                    size_t i = 0;
                    size_t k = 0;

                    // pushing first index
                    sphereComponent->m_meshData->m_indices.push_back(0);

                    // creating the right end of the sphere
                    for(i = 0; i < circleVerticesNum; i++)
                    {
                        sphereComponent->m_meshData->m_positions.push_back(sphereComponent->m_radius);
                        sphereComponent->m_meshData->m_positions.push_back(0);
                        sphereComponent->m_meshData->m_positions.push_back(0);
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

                                sphereComponent->m_meshData->m_indices.push_back(resultIndex);
                                sphereComponent->m_meshData->m_indices.push_back(resultIndex);
                            }
                        }
                        else
                        {
                            for(k = circleVerticesNum; k > 0; --k)
                            {
                                resultIndex = i + k * circleVerticesNum;

                                sphereComponent->m_meshData->m_indices.push_back(resultIndex);
                                sphereComponent->m_meshData->m_indices.push_back(resultIndex);
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

                            sphereComponent->m_meshData->m_indices.push_back(resultIndex);
                            sphereComponent->m_meshData->m_indices.push_back(resultIndex);
                        }

                        sphereComponent->m_meshData->m_indices.push_back(i * circleVerticesNum);
                    }

                    // preparing mesh
                    sphereComponent->m_meshData->prepare();

                    sphereComponent->m_lastAngleIncrement = sphereComponent->m_angleIncrement;
                    sphereComponent->m_lastRadius = sphereComponent->m_radius;
                }
            }
            else
            {
                auto boxComponent = std::dynamic_pointer_cast<BoxGizmo>(gizmoComponent);

                if(boxComponent)
                {
                    if(boxComponent->m_size != boxComponent->m_lastSize)
                    {
                        boxComponent->build();

                        boxComponent->m_lastSize = boxComponent->m_size;
                    }
                }
            }
        }

            // todo: make individual update for box, and other primitives
    SG_END_ITERATE_CACHED_ENTITIES
}
