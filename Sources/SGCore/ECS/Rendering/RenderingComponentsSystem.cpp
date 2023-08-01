//
// Created by stuka on 27.07.2023.
//

#include "RenderingComponentsSystem.h"
#include "IRenderingComponent.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/gtx/quaternion.hpp"

void Core::ECS::RenderingComponentsSystem::update(const std::shared_ptr<Scene>& scene,
                                                  const std::shared_ptr<Core::ECS::Entity>& entity)
{
    std::list<std::shared_ptr<IRenderingComponent>> renderingComponents = entity->getComponents<IRenderingComponent>();
    std::shared_ptr<TransformComponent> transformComponent = entity->getComponent<TransformComponent>();

    for(auto& renderingComponent : renderingComponents)
    {
        if(transformComponent)
        {
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

            // TODO: make checking for lastTransformation != current transformation
            renderingComponent->m_viewMatrix = glm::toMat4(rotationQuat);
            renderingComponent->m_viewMatrix = glm::translate(renderingComponent->m_viewMatrix,
                                                              transformComponent->m_position);
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
        }
    }
}

void Core::ECS::RenderingComponentsSystem::deltaUpdate(const std::shared_ptr<Scene>& scene,
                                                       const std::shared_ptr<Core::ECS::Entity>& entity,
                                                       const double& deltaTime)
{

}
