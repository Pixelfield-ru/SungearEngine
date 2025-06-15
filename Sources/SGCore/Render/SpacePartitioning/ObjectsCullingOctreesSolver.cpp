//
// Created by ilya on 10.03.24.
//

#include "ObjectsCullingOctreesSolver.h"
#include "Octree.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Camera3D.h"
#include "ObjectsCullingOctree.h"
#include "OctreeCullable.h"
#include "SGCore/Render/RenderingBase.h"

void SGCore::ObjectsCullingOctreesSolver::fixedUpdate(const double& dt, const double& fixedDt, OctreesSolver* parent) noexcept
{
    auto lockedScene = parent->getScene();
    
    if(!lockedScene) return;
    
    auto registry = lockedScene->getECSRegistry();

    auto objectsCullingOctrees = registry->view<Octree, ObjectsCullingOctree>();
    auto camerasView = registry->view<Camera3D, RenderingBase, Transform>();
    objectsCullingOctrees.each([&camerasView, &registry, this](Octree::reg_t octree, ObjectsCullingOctree::reg_t&) {
        camerasView.each([&octree, &registry, this]
        (const ECS::entity_t& cameraEntity, Camera3D::reg_t camera3D, RenderingBase::reg_t renderingBase, Transform::reg_t cameraTransform) {
            for(const auto& n : octree->m_notEmptyNodes)
            {
                testNode(cameraEntity, renderingBase->m_frustum, n);
            }
        });
    });
}

void SGCore::ObjectsCullingOctreesSolver::testNode
(const ECS::entity_t& cameraEntity, const Frustum& cameraFrustum, const Ref<OctreeNode>& node) noexcept
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
