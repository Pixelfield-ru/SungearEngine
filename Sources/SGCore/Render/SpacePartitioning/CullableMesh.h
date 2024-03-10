//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_CULLABLEMESH_H
#define SUNGEARENGINE_CULLABLEMESH_H

#include <entt/entity/entity.hpp>
#include <unordered_set>
#include <functional>
#include <set>

#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/EventListener.h"

namespace SGCore
{
    struct OctreeNode;
    
    struct CullableMesh
    {
        CullableMesh();
        // CullableMesh(const CullableMesh&) = default;
        CullableMesh(CullableMesh&&) = default;
        
        EventListener<void(Ref<OctreeNode> node, const entt::entity& thisEntity)> m_nodeLeaveListener;
        //std::function<void(Ref<OctreeNode> node, const entt::entity& thisEntity)> m_nodeLeaveCallback;
        
        std::set<entt::entity> m_visibleCameras;
    };
}

#endif //SUNGEARENGINE_CULLABLEMESH_H
