// Created by ilya on 23.02.24.

#include "UIDocument.h"

#include "Elements/Text.h"
#include "Parser/XML/XMLSourceTreeView.h"

// TODO: Parameter to chose between siml and xml (and other ofc)
void SGCore::UI::UIDocument::doLoad(const InterpolatedPath& path)
{
    // TODO
}

void SGCore::UI::UIDocument::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{
    // TODO
}

void SGCore::UI::UIDocument::doReloadFromDisk(AssetsLoadPolicy loadPolicy,
                                              Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    // TODO
}



std::pair<ptrdiff_t, ptrdiff_t> SGCore::UI::UIDocument::getLocationInFile(const pugi::xml_node& xmlNode) noexcept
{
    // TODO
    return std::make_pair(0, 0);
}

void SGCore::UI::UIDocument::applyDefaultStylesToNonStyledElements() noexcept
{
    // TODO
}

void SGCore::UI::UIDocument::applyDefaultStylesToNonStyledElementsImpl(const Ref<UIElement>& element) noexcept
{
    // TODO
}



SGCore::AssetRef<SGCore::UI::Style>
SGCore::UI::UIDocument::findStyle(const std::string& selectorName) const noexcept
{
    // TODO
    return SGCore::AssetRef<SGCore::UI::Style>();
}

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UIDocument::findElement(const std::string& elementName) const noexcept
{
    // TODO
    return Ref<UIElement>();
}

SGCore::Ref<SGCore::UI::TemplateElement> SGCore::UI::UIDocument::findTemplate(const std::string& templateName) const noexcept
{
    // TODO
    return Ref<TemplateElement>();
}
