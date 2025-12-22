//
// Created by stuka on 22.12.2025.
//

#include "PBRRPTransparentMeshesPass.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/DisableMeshGeometryPass.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Decals/Decal.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"
#include "SGCore/Render/SpacePartitioning/IgnoreOctrees.h"
#include "SGCore/Render/SpacePartitioning/OctreeCullable.h"
#include "SGCore/Utils/Assert.h"

void SGCore::PBRRPTransparentMeshesPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    IGeometryPass::create(parentRenderPipeline);

    m_transparentEntitiesRenderState.m_globalBlendingState.m_useBlending = true;
}

void SGCore::PBRRPTransparentMeshesPass::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline)
{
    auto registry = scene->getECSRegistry();

    auto camerasView = registry->view<EntityBaseInfo, Camera3D, LayeredFrameReceiver, RenderingBase, Transform>();
    auto transparentMeshesView = registry->view<EntityBaseInfo, Mesh, Transform, TransparentEntityTag>(ECS::ExcludeTypes<DisableMeshGeometryPass, Decal>{});

    camerasView.each([&transparentMeshesView, &registry, this]
                             (const ECS::entity_t& cameraEntity,
                              const EntityBaseInfo::reg_t& camera3DBaseInfo,
                              const Camera3D::reg_t& camera3D,
                              LayeredFrameReceiver& layeredFrameReceiver,
                              RenderingBase::reg_t& cameraRenderingBase, Transform::reg_t& cameraTransform) {

        const auto* cameraCSMTarget = registry->tryGet<CSMTarget>(cameraEntity);

        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);

        layeredFrameReceiver.m_layersFrameBuffer->bind();
        layeredFrameReceiver.m_layersFrameBuffer->bindAttachmentsToDrawIn(
            layeredFrameReceiver.m_attachmentToRenderIn
        );

        m_transparentEntitiesRenderState.use();
        layeredFrameReceiver.m_layersFrameBuffer->useStates();

        transparentMeshesView.each([&]
                                      (const ECS::entity_t& meshEntity, EntityBaseInfo::reg_t& meshedEntityBaseInfo,
                                       Mesh::reg_t& mesh, Transform::reg_t& meshTransform,
                                       const auto&) {
            const bool willRender = camera3D->isEntityVisibleForCamera(registry, cameraEntity, meshEntity);

            if(!willRender) return;

            Ref<PostProcessLayer> meshPPLayer = mesh.m_base.m_layeredFrameReceiversMarkup[&layeredFrameReceiver].lock();

            if(!meshPPLayer)
            {
                meshPPLayer = layeredFrameReceiver.getDefaultLayer();
            }

            SG_ASSERT(meshPPLayer != nullptr,
                      "No post process layers in frame receiver were found for mesh! Can not render this mesh.");

            /*renderMesh(registry, meshEntity, meshTransform, mesh,
                       meshedEntityBaseInfo, camera3DBaseInfo, cameraRenderingBase, cameraCSMTarget, meshPPLayer, false, cameraLayeredFrameReceiver);*/

            // todo: make rendering
        });
    });
}
