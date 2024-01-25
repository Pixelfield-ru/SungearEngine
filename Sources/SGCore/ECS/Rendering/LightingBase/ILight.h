//
// Created by stuka on 01.08.2023.
//

#ifndef SUNGEARENGINE_ILIGHTCOMPONENT_H
#define SUNGEARENGINE_ILIGHTCOMPONENT_H

#include <glm/glm.hpp>
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/ECS/Rendering/IRenderingComponent.h"

namespace SGCore
{
    class IFrameBuffer;

    class ILight : public IRenderingComponent
    {
        friend class PBRFRPDirectionalLightsCollector;

    public:
        Ref<IFrameBuffer> m_shadowMap;

        glm::vec4 m_color { 1.0, 1.0, 1.0, 1.0 };
        float m_intensity = 1000.0f;

        std::uint8_t m_samplesCount = 16;

    private:
        glm::vec4 m_lastColor { 0.0, 0.0, 0.0, 0.0 };
        float m_lastIntensity = 0.0f;

        void init() noexcept override { }
    };
}

#endif //SUNGEARENGINE_ILIGHTCOMPONENT_H
