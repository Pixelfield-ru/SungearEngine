//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_OCTREE_H
#define SUNGEARENGINE_OCTREE_H

#include <SGCore/pch.h>

#include "SGCore/ECS/Component.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Math/AABB.h"

namespace SGCore
{
    class DebugDraw;
    
    struct OctreeNode
    {
        friend class OctreesSolver;
        friend struct Octree;
        
        AABB<> m_aabb;
        std::unordered_set<ECS::entity_t> m_overlappedEntities;
        std::unordered_set<ECS::entity_t> m_visibleReceivers;
        std::array<Ref<OctreeNode>, 8> m_children;

        glm::vec4 m_notCollidesDebugColor { 0, 0, 1, 1 };
        glm::vec4 m_collidesDebugColor { 1, 0, 0, 1 };

        void draw(const Ref<DebugDraw>& debugDraw) noexcept;

        [[nodiscard]] bool isSubdivided() const noexcept;

    private:
        bool m_isSubdivided = false;
        // std::atomic<bool> m_isSubdivided = false;
    };
    
    struct Octree : ECS::Component<Ref<Octree>, Ref<const Octree>>
    {
        glm::vec3 m_nodeMinSize { 10 };
        
        [[nodiscard]] bool subdivide(Ref<OctreeNode> node) const noexcept;
        
        SGCore::Ref<SGCore::OctreeNode> subdivideWhileOverlap(const ECS::entity_t& overlappingEntity,
                                                              const AABB<>& aabb,
                                                              const Ref<OctreeNode>& node,
                                                              bool isParentOverlapped = true) const noexcept;
        void clearNodeChildren(Ref<OctreeNode> node) noexcept;
        
        Ref<OctreeNode> m_root = MakeRef<OctreeNode>();
        
        std::unordered_set<Ref<OctreeNode>> m_notEmptyNodes;
    };
}

#endif //SUNGEARENGINE_OCTREE_H
