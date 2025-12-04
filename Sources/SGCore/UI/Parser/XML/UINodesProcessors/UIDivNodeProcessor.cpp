//
// Created by stuka on 24.09.2025.
//

#include "UIDivNodeProcessor.h"

#include "UINodesProcessorsUtils.h"
#include "SGCore/UI/UIElementAttributeType.h"
#include "SGCore/UI/Elements/Div.h"

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UIDivNodeProcessor::allocateElement() noexcept
{
    return MakeRef<Div>();
}

void SGCore::UI::UIDivNodeProcessor::processElement(UIDocument* inDocument,
                                                    const Ref<UIElement>& element,
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
}
