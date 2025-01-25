//
// Created by stuka on 25.01.2025.
//

#ifndef SUNGEARENGINE_UIDIVNODEPROCESSOR_H
#define SUNGEARENGINE_UIDIVNODEPROCESSOR_H

#include "UIElementNodeProcessor.h"

#include "SGCore/UI/Elements/Div.h"

namespace SGCore::UI
{
    template<>
    struct UIElementNodeProcessor<UIElementType::ET_DIV>
    {
        static constexpr UIElementType element_type = UIElementType::ET_DIV;

        static void processElement(const UIDocument* inDocument,
                                   const Ref<Div>& element,
                                   const pugi::xml_node& elementNode) noexcept
        {

        }
    };
}

#endif //SUNGEARENGINE_UIDIVNODEPROCESSOR_H
