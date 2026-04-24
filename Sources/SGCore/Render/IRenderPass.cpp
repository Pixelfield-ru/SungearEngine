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

    camerasView.each([&registry, &func](const ECS::entity_t& cameraEntity,
                                              const EntityBaseInfo& camera3DBaseInfo,
                                              const Camera3D& camera3D,
                                              LayeredFrameReceiver& layeredFrameReceiver,
                                              const RenderingBase& cameraRenderingBase,
                                              const Transform& cameraTransform) {
        const auto* cameraCSMTarget = registry->tryGet<CSMTarget>(cameraEntity);

        CoreMain::getRenderer()->prepareUniformBuffers(cameraRenderingBase, cameraTransform);

        CoreMain::getRenderer()->m_viewMatricesBuffer->bind();
        CoreMain::getRenderer()->m_programDataBuffer->bind();

        layeredFrameReceiver.m_layersFrameBuffer->bind();
        layeredFrameReceiver.m_layersFrameBuffer->bindAttachmentsToDrawIn(
            layeredFrameReceiver.m_attachmentToRenderIn
        );

        /*m_renderState.use();
        layeredFrameReceiver.m_layersFrameBuffer->useStates();*/

        CameraRenderingInfo cameraRenderingInfo;
        cameraRenderingInfo.m_cameraEntity = cameraEntity;
        cameraRenderingInfo.m_cameraInfo = &camera3DBaseInfo;
        cameraRenderingInfo.m_camera3D = &camera3D;
        cameraRenderingInfo.m_cameraFrameReceiver = &layeredFrameReceiver;
        cameraRenderingInfo.m_cameraRenderingBase = &cameraRenderingBase;
        cameraRenderingInfo.m_cameraTransform = &cameraTransform;
        cameraRenderingInfo.m_cameraCSMTarget = cameraCSMTarget;

        func(cameraRenderingInfo);

        layeredFrameReceiver.m_layersFrameBuffer->unbind();
    });
}
