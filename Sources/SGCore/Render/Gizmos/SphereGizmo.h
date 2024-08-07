//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_SPHEREGIZMO_H
#define SUNGEARENGINE_SPHEREGIZMO_H

#include "GizmoBase.h"

namespace SGCore
{
    sg_member()
    struct SphereGizmo
    {
        sg_member()
        GizmoBase m_base;

        sg_member()
        float m_radius = 5.0;
        sg_member()
        float m_angleIncrement = 18.0;

        float m_lastRadius = 0.0;
        float m_lastAngleIncrement = 0.0;
    };
}

#endif //SUNGEARENGINE_SPHEREGIZMO_H
