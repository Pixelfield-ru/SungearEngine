//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_RENDERINGBASE_H
#define SUNGEARENGINE_RENDERINGBASE_H

#include <SGCore/pch.h>

#include "SGCore/Math/Frustum.h"

//#include "RenderingComponentsSystem.h"

namespace SGCore
{
    struct RenderingBase
    {
        float m_fov = 75.0f;
        float m_aspect = 16.0f / 9.0f;
        float m_zNear = 0.1;
        float m_zFar = 2000.0;

        float m_left = -1280.0f;
        float m_right = 1280.0f;
        float m_bottom = -720.0f;
        float m_top = 720.0f;

        glm::mat4 m_projectionMatrix = glm::perspective<float>(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
        glm::mat4 m_orthographicMatrix = glm::ortho<float>(m_left, m_right, m_bottom, m_top);
        glm::mat4 m_viewMatrix { 1 };
        glm::mat4 m_projectionSpaceMatrix { 1 };
        glm::mat4 m_orthographicSpaceMatrix { 1 };
 
        Frustum m_frustum;
        
        float m_lastFov = m_fov;
        float m_lastAspect = m_aspect;
        float m_lastZNear = m_zNear;
        float m_lastZFar = m_zFar;
        
        float m_lastLeft = m_left;
        float m_lastRight = m_right;
        float m_lastBottom = m_bottom;
        float m_lastTop = m_top;
        
        bool m_projectionSpaceMatrixChanged = false;
    };
}

#endif //SUNGEARENGINE_RENDERINGBASE_H
