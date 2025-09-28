//
// Created by stuka on 29.09.2025.
//

#pragma once

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::UI
{
    struct UIElement;
    struct CSSSelector;

    struct UIEventHandler
    {
        virtual ~UIEventHandler() = default;

        virtual bool isChangesSelector() const noexcept = 0;

        virtual Ref<CSSSelector> getSelector() const noexcept = 0;

        virtual void applySelector() noexcept = 0;

        /**
         * Tests event completion for UI element.
         * @param element Element to test event.
         * @return Is event
         */
        virtual bool testElement(UIElement& element) noexcept = 0;
    };
}
