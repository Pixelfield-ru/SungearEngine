#ifndef SUNGEARENGINE_BOXCOMPONENT_H
#define SUNGEARENGINE_BOXCOMPONENT_H

#include "IComplexGizmo.h"

namespace Core::ECS
{
    struct BoxGizmo : public IComplexGizmo
    {
        friend class GizmosMeshesRebuilder;

        glm::vec3 m_size { 5.0, 5.0, 5.0 };

        BoxGizmo();

        void setVertexPosition(const size_t& vertexIdx, const float& x, const float& y, const float& z) noexcept final;

    private:
        glm::vec3 m_lastSize { 5.0, 5.0, 5.0 };

        /**
         * Creates vertices, indexes, and mesh of a cube.
         */
        void build();
    };
}

#endif //SUNGEARENGINE_BOXCOMPONENT_H
