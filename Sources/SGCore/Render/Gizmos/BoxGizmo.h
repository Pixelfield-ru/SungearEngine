#ifndef SUNGEARENGINE_BOXCOMPONENT_H
#define SUNGEARENGINE_BOXCOMPONENT_H

#include <SGCore/pch.h>

#include "SGCore/Transformations/TransformBase.h"
#include "SGCore/Render/MeshBase.h"
#include "GizmoBase.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct BoxGizmo : ECS::Component<BoxGizmo, const BoxGizmo>
    {
        GizmoBase m_base;

        glm::vec3 m_size { 5.0, 5.0, 5.0 };

        glm::vec3 m_lastSize { 0.0, 0.0, 0.0 };
    };
}

#endif //SUNGEARENGINE_BOXCOMPONENT_H
