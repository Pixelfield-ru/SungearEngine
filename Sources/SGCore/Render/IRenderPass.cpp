//
// Created by stuka on 23.12.2025.
//

#include "IRenderPass.h"

#include "Camera3D.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Scene/Scene.h"
#include "RenderingBase.h"
#include "SGCore/Transformations/Transform.h"
#include "LayeredFrameReceiver.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "ShadowMapping/CSM/CSMTarget.h"

void SGCore::IRenderPass::bindUniformBuffers(IShader* toShader) noexcept
{
    auto uniformBuffsIt = m_uniformBuffersToUse.begin();
    while(uniformBuffsIt != m_uniformBuffersToUse.end())
    {
        if(auto lockedUniformBuf = uniformBuffsIt->lock())
        {
            toShader->useUniformBuffer(lockedUniformBuf);

            ++uniformBuffsIt;
        }
        else
        {
            uniformBuffsIt = m_uniformBuffersToUse.erase(uniformBuffsIt);
        }
    }
}

void SGCore::IRenderPass::iterateCameras(const Scene* scene, const std::function<void(CameraRenderingInfo&)>& func) noexcept
{
    auto registry = scene->getECSRegistry();

    auto camerasView = registry->view<EntityBaseInfo, Camera3D, LayeredFrameReceiver, RenderingBase, Transform>();

    camerasView.each([&registry, this, &func](const ECS::entity_t& cameraEntity,
                                              const EntityBaseInfo::reg_t& camera3DBaseInfo,
                                              const Camera3D::reg_t& camera3D,
                                              LayeredFrameReceiver::reg_t& layeredFrameReceiver,
                                              const RenderingBase::reg_t& cameraRenderingBase,
                                              const Transform::reg_t& cameraTransform) {
        const auto* cameraCSMTarget = registry->tryGet<CSMTarget>(cameraEntity);

        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);

        CoreMain::getRenderer()->m_viewMatricesBuffer->bind();
        CoreMain::getRenderer()->m_programDataBuffer->bind();

        layeredFrameReceiver.m_layersFrameBuffer->bind();
        layeredFrameReceiver.m_layersFrameBuffer->bindAttachmentsToDrawIn(
            layeredFrameReceiver.m_attachmentToRenderIn
        );

        m_renderState.use();
        layeredFrameReceiver.m_layersFrameBuffer->useStates();

        CameraRenderingInfo cameraRenderingInfo;
        cameraRenderingInfo.m_cameraEntity = cameraEntity;
        cameraRenderingInfo.m_cameraInfo = &camera3DBaseInfo;
        cameraRenderingInfo.m_camera3D = camera3D.get();
        cameraRenderingInfo.m_cameraFrameReceiver = &layeredFrameReceiver;
        cameraRenderingInfo.m_cameraRenderingBase = cameraRenderingBase.get();
        cameraRenderingInfo.m_cameraTransform = cameraTransform.get();
        cameraRenderingInfo.m_cameraCSMTarget = cameraCSMTarget;

        func(cameraRenderingInfo);

        layeredFrameReceiver.m_layersFrameBuffer->unbind();
    });
}
