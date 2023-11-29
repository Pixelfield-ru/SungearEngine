#ifndef SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
#define SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct GizmosMeshesRebuilder : public ISystem
    {
        GizmosMeshesRebuilder();
        SG_COPY_CTOR(GizmosMeshesRebuilder);
        SG_MOVE_CTOR(GizmosMeshesRebuilder);

        void fixedUpdate(const Ref<Scene>& scene) final;
    };
}

#endif //SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
