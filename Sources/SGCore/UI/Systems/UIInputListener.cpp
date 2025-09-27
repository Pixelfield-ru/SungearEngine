//
// Created by stuka on 27.09.2025.
//

#include "UIInputListener.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/UI/UIComponent.h"

void SGCore::UI::UIInputListener::update(const double& dt, const double& fixedDt)
{
    auto scene = getScene();

    auto mainInputListener = InputManager::getMainInputListener();

    const glm::vec2 mousePos = { mainInputListener->getCursorPositionX(),
                                 mainInputListener->getCursorPositionY() };

    glm::ivec2 windowSize;
    CoreMain::getWindow().getSize(windowSize.x, windowSize.y);

    auto uiComponentsView = scene->getECSRegistry()->view<UIComponent>();
    auto camerasView = scene->getECSRegistry()->view<LayeredFrameReceiver, EntityBaseInfo>();

    camerasView.each([&](const LayeredFrameReceiver::reg_t& layeredFrameReceiver, const EntityBaseInfo& cameraInfo) {
        uiComponentsView.each([&](UIComponent::reg_t& uiComponent) {
            auto attachment2 = layeredFrameReceiver.m_layersFrameBuffer
                    ->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2);

            const glm::vec2 mouseRelativePos = {
                mousePos.x * (attachment2->getWidth() / windowSize.x),
                mousePos.y * (attachment2->getHeight() / windowSize.y)
            };

            // reading from attachment with picking color
            const auto pickedColor = layeredFrameReceiver.m_layersFrameBuffer->readPixelsFromAttachment(mouseRelativePos, SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2);

            uiComponent.m_transformTree.m_elements[0].m_currentElementCache.m_backgroundColor

            bool breakFind = false;
            uiComponent.m_document->iterate([&](UIElement* parent, UIElement* current) {
                if((glm::vec3) current->m_uniqueColor.color() == pickedColor)
                {
                    std::cout << "hovering element: " << current->m_name << std::endl;
                    breakFind = true;
                }
            }, breakFind);
        });
    });
}
