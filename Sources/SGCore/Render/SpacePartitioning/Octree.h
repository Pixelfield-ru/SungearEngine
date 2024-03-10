//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_OCTREE_H
#define SUNGEARENGINE_OCTREE_H

#include <entt/entity/entity.hpp>
#include <vector>
#include <atomic>
#include <unordered_set>

#include "SGCore/Main/CoreGlobals.h"
#include "glm/vec3.hpp"
#include "SGUtils/Math/AABB.h"

namespace SGCore
{
    class DebugDraw;
    
    struct OctreeNode
    {
        friend class OctreesSolver;
        friend struct Octree;
        
        AABB m_aabb;
        std::unordered_set<entt::entity> m_entities;
        std::array<Ref<OctreeNode>, 8> m_children;
        
        glm::vec4 m_notCollidesDebugColor { 0, 0, 1, 1 };
        glm::vec4 m_collidesDebugColor { 1, 0, 0, 1 };
        
        void draw(const Ref<DebugDraw>& debugDraw) noexcept;
        
        [[nodiscard]] bool isSubdivided() const noexcept;
        
    private:
        std::atomic<bool> m_isSubdivided = false;
    };
    
    struct Octree
    {
        glm::vec3 m_nodeMinSize { 10 };
        
        [[nodiscard]] bool subdivide(Ref<OctreeNode> node) const noexcept;
        void subdivideWhileCollidesWithAABB(const AABB& aabb, Ref<OctreeNode> node, std::vector<Ref<OctreeNode>>& collidedNodes, std::vector<Ref<OctreeNode>>& notCollidedNodes) const noexcept;
        void getAllNodesCollideWith(const AABB& aabb, Ref<OctreeNode> node, std::vector<Ref<OctreeNode>>& output) const noexcept;
        void clearNodesBranchEntities(Ref<OctreeNode> node) const noexcept;
        void clearNodeChildren(Ref<OctreeNode> node) noexcept;
        
        Ref<OctreeNode> m_root = MakeRef<OctreeNode>();
    };
}

#endif //SUNGEARENGINE_OCTREE_H
