#include "PrimitivesUpdaterSystem.h"
#include "IPrimitiveComponent.h"
#include "LineComponent.h"
#include "SphereComponent.h"
#include "glm/gtx/rotate_vector.hpp"
#include "BoxComponent.h"
#include "SGCore/Graphics/Defines.h"
#include "SGCore/ECS/ECSWorld.h"

// todo: make optimization

void Core::ECS::PrimitivesUpdaterSystem::fixedUpdate(const std::shared_ptr<Scene>& scene)
{
    for(const auto& layer : m_cachedEntities)
    {
        for(const auto& cachedEntity: layer.second)
        {
            if(!cachedEntity.second) continue;

            auto transformComponent = cachedEntity.second->getComponent<TransformComponent>();
            if(!transformComponent) continue;
            auto primitiveComponents = cachedEntity.second->getComponents<IPrimitiveComponent>();

            for(const auto& primitiveComponent: primitiveComponents)
            {
                const auto& materialShader = primitiveComponent->m_mesh->m_material->getCurrentShader();

                if(!materialShader) continue;

                // ----------------------- checking for transformations flags changed

                if(primitiveComponent->m_followEntityTRS != primitiveComponent->m_lastFollowEntityTRS)
                {
                    materialShader->setAssetModifiedChecking(false);

                    if(primitiveComponent->m_followEntityTRS.x)
                    {
                        materialShader->addDefines(SGShaderDefineType::SGG_OTHER_DEFINE,
                                                   {Graphics::ShaderDefine(SG_TRANSLATE_WITH_OBJECT, "")}
                        );
                    }
                    else
                    {
                        materialShader->removeDefine(SGShaderDefineType::SGG_OTHER_DEFINE,
                                                     SG_TRANSLATE_WITH_OBJECT
                        );
                    }

                    // ---------------

                    if(primitiveComponent->m_followEntityTRS.y)
                    {
                        materialShader->addDefines(SGShaderDefineType::SGG_OTHER_DEFINE,
                                                   {Graphics::ShaderDefine(SG_ROTATE_WITH_OBJECT, "")}
                        );
                    }
                    else
                    {
                        materialShader->removeDefine(SGShaderDefineType::SGG_OTHER_DEFINE,
                                                     SG_ROTATE_WITH_OBJECT
                        );
                    }

                    // ---------------

                    if(primitiveComponent->m_followEntityTRS.z)
                    {
                        materialShader->addDefines(SGShaderDefineType::SGG_OTHER_DEFINE,
                                                   {Graphics::ShaderDefine(SG_SCALE_WITH_OBJECT, "")}
                        );
                    }
                    else
                    {
                        materialShader->removeDefine(SGShaderDefineType::SGG_OTHER_DEFINE,
                                                     SG_SCALE_WITH_OBJECT
                        );
                    }

                    materialShader->setAssetModifiedChecking(true);

                    primitiveComponent->m_lastFollowEntityTRS = primitiveComponent->m_followEntityTRS;
                }

                // -------------------------------

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

                // ------------------- individual for every primitive component

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
                        for(i = 0; i < circleVerticesNum; i++)
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
                                for(k = 0; k <= circleVerticesNum; ++k)
                                {
                                    resultIndex = i + k * circleVerticesNum;

                                    sphereComponent->m_mesh->m_indices.push_back(resultIndex);
                                    sphereComponent->m_mesh->m_indices.push_back(resultIndex);
                                }
                            }
                            else
                            {
                                for(k = circleVerticesNum; k > 0; --k)
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
                else
                {
                    auto boxComponent = std::dynamic_pointer_cast<BoxComponent>(primitiveComponent);

                    if(boxComponent)
                    {
                        if(boxComponent->m_size != boxComponent->m_lastSize)
                        {
                            boxComponent->build();

                            boxComponent->m_lastSize = boxComponent->m_size;
                        }
                    }
                }

                // todo: make individual update for box, and other primitives
            }
        }
    }
}

void Core::ECS::PrimitivesUpdaterSystem::cacheEntity(const std::shared_ptr<Entity>& entity)
{
    cacheEntityComponents<IPrimitiveComponent, TransformComponent>(entity);
}
