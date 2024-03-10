//
// Created by ilya on 10.03.24.
//

#include "OctreesSolver.h"

#include "SGCore/Scene/Scene.h"
#include "Octree.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "IgnoreOctrees.h"
#include "SGCore/Render/Camera3D.h"
#include "ObjectsCullingOctree.h"
#include "CullableInfo.h"

void SGCore::OctreesSolver::setScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_scene = scene;
    
    auto transformationsUpdater = scene->getSystem<TransformationsUpdater>();
    if(transformationsUpdater)
    {
        (*transformationsUpdater->m_transformChangedEvent) += m_transformChangedListener;
    }
}

void SGCore::OctreesSolver::parallelUpdate(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = m_scene.lock();
    
    if(!lockedScene) return;
    
    auto& registry = lockedScene->getECSRegistry();
    
    auto octreesView = registry.view<Ref<Octree>>();
    
    octreesView.each([this, &registry](const entt::entity& entity, Ref<Octree> octree) {
        // octree->clearNodesBranchEntities(octree->m_root);
        if(octree->m_root->m_isSubdivided)
        {
            for(size_t i = 0; i < m_lastSize; ++i)
            {
                auto p = m_changedTransforms[i];
                
                if(registry.all_of<IgnoreOctrees>(p.first))
                {
                    continue;
                }
                
                std::vector<Ref<OctreeNode>> collidedNodes;
                std::vector<Ref<OctreeNode>> notCollidedNodes;
                octree->subdivideWhileCollidesWithAABB(p.second->m_ownTransform.m_aabb, octree->m_root, collidedNodes, notCollidedNodes);
                for(const auto& node : notCollidedNodes)
                {
                    node->m_entities.erase(p.first);
                    auto* tmpCullableInfo = registry.try_get<Ref<CullableInfo>>(p.first);
                    if(tmpCullableInfo)
                    {
                        (*(*tmpCullableInfo)->m_onNodeLeave)(node, p.first);
                    }
                }
                for(const auto& node : collidedNodes)
                {
                    node->m_entities.insert(p.first);
                }
            }
        }
        else // check all transformations
        {
            auto transformationsView = registry.view<Ref<Transform>>();
            transformationsView.each([&octree, &registry](const entt::entity& transformEntity, Ref<Transform> transform) {
                if(!registry.all_of<IgnoreOctrees>(transformEntity))
                {
                    std::vector<Ref<OctreeNode>> collidedNodes;
                    std::vector<Ref<OctreeNode>> notCollidedNodes;
                    octree->subdivideWhileCollidesWithAABB(transform->m_ownTransform.m_aabb, octree->m_root,
                                                           collidedNodes, notCollidedNodes);
                    
                    for(const auto& node : notCollidedNodes)
                    {
                        node->m_entities.erase(transformEntity);
                        auto* tmpCullableInfo = registry.try_get<Ref<CullableInfo>>(transformEntity);
                        if(tmpCullableInfo)
                        {
                            (*(*tmpCullableInfo)->m_onNodeLeave)(node, transformEntity);
                        }
                    }
                    for(const auto& node : collidedNodes)
                    {
                        node->m_entities.insert(transformEntity);
                    }
                }
            });
        }
    });
    
    /*auto objectsCullingOctrees = lockedScene->getECSRegistry().view<Ref<Octree>, Ref<ObjectsCullingOctree>>();
    auto camerasView = lockedScene->getECSRegistry().view<Camera3D>();
    objectsCullingOctrees.each([&camerasView](Ref<Octree> octree, Ref<ObjectsCullingOctree>) {
    
    });*/
    
    std::lock_guard g(m_solveMutex);
    m_lastSize = 0;
    m_changedTransforms.clear();
}

void SGCore::OctreesSolver::onTransformChanged(const entt::entity& entity, const SGCore::Ref<const SGCore::Transform>& transform) noexcept
{
    std::lock_guard g(m_solveMutex);
    m_lastSize = m_changedTransforms.size();
    m_changedTransforms.push_back({ entity, transform });
}
