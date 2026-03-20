//
// Created by stuka on 10.07.2025.
//

#pragma once

#include <glm/mat4x4.hpp>

#include "SGCore/Main/CoreMain.h"

namespace SGCore
{
    class IFrameBuffer;

    struct SGCORE_EXPORT CSMCascade
    {
        float m_level = 0.0f;
        glm::ivec2 m_frameBufferSize { 2048, 2048 };
        glm::mat4 m_projectionSpaceMatrix = glm::mat4(1.0f);
        float m_bias = 0.002f;
        Ref<IFrameBuffer> m_frameBuffer;

        void createFrameBuffer() noexcept;
    };
}
