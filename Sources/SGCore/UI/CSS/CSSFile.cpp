//
// Created by stuka on 17.01.2025.
//
#include "CSSFile.h"

#include <antlr4-runtime.h>
#include "SGCore/UI/ANTLR4CSS3Generated/css3Lexer.h"
#include "SGCore/UI/ANTLR4CSS3Generated/css3Parser.h"
#include "ANTLRCSSListener.h"

#include "SGCore/Utils/FileUtils.h"

#include "SGCore/Memory/AssetManager.h"

SGCore::AssetRef<SGCore::UI::CSSSelector> SGCore::UI::CSSFile::findSelector(const std::string& name) const noexcept
{
    auto foundIt = std::find_if(m_selectors.begin(), m_selectors.end(), [&name](const AssetRef<CSSSelector>& selector) {
        return selector->getName() == name;
    });

    return foundIt == m_selectors.end() ? nullptr : *foundIt;
}

void SGCore::UI::CSSFile::doLoad(const InterpolatedPath& path)
{
    auto uiBodySelector = getParentAssetManager()->getOrAddAssetByPath<CSSSelector>(getPath() / "selectors" / "body");
    uiBodySelector->m_name = "body";
    m_selectors.push_back(uiBodySelector);

    antlr4::ANTLRInputStream input(FileUtils::readFile(path.resolved()));

    css3Lexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    css3Parser parser(&tokens);

    css3Parser::StylesheetContext* tree = parser.stylesheet();

    ANTLRCSSListener listener;
    listener.m_toCSSFile = this;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
    listener.resolvePseudos();
}

void SGCore::UI::CSSFile::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

void SGCore::UI::CSSFile::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                           SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{

}
