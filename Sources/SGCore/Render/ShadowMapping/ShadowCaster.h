//
// Created by stuka on 08.07.2025.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    /// Tag to indicate object that can cast shadows.
    struct SGCORE_EXPORT ShadowCaster : ECS::Component<ShadowCaster, const ShadowCaster>
    {
    private:
        bool m_dummy = false;
    };
}
