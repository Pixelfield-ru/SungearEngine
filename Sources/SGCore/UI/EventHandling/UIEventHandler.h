//
// Created by stuka on 29.09.2025.
//

#pragma once

#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/UI/TransformTree/UITransformTree.h"
#include "SGCore/Utils/TypeTraits.h"

#define SG_DECLARE_UI_EVENT_HANDLER_NAME(name) \
    static consteval size_t getEventNameHashStatic() noexcept \
    { \
        static constexpr size_t name_hash = SGCore::constexprHash(#name); \
        return name_hash; \
    } \
    size_t getEventNameHash() noexcept final \
    { \
        static constexpr size_t name_hash = SGCore::constexprHash(#name); \
        return name_hash; \
    }

namespace SGCore
{
    class Scene;
}

namespace SGCore::UI
{
    struct UIElement;
    struct CSSStyle;
    struct UIDocument;
    struct UITransformTree;

    struct UIEventHandler
    {
        virtual ~UIEventHandler() = default;

        virtual bool isChangesStyle() const noexcept = 0;

        /**
         * Tests event completion for UI element.
         * @param element Element to test event.
         * @return Is event
         */
        virtual bool testElement(UIElement& element,
                                 UITransformTree& transformTree,
                                 std::int64_t elementIndexInTransformTree,
                                 UIDocument& document,
                                 Scene& scene,
                                 LayeredFrameReceiver::reg_t& attachedCameraReceiver) noexcept = 0;

        virtual void callEvent(UIElement& element) noexcept = 0;

        virtual size_t getEventNameHash() noexcept = 0;
    };
}
