//
// Created by stuka on 04.07.2023.
//

#include "glm/gtx/rotate_vector.hpp"

#include "CameraMovement3DSystem.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/Math.h"
#include "SGCore/ECS/ECSUtils.h"

SGCore::CameraMovement3DSystem::CameraMovement3DSystem()
{
    m_componentsCollector.configureCachingFunction<Camera, Transform>();
}

void SGCore::CameraMovement3DSystem::fixedUpdate
(const Ref<Scene>& scene)
{
    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsCollector.m_cachedEntities, layer, cachedEntity)
            Ref<Camera> cameraComponent = cachedEntity.getComponent<Camera>();
            Ref<Transform> transformComponent = cachedEntity.getComponent<Transform>();

            if(!transformComponent || !cameraComponent) return;

            // rotate camera by mouse
            if(!transformComponent->m_blockRotation)
            {
                transformComponent->m_rotation.x +=
                        (float) InputManager::getMainInputListener()->getCursorPositionDeltaY() *
                        cameraComponent->m_rotationSensitive;
                transformComponent->m_rotation.y +=
                        (float) InputManager::getMainInputListener()->getCursorPositionDeltaX() *
                        cameraComponent->m_rotationSensitive;
            }

            // restore camera`s transformation
            if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_R))
            {
                transformComponent->m_rotation.x = transformComponent->m_rotation.y = transformComponent->m_rotation.z =
                transformComponent->m_position.x = transformComponent->m_position.y = transformComponent->m_position.z = 0.0f;
            }

            glm::vec3 rotatedForward = MathUtils::forward3;
            glm::vec3 rotatedLeft = MathUtils::left3;

            if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_W) ||
               InputManager::getMainInputListener()->keyboardKeyDown(KEY_S))
            {
                rotatedForward = glm::rotate(rotatedForward,
                                             glm::radians(-transformComponent->m_rotation.x),
                                             glm::vec3(1, 0, 0));
                rotatedForward = glm::rotate(rotatedForward,
                                             glm::radians(-transformComponent->m_rotation.y),
                                             glm::vec3(0, 1, 0));
            }

            if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_A) ||
               InputManager::getMainInputListener()->keyboardKeyDown(KEY_D))
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

            if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_LEFT_CONTROL))
            {
                finalCameraSpeed /= 6.0;
            }

            if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_W))
            {
                transformComponent->m_position -= rotatedForward * finalCameraSpeed;
            }
            if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_S))
            {
                transformComponent->m_position += rotatedForward * finalCameraSpeed;
            }
            if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_A))
            {
                transformComponent->m_position += rotatedLeft * finalCameraSpeed;
            }
            if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_D))
            {
                transformComponent->m_position -= rotatedLeft * finalCameraSpeed;
            }

            if(InputManager::getMainInputListener()->keyboardKeyReleased(KEY_ESCAPE))
            {
                CoreMain::getWindow().setHideAndCentralizeCursor(
                        !CoreMain::getWindow().isHideAndCentralizeCursor());

                transformComponent->m_blockRotation = !CoreMain::getWindow().isHideAndCentralizeCursor();
            }
    SG_END_ITERATE_CACHED_ENTITIES
}
