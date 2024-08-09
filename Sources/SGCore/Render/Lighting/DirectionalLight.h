//
// Created by stuka on 03.02.2024.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHT_H
#define SUNGEARENGINE_DIRECTIONALLIGHT_H

#include "LightBase.h"

namespace SGCore
{
    sg_struct()
    struct DirectionalLight
    {
        DirectionalLight() noexcept;

        sg_member()
        LightBase m_base;
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHT_H
