//
// Created by stuka on 03.02.2024.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHT_H
#define SUNGEARENGINE_DIRECTIONALLIGHT_H

#include "LightBase.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct DirectionalLight : ECS::Component<DirectionalLight, const DirectionalLight>
    {
        DirectionalLight() noexcept;

        LightBase m_base;
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHT_H
