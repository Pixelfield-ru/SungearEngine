//
// Created by stuka on 22.11.2025.
//

#pragma once

#include "UIElementNodeProcessor.h"

namespace SGCore::UI
{
    struct TemplateElement;

    struct UITemplateUsageProcessor
    {
        static void processElement(const Ref<TemplateElement>& templateElement,
                                   UIDocument* inDocument,
                                   const Ref<UIElement>& element,
                                   const pugi::xml_node& elementNode) noexcept;
    };
}
