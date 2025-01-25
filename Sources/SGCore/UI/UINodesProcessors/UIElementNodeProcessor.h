//
// Created by stuka on 25.01.2025.
//

#ifndef SUNGEARENGINE_UIELEMENTNODEPROCESSOR_H
#define SUNGEARENGINE_UIELEMENTNODEPROCESSOR_H

#include "SGCore/UI/UIElement.h"
#include "SGCore/UI/UIDocument.h"

namespace SGCore::UI
{
    /**
     * Processes element and its attributes.
     * You must implement all following functions and variables in your specialization of UIElementNodeProcessor.
     * @tparam ElementType Type of element.
     */
    template<UIElementType ElementType>
    struct UIElementNodeProcessor
    {
        static constexpr UIElementType element_type = ElementType;

        static_assert(always_false_obj<ElementType>::value, "UIElementNodeProcessor for this element is not implemented!");

        static void processElement(const UIDocument* inDocument,
                                   const Ref<UIElement>& element,
                                   const pugi::xml_node& elementNode) noexcept
        {

        }
    };
}

#endif //SUNGEARENGINE_UIELEMENTNODEPROCESSOR_H
