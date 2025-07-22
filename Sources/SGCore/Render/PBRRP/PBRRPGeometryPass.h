//
// Created by stuka on 25.11.2023.
//

#ifndef SUNGEARENGINE_PBRRPGEOMETRYPASS_H
#define SUNGEARENGINE_PBRRPGEOMETRYPASS_H

#include <SGCore/pch.h>

#include "SGCore/Render/IRenderPass.h"
#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"
#include "SGCore/Render/Terrain/Terrain.h"

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
        AssetRef<IShader> m_batchShader;
        AssetRef<IShader> m_instancingShader;

        void renderMesh(const Ref<ECS::registry_t>& registry,
                        const ECS::entity_t& meshEntity,
                        const Transform::reg_t& meshTransform,
                        Mesh::reg_t& mesh,
                        EntityBaseInfo::reg_t& meshedEntityBaseInfo,
                        const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                        const RenderingBase::reg_t& cameraRenderingBase,
                        const CSMTarget::reg_t* cameraCSMTarget,
                        const Ref<PostProcessLayer>& meshPPLayer,
                        bool isTransparentPass,
                        LayeredFrameReceiver::reg_t* forLayeredFrameReceiver) noexcept;

        void renderTerrainMesh(const Ref<ECS::registry_t>& registry,
                               const ECS::entity_t& terrainEntity,
                               const Transform::reg_t& terrainTransform,
                               Mesh::reg_t& mesh,
                               const Terrain::reg_t& terrain,
                               EntityBaseInfo::reg_t& terrainEntityBaseInfo,
                               const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                               const Ref<PostProcessLayer>& terrainPPLayer) noexcept;

        void renderOctreeNode(const Ref<ECS::registry_t>& registry,
                              const EntityBaseInfo::reg_t& forCamera3DBaseInfo,
                              const RenderingBase::reg_t& cameraRenderingBase,
                              const CSMTarget::reg_t* cameraCSMTarget,
                              const ECS::entity_t& forCamera,
                              LayeredFrameReceiver* cameraLayeredFrameReceiver,
                              const Ref<OctreeNode>& node) noexcept;
    };
}

#endif //SUNGEARENGINE_PBRRPGEOMETRYPASS_H
