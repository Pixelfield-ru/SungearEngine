//
// Created by stuka on 04.12.2025.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore::Navigation
{
    struct NavObstacle : ECS::Component<NavObstacle, const NavObstacle>
    {
    private:
        bool m_dummy = true;
    };
}
