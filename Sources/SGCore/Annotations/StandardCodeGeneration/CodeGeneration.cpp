//
// Created by stuka on 10.09.2024.
//

#include "CodeGeneration.h"

void SGCore::CodeGen::Generator::generate(SGCore::AnnotationsProcessor& annotationsProcessor,
                                          const std::filesystem::path& templateFile,
                                          const std::filesystem::path& outputFile) noexcept
{
    std::string templateFileText = Utils::reduce(FileUtils::readFile(templateFile));

    if(templateFileText.empty())
    {
        m_AST->m_type = Lang::Tokens::K_EOF;

        return;
    }

    std::string currentWord;

    auto currentToken = m_AST;

    for(size_t ci = 0; ci < templateFileText.size(); ++ci)
    {
        char curChar = templateFileText[ci];

        if(curChar == '\n')
        {
            addToken(currentToken, Lang::Tokens::K_LINEEND);
            //currentToken = currentToken->m_parent;
            currentWord = "";

            continue;
        }

        if(curChar == ' ')
        {
            currentWord = "";
            continue;
        }

        currentWord += curChar;

        Lang::Tokens currentTokenType = getTokenByName(currentWord);

        if(currentTokenType == Lang::Tokens::K_STARTEXPR)
        {
            std::printf("startexpr\n");
            auto newBranch = addToken(currentToken, Lang::Tokens::K_STARTEXPR);
            // currentToken = newBranch;
            currentWord = "";
        }

        if(currentTokenType == Lang::Tokens::K_FOR)
        {
            auto newBranch = addToken(currentToken, Lang::Tokens::K_FOR);
            currentToken = newBranch;
            currentWord = "";
        }

        if(currentTokenType == Lang::Tokens::K_ENDFOR)
        {
            addToken(currentToken, Lang::Tokens::K_ENDFOR);
            currentToken = currentToken->m_parent;
            currentWord = "";
        }

        if(currentTokenType == Lang::Tokens::K_IF)
        {
            auto newBranch = addToken(currentToken, Lang::Tokens::K_IF);
            currentToken = newBranch;
            currentWord = "";
        }

        if(currentTokenType == Lang::Tokens::K_ENDIF)
        {
            addToken(currentToken, Lang::Tokens::K_ENDIF);
            currentToken = currentToken->m_parent;
            currentWord = "";
        }

        // always in the end of this cycle
        if(ci == templateFileText.size() - 1)
        {

        }
    }

    std::printf("end\n");
}

std::shared_ptr<SGCore::CodeGen::Lang::ASTToken>
SGCore::CodeGen::Generator::addToken(const std::shared_ptr<Lang::ASTToken>& toToken,
                                     Lang::Tokens tokenType) noexcept
{
    auto newToken = std::make_shared<Lang::ASTToken>(tokenType);
    newToken->m_parent = toToken;
    toToken->m_children.push_back(newToken);

    return newToken;
}

SGCore::CodeGen::Lang::Tokens SGCore::CodeGen::Generator::getTokenByName(const std::string& tokenName) const noexcept
{
    const auto foundIt = m_tokensLookup.find(tokenName);
    if(foundIt == m_tokensLookup.end())
    {
        return Lang::Tokens::K_UNKNOWN;
    }

    return foundIt->second;
}
