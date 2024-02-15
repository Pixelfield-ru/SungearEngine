//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_RENDERINGBASE_H
#define SUNGEARENGINE_RENDERINGBASE_H

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>

//#include "RenderingComponentsSystem.h"

namespace SGCore
{
    struct RenderingBase
    {
        float m_fov = 75.0f;
        float m_aspect = 16.0f / 9.0f;

        float m_zNear = 0.1;
        float m_zFar = 2000.0;

        bool m_spaceMatrixChanged = false;

        glm::mat4 m_projectionMatrix = glm::perspective<float>(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
        glm::mat4 m_viewMatrix { 1 };
        glm::mat4 m_spaceMatrix { 1 };

    // private:
        float m_lastFov = m_fov;

        float m_lastAspect = m_aspect;
        float m_lastZNear = m_zNear;

        float m_lastZFar = m_zFar;
    };
}

#endif //SUNGEARENGINE_RENDERINGBASE_H