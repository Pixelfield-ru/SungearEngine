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
#include "OctreeCullableInfo.h"

SGCore::OctreesSolver::OctreesSolver()
{
    startThread();
}

void SGCore::OctreesSolver::setScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_scene = scene;
    
    auto transformationsUpdater = scene->getSystem<TransformationsUpdater>();
    if(transformationsUpdater)
    {
        (*transformationsUpdater->onTransformChanged) += m_transformChangedListener;
    }
}

void SGCore::OctreesSolver::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{
    auto lockedScene = m_scene.lock();
    
    if(!lockedScene) return;
    
    auto& registry = lockedScene->getECSRegistry();
    
    auto octreesView = registry.view<Ref<Octree>>();
    
    octreesView.each([this, &registry](const entity_t& entity, Ref<Octree> octree) {
        if(octree->m_root->m_isSubdivided)
        {
            for(const auto& p : m_changedTransforms)
            {
                if(registry.all_of<IgnoreOctrees>(p.first))
                {
                    continue;
                }
                
                auto* tmpCullableInfo = registry.try_get<Ref<OctreeCullableInfo>>(p.first);
                auto cullableInfo = (tmpCullableInfo ? *tmpCullableInfo : nullptr);
                
                if(cullableInfo)
                {
                    auto lockedParentNode = cullableInfo->m_parentNode.lock();
                    if(lockedParentNode)
                    {
                        lockedParentNode->m_overlappedEntities.erase(p.first);
                        if(lockedParentNode->m_overlappedEntities.empty())
                        {
                            octree->m_notEmptyNodes.erase(lockedParentNode);
                        }
                    }
                    auto foundNode =  octree->subdivideWhileOverlap(p.first, p.second->m_ownTransform.m_aabb, octree->m_root);
                    cullableInfo->m_parentNode = foundNode;
                    if(foundNode)
                    {
                        foundNode->m_overlappedEntities.insert(p.first);
                        octree->m_notEmptyNodes.insert(foundNode);
                    }
                }
            }
            
            m_changedTransforms.clear();
        }
        else // check all transformations
        {
            auto transformationsView = registry.view<Ref<Transform>>();
            transformationsView.each([&octree, &registry](const entity_t& transformEntity, Ref<Transform> transform) {
                if(!registry.all_of<IgnoreOctrees>(transformEntity))
                {
                    auto* tmpCullableInfo = registry.try_get<Ref<OctreeCullableInfo>>(transformEntity);
                    auto cullableInfo = (tmpCullableInfo ? *tmpCullableInfo : nullptr);
                    
                    if(cullableInfo)
                    {
                        auto lockedParentNode = cullableInfo->m_parentNode.lock();
                        if(lockedParentNode)
                        {
                            lockedParentNode->m_overlappedEntities.erase(transformEntity);
                            if(lockedParentNode->m_overlappedEntities.empty())
                            {
                                octree->m_notEmptyNodes.erase(lockedParentNode);
                            }
                        }
                        auto foundNode =  octree->subdivideWhileOverlap(transformEntity, transform->m_ownTransform.m_aabb, octree->m_root);
                        cullableInfo->m_parentNode = foundNode;
                        if(foundNode)
                        {
                            foundNode->m_overlappedEntities.insert(transformEntity);
                            octree->m_notEmptyNodes.insert(foundNode);
                        }
                    }
                }
            });
            
            m_changedTransforms.clear();
        }
    });
    
    IParallelSystem::fixedUpdate(dt, fixedDt);
}

void SGCore::OctreesSolver::onTransformChanged(const entity_t& entity, const SGCore::Ref<const SGCore::Transform>& transform) noexcept
{
    m_changedTransforms.emplace_back(entity, transform);
}
