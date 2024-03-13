//
// Created by ilya on 10.03.24.
//

#include "Octree.h"

#include "SGCore/Render/DebugDraw.h"

void SGCore::OctreeNode::draw(const SGCore::Ref<SGCore::DebugDraw>& debugDraw) noexcept
{
    if(debugDraw->m_mode == DebugDrawMode::NO_DEBUG) return;
    
    debugDraw->drawAABB(m_aabb.m_min, m_aabb.m_max,
                        (!m_overlappedEntities.empty()) ? m_collidesDebugColor : m_notCollidesDebugColor);

    if(m_isSubdivided)
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

SGCore::Octree::Octree()
{
    m_allNodes[m_root->m_aabb] = m_root;
}

SGCore::Octree::~Octree()
{
    m_allNodes.clear();
}

void SGCore::Octree::draw(const SGCore::Ref<SGCore::DebugDraw>& debugDraw) const noexcept
{
    for(const auto& p : m_allNodes)
    {
        if(debugDraw->m_mode == DebugDrawMode::NO_DEBUG) return;
        
        debugDraw->drawAABB(p.first.m_min, p.first.m_max, { 0.0, 0.0, 1.0, 1.0});
    }
}

bool SGCore::Octree::subdivide(Ref<OctreeNode> node) const noexcept
{
    glm::vec3 globalHalf = node->m_aabb.getGlobalCenter();
    glm::vec3 newNodeSize = (globalHalf - node->m_aabb.m_min);
    
    if(newNodeSize.x < m_nodeMinSize.x || newNodeSize.y < m_nodeMinSize.y || newNodeSize.z < m_nodeMinSize.z)
    {
        return false;
    }
    
    if(!node->m_isSubdivided)
    {
        for(auto& n : node->m_children)
        {
            n = MakeRef<OctreeNode>();
        }
        
        node->m_children[0]->m_aabb.m_min = node->m_aabb.m_min;
        node->m_children[0]->m_aabb.m_max = node->m_aabb.m_min + newNodeSize;
        
        auto i0 = m_allNodes.find(node->m_children[0]->m_aabb);
        if(i0 != m_allNodes.end())
        {
            node->m_children[0] = i0->second;
        }
        
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
        
        for(auto& c : node->m_children)
        {
            auto it = m_allNodes.find(c->m_aabb);
            if(it != m_allNodes.end())
            {
                c = it->second;
            }
        }
        
        node->m_isSubdivided = true;
    }

    return true;
}

SGCore::Ref<SGCore::OctreeNode> SGCore::Octree::subdivideWhileOverlap(const entt::entity& overlappingEntity,
                                                                      const AABB& aabb,
                                                                      Ref<OctreeNode> node,
                                                                      bool isParentOverlapped) const noexcept
{
    if(!node) return nullptr;
    
    bool overlapped = isParentOverlapped && aabb.isOverlappedBy(node->m_aabb); // node->m_aabb.isOverlappedBy(aabb);
    
    if(overlapped)
    {
        node->m_overlappedEntities.insert(overlappingEntity);
        subdivide(node);
    }
    else
    {
        node->m_overlappedEntities.erase(overlappingEntity);
    }
    
    if(node->m_isSubdivided)
    {
        for(const auto& child : node->m_children)
        {
            if(overlapped && aabb.isOverlappedBy(child->m_aabb))
            {
                node->m_overlappedEntities.erase(overlappingEntity);
                return subdivideWhileOverlap(overlappingEntity, aabb, child, overlapped);
            }
            else
            {
                child->m_overlappedEntities.erase(overlappingEntity);
            }
        }
    }
    
    return overlapped ? node : nullptr;
}

void SGCore::Octree::clearNodeChildren(SGCore::Ref<SGCore::OctreeNode> node) noexcept
{
    /*for(auto& child : node->m_children)
    {
        child = nullptr;
    }*/
    
    m_allNodes.clear();
    m_allNodes[m_root->m_aabb] = m_root;
    
    node->m_isSubdivided = false;
}

SGCore::Ref<SGCore::OctreeNode> SGCore::Octree::getOverlappingNode(const SGCore::AABB& aabb) noexcept
{
    using vec3l = glm::vec<3, size_t, glm::defaultp>;
    
    if(!aabb.isOverlappedBy(m_root->m_aabb))
    {
        return nullptr;
    }
    
    const glm::vec3 objectSize = aabb.getLocalCenter() * 2.0f;
    
    float objectMaxSideLength = std::min(std::min(objectSize.x, objectSize.y), objectSize.z);
    
    AABB objectLocalAABB;
    objectLocalAABB.m_max = { objectMaxSideLength, objectMaxSideLength, objectMaxSideLength };
    
    vec3l divRes = {
            std::ceil(objectLocalAABB.m_max.x / m_nodeMinSize.x),
            std::ceil(objectLocalAABB.m_max.y / m_nodeMinSize.y),
            std::ceil(objectLocalAABB.m_max.z / m_nodeMinSize.z)
    };
    
    glm::vec3 curSize = m_nodeMinSize;
    
    while(divRes.x > 0)
    {
        divRes.x -= std::ceil(curSize.x / m_nodeMinSize.x);
        curSize.x *= 2.0f;
    }
    
    while(divRes.y > 0)
    {
        divRes.y -= std::ceil(curSize.y / m_nodeMinSize.y);
        curSize.y *= 2.0f;
    }
    
    while(divRes.z > 0)
    {
        divRes.z -= std::ceil(curSize.z / m_nodeMinSize.z);
        curSize.z *= 2.0f;
    }
    
    glm::vec3 rootSize = m_root->m_aabb.getLocalCenter() * 2.0f;
    
    if(curSize.x > rootSize.x ||
       curSize.y > rootSize.y ||
       curSize.z > rootSize.z)
    {
        return nullptr;
    }
    // 300 / 10 = 30
    // 30 - 1 = 29 (sz: 10)
    // 29 - 2 = 27 (sz: 20)
    // 27 - 4 = 23 (sz: 40)
    // 23 - 8 = 15 (sz: 80)
    // 15 - 16 = -1 (sz: 160)
    // result sz: 320
    
    AABB localOverlappingAABB;
    localOverlappingAABB.m_max = curSize;
    
    vec3l objectMinPointTreeOverlappingNodeIdx;
    vec3l objectMaxPointTreeOverlappingNodeIdx;
    
    glm::vec3 minDif = aabb.m_min - m_root->m_aabb.m_min;
    glm::vec3 maxDif = aabb.m_max - m_root->m_aabb.m_min;
    
    objectMinPointTreeOverlappingNodeIdx.x = std::floor(minDif.x / localOverlappingAABB.m_max.x);
    objectMinPointTreeOverlappingNodeIdx.y = std::floor(minDif.y / localOverlappingAABB.m_max.y);
    objectMinPointTreeOverlappingNodeIdx.z = std::floor(minDif.z / localOverlappingAABB.m_max.z);
    
    objectMaxPointTreeOverlappingNodeIdx.x = std::floor(maxDif.x / localOverlappingAABB.m_max.x);
    objectMaxPointTreeOverlappingNodeIdx.y = std::floor(maxDif.y / localOverlappingAABB.m_max.y);
    objectMaxPointTreeOverlappingNodeIdx.z = std::floor(maxDif.z / localOverlappingAABB.m_max.z);
    
    // overlapping local aabb was found. now creating global overlapping aabb
    if(objectMinPointTreeOverlappingNodeIdx == objectMaxPointTreeOverlappingNodeIdx)
    {
        AABB globalOverlappingAABB;
        globalOverlappingAABB.m_min = m_root->m_aabb.m_min + glm::vec3 {
            localOverlappingAABB.m_max.x * (float) objectMinPointTreeOverlappingNodeIdx.x,
            localOverlappingAABB.m_max.y * (float) objectMinPointTreeOverlappingNodeIdx.y,
            localOverlappingAABB.m_max.z * (float) objectMinPointTreeOverlappingNodeIdx.z
        };
        
        globalOverlappingAABB.m_max = localOverlappingAABB.m_min + localOverlappingAABB.m_max;
        
        auto foundNodeIt = m_allNodes.find(globalOverlappingAABB);
        if(foundNodeIt == m_allNodes.end())
        {
            Ref<OctreeNode> newNode = MakeRef<OctreeNode>();
            newNode->m_aabb = globalOverlappingAABB;
            m_allNodes[globalOverlappingAABB] = newNode;
            return newNode;
        }
        else
        {
            return foundNodeIt->second;
        }
    }
    else // if max is in one aabb and min in other
    {
        // calculate the bigger AABB
        AABB nextAABB;
        nextAABB.m_min = m_root->m_aabb.m_min + glm::vec3 {
                localOverlappingAABB.m_max.x * (float) objectMinPointTreeOverlappingNodeIdx.x,
                localOverlappingAABB.m_max.y * (float) objectMinPointTreeOverlappingNodeIdx.y,
                localOverlappingAABB.m_max.z * (float) objectMinPointTreeOverlappingNodeIdx.z
        };
        nextAABB.m_max = nextAABB.m_min + localOverlappingAABB.m_max * 2.0f;
        
        return getOverlappingNode(nextAABB);
    }
}
