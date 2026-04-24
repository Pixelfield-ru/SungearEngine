//
// Created by stuka on 04.07.2023.
//

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"

#include "Controllables3DUpdater.h"
#include "SGCore/Input/PCInput.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Scene/Scene.h"
#include "Transform.h"
#include "SGCore/Render/RenderingBase.h"
#include "Controllable3D.h"

void SGCore::Controllables3DUpdater::fixedUpdate(double dt, double fixedDt)
{
    auto lockedScene = getScene();

    if(!lockedScene) return;
    
    float finalDt = dt * 300.0f;
    // finalDt = 1.0f;

    auto controllablesView = lockedScene->getECSRegistry()->view<Transform, Controllable3D>();

    controllablesView.each([&finalDt](Transform::reg_t& transform, Controllable3D::reg_t& controllable3D) {
        TransformBase& localTransform = transform.m_localTransform;
        TransformBase& worldTransform = transform.m_worldTransform;

        const float inverseFactor = controllable3D.m_inverseMouse ? -1.0f : 1.0f;

        if(!localTransform.m_blockRotation)
        {
            controllable3D.m_pitchYawRoll.x -= (float) Input::PC::getCursorPositionDeltaY() * controllable3D.m_rotationSensitive * inverseFactor;
            controllable3D.m_pitchYawRoll.y -= (float) Input::PC::getCursorPositionDeltaX() * controllable3D.m_rotationSensitive * inverseFactor;

            const glm::vec3 rotation = {
                glm::radians(controllable3D.m_pitchYawRoll.x),
                glm::radians(controllable3D.m_pitchYawRoll.y),
                glm::radians(controllable3D.m_pitchYawRoll.z)
            };

            localTransform.m_rotation = glm::quat(rotation);
        }

        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_R))
        {
            localTransform.m_position.x = localTransform.m_position.y = localTransform.m_position.z = 0.0f;
            localTransform.m_rotation = glm::identity<glm::quat>();
        }

        float finalCameraSpeed = controllable3D.m_movementSpeed;

        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_LEFT_SHIFT))
        {
            finalCameraSpeed *= 6.0;
        }

        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_LEFT_CONTROL))
        {
            finalCameraSpeed /= 6.0;
        }

        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_W))
        {
            localTransform.m_position += worldTransform.m_forward * finalCameraSpeed * finalDt;
        }
        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_S))
        {
            localTransform.m_position -= worldTransform.m_forward * finalCameraSpeed * finalDt;
        }
        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_A))
        {
            localTransform.m_position += worldTransform.m_right * finalCameraSpeed * finalDt;
        }
        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_D))
        {
            localTransform.m_position -= worldTransform.m_right * finalCameraSpeed * finalDt;
        }

        if(Input::PC::keyboardKeyReleased(Input::KeyboardKey::KEY_ESCAPE))
        {
            CoreMain::getWindow().setHideAndCentralizeCursor(
                    !CoreMain::getWindow().isHideAndCentralizeCursor());

            localTransform.m_blockRotation = !CoreMain::getWindow().isHideAndCentralizeCursor();
        }
    });
}
