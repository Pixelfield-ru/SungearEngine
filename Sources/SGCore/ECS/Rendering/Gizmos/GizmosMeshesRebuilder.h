#ifndef SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
#define SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct GizmosMeshesRebuilder : public ISystem
    {
        void fixedUpdate(const Ref<Scene>& scene) final;

        SG_CUSTOM_CTOR_SINGLETON(GizmosMeshesRebuilder)
        SG_COPY_SINGLETON(GizmosMeshesRebuilder)
        SG_MOVE_SINGLETON(GizmosMeshesRebuilder)
    };
}

#endif //SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
