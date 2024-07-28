//
// Created by stuka on 03.02.2024.
//

#ifndef SUNGEARENGINE_LIGHTBASE_H
#define SUNGEARENGINE_LIGHTBASE_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Render/RenderingBase.h"

namespace SGCore
{
    class IFrameBuffer;

    sg_struct()
    struct LightBase
    {
        Ref<IFrameBuffer> m_shadowMap;

        sg_member()
        glm::vec4 m_color { 1.0, 1.0, 1.0, 1.0 };
        sg_member()
        float m_intensity = 1000.0f;

        glm::vec4 m_lastColor { 0.0, 0.0, 0.0, 0.0 };
        float m_lastIntensity = 0.0f;

        sg_member()
        std::uint8_t m_samplesCount = 16;
    };
}

#endif //SUNGEARENGINE_LIGHTBASE_H
