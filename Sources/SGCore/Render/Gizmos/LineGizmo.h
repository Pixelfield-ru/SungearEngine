//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_LINEGIZMO_H
#define SUNGEARENGINE_LINEGIZMO_H

#include "GizmoBase.h"

namespace SGCore
{
    sg_struct()
    struct LineGizmo
    {
        LineGizmo() noexcept;

        sg_member()
        GizmoBase m_base;

        sg_member()
        glm::vec3 m_start { 0.0, 0.0, 0.0 };
        sg_member()
        glm::vec3 m_end { 1.0, 1.0, 1.0 };

        glm::vec3 m_lastStart { 0.0, 0.0, 0.0 };
        glm::vec3 m_lastEnd { 0.0, 0.0, 0.0 };
    };
}

#endif //SUNGEARENGINE_LINEGIZMO_H
