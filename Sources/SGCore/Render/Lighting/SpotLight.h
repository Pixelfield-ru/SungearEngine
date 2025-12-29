//
// Created by stuka on 03.02.2024.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHT_H
#define SUNGEARENGINE_DIRECTIONALLIGHT_H

#include "LightBase.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SpotLight : ECS::Component<SpotLight, const SpotLight>
    {
        SpotLight() noexcept;

        LightBase m_base;

        glm::vec3 m_basicDirection = glm::vec3(0.0f, 0.0f, -1.0f);

        void setInnerCutoffAngle(float angle) noexcept;
        float getInnerCutoffAngle() const noexcept;

        void setOuterCutoffAngle(float angle) noexcept;
        float getOuterCutoffAngle() const noexcept;

        float getInnerCutoff() const noexcept;
        float getOuterCutoff() const noexcept;

    private:
        float m_innerCutoffAngle {};
        float m_outerCutoffAngle {};

        float m_innerCutoff {};
        float m_outerCutoff {};
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHT_H
