//
// Created by stuka on 02.05.2023.
//

#include "TransformationsSystem.h"
#include "TransformComponent.h"
#include "glm/ext/matrix_transform.hpp"

void Core::ECS::TransformationsSystem::update
(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity)
{
    std::shared_ptr<TransformComponent> transformComponent = entity->getComponent<TransformComponent>();
    if(!transformComponent) return;

    bool transformationChanged = false;

    // checking position for changes. if changed then update
    if(transformComponent->m_lastPosition != transformComponent->m_position)
    {
        transformComponent->m_translationMatrix = glm::translate(transformComponent->m_translationMatrix, transformComponent->m_position - transformComponent->m_lastPosition);
        transformComponent->m_lastPosition = transformComponent->m_position;

        transformationChanged = true;
    }

    // checking rotation for changes. if changed then update
    if(transformComponent->m_lastRotation != transformComponent->m_rotation)
    {
        transformComponent->m_rotationMatrix = glm::rotate(transformComponent->m_rotationMatrix,
                                                           glm::radians(transformComponent->m_rotation.x - transformComponent->m_lastRotation.x),
                                                           glm::vec3(1, 0, 0));
        transformComponent->m_rotationMatrix = glm::rotate(transformComponent->m_rotationMatrix,
                                                           glm::radians(transformComponent->m_rotation.y - transformComponent->m_lastRotation.y),
                                                           glm::vec3(0, 1, 0));
        transformComponent->m_rotationMatrix = glm::rotate(transformComponent->m_rotationMatrix,
                                                           glm::radians(transformComponent->m_rotation.z - transformComponent->m_lastRotation.z),
                                                           glm::vec3(0, 0, 1));

        transformComponent->m_lastRotation = transformComponent->m_rotation;

        transformationChanged = true;
    }

    // checking scale for changes. if changed then update
    if(transformComponent->m_lastScale != transformComponent->m_scale)
    {
        transformComponent->m_scaleMatrix = glm::scale(transformComponent->m_scaleMatrix, transformComponent->m_scale - transformComponent->m_lastScale);
        transformComponent->m_lastScale = transformComponent->m_scale;

        transformationChanged = true;
    }

    // if any matrix is changed then update model matrix
    if(transformationChanged)
    {
        transformComponent->m_modelMatrix = transformComponent->m_translationMatrix * transformComponent->m_rotationMatrix * transformComponent->m_scaleMatrix;
    }
}

void Core::ECS::TransformationsSystem::deltaUpdate
(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity, const double& deltaTime)
{

}
