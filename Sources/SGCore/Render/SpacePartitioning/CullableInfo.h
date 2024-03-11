//
// Created by ilya on 11.03.24.
//

#ifndef SUNGEARENGINE_CULLABLEINFO_H
#define SUNGEARENGINE_CULLABLEINFO_H

#include <functional>
#include <unordered_set>
#include <entt/entity/entity.hpp>

#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/Event.h"

namespace SGCore
{
    struct OctreeNode;
    
    struct CullableInfo
    {
        std::unordered_set<Ref<OctreeNode>> m_intersectingNodes;

        bool isVisible(const entt::entity& forReceiverEntity) const noexcept;

        Event<void(Ref<OctreeNode> node, const entt::entity& thisEntity)> m_onNodeLeave =
                MakeEvent<void(Ref<OctreeNode> node, const entt::entity& thisEntity)>();
    };
}

#endif //SUNGEARENGINE_CULLABLEINFO_H
