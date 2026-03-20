//
// Created by stuka on 28.02.2026.
//

#pragma once

#include "SGCore/Render/PostProcess/PostProcessEffect.h"

namespace SGCore
{
    struct SGCORE_EXPORT NoPPEffect : PostProcessEffect
    {
        NoPPEffect() noexcept;
    };
}
