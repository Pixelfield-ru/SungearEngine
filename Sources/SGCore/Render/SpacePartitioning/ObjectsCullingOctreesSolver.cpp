//
// Created by ilya on 10.03.24.
//

#include "ObjectsCullingOctreesSolver.h"
#include "Octree.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Camera3D.h"
#include "ObjectsCullingOctree.h"
#include "CullableMesh.h"
#include "SGCore/Render/RenderingBase.h"

void SGCore::ObjectsCullingOctreesSolver::parallelUpdate(const double& dt, const double& fixedDt, Ref<OctreesSolver> parent) noexcept
{
    auto lockedScene = parent->getScene().lock();
    
    if(!lockedScene) return;
    
    auto& registry = lockedScene->getECSRegistry();

    auto objectsCullingOctrees = registry.view<Ref<Octree>, Ref<ObjectsCullingOctree>>();
    auto camerasView = registry.view<Ref<Camera3D>, Ref<RenderingBase>, Ref<Transform>>();
    objectsCullingOctrees.each([&camerasView, &registry, this](Ref<Octree> octree, Ref<ObjectsCullingOctree>&) {
        camerasView.each([&octree, &registry, this]
        (const entt::entity& cameraEntity, Ref<Camera3D> camera3D, Ref<RenderingBase> renderingBase, Ref<Transform> cameraTransform) {
            testNode(registry, cameraEntity, renderingBase->m_frustum, octree->m_root);
        });
    });
}

void SGCore::ObjectsCullingOctreesSolver::testNode
(entt::registry& registry, const entt::entity& cameraEntity, const Frustum& cameraFrustum, SGCore::Ref<SGCore::OctreeNode> node) noexcept
{
    if(cameraFrustum.testAABB(node->m_aabb.m_min, node->m_aabb.m_max))
    {
        node->m_visibleReceivers.insert(cameraEntity);
        /*for(const auto& e : node->m_entities)
        {
            auto* tmpCullableMesh = registry.try_get<Ref<CullableMesh>>(e);
            Ref<CullableMesh> cullableMesh = (tmpCullableMesh ? *tmpCullableMesh : nullptr);
            
            if(cullableMesh)
            {
                ++cullableMesh->m_visibleNodesCount;
                cullableMesh->m_visibleCameras.insert(cameraEntity);
            }
        }*/
    }
    else
    {
        node->m_visibleReceivers.erase(cameraEntity);
        /*for(const auto& e : node->m_entities)
        {
            auto* tmpCullableMesh = registry.try_get<Ref<CullableMesh>>(e);
            Ref<CullableMesh> cullableMesh = (tmpCullableMesh ? *tmpCullableMesh : nullptr);

            if(cullableMesh)
            {
                --cullableMesh->m_visibleNodesCount;
                if(cullableMesh->m_visibleNodesCount <= 0)
                {
                    cullableMesh->m_visibleCameras.erase(cameraEntity);
                }
            }
        }*/
    }
    
    if(node->isSubdivided())
    {
        for(const auto& child : node->m_children)
        {
            testNode(registry, cameraEntity, cameraFrustum, child);
        }
    }
}
