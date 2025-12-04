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

    // firstly processing attributes to add all attributes values in template
    for(const auto& setting : elementNode.children())
    {
        if(setting.name() == std::string("attribute"))
        {
            const auto attribNameAttrib = setting.attribute("name");

            if(!attribNameAttrib)
            {
                LOG_W(SGCORE_TAG,
                      "In UI document by path '{}': attribute of template '{}' must have name",
                      SGCore::Utils::toUTF8(inDocument->getPath().resolved().u16string()),
                      templateElement->m_name)
                continue;
            }

            auto& templateAttrib = templateElement->m_attributes[attribNameAttrib.as_string()];

            if(const auto defaultValueAttrib = setting.attribute("defaultValue"))
            {
                templateAttrib.m_defaultValue = SGCore::Utils::fromUTF8<char32_t>(defaultValueAttrib.as_string());
                templateAttrib.m_isRequired = false;
            }
        }
    }

    for(const auto& setting : elementNode.children())
    {
        if(setting.name() == std::string("elements"))
        {
            for(const auto& unrollNode : setting.children())
            {
                // todo: maybe pass not nullptr but templateElement
                // todo: make attributes storage in template element
                // todo: make function to get attribute or placeholder
                auto unrolledElement = UIDocument::processUIElement(*inDocument, nullptr, unrollNode);
                if(unrolledElement)
                {
                    templateElement->m_children.push_back(std::move(unrolledElement));
                }
            }
        }
    }

    inDocument->m_templates.push_back(std::static_pointer_cast<TemplateElement>(element));
}
