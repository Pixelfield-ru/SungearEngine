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

SGCore::AssetRef<SGCore::UI::Style> SGCore::UI::CSSFile::findStyle(const std::string& selector) const noexcept
{
    auto foundIt = std::find_if(m_styles.begin(), m_styles.end(), [&selector](const AssetRef<Style>& style) {
        return style->getSelector() == selector;
    });

    return foundIt == m_styles.end() ? nullptr : *foundIt;
}

const std::vector<SGCore::AssetRef<SGCore::UI::Style>>& SGCore::UI::CSSFile::getStyles() const noexcept
{
    return m_styles;
}

void SGCore::UI::CSSFile::doLoad(const InterpolatedPath& path)
{
    m_importedFiles.clear();
    m_styles.clear();

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
    for(const auto& importedFile : m_importedFiles)
    {
        importedFile->reloadFromDisk(loadPolicy, lazyLoadInThread);
    }

    doLoad(getPath());
}
