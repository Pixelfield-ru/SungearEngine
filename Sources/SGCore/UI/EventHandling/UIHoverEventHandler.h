//
// Created by stuka on 01.10.2025.
//

#pragma once

#include "UIEventHandler.h"

namespace SGCore::UI
{
    struct UIHoverEventHandler : UIEventHandler
    {
        SG_DECLARE_UI_EVENT_HANDLER_NAME(hover)

        bool isChangesStyle() const noexcept final;

        bool testElement(UIElement& element, UIDocument& document, Scene& scene, LayeredFrameReceiver::reg_t& attachedCameraReceiver) noexcept final;
    };
}
