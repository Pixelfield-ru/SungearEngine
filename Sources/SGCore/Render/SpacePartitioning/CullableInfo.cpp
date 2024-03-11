//
// Created by Ilya on 11.03.2024.
//
#include "CullableInfo.h"
#include "Octree.h"

bool SGCore::CullableInfo::isVisible(const entt::entity& forReceiverEntity) const noexcept
{
    for (auto node : m_intersectingNodes)
    {
        auto lockedNode = node.lock();

        if(!lockedNode) continue;

        if(lockedNode->m_visibleReceivers.contains(forReceiverEntity))
        {
            return true;
        }
    }

    return false;
}

