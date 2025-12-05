//
// Created by stuka on 04.12.2025.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct NavObstacle : ECS::Component<NavObstacle, const NavObstacle>
    {
    private:
        bool m_dummy = true;
    };
}
