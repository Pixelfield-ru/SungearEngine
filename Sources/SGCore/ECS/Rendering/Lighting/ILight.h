//
// Created by stuka on 01.08.2023.
//

#ifndef SUNGEARENGINE_ILIGHTCOMPONENT_H
#define SUNGEARENGINE_ILIGHTCOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "GLM/glm/glm.hpp"

namespace SGCore
{
    class ILight : public IComponent
    {
        friend class DirectionalLightsCollector;

    public:
        glm::vec4 m_color { 1.0, 1.0, 1.0, 1.0 };
        float m_intensity = 1.0f;

    private:
        glm::vec4 m_lastColor { 0.0, 0.0, 0.0, 0.0 };
        float m_lastIntensity = 0.0f;

        void init() noexcept override { }
    };
}

#endif //SUNGEARENGINE_ILIGHTCOMPONENT_H
