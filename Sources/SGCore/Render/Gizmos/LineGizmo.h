//
// Created by stuka on 02.02.2024.
//

#pragma once

#include "GizmoBase.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SGCORE_EXPORT LineGizmo : ECS::Component<LineGizmo, const LineGizmo>
    {
        LineGizmo() noexcept;

        GizmoBase m_base;

        glm::vec3 m_start { 0.0, 0.0, 0.0 };
        glm::vec3 m_end { 1.0, 1.0, 1.0 };

        glm::vec3 m_lastStart { 0.0, 0.0, 0.0 };
        glm::vec3 m_lastEnd { 0.0, 0.0, 0.0 };
    };
}
