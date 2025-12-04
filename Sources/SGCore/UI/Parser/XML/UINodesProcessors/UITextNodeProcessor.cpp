//
// Created by stuka on 24.09.2025.
//

#include "UITextNodeProcessor.h"

#include "UINodesProcessorsUtils.h"

#include "SGCore/UI/UIDocument.h"
#include "SGCore/UI/UIElementAttributeType.h"
#include "SGCore/UI/Utils.h"
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

    const std::string text = elementNode.text().as_string();
    // std::u32string text = SGCore::Utils::fromUTF8<char32_t>(elementNode.text().as_string());
    if(const auto variable = UI::Utils::extractVariable(text))
    {
        const auto binding = inDocument->m_bindingsStorage.getBinding(*variable);
        textElement->m_text = binding;
    }
    else
    {
        textElement->m_text = SGCore::Utils::fromUTF8<char32_t>(text);
    }

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
                const auto foundSelector = inDocument->findStyle(attribute.value());

                if(foundSelector)
                {
                    textElement->m_mainStyle = foundSelector;
                }
                else
                {
                    // todo: maybe setting some default selector??
                    textElement->m_mainStyle = AssetManager::getInstance()->getOrAddAssetByAlias<Style>("sgui_default_text_style");
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
