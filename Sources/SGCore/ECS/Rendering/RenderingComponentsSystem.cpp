//
// Created by stuka on 27.07.2023.
//

#include "RenderingComponentsSystem.h"
#include "IRenderingComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/gtx/quaternion.hpp"
#include "SGCore/ECS/ECSWorld.h"

void Core::ECS::RenderingComponentsSystem::FPSNotRelativeFixedUpdate(const std::shared_ptr<Scene>& scene)
{
    auto thisSystemCachedEntities = ECSWorld::getSystemCachedEntities<RenderingComponentsSystem>();

    if(thisSystemCachedEntities == nullptr) return;

    for (const auto& cachedEntities : thisSystemCachedEntities->m_cachedEntities)
    {
        if(cachedEntities.second == nullptr) continue;

        std::list<std::shared_ptr<IRenderingComponent>> renderingComponents = cachedEntities.second->getComponents<IRenderingComponent>();
        std::shared_ptr<TransformComponent> transformComponent = cachedEntities.second->getComponent<TransformComponent>();

        if(!transformComponent) continue;

        glm::quat rotationQuat = glm::angleAxis(
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

        for(auto& renderingComponent : renderingComponents)
        {

            // TODO: make checking for lastTransformation != current transformation
            renderingComponent->m_viewMatrix = glm::toMat4(rotationQuat);
            renderingComponent->m_viewMatrix = glm::translate(renderingComponent->m_viewMatrix,
                                                              -transformComponent->m_position
            );
            renderingComponent->m_viewMatrix =
                    glm::scale(renderingComponent->m_viewMatrix, transformComponent->m_scale);

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
            }
        }
    }
}

void Core::ECS::RenderingComponentsSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) const
{
    ECSWorld::cacheComponents<RenderingComponentsSystem, IRenderingComponent, TransformComponent>(entity);
}
