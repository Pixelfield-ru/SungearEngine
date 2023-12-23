//
// Created by stuka on 27.07.2023.
//

#include "RenderingComponentsUpdater.h"
#include "IRenderingComponent.h"
#include "SGCore/ECS/Transformations/Transform.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/gtx/quaternion.hpp"
#include "SGCore/ECS/ECSUtils.h"

SGCore::RenderingComponentsUpdater::RenderingComponentsUpdater()
{
    m_componentsCollector.configureCachingFunction<IRenderingComponent, Transform>();
}

void SGCore::RenderingComponentsUpdater::fixedUpdate(const Ref<Scene>& scene)
{
    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsCollector.m_cachedEntities, layer, cachedEntity)
            std::list<Ref<IRenderingComponent>> renderingComponents = cachedEntity.getComponents<IRenderingComponent>();
            Ref<Transform> transformComponent = cachedEntity.getComponent<Transform>();

            if(!transformComponent) continue;

            bool viewMatrixChanged = transformComponent->m_rotationChanged ||
                                     transformComponent->m_positionChanged ||
                                     transformComponent->m_scaleChanged;

            glm::quat rotationQuat;
            if(viewMatrixChanged)
            {
                rotationQuat = glm::angleAxis(
                        glm::radians(transformComponent->m_rotation.x),
                        glm::vec3(1, 0, 0)
                );
                rotationQuat *= glm::angleAxis(
                        glm::radians(transformComponent->m_rotation.y),
                        glm::vec3(0, 1, 0)
                );
                rotationQuat *= glm::angleAxis(
                        glm::radians(transformComponent->m_rotation.z),
                        glm::vec3(0, 0, 1)
                );
            }

            for(auto& renderingComponent: renderingComponents)
            {
                renderingComponent->m_spaceMatrixChanged = false;

                bool projectionMatrixChanged = false;

                // TODO: make checking for lastTransformation != current transformation
                if(viewMatrixChanged)
                {
                    renderingComponent->m_viewMatrix = glm::toMat4(rotationQuat);
                    renderingComponent->m_viewMatrix = glm::translate(renderingComponent->m_viewMatrix,
                                                                      -transformComponent->m_position
                    );
                    renderingComponent->m_viewMatrix =
                            glm::scale(renderingComponent->m_viewMatrix, transformComponent->m_scale);
                }

                // if some part of projection matrix of camera is changed
                if(renderingComponent->m_lastFov != renderingComponent->m_fov ||
                   renderingComponent->m_lastAspect != renderingComponent->m_aspect ||
                   renderingComponent->m_lastZNear != renderingComponent->m_zNear ||
                   renderingComponent->m_lastZFar != renderingComponent->m_zFar)
                {
                    renderingComponent->m_projectionMatrix = glm::perspective<float>(
                            glm::radians(renderingComponent->m_fov),
                            renderingComponent->m_aspect,
                            renderingComponent->m_zNear,
                            renderingComponent->m_zFar
                    );

                    renderingComponent->m_lastFov = renderingComponent->m_fov;
                    renderingComponent->m_lastAspect = renderingComponent->m_aspect;
                    renderingComponent->m_lastZNear = renderingComponent->m_zNear;
                    renderingComponent->m_lastZFar = renderingComponent->m_zFar;

                    projectionMatrixChanged = true;
                }

                if(viewMatrixChanged || projectionMatrixChanged)
                {
                    renderingComponent->m_spaceMatrix =
                            renderingComponent->m_projectionMatrix * renderingComponent->m_viewMatrix;
                    renderingComponent->m_spaceMatrixChanged = true;
                }
            }
    SG_END_ITERATE_CACHED_ENTITIES
}