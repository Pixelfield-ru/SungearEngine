//
// Created by stuka on 25.01.2025.
//

#ifndef SUNGEARENGINE_UIROOTNODEPROCESSOR_H
#define SUNGEARENGINE_UIROOTNODEPROCESSOR_H

#include "UIElementNodeProcessor.h"

namespace SGCore::UI
{
    template<>
    struct UIElementNodeProcessor<UIElementType::ET_ROOT>
    {
        static constexpr UIElementType element_type = UIElementType::ET_ROOT;

        static void processElement(const UIDocument* inDocument,
                                   const Ref<UIRoot>& element,
                                   const pugi::xml_node& elementNode) noexcept
        {

        }
    };
}

#endif //SUNGEARENGINE_UIROOTNODEPROCESSOR_H
