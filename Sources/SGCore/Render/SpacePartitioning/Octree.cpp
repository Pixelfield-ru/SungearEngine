//
// Created by ilya on 10.03.24.
//

#include "Octree.h"

#include "SGCore/Render/DebugDraw.h"

void SGCore::OctreeNode::draw(const SGCore::Ref<SGCore::DebugDraw>& debugDraw) noexcept
{
    if(debugDraw->m_mode == DebugDrawMode::NO_DEBUG) return;
    
    debugDraw->drawAABB(m_aabb.m_min, m_aabb.m_max,
                        /*(!m_entities.empty()) ? m_collidesDebugColor :*/ m_notCollidesDebugColor);

    if(m_isSubdivided.load())
    {
        for(std::uint8_t i = 0; i < 8; ++i)
        {
            m_children[i]->draw(debugDraw);
        }
    }
}

bool SGCore::OctreeNode::isSubdivided() const noexcept
{
    return m_isSubdivided;
}

bool SGCore::Octree::subdivide(Ref<OctreeNode> node) const noexcept
{
    glm::vec3 globalHalf = node->m_aabb.getGlobalCenter();
    glm::vec3 newNodeSize = (globalHalf - node->m_aabb.m_min);
    
    if(newNodeSize.x < m_nodeMinSize.x || newNodeSize.y < m_nodeMinSize.y || newNodeSize.z < m_nodeMinSize.z)
    {
        return false;
    }
    
    if(!node->m_isSubdivided.load())
    {
        for(auto& n : node->m_children)
        {
            n = MakeRef<OctreeNode>();
        }
        
        node->m_children[0]->m_aabb.m_min = node->m_aabb.m_min;
        node->m_children[0]->m_aabb.m_max = node->m_aabb.m_min + newNodeSize;
        
        node->m_children[1]->m_aabb.m_min = { node->m_aabb.m_min.x + newNodeSize.x, node->m_aabb.m_min.y,
                                            node->m_aabb.m_min.z };
        node->m_children[1]->m_aabb.m_max = { node->m_aabb.m_min.x + newNodeSize.x * 2.0f,
                                            node->m_aabb.m_min.y + newNodeSize.y, node->m_aabb.m_min.z + newNodeSize.z };
        
        node->m_children[2]->m_aabb.m_min = { node->m_aabb.m_min.x, node->m_aabb.m_min.y + newNodeSize.y,
                                            node->m_aabb.m_min.z };
        node->m_children[2]->m_aabb.m_max = { node->m_aabb.m_min.x + newNodeSize.x,
                                            node->m_aabb.m_min.y + newNodeSize.y * 2.0f,
                                            node->m_aabb.m_min.z + newNodeSize.z };
        
        node->m_children[3]->m_aabb.m_min = { node->m_aabb.m_min.x + newNodeSize.x, node->m_aabb.m_min.y + newNodeSize.y,
                                            node->m_aabb.m_min.z };
        node->m_children[3]->m_aabb.m_max = { node->m_aabb.m_min.x + newNodeSize.x * 2.0f,
                                            node->m_aabb.m_min.y + newNodeSize.y * 2.0f,
                                            node->m_aabb.m_min.z + newNodeSize.z };
        
        node->m_children[4]->m_aabb.m_min = { node->m_aabb.m_min.x, node->m_aabb.m_min.y,
                                            node->m_aabb.m_min.z + newNodeSize.z };
        node->m_children[4]->m_aabb.m_max = { node->m_aabb.m_min.x + newNodeSize.x, node->m_aabb.m_min.y + newNodeSize.y,
                                            node->m_aabb.m_min.z + newNodeSize.z * 2.0f };
        
        node->m_children[5]->m_aabb.m_min = { node->m_aabb.m_min.x + newNodeSize.x, node->m_aabb.m_min.y,
                                            node->m_aabb.m_min.z + newNodeSize.z };
        node->m_children[5]->m_aabb.m_max = { node->m_aabb.m_min.x + newNodeSize.x * 2.0f,
                                            node->m_aabb.m_min.y + newNodeSize.y,
                                            node->m_aabb.m_min.z + newNodeSize.z * 2.0f };
        
        node->m_children[6]->m_aabb.m_min = { node->m_aabb.m_min.x, node->m_aabb.m_min.y + newNodeSize.y,
                                            node->m_aabb.m_min.z + newNodeSize.z };
        node->m_children[6]->m_aabb.m_max = { node->m_aabb.m_min.x + newNodeSize.x,
                                            node->m_aabb.m_min.y + newNodeSize.y * 2.0f,
                                            node->m_aabb.m_min.z + newNodeSize.z * 2.0f };
        
        node->m_children[7]->m_aabb.m_min = { node->m_aabb.m_min + newNodeSize };
        node->m_children[7]->m_aabb.m_max = { node->m_aabb.m_min + newNodeSize * 2.0f };
        
        node->m_isSubdivided.store(true);
    }
    
    return true;
}

void SGCore::Octree::subdivideWhileCollidesWithAABB(const SGCore::AABB& aabb, Ref<OctreeNode> node, std::vector<Ref<OctreeNode>>& collidedNodes, std::vector<Ref<OctreeNode>>& notCollidedNodes) const noexcept
{
    if(!node) return;
    
    if(node->m_aabb.isCollidesWith(aabb))
    {
        collidedNodes.push_back(node);
        
        if(subdivide(node))
        {
            if(node->m_isSubdivided.load())
            {
                for(std::uint8_t i = 0; i < 8; ++i)
                {
                    subdivideWhileCollidesWithAABB(aabb, node->m_children[i], collidedNodes, notCollidedNodes);
                }
            }
        }
    }
    else
    {
        notCollidedNodes.push_back(node);
    }
}

void SGCore::Octree::getAllNodesCollideWith(const SGCore::AABB& aabb, SGCore::Ref<SGCore::OctreeNode> node, std::vector<Ref<OctreeNode>>& output) const noexcept
{
    if(!node) return;
    
    if(node->m_aabb.isCollidesWith(aabb))
    {
        output.push_back(node);
    }
    
    if(node->m_isSubdivided.load())
    {
        for(auto child : node->m_children)
        {
            getAllNodesCollideWith(aabb, child, output);
        }
    }
}

void SGCore::Octree::clearNodesBranchEntities(SGCore::Ref<SGCore::OctreeNode> node) const noexcept
{
    if(!node) return;
    
    // node->m_entities.clear();
    
    if(node->m_isSubdivided.load())
    {
        for(const auto& child : node->m_children)
        {
            clearNodesBranchEntities(child);
        }
    }
}

void SGCore::Octree::clearNodeChildren(SGCore::Ref<SGCore::OctreeNode> node) noexcept
{
    for(auto& child : node->m_children)
    {
        child = nullptr;
    }
    
    node->m_isSubdivided.store(false);
}
