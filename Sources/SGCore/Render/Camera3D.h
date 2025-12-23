//
// Created by ilya on 25.02.24.
//

#ifndef SUNGEARENGINE_CAMERA3D_H
#define SUNGEARENGINE_CAMERA3D_H

#include <unordered_set>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Scene/EntityRef.h"

namespace SGCore
{
    struct Camera3D : ECS::Component<Ref<Camera3D>, Ref<const Camera3D>>
    {
        std::unordered_set<ECS::entity_t> m_pickedEntities;
        // todo: make saving
        std::vector<EntityRef> m_cullingOctrees;

        bool isEntityVisibleForCamera(const Ref<ECS::registry_t>& registry, ECS::entity_t thisCameraEntity, ECS::entity_t entity) const noexcept;

    private:
        volatile int m_dummy = 0;
    };
}

#endif //SUNGEARENGINE_CAMERA3D_H
