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
#include "OctreeCullableInfo.h"

void SGCore::ObjectsCullingOctreesSolver::fixedUpdate(const double& dt, const double& fixedDt, Ref<OctreesSolver> parent) noexcept
{
    auto lockedScene = parent->getScene().lock();
    
    if(!lockedScene) return;
    
    auto& registry = lockedScene->getECSRegistry();

    auto objectsCullingOctrees = registry->view<Ref<Octree>, Ref<ObjectsCullingOctree>>();
    auto camerasView = registry->view<Ref<Camera3D>, Ref<RenderingBase>, Ref<Transform>>();
    objectsCullingOctrees.each([&camerasView, &registry, this](Ref<Octree> octree, Ref<ObjectsCullingOctree>&) {
        camerasView.each([&octree, &registry, this]
        (const entity_t& cameraEntity, Ref<Camera3D> camera3D, Ref<RenderingBase> renderingBase, Ref<Transform> cameraTransform) {
            for(const auto& n : octree->m_notEmptyNodes)
            {
                testNode(cameraEntity, renderingBase->m_frustum, n);
            }
        });
    });
}

void SGCore::ObjectsCullingOctreesSolver::testNode
(const entity_t& cameraEntity, const Frustum& cameraFrustum, const Ref<OctreeNode>& node) noexcept
{
    bool isInFrustum = cameraFrustum.testAABB(node->m_aabb.m_min, node->m_aabb.m_max);
    
    if(isInFrustum)
    {
        node->m_visibleReceivers.insert(cameraEntity);
    }
    else
    {
        node->m_visibleReceivers.erase(cameraEntity);
    }
}
