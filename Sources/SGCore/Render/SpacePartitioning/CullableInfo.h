//
// Created by ilya on 11.03.24.
//

#ifndef SUNGEARENGINE_CULLABLEINFO_H
#define SUNGEARENGINE_CULLABLEINFO_H

#include <functional>
#include <entt/entity/entity.hpp>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct OctreeNode;
    
    struct CullableInfo
    {
        Event<void(Ref<OctreeNode> node, const entt::entity& thisEntity)> m_onNodeLeave =
                MakeEvent<void(Ref<OctreeNode> node, const entt::entity& thisEntity)>();
    };
}

#endif //SUNGEARENGINE_CULLABLEINFO_H
