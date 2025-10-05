//
// Created by stuka on 05.10.2025.
//

#include "UIButtonNodeProcessor.h"

#include "UINodesProcessorsUtils.h"
#include "SGCore/UI/UIElementAttributeType.h"
#include "SGCore/UI/Elements/Button.h"
#include "SGCore/UI/Elements/Text.h"

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UIButtonNodeProcessor::allocateElement() noexcept
{
    return MakeRef<Button>();
}

void SGCore::UI::UIButtonNodeProcessor::processElement(UIDocument* inDocument,
                                                   const Ref<UIElement>& element,
                                                   const pugi::xml_node& elementNode) noexcept
{
    //  = Utils::fromUTF8<char32_t>(elementNode.text().as_string());

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
                    element->m_mainStyle = foundSelector;
                }
                else
                {
                    // todo: maybe setting some default selector??
                    element->m_mainStyle = nullptr;
                }

                break;
            }
            case UIElementAttributeType::AT_NAME:
            {
                element->m_name = attribute.value();

                break;
            }
            default:
            {
                break;
            }
        }
    }

    if(std::strlen(elementNode.text().as_string()) == 0) return;

    auto textElement = MakeRef<Text>();
    textElement->m_text = Utils::fromUTF8<char32_t>(elementNode.text().as_string());
    textElement->m_mainStyle = element->m_mainStyle;

    element->m_children.push_back(textElement);
}
