#ifndef SUNGEARENGINE_BOXCOMPONENT_H
#define SUNGEARENGINE_BOXCOMPONENT_H

#include <SGCore/pch.h>

#include "SGCore/Transformations/TransformBase.h"
#include "SGCore/Render/MeshBase.h"
#include "GizmoBase.h"

namespace SGCore
{
    sg_struct()
    struct BoxGizmo
    {
        sg_member()
        GizmoBase m_base;

        sg_member()
        glm::vec3 m_size { 5.0, 5.0, 5.0 };

        sg_member()
        glm::vec3 m_lastSize { 0.0, 0.0, 0.0 };
    };
}

#endif //SUNGEARENGINE_BOXCOMPONENT_H
