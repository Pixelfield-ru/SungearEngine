//
// Created by ilya on 23.02.24.
//

#include "UIDocument.h"
#include <spdlog/spdlog.h>

#include "CSS/CSSFile.h"
#include "Elements/Text.h"
#include "SGCore/Logger/Logger.h"

#include "UINodesProcessors/UIElementsProcessorsRegistry.h"
#include "UINodesProcessors/UIRootNodeProcessor.h"
#include "CSS/CSSFile.h"
#include "Elements/TemplateElement.h"

void SGCore::UI::UIDocument::doLoad(const InterpolatedPath& path)
{
    m_includedCSSFiles.clear();
    m_includedUIDocuments.clear();

    m_templates.clear();

    const std::string u8Path = Utils::toUTF8(path.resolved().u16string());

    pugi::xml_parse_result parseResult = m_document.load_file(u8Path.c_str());
    
    if(!parseResult)
    {
        LOG_E(SGCORE_TAG, "Error while parsing XML document by path '{}': '{}'", u8Path, parseResult.description());
        return;
    }

    auto rootElement = processUIElement(*this, nullptr, m_document.child("xml"));

    if(!rootElement || rootElement->getTypeHash() != UIRoot::getTypeHashStatic())
    {
        LOG_E(SGCORE_TAG,
              "Can not process UIDocument: root element must be <xml>.\n"
              "In XML file: '{}'",
              u8Path);
        return;
    }

    m_rootElement = nullptr;
    m_rootElement = std::static_pointer_cast<UIRoot>(rootElement);

    applyDefaultStylesToNonStyledElements();
}

void SGCore::UI::UIDocument::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{

}

void SGCore::UI::UIDocument::doReloadFromDisk(AssetsLoadPolicy loadPolicy,
                                              Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    for(auto cssFile : m_includedCSSFiles)
    {
        cssFile->reloadFromDisk(loadPolicy, lazyLoadInThread);
    }

    for(auto uiDocument : m_includedUIDocuments)
    {
        uiDocument->reloadFromDisk(loadPolicy, lazyLoadInThread);
    }

    doLoad(getPath());
}

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UIDocument::processUIElement(UIDocument& inDocument,
                                                                            const Ref<UIElement>& parent,
                                                                            const pugi::xml_node& xmlNode) noexcept
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

    auto nodeProcessor = UIElementsProcessorsRegistry::getProcessor(xmlNode.name());
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
    Ref<UIElement> outputElement = nodeProcessor->allocateElement();

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
    if(outputElement->getTypeHash() == TemplateElement::getTypeHashStatic())
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

void SGCore::UI::UIDocument::applyDefaultStylesToNonStyledElements() noexcept
{
    applyDefaultStylesToNonStyledElementsImpl(m_rootElement);
}

void SGCore::UI::UIDocument::applyDefaultStylesToNonStyledElementsImpl(const Ref<UIElement>& element) noexcept
{
    if(!element->m_mainStyle)
    {
        for(const auto& cssFile : m_includedCSSFiles)
        {
            for(const auto& style : cssFile->getStyles())
            {
                if(element->getTypeHash() == style->getSelectorHash())
                {
                    element->m_mainStyle = style;
                }
            }
        }
    }

    for(const auto& child : element->m_children)
    {
        applyDefaultStylesToNonStyledElementsImpl(child);
    }
}

SGCore::AssetRef<SGCore::UI::CSSStyle>
SGCore::UI::UIDocument::findStyle(const std::string& selectorName) const noexcept
{
    if(selectorName.empty()) return nullptr;

    for(auto&& cssFile : m_includedCSSFiles)
    {
        if(auto selector = cssFile->findStyle(selectorName))
        {
            return selector;
        }
    }

    return nullptr;
}

Ref<SGCore::UI::UIElement> SGCore::UI::UIDocument::findElement(const std::string& elementName) const noexcept
{
    if(elementName.empty()) return nullptr;

    return m_rootElement->findElement(elementName);
}

Ref<SGCore::UI::TemplateElement> SGCore::UI::UIDocument::findTemplate(const std::string& templateName) const noexcept
{
    if(templateName.empty()) return nullptr;

    for(auto&& uiTemplate : m_templates)
    {
        if(uiTemplate->m_name == templateName)
        {
            return uiTemplate;
        }
    }

    for(auto&& uiDocument : m_includedUIDocuments)
    {
        if(auto uiTemplate = uiDocument->findTemplate(templateName))
        {
            return uiTemplate;
        }
    }

    return nullptr;
}
