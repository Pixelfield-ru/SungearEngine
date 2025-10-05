//
// Created by stuka on 25.01.2025.
//

#pragma once

#include "SGCore/UI/UIElement.h"

namespace SGCore::UI
{
    /**
     * Processes element and its attributes.
     * You must implement all following functions and variables in your implementation of UIElementNodeProcessor.
     * @tparam ElementType Type of element.
     */
    struct UIElementNodeProcessor
    {
        virtual ~UIElementNodeProcessor() = default;

        virtual Ref<UIElement> allocateElement() noexcept = 0;

        virtual void processElement(UIDocument* inDocument,
                                    const Ref<UIElement>& element,
                                    const pugi::xml_node& elementNode) noexcept = 0;
    };
}
