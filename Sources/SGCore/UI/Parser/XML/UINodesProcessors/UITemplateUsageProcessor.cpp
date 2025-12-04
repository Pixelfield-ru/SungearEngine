//
// Created by stuka on 22.11.2025.
//

#include "UITemplateUsageProcessor.h"

#include "UIElementsProcessorsRegistry.h"
#include "SGCore/UI/UIDocument.h"
#include "SGCore/UI/Elements/TemplateElement.h"

void SGCore::UI::UITemplateUsageProcessor::processElement(const Ref<TemplateElement>& templateElement,
                                                          UIDocument* inDocument,
                                                          const Ref<UIElement>& element,
                                                          const pugi::xml_node& elementNode) noexcept
{
    std::vector<Ref<UIElement>> unrolledElements;

    // unrolling template
    for(const auto& childElement : templateElement->m_children)
    {
        const auto instantiation = childElement->copy();
        instantiation->m_parent = element;

        element->m_children.push_back(instantiation);
        unrolledElements.push_back(instantiation);
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

            Ref<UIElement> place;
            if(element->m_places.contains(placeName))
            {
                place = element;
            }
            else
            {
                for(const auto& instantiation : unrolledElements)
                {
                    if(instantiation->m_places.contains(placeName))
                    {
                        place = instantiation;
                        break;
                    }

                    place = instantiation->findPlace(placeName);
                }
            }

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

    const auto nodeColumnLine = inDocument->getLocationInFile(elementNode);

    // checking if template usage has unknown attributes
    for(const auto& attrib : elementNode.attributes())
    {
        if(!templateElement->m_attributes.contains(attrib.name()))
        {
            LOG_W(SGCORE_TAG,
                  "In UI Document by path '{}' in instantiation of template '{}': unknown attribute '{}' of template was used. Attribute was ignored\n"
                  "Line: {}\n"
                  "Column: {}",
                  SGCore::Utils::toUTF8(inDocument->getPath().resolved().u16string()),
                  templateElement->m_name,
                  attrib.name(),
                  nodeColumnLine.first,
                  nodeColumnLine.second)
        }
    }

    // then processing template attributes
    for(const auto& [templateAttribName, templateAttrib] : templateElement->m_attributes)
    {
        const auto& attrib = elementNode.attribute(templateAttribName);
        if(!attrib && templateAttrib.m_isRequired)
        {
            LOG_E(SGCORE_TAG,
                  "In UI Document by path '{}' in instantiation of template '{}': attribute '{}' is required\n"
                  "Line: {}\n"
                  "Column: {}",
                  SGCore::Utils::toUTF8(inDocument->getPath().resolved().u16string()),
                  templateElement->m_name,
                  templateAttribName,
                  nodeColumnLine.first,
                  nodeColumnLine.second)
            continue;
        }

        // todo: continue
    }
}
