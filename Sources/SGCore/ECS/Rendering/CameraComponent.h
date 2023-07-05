#pragma once

#ifndef SUNGEARENGINE_CAMERACOMPONENT_H
#define SUNGEARENGINE_CAMERACOMPONENT_H

#include "SGCore/ECS/IComponent.h"

#include "Camera3DMovementSystem.h"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"

namespace Core::ECS
{
    class CameraComponent : public IComponent
    {
        friend class Camera3DMovementSystem;

    public:
        float m_fov = 75.0f;
        float m_aspect = 16.0f / 9.0f;

        float m_zNear = 0.1f;
        float m_zFar = 1000.0f;

        float m_movementSpeed = 5.0f;
        float m_rotationSensitive = 0.075f;

        glm::mat4 m_projectionMatrix = glm::perspective<float>(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
        glm::mat4 m_viewMatrix = glm::mat4(1);

        constexpr const static glm::vec3 left = glm::vec3(-1, 0, 0);
        constexpr const static glm::vec3 forward = glm::vec3(0, 0, 1);
        constexpr const static glm::vec3 up = glm::vec3(0, 1, 0);

    private:
        void init() noexcept final { }

        float m_lastFov = m_fov;

        float m_lastAspect = m_aspect;
        float m_lastZNear = m_zNear;

        float m_lastZFar = m_zFar;
    };
}

#endif //SUNGEARENGINE_CAMERACOMPONENT_H
