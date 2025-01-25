//
// Created by ilya on 23.02.24.
//

#include "UIDocument.h"
#include <spdlog/spdlog.h>
#include "SGCore/Logger/Logger.h"

#include "UINodesProcessors/UIElementNodeProcessor.h"
#include "UINodesProcessors/UIRootNodeProcessor.h"
#include "UINodesProcessors/UIIncludeNodeProcessor.h"
#include "UINodesProcessors/UIDivNodeProcessor.h"

void SGCore::UI::UIDocument::doLoad(const InterpolatedPath& path)
{
    const std::string u8Path = Utils::toUTF8(path.resolved().u16string());

    pugi::xml_parse_result parseResult = m_document.load_file(u8Path.c_str());
    
    if(!parseResult)
    {
        LOG_E(SGCORE_TAG, "Error while parsing XML document by path '{}': '{}'", u8Path, parseResult.description());
        return;
    }

    auto rootElement = processUIElement(m_document.child("xml"));

    if(!rootElement || rootElement->getType() != UIElementType::ET_ROOT)
    {
        LOG_E(SGCORE_TAG,
              "Can not process UIDocument: root element must <xml>.\n"
              "In XML file: '{}'",
              u8Path);
        return;
    }
}

void SGCore::UI::UIDocument::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

void SGCore::UI::UIDocument::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                              SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    doLoad(getPath());
}

SGCore::Ref<SGCore::UI::UIElement>
SGCore::UI::UIDocument::processUIElement(const pugi::xml_node& xmlNode) const noexcept
{
    if(!xmlNode)
    {
        LOG_E(SGCORE_TAG,
              "Can not process UIDocument correctly: invalid node.\n"
              "In XML file: '{}'",
              Utils::toUTF8(getPath().resolved().u16string()));

        return nullptr;
    }

    std::cout << "node: " << xmlNode.name() << std::endl;

    const UIElementType elementType = getUIElementTypeFromString(xmlNode.name());

    Ref<UIElement> outputElement;

    switch(elementType)
    {
        case UIElementType::ET_ROOT:
        {
            auto element = MakeRef<UIRoot>();

            UIElementNodeProcessor<UIElementType::ET_ROOT>::processElement(this, element, xmlNode);

            outputElement = element;

            break;
        }
        case UIElementType::ET_INCLUDE:
        {
            UIElementNodeProcessor<UIElementType::ET_INCLUDE>::processElement(this, nullptr, xmlNode);

            break;
        }
        case UIElementType::ET_DIV:
        {
            auto element = MakeRef<Div>();

            UIElementNodeProcessor<UIElementType::ET_DIV>::processElement(this, element, xmlNode);

            outputElement = element;

            break;
        }
        case UIElementType::ET_TEXT:break;
        case UIElementType::ET_UNKNOWN:
        {
            LOG_E(SGCORE_TAG,
                  "Can not process UIDocument correctly: unknown node '{}'.\n"
                  "In XML file: '{}'",
                  xmlNode.name(),
                  Utils::toUTF8(getPath().resolved().u16string()));

            return nullptr;
        }
    }

    if(!outputElement) return nullptr;

    outputElement->m_type = elementType;

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
