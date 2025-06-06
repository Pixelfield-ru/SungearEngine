//
// Created by stuka on 04.07.2023.
//

#include "glm/gtx/rotate_vector.hpp"

#include "Controllables3DUpdater.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Math/MathUtils.h"
#include "SGCore/Scene/Scene.h"
#include "Transform.h"
#include "SGCore/Render/RenderingBase.h"
#include "Controllable3D.h"

void SGCore::Controllables3DUpdater::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto lockedScene = getScene();

    if(!lockedScene) return;
    
    float finalDt = dt * 300.0f;
    // finalDt = 1.0f;

    auto controllablesView = lockedScene->getECSRegistry()->view<Transform, Controllable3D>();

    controllablesView.each([&finalDt](Transform::reg_t& transform, Controllable3D::reg_t& controllable3D) {
        TransformBase& ownTransform = transform->m_ownTransform;
        TransformBase& finalTransform = transform->m_finalTransform;
        const auto& mainListener = InputManager::getMainInputListener();

        if(!ownTransform.m_blockRotation)
        {
            ownTransform.m_yawPitchRoll.x -=
                    (float) mainListener->getCursorPositionDeltaY() *
                    controllable3D.m_rotationSensitive;
            ownTransform.m_yawPitchRoll.y -=
                    (float) mainListener->getCursorPositionDeltaX() *
                    controllable3D.m_rotationSensitive;
        }

        if(mainListener->keyboardKeyDown(KeyboardKey::KEY_R))
        {
            ownTransform.m_yawPitchRoll.x = ownTransform.m_yawPitchRoll.y = ownTransform.m_yawPitchRoll.z =
            ownTransform.m_position.x = ownTransform.m_position.y = ownTransform.m_position.z = 0.0f;
        }

        float finalCameraSpeed = controllable3D.m_movementSpeed;

        if(mainListener->keyboardKeyDown(KeyboardKey::KEY_LEFT_SHIFT))
        {
            finalCameraSpeed *= 6.0;
        }

        if(mainListener->keyboardKeyDown(KeyboardKey::KEY_LEFT_CONTROL))
        {
            finalCameraSpeed /= 6.0;
        }

        if(mainListener->keyboardKeyDown(KeyboardKey::KEY_W))
        {
            ownTransform.m_position += finalTransform.m_forward * finalCameraSpeed * finalDt;
        }
        if(mainListener->keyboardKeyDown(KeyboardKey::KEY_S))
        {
            ownTransform.m_position -= finalTransform.m_forward * finalCameraSpeed * finalDt;
        }
        if(mainListener->keyboardKeyDown(KeyboardKey::KEY_A))
        {
            ownTransform.m_position += finalTransform.m_right * finalCameraSpeed * finalDt;
        }
        if(mainListener->keyboardKeyDown(KeyboardKey::KEY_D))
        {
            ownTransform.m_position -= finalTransform.m_right * finalCameraSpeed * finalDt;
        }

        if(mainListener->keyboardKeyReleased(KeyboardKey::KEY_ESCAPE))
        {
            CoreMain::getWindow().setHideAndCentralizeCursor(
                    !CoreMain::getWindow().isHideAndCentralizeCursor());

            ownTransform.m_blockRotation = !CoreMain::getWindow().isHideAndCentralizeCursor();
        }
    });
}
