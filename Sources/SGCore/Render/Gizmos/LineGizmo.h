//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_LINEGIZMO_H
#define SUNGEARENGINE_LINEGIZMO_H

#include "GizmoBase.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct LineGizmo : ECS::Component<LineGizmo, const LineGizmo>
    {
        LineGizmo() noexcept;

        GizmoBase m_base;

        glm::vec3 m_start { 0.0, 0.0, 0.0 };
        glm::vec3 m_end { 1.0, 1.0, 1.0 };

        glm::vec3 m_lastStart { 0.0, 0.0, 0.0 };
        glm::vec3 m_lastEnd { 0.0, 0.0, 0.0 };
    };
}

#endif //SUNGEARENGINE_LINEGIZMO_H
