//
// Created by stuka on 10.07.2025.
//

#ifndef SUNGEARENGINE_CSMLEVEL_H
#define SUNGEARENGINE_CSMLEVEL_H

#include "SGCore/Main/CoreMain.h"

namespace SGCore
{
    class IFrameBuffer;

    struct CSMCascade
    {
        float m_level = 0.0f;
        glm::ivec2 m_frameBufferSize { 2048, 2048 };
        glm::mat4 m_projectionSpaceMatrix = glm::mat4(1.0f);
        Ref<IFrameBuffer> m_frameBuffer;

        void createFrameBuffer() noexcept;
    };
}

#endif // SUNGEARENGINE_CSMLEVEL_H
