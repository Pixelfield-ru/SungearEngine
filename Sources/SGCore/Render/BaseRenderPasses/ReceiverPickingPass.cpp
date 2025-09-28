//
// Created by stuka on 29.09.2025.
//

#include "ReceiverPickingPass.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/LayeredFrameReceiver.h"

void SGCore::ReceiverPickingPass::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline)
{
    auto mainInputListener = InputManager::getMainInputListener();

    const glm::vec2 mousePos = { mainInputListener->getCursorPositionX(),
                                 mainInputListener->getCursorPositionY() };

    glm::ivec2 windowSize;
    CoreMain::getWindow().getSize(windowSize.x, windowSize.y);

    auto camerasView = scene->getECSRegistry()->view<LayeredFrameReceiver>();

    camerasView.each([&](LayeredFrameReceiver& receiver) {
        auto attachment2 = receiver.m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2);

        if(!attachment2) return;

        const glm::vec2 mouseRelativePos = {
            mousePos.x * (attachment2->getWidth() / windowSize.x),
            mousePos.y * (attachment2->getHeight() / windowSize.y)
        };

        // reading from attachment with picking color
        const auto pickedColor = receiver.m_layersFrameBuffer->readPixelsFromAttachment(mouseRelativePos, SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2);

        receiver.m_pickingColorUnderMouse = pickedColor;
    });
}
