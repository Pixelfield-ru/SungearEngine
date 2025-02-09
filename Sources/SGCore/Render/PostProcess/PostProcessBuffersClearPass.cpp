//
// Created by stuka on 08.02.2025.
//

#include "PostProcessBuffersClearPass.h"

#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/LayeredFrameReceiver.h"

void SGCore::PostProcessBuffersClearPass::render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline)
{
    auto ecsRegistry = scene->getECSRegistry();

    auto receiversView = ecsRegistry->view<LayeredFrameReceiver>();

    receiversView.each([](const LayeredFrameReceiver::reg_t& layeredFrameReceiver) {
        layeredFrameReceiver.clearPostProcessFrameBuffers();
    });
}
