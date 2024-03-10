//
// Created by ilya on 10.03.24.
//

#include "OctreesSolver.h"

#include "SGCore/Scene/Scene.h"
#include "Octree.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Transformations/TransformationsUpdater.h"
#include "IgnoreOctrees.h"

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
    
    auto octreesView = lockedScene->getECSRegistry().view<Ref<Octree>>();
    
    octreesView.each([this, lockedScene](const entt::entity& entity, Ref<Octree> octree) {
        octree->clearNodesBranchEntities(octree->m_root);
        if(octree->m_root->m_isSubdivided)
        {
            for(size_t i = 0; i < m_lastSize; ++i)
            {
                auto p = m_changedTransforms[i];
                
                if(lockedScene->getECSRegistry().all_of<IgnoreOctrees>(p.first))
                {
                    continue;
                }
                
                std::vector<Ref<OctreeNode>> collidedNodes;
                octree->subdivideWhileCollidesWithAABB(p.second->m_ownTransform.m_aabb, octree->m_root, collidedNodes);
                for(const auto& node : collidedNodes)
                {
                    node->m_entities.push_back(entity);
                }
            }
        }
        else // check all transformations
        {
            auto transformationsView = lockedScene->getECSRegistry().view<Ref<Transform>>();
            transformationsView.each([&octree, &lockedScene](const entt::entity& transformEntity, Ref<Transform> transform) {
                if(!lockedScene->getECSRegistry().all_of<IgnoreOctrees>(transformEntity))
                {
                    std::vector<Ref<OctreeNode>> collidedNodes;
                    octree->subdivideWhileCollidesWithAABB(transform->m_ownTransform.m_aabb, octree->m_root,
                                                           collidedNodes);
                    for(const auto& node : collidedNodes)
                    {
                        node->m_entities.push_back(transformEntity);
                    }
                }
            });
        }
    });
    
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
