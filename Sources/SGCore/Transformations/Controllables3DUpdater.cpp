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
        TransformBase& ownTransform = transform->m_ownTransform;
        TransformBase& finalTransform = transform->m_finalTransform;

        const float inverseFactor = controllable3D.m_inverseMouse ? -1.0f : 1.0f;

        if(!ownTransform.m_blockRotation)
        {
            controllable3D.m_pitchYawRoll.x -= (float) Input::PC::getCursorPositionDeltaY() * controllable3D.m_rotationSensitive * inverseFactor;
            controllable3D.m_pitchYawRoll.y -= (float) Input::PC::getCursorPositionDeltaX() * controllable3D.m_rotationSensitive * inverseFactor;

            const glm::vec3 rotation = {
                glm::radians(controllable3D.m_pitchYawRoll.x),
                glm::radians(controllable3D.m_pitchYawRoll.y),
                glm::radians(controllable3D.m_pitchYawRoll.z)
            };

            ownTransform.m_rotation = glm::quat(rotation);
        }

        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_R))
        {
            ownTransform.m_position.x = ownTransform.m_position.y = ownTransform.m_position.z = 0.0f;
            ownTransform.m_rotation = glm::identity<glm::quat>();
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
            ownTransform.m_position += finalTransform.m_forward * finalCameraSpeed * finalDt;
        }
        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_S))
        {
            ownTransform.m_position -= finalTransform.m_forward * finalCameraSpeed * finalDt;
        }
        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_A))
        {
            ownTransform.m_position += finalTransform.m_right * finalCameraSpeed * finalDt;
        }
        if(Input::PC::keyboardKeyDown(Input::KeyboardKey::KEY_D))
        {
            ownTransform.m_position -= finalTransform.m_right * finalCameraSpeed * finalDt;
        }

        if(Input::PC::keyboardKeyReleased(Input::KeyboardKey::KEY_ESCAPE))
        {
            CoreMain::getWindow().setHideAndCentralizeCursor(
                    !CoreMain::getWindow().isHideAndCentralizeCursor());

            ownTransform.m_blockRotation = !CoreMain::getWindow().isHideAndCentralizeCursor();
        }
    });
}
