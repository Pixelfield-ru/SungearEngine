//
// Created by stuka on 08.10.2025.
//

#pragma once

#include "UIElementNodeProcessor.h"

namespace SGCore::UI
{
    struct UITemplateNodeProcessor : UIElementNodeProcessor
    {
        Ref<UIElement> allocateElement() noexcept override;

        void processElement(UIDocument* inDocument,
                            const Ref<UIElement>& element,
                            const pugi::xml_node& elementNode) noexcept override;
    };
}
