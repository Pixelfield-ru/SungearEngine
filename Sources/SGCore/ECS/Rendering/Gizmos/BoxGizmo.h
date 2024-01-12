#ifndef SUNGEARENGINE_BOXCOMPONENT_H
#define SUNGEARENGINE_BOXCOMPONENT_H

#include "IComplexGizmo.h"

namespace SGCore
{
    struct BoxGizmo : public IComplexGizmo
    {
        friend class GizmosMeshesRebuilder;

        glm::vec3 m_size { 5.0, 5.0, 5.0 };

        BoxGizmo(const Ref<IRenderPipeline>& pipeline) noexcept;

    private:
        glm::vec3 m_lastSize { 5.0, 5.0, 5.0 };

        /**
         * Creates vertices, indexes, and mesh of a cube.
         */
        void build();
    };
}

#endif //SUNGEARENGINE_BOXCOMPONENT_H
