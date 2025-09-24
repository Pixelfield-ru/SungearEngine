//
// Created by stuka on 25.01.2025.
//

#pragma once

#include "UIElementNodeProcessor.h"

namespace SGCore::UI
{
    struct UIRootNodeProcessor : UIElementNodeProcessor
    {
        SG_DECLARE_UI_ELEMENT_PROCESSOR_NODE_NAME(xml)

        Ref<SGCore::UI::UIElement> allocateElement() noexcept final;

        void processElement(UIDocument* inDocument,
                            const Ref<UIElement>& element,
                            const pugi::xml_node& elementNode) noexcept final
        {

        }
    };
}
