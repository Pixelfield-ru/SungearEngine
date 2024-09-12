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
            analyzeCurrentWordAndCharForTokens(currentToken, currentWord, curChar);
            currentWord = "";
            m_isExprStarted = false;

            continue;
        }

        if(curChar == ' ')
        {
            analyzeCurrentWordAndCharForTokens(currentToken, currentWord, curChar);
            currentWord = "";
            continue;
        }

        currentWord += curChar;

        analyzeCurrentWordAndCharForTokens(currentToken, currentWord, curChar);

        // always in the end of this cycle
        if(ci == templateFileText.size() - 1)
        {

        }
    }

    std::printf("end\n");
}

void SGCore::CodeGen::Generator::analyzeCurrentWordAndCharForTokens(std::shared_ptr<Lang::ASTToken>& currentToken,
                                                                    std::string& word, char curChar) noexcept
{
    Lang::Tokens currentWordTokenType = getTokenByName(word);
    Lang::Tokens currentCharTokenType = getTokenByName(std::string() + curChar);

    if(currentWordTokenType == Lang::Tokens::K_STARTEXPR)
    {
        m_isExprStarted = true;
        // auto newBranch = addToken(currentToken, Lang::Tokens::K_STARTEXPR);
        // currentToken = newBranch;
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_FOR && std::isspace(curChar))
    {
        currentToken = addToken(currentToken, Lang::Tokens::K_FOR);
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_ENDFOR && isSpace(curChar))
    {
        addToken(currentToken, Lang::Tokens::K_ENDFOR);
        gotoParent(currentToken);
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_IF && isSpace(curChar))
    {
        currentToken = addToken(currentToken, Lang::Tokens::K_IF);
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_ELSE && isSpace(curChar))
    {
        currentToken = addToken(currentToken, Lang::Tokens::K_ELSE);
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_ENDIF && isSpace(curChar))
    {
        if(currentToken->m_type == Lang::Tokens::K_ELSE)
        {
            gotoParent(currentToken);
        }
        addToken(currentToken, Lang::Tokens::K_ENDIF);
        currentToken = currentToken->m_parent;
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_IN && isSpace(curChar))
    {
        addToken(currentToken, Lang::Tokens::K_IN);
        word = "";
    }
    else if(m_isExprStarted && currentCharTokenType == Lang::Tokens::K_LPAREN)
    {
        //addToken(currentToken, Lang::Tokens::K_LPAREN);
        if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, ' ');
        }
        addToken(currentToken, Lang::Tokens::K_LPAREN);
        word = "";
    }
    else if(m_isExprStarted && currentCharTokenType == Lang::Tokens::K_RPAREN)
    {
        if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, ' ');
        }
        addToken(currentToken, Lang::Tokens::K_RPAREN);
        word = "";
    }
    else if(m_isExprStarted && currentCharTokenType == Lang::Tokens::K_DOT)
    {
        if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, ' ');
        }
        addToken(currentToken, Lang::Tokens::K_DOT);
        word = "";
    }
    else
    {
        if((isSpace(curChar) || curChar == '.') && m_isExprStarted && !word.empty())
        {
            addToken(currentToken, Lang::Tokens::K_VAR)->m_name = std::string(word.begin(), word.end());
        }
    }
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

bool SGCore::CodeGen::Generator::isSpace(char c) noexcept
{
    return c == ' ' || c == '\n';
}

void SGCore::CodeGen::Generator::gotoParent(std::shared_ptr<Lang::ASTToken>& token) const noexcept
{
    if(token && token->m_parent)
    {
        token = token->m_parent;
    }
}
