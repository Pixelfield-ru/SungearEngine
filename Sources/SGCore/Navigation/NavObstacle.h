//
// Created by stuka on 04.12.2025.
//

#pragma once

#include "SGCore/ECS/Component.h"
#include <sgcore_export.h>

namespace SGCore::Navigation
{
    struct SGCORE_EXPORT NavObstacle : ECS::Component<NavObstacle, const NavObstacle>
    {
    private:
        bool m_dummy = true;
    };
}
