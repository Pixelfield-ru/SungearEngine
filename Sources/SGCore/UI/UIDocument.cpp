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

void SGCore::UI::UIDocument::doLoad(const InterpolatedPath& path)
{
    for(auto cssFile : m_includedCSSFiles)
    {
        cssFile->getParentAssetManager()->removeAsset(cssFile);
    }

    m_includedCSSFiles.clear();

    const std::string u8Path = Utils::toUTF8(path.resolved().u16string());

    pugi::xml_parse_result parseResult = m_document.load_file(u8Path.c_str());
    
    if(!parseResult)
    {
        LOG_E(SGCORE_TAG, "Error while parsing XML document by path '{}': '{}'", u8Path, parseResult.description());
        return;
    }

    auto rootElement = processUIElement(m_document.child("xml"));

    if(!rootElement || rootElement->getTypeHash() != UIRootNodeProcessor::getNodeTypeHashStatic())
    {
        LOG_E(SGCORE_TAG,
              "Can not process UIDocument: root element must be <xml>.\n"
              "In XML file: '{}'",
              u8Path);
        return;
    }

    m_rootElement = nullptr;
    m_rootElement = std::static_pointer_cast<UIRoot>(rootElement);
}

void SGCore::UI::UIDocument::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{

}

void SGCore::UI::UIDocument::doReloadFromDisk(AssetsLoadPolicy loadPolicy,
                                              Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    doLoad(getPath());
}

SGCore::Ref<SGCore::UI::UIElement>
SGCore::UI::UIDocument::processUIElement(const pugi::xml_node& xmlNode) noexcept
{
    if(!xmlNode)
    {
        LOG_E(SGCORE_TAG,
              "Cannot process UIDocument correctly: invalid node.\n"
              "In XML file: '{}'",
              Utils::toUTF8(getPath().resolved().u16string()));

        return nullptr;
    }

    std::cout << "node: " << xmlNode.name() << std::endl;

    auto nodeProcessor = UIElementsProcessorsRegistry::getProcessor(xmlNode.name());
    if(!nodeProcessor)
    {
        LOG_W(SGCORE_TAG, "In UIDocument by path '{}': cannot process node '{}': processor for this node does not exist.", Utils::toUTF8(getPath().resolved().u16string()), xmlNode.name());
        return nullptr;
    }
    Ref<UIElement> outputElement = nodeProcessor->allocateElement();
    nodeProcessor->processElement(this, outputElement, xmlNode);
    if(outputElement)
    {
        outputElement->m_typeHash = nodeProcessor->getNodeTypeHash();
    }

    if(!outputElement) return nullptr;

    for(const pugi::xml_node& child : xmlNode)
    {
        auto processedChild = processUIElement(child);

        if(processedChild)
        {
            outputElement->m_children.push_back(processedChild);
        }
    }

    return outputElement;
}

SGCore::AssetRef<SGCore::UI::CSSSelector>
SGCore::UI::UIDocument::findSelector(const std::string& selectorName) const noexcept
{
    for(size_t i = 0; i < m_includedCSSFiles.size(); ++i)
    {
        auto selector = m_includedCSSFiles[i]->findSelector(selectorName);

        if(selector)
        {
            return selector;
        }
    }

    return nullptr;
}

Ref<SGCore::UI::UIElement> SGCore::UI::UIDocument::findElement(const std::string& elementName) const noexcept
{
    return m_rootElement->findElement(elementName);
}
