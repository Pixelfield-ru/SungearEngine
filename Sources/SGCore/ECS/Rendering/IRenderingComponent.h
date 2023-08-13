//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_IRENDERINGCOMPONENT_H
#define SUNGEARENGINE_IRENDERINGCOMPONENT_H

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "SGCore/ECS/IComponent.h"
//#include "RenderingComponentsSystem.h"

namespace Core::ECS
{
    class IRenderingComponent : public IComponent
    {
        friend class RenderingComponentsSystem;

    public:
        float m_fov = 75.0f;
        float m_aspect = 16.0f / 9.0f;

        float m_zNear = 0.1;
        float m_zFar = 260.0;

        float m_movementSpeed = 5.0f;
        float m_rotationSensitive = 0.075f;

        glm::mat4 m_projectionMatrix = glm::perspective<float>(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
        glm::mat4 m_viewMatrix = glm::mat4(1);

        constexpr const static glm::vec3 left = glm::vec3(-1, 0, 0);
        constexpr const static glm::vec3 forward = glm::vec3(0, 0, 1);
        constexpr const static glm::vec3 up = glm::vec3(0, 1, 0);

    private:
        void init() noexcept override { }

        float m_lastFov = m_fov;

        float m_lastAspect = m_aspect;
        float m_lastZNear = m_zNear;

        float m_lastZFar = m_zFar;
    };
}

#endif //SUNGEARENGINE_IRENDERINGCOMPONENT_H
