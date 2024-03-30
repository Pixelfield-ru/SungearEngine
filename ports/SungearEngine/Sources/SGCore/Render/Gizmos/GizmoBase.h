//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_GIZMOBASE_H
#define SUNGEARENGINE_GIZMOBASE_H

#include "glm/glm.hpp"
#include "SGCore/Render/MeshBase.h"

namespace SGCore
{
    struct GizmoBase
    {
        GizmoBase() noexcept;

        glm::vec4 m_color { 1.0, 0.0, 0.0, 1.0 };
    // private:
        glm::bvec3 m_lastFollowEntityTRS = glm::vec3 { false };

        glm::vec4 m_lastColor = glm::vec4 { 1.0, 1.0, 1.0, 1.0 };

        MeshBase m_meshBase;
    };
}

#endif //SUNGEARENGINE_GIZMOBASE_H
