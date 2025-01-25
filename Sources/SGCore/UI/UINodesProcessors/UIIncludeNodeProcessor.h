//
// Created by stuka on 25.01.2025.
//

#ifndef SUNGEARENGINE_UIINCLUDENODEPROCESSOR_H
#define SUNGEARENGINE_UIINCLUDENODEPROCESSOR_H

#include "UIElementNodeProcessor.h"

namespace SGCore::UI
{
    template<>
    struct UIElementNodeProcessor<UIElementType::ET_INCLUDE>
    {
        static constexpr UIElementType element_type = UIElementType::ET_INCLUDE;

        static void processElement(const UIDocument* inDocument,
                                   const Ref<UIElement>& element,
                                   const pugi::xml_node& elementNode) noexcept
        {

        }
    };
}

#endif //SUNGEARENGINE_UIINCLUDENODEPROCESSOR_H
