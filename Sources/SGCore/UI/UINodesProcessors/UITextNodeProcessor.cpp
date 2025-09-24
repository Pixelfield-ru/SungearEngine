//
// Created by stuka on 24.09.2025.
//

#include "UITextNodeProcessor.h"

#include "UINodesProcessorsUtils.h"

#include "SGCore/UI/UIDocument.h"
#include "SGCore/UI/UIElementAttributeType.h"
#include "SGCore/UI/Elements/Text.h"

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UITextNodeProcessor::allocateElement() noexcept
{
    return MakeRef<Text>();
}

void SGCore::UI::UITextNodeProcessor::processElement(UIDocument* inDocument,
                                                     const Ref<UIElement>& element,
                                                     const pugi::xml_node& elementNode) noexcept
{
    auto* textElement = static_cast<Text*>(element.get());

    textElement->m_text = Utils::fromUTF8<char32_t>(elementNode.text().as_string());

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
                    textElement->m_selector = foundSelector;
                }
                else
                {
                    // todo: maybe setting some default selector??
                    textElement->m_selector = AssetManager::getInstance()->getOrAddAssetByAlias<CSSSelector>("sgui_default_text_style");
                }

                textElement->clearGlyphs();

                break;
            }
            case UIElementAttributeType::AT_NAME:
            {
                textElement->m_name = attribute.value();

                break;
            }
            default:
            {
                break;
            }
        }
    }
}
