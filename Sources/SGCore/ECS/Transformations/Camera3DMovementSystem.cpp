//
// Created by stuka on 04.07.2023.
//

#include "Camera3DMovementSystem.h"
#include "SGCore/ECS/Rendering/CameraComponent.h"

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/Input/InputManager.h"
#include "glm/gtx/rotate_vector.hpp"
#include "SGCore/Main/CoreMain.h"

void Core::ECS::Camera3DMovementSystem::update
(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity)
{

}

void Core::ECS::Camera3DMovementSystem::deltaUpdate
(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity, const double& deltaTime)
{
    std::shared_ptr<CameraComponent> cameraComponent = entity->getComponent<CameraComponent>();
    std::shared_ptr<TransformComponent> transformComponent = entity->getComponent<TransformComponent>();

    if(!transformComponent || !cameraComponent) return;

    // rotate camera by mouse
    transformComponent->m_rotation.x += (float) InputManager::getMainInputListener()->getCursorPositionDeltaY() * cameraComponent->m_rotationSensitive;
    transformComponent->m_rotation.y += (float) InputManager::getMainInputListener()->getCursorPositionDeltaX() * cameraComponent->m_rotationSensitive;

    // restore camera`s transformation
    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_R))
    {
        transformComponent->m_rotation.x = transformComponent->m_rotation.y = transformComponent->m_rotation.z =
        transformComponent->m_position.x = transformComponent->m_position.y = transformComponent->m_position.z = 0.0f;
    }

    glm::vec3 rotatedForward = CameraComponent::forward;
    glm::vec3 rotatedLeft = CameraComponent::left;

    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_W) || InputManager::getMainInputListener()->keyboardKeyDown(KEY_S))
    {
        rotatedForward = glm::rotate(rotatedForward,
                                     glm::radians(-transformComponent->m_rotation.x),
                                     glm::vec3(1, 0, 0));
        rotatedForward = glm::rotate(rotatedForward,
                                     glm::radians(-transformComponent->m_rotation.y),
                                     glm::vec3(0, 1, 0));
    }

    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_A) || InputManager::getMainInputListener()->keyboardKeyDown(KEY_D))
    {
        rotatedLeft = glm::rotate(rotatedLeft,
                                  glm::radians(-transformComponent->m_rotation.x),
                                  glm::vec3(1, 0, 0));
        rotatedLeft = glm::rotate(rotatedLeft,
                                  glm::radians(-transformComponent->m_rotation.y),
                                  glm::vec3(0, 1, 0));
    }

    float finalCameraSpeed = cameraComponent->m_movementSpeed;

    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_LEFT_SHIFT))
    {
        finalCameraSpeed *= 6.0;
    }

    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_W))
    {
        transformComponent->m_position -= rotatedForward * finalCameraSpeed * (float) deltaTime;
    }
    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_S))
    {
        transformComponent->m_position += rotatedForward * finalCameraSpeed * (float) deltaTime;
    }
    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_A))
    {
        transformComponent->m_position += rotatedLeft * finalCameraSpeed * (float) deltaTime;
    }
    if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_D))
    {
        transformComponent->m_position -= rotatedLeft * finalCameraSpeed * (float) deltaTime;
    }

    if(InputManager::getMainInputListener()->keyboardKeyReleased(KEY_ESCAPE))
    {
        Core::Main::CoreMain::getWindow().setHideAndCentralizeCursor(!Core::Main::CoreMain::getWindow().isHideAndCentralizeCursor());
    }
}
