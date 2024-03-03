//
// Created by stuka on 04.07.2023.
//

#include "glm/gtx/rotate_vector.hpp"

#include "Controllables3DUpdater.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Main/CoreMain.h"
#include "SGUtils/MathUtils.h"
#include "SGCore/Scene/Scene.h"
#include "Transform.h"
#include "SGCore/Render/RenderingBase.h"
#include "Controllable3D.h"

void SGCore::Controllables3DUpdater::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto lockedScene = m_scene.lock();
    if(!lockedScene) return;
    
    float finalDt = dt * 300.0f;
    // finalDt = 1.0f;
    
    auto controllablesView = lockedScene->getECSRegistry().view<Ref<Transform>, Controllable3D>();

    controllablesView.each([&finalDt](Ref<Transform>& transform, Controllable3D& controllable3D) {
        TransformBase& ownTransform = transform->m_ownTransform;

        if(!ownTransform.m_blockRotation)
        {
            ownTransform.m_rotation.x +=
                    (float) InputManager::getMainInputListener()->getCursorPositionDeltaY() *
                    controllable3D.m_rotationSensitive;
            ownTransform.m_rotation.y +=
                    (float) InputManager::getMainInputListener()->getCursorPositionDeltaX() *
                    controllable3D.m_rotationSensitive;
        }

        // restore camera`s transformation
        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_R))
        {
            ownTransform.m_rotation.x = ownTransform.m_rotation.y = ownTransform.m_rotation.z =
            ownTransform.m_position.x = ownTransform.m_position.y = ownTransform.m_position.z = 0.0f;

            // ownTransform.m_translationMatrix = ownTransform.m_rotationMatrix = ownTransform.m_scaleMatrix = ownTransform.m_modelMatrix = glm::identity<glm::mat4>();
        }

        glm::vec3 rotatedForward = MathUtils::forward3;
        glm::vec3 rotatedLeft = MathUtils::left3;

        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_W) ||
           InputManager::getMainInputListener()->keyboardKeyDown(KEY_S))
        {
            rotatedForward = glm::rotate(rotatedForward,
                                         glm::radians(-ownTransform.m_rotation.x),
                                         glm::vec3(1, 0, 0));
            rotatedForward = glm::rotate(rotatedForward,
                                         glm::radians(-ownTransform.m_rotation.y),
                                         glm::vec3(0, 1, 0));
        }

        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_A) ||
           InputManager::getMainInputListener()->keyboardKeyDown(KEY_D))
        {
            rotatedLeft = glm::rotate(rotatedLeft,
                                      glm::radians(-ownTransform.m_rotation.x),
                                      glm::vec3(1, 0, 0));
            rotatedLeft = glm::rotate(rotatedLeft,
                                      glm::radians(-ownTransform.m_rotation.y),
                                      glm::vec3(0, 1, 0));
        }

        float finalCameraSpeed = controllable3D.m_movementSpeed;

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
            ownTransform.m_position -= rotatedForward * finalCameraSpeed * finalDt;
        }
        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_S))
        {
            ownTransform.m_position += rotatedForward * finalCameraSpeed * finalDt;
        }
        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_A))
        {
            ownTransform.m_position += rotatedLeft * finalCameraSpeed * finalDt;
        }
        if(InputManager::getMainInputListener()->keyboardKeyDown(KEY_D))
        {
            ownTransform.m_position -= rotatedLeft * finalCameraSpeed * finalDt;
        }

        if(InputManager::getMainInputListener()->keyboardKeyReleased(KEY_ESCAPE))
        {
            CoreMain::getWindow().setHideAndCentralizeCursor(
                    !CoreMain::getWindow().isHideAndCentralizeCursor());

            ownTransform.m_blockRotation = !CoreMain::getWindow().isHideAndCentralizeCursor();
        }
    });
}
