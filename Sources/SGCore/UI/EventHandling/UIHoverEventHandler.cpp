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

bool SGCore::UI::UIHoverEventHandler::testElement(UIElement& element,
                                                  UITransformTree& transformTree,
                                                  std::int64_t elementIndexInTransformTree,
                                                  UIDocument& document,
                                                  Scene& scene,
                                                  LayeredFrameReceiver::reg_t& attachedCameraReceiver) noexcept
{
    const auto* tmp = &element;
    const auto& transformElement = transformTree.m_elements[elementIndexInTransformTree];
    const auto elementColor = transformElement.m_elementCurrentCache.m_uniqueColor.color();
    if((glm::vec3) elementColor == attachedCameraReceiver.m_pickingColorUnderMouse && element.getTypeHash() != UIRoot::getTypeHashStatic())
    {
        return true;
    }

    return false;

    // return (glm::vec3) elementColor == attachedCameraReceiver.m_pickingColorUnderMouse;
}

void SGCore::UI::UIHoverEventHandler::callEvent(UIElement& element) noexcept
{
    element.onPointerHover(&element);
}
