//
// Created by stuka on 29.09.2025.
//

#include "ReceiverPickingPass.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Input/PCInput.h"

void SGCore::ReceiverPickingPass::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline)
{
    const glm::vec2 mousePos = { Input::PC::getCursorPositionX(),
                                 Input::PC::getCursorPositionY() };

    glm::ivec2 windowSize;
    CoreMain::getWindow().getSize(windowSize.x, windowSize.y);

    auto camerasView = scene->getECSRegistry()->view<LayeredFrameReceiver>();

    camerasView.each([&](LayeredFrameReceiver& receiver) {
        auto attachment2 = receiver.m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2);

        if(!attachment2 || windowSize.x == 0 || windowSize.y == 0) return;

        const glm::vec2 mouseRelativePos = {
            mousePos.x * ((float) attachment2->getWidth() / (float) windowSize.x),
            mousePos.y * ((float) attachment2->getHeight() / (float) windowSize.y)
        };

        // reading from attachment with picking color
        const auto pickedColor = receiver.m_layersFrameBuffer->readPixelsFromAttachment(mouseRelativePos, SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2);

        receiver.m_pickingColorUnderMouse = pickedColor;
    });
}
