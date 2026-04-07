// Created by ilya on 23.02.24.

#include "UIDocument.h"

#include "Elements/Text.h"
#include "Parser/SIIML/Node/Node.h"
#include "Parser/XML/XMLSourceTreeView.h"

std::optional<std::string> readFile(const std::filesystem::path& filePath) {
    auto file = std::ifstream(filePath.string(), std::ios::binary);
    if (!file.is_open()) {
        return std::nullopt;
    }

    std::string content {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    file.close();

    return content;
}

// TODO: Parameter to chose between siml and xml (and other ofc)
void SGCore::UI::UIDocument::doLoad(const InterpolatedPath& path)
{
    auto fileContent = readFile(path.resolved());
    if (!fileContent) {
        LOG_E(SGCORE_TAG, "Can't load UI document: Can't read XML by path '{}'", path.resolved().string())
    }
    const Scope<UISourceTreeViewHandler> handler = MakeScope<SIML::NodeRootHandler>(*fileContent);
    if (const auto result =
        SGCore::UI::Deserialization::Deserializer<Scope<UIRoot>>::deserializeInto(
            handler->getRoot(), m_rootElement, SGCore::UI::Deserialization::DeserScope{nullptr}
        )
    ) {
        LOG_E(SGCORE_TAG, "Can't load UI document: {}", *result)
        return;
    }
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
