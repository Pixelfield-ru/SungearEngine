//
// Created by stuka on 23.11.2025.
//

#include "UIPlaceNodeProcessor.h"

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UIPlaceNodeProcessor::allocateElement() noexcept
{
    return nullptr;
}

void SGCore::UI::UIPlaceNodeProcessor::processElement(UIDocument* inDocument,
                                                      const Ref<UIElement>& element,
                                                      const pugi::xml_node& elementNode) noexcept
{
    if(const auto placeNameAttrib = elementNode.attribute("name"))
    {
        // outputElement->m_places.insert(placeNameAttrib.as_string());
    }
    else
    {
        SG_LOG_W("Place node must have 'name' attribute");
    }
}
