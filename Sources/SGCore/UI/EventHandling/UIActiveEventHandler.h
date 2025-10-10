//
// Created by stuka on 04.10.2025.
//

#pragma once

#include "UIEventHandler.h"

namespace SGCore::UI
{
    struct UIActiveEventHandler : UIEventHandler
    {
        SG_DECLARE_UI_EVENT_HANDLER_NAME(active)

        bool isChangesStyle() const noexcept final;

        bool testElement(UIElement& element,
                         UITransformTree& transformTree,
                         std::int64_t elementIndexInTransformTree,
                         UIDocument& document,
                         Scene& scene,
                         LayeredFrameReceiver::reg_t& attachedCameraReceiver) noexcept final;

        void callEvent(UIElement& element) noexcept final;
    };
}
