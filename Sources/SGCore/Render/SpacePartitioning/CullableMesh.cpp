//
// Created by ilya on 11.03.24.
//
#include "CullableMesh.h"
#include "SGCore/Main/CoreGlobals.h"

SGCore::CullableMesh::CullableMesh()
{
    m_nodeLeaveListener = MakeEventListener<void(Ref<OctreeNode> node, const entt::entity& thisEntity)>(
            [this](Ref<OctreeNode> node, const entt::entity& thisEntity) {
                m_visibleCamerasUpdatedFlag = false;
                m_visibleCameras.clear();
                m_visibleCamerasUpdatedFlag = true;
            });
}

