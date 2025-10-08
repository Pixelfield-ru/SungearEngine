//
// Created by stuka on 08.10.2025.
//

#include "UITemplateNodeProcessor.h"

#include "SGCore/UI/UIDocument.h"
#include "SGCore/UI/Elements/TemplateElement.h"

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UITemplateNodeProcessor::allocateElement() noexcept
{
    return MakeRef<TemplateElement>();
}

void SGCore::UI::UITemplateNodeProcessor::processElement(UIDocument* inDocument,
                                                         const Ref<UIElement>& element,
                                                         const pugi::xml_node& elementNode) noexcept
{
    auto* templateElement = static_cast<TemplateElement*>(element.get());

    auto nameAttrib = elementNode.find_attribute([](const pugi::xml_attribute& attribute) {
        return attribute.value() == std::string("name");
    });

    templateElement->m_name = nameAttrib.as_string();

    for(const auto& setting : elementNode.children())
    {
        if(setting.name() == std::string("elements"))
        {
            for(const auto& unrollNode : setting.children())
            {
                auto unrolledElement = UIDocument::processUIElement(*inDocument, unrollNode);
                if(unrolledElement)
                {
                    templateElement->m_children.push_back(std::move(unrolledElement));
                }
            }
        }

        if(setting.name() == std::string("attribute"))
        {

        }
    }
}
