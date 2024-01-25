//
// Created by stuka on 04.07.2023.
//

#include "glm/gtx/rotate_vector.hpp"

#include "CameraMovement3DSystem.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Main/CoreMain.h"
#include "SGUtils/Math.h"
#include "SGCore/Scene/Scene.h"

void SGCore::CameraMovement3DSystem::fixedUpdate
(const Ref<Scene>& scene)
{
    m_cameras.each([](ICamera* camera, Transform* transform) {
// rotate camera by mouse
        if(!transform->m_blockRotation)
        {
            transform->m_rotation.x +=
                    (float) InputManager::getMainInputListener()->getCursorPositionDeltaY() *
                            camera->m_rotationSensitive;
            transform->m_rotation.y +=
                    (float) InputManager::getMainInputListener()->getCursorPositionDeltaX() *
                            camera->m_rotationSensitive;
        }

        // restore camera`s transformation
        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_R))
        {
            transform->m_rotation.x = transform->m_rotation.y = transform->m_rotation.z =
            transform->m_position.x = transform->m_position.y = transform->m_position.z = 0.0f;
        }

        glm::vec3 rotatedForward = SGUtils::MathUtils::forward3;
        glm::vec3 rotatedLeft = SGUtils::MathUtils::left3;

        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_W) ||
           InputManager::getMainInputListener()->keyboardKeyDown(KEY_S))
        {
            rotatedForward = glm::rotate(rotatedForward,
                                         glm::radians(-transform->m_rotation.x),
                                         glm::vec3(1, 0, 0));
            rotatedForward = glm::rotate(rotatedForward,
                                         glm::radians(-transform->m_rotation.y),
                                         glm::vec3(0, 1, 0));
        }

        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_A) ||
           InputManager::getMainInputListener()->keyboardKeyDown(KEY_D))
        {
            rotatedLeft = glm::rotate(rotatedLeft,
                                      glm::radians(-transform->m_rotation.x),
                                      glm::vec3(1, 0, 0));
            rotatedLeft = glm::rotate(rotatedLeft,
                                      glm::radians(-transform->m_rotation.y),
                                      glm::vec3(0, 1, 0));
        }

        float finalCameraSpeed = camera->m_movementSpeed;

        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_LEFT_SHIFT))
        {
            finalCameraSpeed *= 6.0;
        }

        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_LEFT_CONTROL))
        {
            finalCameraSpeed /= 6.0;
        }

        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_W))
        {
            transform->m_position -= rotatedForward * finalCameraSpeed;
        }
        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_S))
        {
            transform->m_position += rotatedForward * finalCameraSpeed;
        }
        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_A))
        {
            transform->m_position += rotatedLeft * finalCameraSpeed;
        }
        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_D))
        {
            transform->m_position -= rotatedLeft * finalCameraSpeed;
        }

        if(InputManager::getMainInputListener()->keyboardKeyReleased(KEY_ESCAPE))
        {
            CoreMain::getWindow().setHideAndCentralizeCursor(
                    !CoreMain::getWindow().isHideAndCentralizeCursor());

            transform->m_blockRotation = !CoreMain::getWindow().isHideAndCentralizeCursor();
        }
    });
}

void SGCore::CameraMovement3DSystem::attachToScene(const SGCore::Ref<SGCore::Scene>& scene)
{
    m_cameras = scene->getECSRegistry()->view<ICamera, Transform>();
}
