//
// Created by stuka on 04.10.2025.
//

#include "UIActiveEventHandler.h"

#include "SGCore/Input/InputManager.h"
#include "SGCore/UI/UIElement.h"

bool SGCore::UI::UIActiveEventHandler::isChangesStyle() const noexcept
{
    return true;
}

bool SGCore::UI::UIActiveEventHandler::testElement(UIElement& element, UIDocument& document, Scene& scene, LayeredFrameReceiver::reg_t& attachedCameraReceiver) noexcept
{
    return attachedCameraReceiver.m_pickingColorUnderMouse == (glm::vec3) element.m_uniqueColor.color() && InputManager::getMainInputListener()->mouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT);
}

void SGCore::UI::UIActiveEventHandler::callEvent(UIElement& element) noexcept
{
    if(InputManager::getMainInputListener()->mouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
    {
        element.onPointerDown(&element);
    }

    if(InputManager::getMainInputListener()->mouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
    {
        element.onPointerHold(&element);
    }

    if(InputManager::getMainInputListener()->mouseButtonReleased(MouseButton::MOUSE_BUTTON_LEFT))
    {
        element.onPointerUp(&element);
    }
}
