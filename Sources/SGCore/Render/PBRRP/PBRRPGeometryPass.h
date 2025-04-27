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
#include "SGCore/Render/LayeredFrameReceiver.h"

namespace SGCore
{
    struct OctreeNode;
    struct PostProcessLayer;
    
    struct PBRRPGeometryPass : public IGeometryPass
    {
        RenderState m_opaqueEntitiesRenderState;
        RenderState m_transparentEntitiesRenderState;
        RenderState m_afterRenderState;

        void create(const Ref<IRenderPipeline>& parentRenderPipeline) final;
        
        void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) final;
        
    private:
        void renderMesh(const Ref<ECS::registry_t>& registry,
                        const ECS::entity_t& meshEntity,
                        const Transform::reg_t& meshTransform,
                        Mesh::reg_t& mesh,
                        EntityBaseInfo::reg_t& meshedEntityBaseInfo,
                        const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                        const Ref<PostProcessLayer>& meshPPLayer,
                        bool isTransparentPass,
                        LayeredFrameReceiver::reg_t* forLayeredFrameReceiver) noexcept;

        void renderOctreeNode(const Ref<ECS::registry_t>& registry,
                              const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                              const ECS::entity_t& forCamera,
                              LayeredFrameReceiver* cameraLayeredFrameReceiver,
                              const Ref<OctreeNode>& node) noexcept;

        // contains only unique materials. first - hash of material
        std::unordered_set<size_t> m_collectedDecalsUniqueMaterialsHashes;
        std::vector<AssetRef<IMaterial>> m_collectedDecalsUniqueMaterials;
        Ref<IUniformBuffer> m_collectedDecalsUniformBuffer;
    };
}

#endif //SUNGEARENGINE_PBRRPGEOMETRYPASS_H
