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
#include "OctreeCullable.h"

SGCore::OctreesSolver::OctreesSolver()
{
    m_thread->start();
    // startThread();
}

void SGCore::OctreesSolver::onAddToScene(const Scene* scene)
{
    if(!scene) return;
    
    auto transformationsUpdater = scene->getSystem<TransformationsUpdater>();
    if(transformationsUpdater)
    {
        transformationsUpdater->onTransformChanged += m_transformChangedListener;
    }
}

void SGCore::OctreesSolver::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = getScene();
    
    if(!lockedScene) return;
    
    auto registry = lockedScene->getECSRegistry();
    
    auto octreesView = registry->view<Octree>();
    
    octreesView.each([this, &registry](const ECS::entity_t& entity, Octree::reg_t octree) {
        if(octree->m_root->m_isSubdivided)
        {
            for(const auto& p : m_changedTransforms)
            {
                if(registry->allOf<IgnoreOctrees>(p.first))
                {
                    continue;
                }
                
                auto* tmpCullable = registry->tryGet<OctreeCullable>(p.first);
                auto cullable = (tmpCullable ? *tmpCullable : nullptr);
                
                if(!cullable) continue;

                const auto foundNode =  octree->subdivideWhileOverlap(p.first, p.second->m_finalTransform.m_aabb, octree->m_root);
                if(foundNode)
                {
                    cullable->m_parentNodes[entity] = foundNode;
                }
            }
            
            m_changedTransforms.clear();
        }
        else // check all transformations
        {
            auto transformationsView = registry->view<Transform>();
            transformationsView.each([&octree, &registry, &entity](const ECS::entity_t& transformEntity, Transform::reg_t transform) {
                if(registry->allOf<IgnoreOctrees>(transformEntity)) return;

                auto* tmpCullable = registry->tryGet<OctreeCullable>(transformEntity);
                auto cullable = (tmpCullable ? *tmpCullable : nullptr);

                if(!cullable) return;

                const auto foundNode =  octree->subdivideWhileOverlap(transformEntity, transform->m_finalTransform.m_aabb, octree->m_root);
                if(foundNode)
                {
                    cullable->m_parentNodes[entity] = foundNode;
                }
            });
            
            m_changedTransforms.clear();
        }
    });
    
    IParallelSystem::fixedUpdate(dt, fixedDt);
}

void SGCore::OctreesSolver::onTransformChanged(const ECS::entity_t& entity, const SGCore::Transform::const_reg_t& transform) noexcept
{
    std::lock_guard lock(m_changedTransformsMutex);
    m_changedTransforms.emplace_back(entity, transform);
}
