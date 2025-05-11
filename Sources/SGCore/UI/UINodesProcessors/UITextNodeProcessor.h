//
// Created by stuka on 16.03.2025.
//

#ifndef UITEXTNODEPROCESSOR_H
#define UITEXTNODEPROCESSOR_H

#include "UINodesProcessorsUtils.h"
#include "SGCore/UI/UIElementType.h"

#include "SGCore/UI/UIDocument.h"
#include "SGCore/UI/UIElementAttributeType.h"
#include "SGCore/UI/Elements/Text.h"

namespace SGCore::UI
{
    template<>
    struct UIElementNodeProcessor<UIElementType::ET_TEXT>
    {
        static constexpr UIElementType element_type = UIElementType::ET_TEXT;

        static void processElement(const UIDocument* inDocument,
                                   const Ref<Text>& element,
                                   const pugi::xml_node& elementNode) noexcept
        {
            element->m_text = Utils::fromUTF8<char32_t>(elementNode.text().as_string());

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
                            element->m_selector = AssetManager::getInstance()->getOrAddAssetByAlias<CSSSelector>("sgui_default_text_style");
                        }

                        element->clearGlyphs();

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

#endif //UITEXTNODEPROCESSOR_H
