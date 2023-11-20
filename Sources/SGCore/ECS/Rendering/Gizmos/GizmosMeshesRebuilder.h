#ifndef SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
#define SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    class GizmosMeshesRebuilder : public ISystem
    {
        SG_DECLARE_SINGLETON(GizmosMeshesRebuilder)

    public:
        void fixedUpdate(const Ref<Scene>& scene) final;
    };
}

#endif //SUNGEARENGINE_PRIMITIVESUPDATERSYSTEM_H
