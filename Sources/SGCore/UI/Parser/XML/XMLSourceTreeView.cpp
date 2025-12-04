//
// Created by UserPC on 03.12.2025.
//

#include "XMLSourceTreeView.h"

#include "SGCore/UI/UIDocument.h"

std::optional<std::string> readFile(const std::filesystem::path& filePath) noexcept
{
    std::ifstream file(filePath, std::ios::binary);
    if(!file.is_open())
    {
        return std::nullopt;
    }

    std::string fileContent;

    // Читаем весь файл сразу в строку
    fileContent.assign(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());

    return fileContent;
}

auto processUIElement(SGCore::UI::UIDocument& inDocument, const SGCore::Ref<SGCore::UI::UIElement>& parent, const pugi::xml_node& xmlNode) noexcept
{
    if(!xmlNode)
    {
        LOG_E(SGCORE_TAG,
              "Cannot process UIDocument correctly: invalid node.\n"
              "In XML file: '{}'",
              Utils::toUTF8(inDocument.getPath().resolved().u16string()));

        return nullptr;
    }

    std::cout << "node: " << xmlNode.name() << std::endl;

    const auto foundTemplate = inDocument.findTemplate(xmlNode.name());
    const bool usesTemplate = foundTemplate != nullptr;

    auto nodeProcessor = SGCore::UI::UIElementsProcessorsRegistry::getProcessor(xmlNode.name());
    if(!nodeProcessor)
    {
        // if template then node processor is not required. in this case template will just unroll without intermediate element
        if(usesTemplate)
        {
            if(parent)
            {
                UITemplateUsageProcessor::processElement(foundTemplate, &inDocument, parent, xmlNode);
            }

            return nullptr;
        }

        LOG_W(SGCORE_TAG, "In UIDocument by path '{}': cannot process node '{}': processor for this node does not exist.", Utils::toUTF8(inDocument.getPath().resolved().u16string()), xmlNode.name());
        return nullptr;
    }

    // NOTE (for template): creating user-defined intermediate element if element uses template
    SGCore::Ref<SGCore::UI::UIElement> outputElement = nodeProcessor->allocateElement();
    if(outputElement)
    {
        outputElement->m_parent = parent;
    }

    if(!usesTemplate)
    {
        // assigning place
        for(const auto placeNode : xmlNode.children("place"))
        {
            if(const auto placeNameAttrib = placeNode.attribute("name"))
            {
                outputElement->m_places.insert(placeNameAttrib.as_string());
            }
            else
            {
                LOG_W(SGCORE_TAG, "Place node must have 'name' attribute")
            }
        }
    }

    // if current node uses template
    if(usesTemplate)
    {
        // then unroll template in user-defined intermediate element
        UITemplateUsageProcessor::processElement(foundTemplate, &inDocument, outputElement, xmlNode);
    }

    nodeProcessor->processElement(&inDocument, outputElement, xmlNode);

    // if element uses template then we are not processing children because they were processed in UITemplateUsageProcessor
    if(usesTemplate)
    {
        return outputElement;
    }

    if(!outputElement) return nullptr;
    // if we proceed <template> tag then do not add this template to elements tree
    if(outputElement->getTypeHash() == SGCore::UI::TemplateElement::getTypeHashStatic())
    {
        return nullptr;
    }

    for(const pugi::xml_node& child : xmlNode)
    {
        auto processedChild = processUIElement(inDocument, outputElement, child);

        if(processedChild)
        {
            outputElement->m_children.push_back(processedChild);
        }
    }

    return outputElement;
}

std::optional<std::unique_ptr<SGCore::UI::UIRoot>> SGCore::UI::XMLSourceTreeView::parse(InterpolatedPath path) {
    const auto xmlContent = readFile(path.resolved());

    if(!xmlContent)
    {
        LOG_E(SGCORE_TAG,
              "In UI document: Cannot read XML file by path '{}'",
              SGCore::Utils::toUTF8(path.resolved().u16string()))
        return std::nullopt;
    }

    // pugi::xml_parse_result parseResult = m_document.load_string(xmlContent->c_str());

    auto document = pugi::xml_document();
    pugi::xml_parse_result parseResult = document.load_string(xmlContent.value().c_str());

    if(!parseResult)
    {
        LOG_E(SGCORE_TAG, "Error while parsing XML document by path '{}': '{}'", u8Path, parseResult.description());
        return;
    }

    auto rootElement = processUIElement(*this, nullptr, document.child("xml"));

    if(!rootElement || rootElement->getTypeHash() != UIRoot::getTypeHashStatic())
    {
        LOG_E(SGCORE_TAG,
              "Can not process UIDocument: root element must be <xml>.\n"
              "In XML file: '{}'",
              u8Path);
        return;
    }

    return std::static_pointer_cast<UIRoot>(rootElement);

    // applyDefaultStylesToNonStyledElements();
}

