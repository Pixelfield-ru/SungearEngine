//
// Created by stuka on 24.12.2025.
//

#include "PBRRPBatchingPass.h"

#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Batching/Batch.h"
#include "SGCore/Render/RenderAbilities/EnableBatchingPass.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"
#include "SGCore/Scene/Scene.h"

void SGCore::PBRRPBatchingPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    IGeometryPass::create(parentRenderPipeline);

    const auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
            *parentRenderPipeline->m_shadersPaths["StandardTerrainShader"]);

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());
}

void SGCore::PBRRPBatchingPass::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline)
{
    const auto registry = scene->getECSRegistry();

    auto batchesView = registry->view<Batch, EnableBatchingPass>();

    iterateCameras(scene, [&](const CameraRenderingInfo& cameraRenderingInfo) {
        batchesView.each([&](Batch& batch, auto) {
            // todo: add getting batch layer
            const Ref<PostProcessLayer> meshPPLayer = cameraRenderingInfo.m_cameraFrameReceiver->getDefaultLayer();

            batch.bind(m_shader.get());

            bindUniformBuffers(m_shader.get());

            if(cameraRenderingInfo.m_cameraCSMTarget)
            {
                cameraRenderingInfo.m_cameraCSMTarget->bindUniformsToShader(
                    m_shader.get(), cameraRenderingInfo.m_cameraRenderingBase->m_zFar, 5);
            }

            CoreMain::getRenderer()->renderArray(
                batch.getVertexArray(),
                batch.m_batchRenderState,
                batch.getTrianglesCount(),
                0
            );
        });
    });
}
