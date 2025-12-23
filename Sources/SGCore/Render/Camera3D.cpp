//
// Created by stuka on 17.12.2024.
//
#include "Camera3D.h"

#include "SGCore/ECS/Registry.h"
#include "SpacePartitioning/IgnoreOctrees.h"
#include "SpacePartitioning/Octree.h"
#include "SpacePartitioning/OctreeCullable.h"

bool SGCore::Camera3D::isEntityVisibleForCamera(const Ref<ECS::registry_t>& registry, ECS::entity_t thisCameraEntity, ECS::entity_t entity) const noexcept
{
    auto* tmpCullable = registry->tryGet<OctreeCullable>(entity);
    const Ref<OctreeCullable> cullable = (tmpCullable ? *tmpCullable : nullptr);

    if(cullable && !registry->allOf<IgnoreOctrees>(entity))
    {
        for(const auto& octree : m_cullingOctrees)
        {
            const auto it = cullable->m_parentNodes.find(*octree);
            if(it == cullable->m_parentNodes.end()) return true;

            const auto lockedNode = it->second.lock();

            if(lockedNode->m_visibleReceivers.find(thisCameraEntity) == lockedNode->m_visibleReceivers.end()) continue;

            return true;
        }

        return false;
    }
    else
    {
        // todo: make optional culling by AABB of object
    }

    return true;
}
