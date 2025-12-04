//
// Created by ilya on 23.02.24.
//

#include "UIDocument.h"
#include <spdlog/spdlog.h>

#include "Parser/XML/CSS/CSSFile.h"
#include "Elements/Text.h"
#include "Parser/UISourceTreeView.h"
#include "Parser/XML/XMLSourceTreeView.h"
#include "SGCore/Logger/Logger.h"

#include "UINodesProcessors/UIElementsProcessorsRegistry.h"
#include "UINodesProcessors/UIRootNodeProcessor.h"
#include "Parser/XML/CSS/CSSFile.h"

// TODO: Parameter to chose between siml and xml (and other ofc)
void SGCore::UI::UIDocument::doLoad(const InterpolatedPath& path)
{
    auto parser = XMLSourceTreeView();

    auto result = parser.parse(path);
    if (!result) return; // TODO
    m_rootElement = Ref(std::move(*result));
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



std::pair<ptrdiff_t, ptrdiff_t> SGCore::UI::UIDocument::getLocationInFile(const pugi::xml_node& xmlNode) noexcept
{
    const auto offset = xmlNode.offset_debug();

    const auto it = std::ranges::lower_bound(m_debugOffsets, offset);
    const size_t index = it - m_debugOffsets.begin();

    return std::make_pair(1 + index, index == 0 ? offset + 1 : offset - m_debugOffsets[index - 1]);
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



SGCore::AssetRef<SGCore::UI::Style>
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
