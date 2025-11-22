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

    const auto nameAttrib = elementNode.attribute("name");
    if(nameAttrib)
    {
        templateElement->m_name = nameAttrib.as_string();
    }
    else
    {
        LOG_W(SGCORE_TAG, "XML template element must have name attribute.")
    }
    /*auto nameAttrib = elementNode.find_attribute([](const pugi::xml_attribute& attribute) {
        return attribute.name() == std::string("name");
    });*/

    for(const auto& setting : elementNode.children())
    {
        if(setting.name() == std::string("elements"))
        {
            for(const auto& unrollNode : setting.children())
            {
                auto unrolledElement = UIDocument::processUIElement(*inDocument, nullptr, unrollNode);
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

    inDocument->m_templates.push_back(std::static_pointer_cast<TemplateElement>(element));
}
