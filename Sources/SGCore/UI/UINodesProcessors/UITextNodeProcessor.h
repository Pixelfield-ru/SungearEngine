//
// Created by stuka on 16.03.2025.
//

#pragma once

#include "UIElementNodeProcessor.h"

namespace SGCore::UI
{
    struct UITextNodeProcessor : UIElementNodeProcessor
    {
        Ref<UIElement> allocateElement() noexcept final;

        void processElement(UIDocument* inDocument,
                            const Ref<UIElement>& element,
                            const pugi::xml_node& elementNode) noexcept final;
    };
}
