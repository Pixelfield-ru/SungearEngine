//
// Created by stuka on 25.11.2023.
//

#ifndef SUNGEARENGINE_PBRRPGEOMETRYPASS_H
#define SUNGEARENGINE_PBRRPGEOMETRYPASS_H

#include <SGCore/pch.h>

#include "SGCore/Render/IRenderPass.h"
#include "SGCore/Transformations/TransformBase.h"
#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"
#include "SGCore/Math/Frustum.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Render/Mesh.h"

namespace SGCore
{
    struct OctreeNode;
    struct PostProcessLayer;
    
    struct PBRRPGeometryPass : public IGeometryPass
    {
        RenderState m_opaqueEntitiesRenderState;
        RenderState m_transparentEntitiesRenderState;

        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;
        
        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;
        
    private:
        void renderMesh(const Ref<ECS::registry_t>& registry,
                        const ECS::entity_t& meshEntity,
                        const Transform::reg_t& meshTransform,
                        Mesh::reg_t& mesh,
                        EntityBaseInfo::reg_t& meshedEntityBaseInfo,
                        const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                        const Ref<PostProcessLayer>& meshPPLayer) noexcept;

        void renderOctreeNode(const Ref<ECS::registry_t>& registry,
                              const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                              const ECS::entity_t& forCamera,
                              LayeredFrameReceiver* cameraLayeredFrameReceiver,
                              const Ref<OctreeNode>& node) noexcept;
    };
}

#endif //SUNGEARENGINE_PBRRPGEOMETRYPASS_H
