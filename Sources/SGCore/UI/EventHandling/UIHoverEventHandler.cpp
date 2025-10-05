//
// Created by stuka on 01.10.2025.
//

#include "UIHoverEventHandler.h"

#include "SGCore/UI/UIElement.h"
#include "SGCore/UI/UIDocument.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/LayeredFrameReceiver.h"

bool SGCore::UI::UIHoverEventHandler::isChangesStyle() const noexcept
{
    return true;
}

bool SGCore::UI::UIHoverEventHandler::testElement(UIElement& element, UIDocument& document, Scene& scene, LayeredFrameReceiver::reg_t& attachedCameraReceiver) noexcept
{
    return (glm::vec3) element.m_uniqueColor.color() == attachedCameraReceiver.m_pickingColorUnderMouse;
}

void SGCore::UI::UIHoverEventHandler::callEvent(UIElement& element) noexcept
{
    element.onPointerHover(&element);
}
