//
// Created by stuka on 02.02.2024.
//

#pragma once

#include "GizmoBase.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SGCORE_EXPORT SphereGizmo : ECS::Component<SphereGizmo, const SphereGizmo>
    {
        GizmoBase m_base;

        float m_radius = 5.0;
        float m_angleIncrement = 18.0;

        float m_lastRadius = 0.0;
        float m_lastAngleIncrement = 0.0;
    };
}
