//
// Created by stuka on 04.10.2025.
//

#include "UIActiveEventHandler.h"

#include "SGCore/Input/PCInput.h"
#include "SGCore/UI/UIElement.h"

bool SGCore::UI::UIActiveEventHandler::isChangesStyle() const noexcept
{
    return true;
}

bool SGCore::UI::UIActiveEventHandler::testElement(UIElement& element,
                                                   UITransformTree& transformTree,
                                                   std::int64_t elementIndexInTransformTree,
                                                   UIDocument& document,
                                                   Scene& scene,
                                                   LayeredFrameReceiver::reg_t& attachedCameraReceiver) noexcept
{
    const auto& transformElement = transformTree.m_elements[elementIndexInTransformTree];
    const auto elementColor = transformElement.m_elementCurrentCache.m_uniqueColor.color();
    // std::cout << "color under mouse: " << attachedCameraReceiver.m_pickingColorUnderMouse.x << ", " << attachedCameraReceiver.m_pickingColorUnderMouse.y << ", " << attachedCameraReceiver.m_pickingColorUnderMouse.z << std::endl;
    return (glm::vec3) elementColor == attachedCameraReceiver.m_pickingColorUnderMouse && Input::PC::mouseButtonDown(Input::MouseButton::MOUSE_BUTTON_LEFT);
}

void SGCore::UI::UIActiveEventHandler::callEvent(UIElement& element) noexcept
{
    if(Input::PC::mouseButtonPressed(Input::MouseButton::MOUSE_BUTTON_LEFT))
    {
        element.onPointerDown(&element);
    }

    if(Input::PC::mouseButtonDown(Input::MouseButton::MOUSE_BUTTON_LEFT))
    {
        element.onPointerHold(&element);
    }

    if(Input::PC::mouseButtonReleased(Input::MouseButton::MOUSE_BUTTON_LEFT))
    {
        element.onPointerUp(&element);
    }
}
