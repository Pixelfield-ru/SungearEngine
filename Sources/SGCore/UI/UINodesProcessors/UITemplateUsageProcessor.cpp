//
// Created by stuka on 22.11.2025.
//

#include "UITemplateUsageProcessor.h"

#include "SGCore/UI/UIDocument.h"
#include "SGCore/UI/Elements/TemplateElement.h"

void SGCore::UI::UITemplateUsageProcessor::processElement(const Ref<TemplateElement>& templateElement,
                                                          UIDocument* inDocument,
                                                          const Ref<UIElement>& element,
                                                          const pugi::xml_node& elementNode) noexcept
{
    // unrolling template
    for(const auto& childElement : templateElement->m_children)
    {
        element->m_children.push_back(childElement->copy());
    }

    // then processing places
    for(const auto& childNode : elementNode.children())
    {
        if(childNode.name() == std::string("place-at"))
        {
            const auto placeNameAttrib = childNode.attribute("name");
            if(!placeNameAttrib)
            {
                LOG_W(SGCORE_TAG, "XML place element must have name attribute.")
                continue;
            }

            std::vector<Ref<UIElement>> placedElements;

            for(const auto& placedXMLElement : childNode.children())
            {
                auto unrolledElement = UIDocument::processUIElement(*inDocument, nullptr, placedXMLElement);
                if(unrolledElement)
                {
                    placedElements.push_back(std::move(unrolledElement));
                }
            }

            const std::string placeName = placeNameAttrib.as_string();

            const auto place = element->m_places.contains(placeName) ? element : element->findPlace(placeName);

            if(!place)
            {
                LOG_E(SGCORE_TAG,
                      "Unable to find place with name '{}' to insert elements in template '{}'",
                      placeName,
                      templateElement->m_name)
                continue;
            }

            // adding all elements under place in place
            for(const auto& placedElement : placedElements)
            {
                place->m_children.push_back(placedElement->copy());
            }
        }
    }
}
