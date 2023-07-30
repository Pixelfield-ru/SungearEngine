//
// Created by stuka on 27.07.2023.
//

#include "RenderingComponentsSystem.h"
#include "IRenderingComponent.h"

void Core::ECS::RenderingComponentsSystem::update(const std::shared_ptr<Scene>& scene,
                                                  const std::shared_ptr<Core::ECS::Entity>& entity)
{
    std::shared_ptr<IRenderingComponent> renderingComponent = entity->getComponent<IRenderingComponent>();

    if(renderingComponent != nullptr)
    {
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
