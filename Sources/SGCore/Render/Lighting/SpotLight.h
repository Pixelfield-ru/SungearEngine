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

        float m_cutoffRadius = 20;
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHT_H
