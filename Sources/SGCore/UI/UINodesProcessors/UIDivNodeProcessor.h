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
            for(const auto& attribute : elementNode.attributes())
            {
                const UIElementAttributeType attributeType = getUIElementAttributeTypeFromString(attribute.name());

                if(attributeType == UIElementAttributeType::AT_UNKNOWN)
                {
                    UINodesProcessorsUtils::printUnknownAttributeError(inDocument, attribute, elementNode);

                    continue;
                }

                switch(attributeType)
                {
                    case UIElementAttributeType::AT_CLASS:
                    {
                        const auto foundSelector = inDocument->findSelector(attribute.value());

                        if(foundSelector)
                        {
                            element->m_selector = foundSelector;
                        }
                        else
                        {
                            // todo: maybe setting some default selector??
                            element->m_selector = nullptr;
                        }

                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
    };
}

#endif //SUNGEARENGINE_UIDIVNODEPROCESSOR_H
