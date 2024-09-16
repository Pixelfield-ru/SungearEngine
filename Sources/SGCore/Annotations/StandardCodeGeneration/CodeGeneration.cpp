//
// Created by stuka on 10.09.2024.
//

#include "CodeGeneration.h"

SGCore::CodeGen::Generator::Generator()
{
    Lang::Type iterableType;
    iterableType.m_name = "iterable";
    m_currentTypes.push_back(iterableType);

    Lang::Type genericVectorType;
    genericVectorType.m_name = "generic_vector";
    genericVectorType.m_extends.push_back(iterableType);
    m_currentTypes.push_back(genericVectorType);

    Lang::Type boolType;
    boolType.m_name = "bool";

    Lang::Type stringType;
    stringType.m_name = "string";
    stringType.m_extends.push_back(iterableType);
    m_currentTypes.push_back(stringType);

    Lang::Type uintType;
    uintType.m_name = "uint";
    m_currentTypes.push_back(boolType);

    Lang::Type intType;
    intType.m_name = "int";
    m_currentTypes.push_back(intType);

    // adding annotations processor types
    Lang::Type cppStructType;
    cppStructType.m_name = "cpp_struct";
    cppStructType.m_members["name"] = stringType;
    cppStructType.m_members["filePath"] = stringType;
    cppStructType.m_members["templates"] = genericVectorType;
    cppStructType.m_members["fullNameWithTemplates"] = stringType;
    cppStructType.m_members["baseTypes"] = genericVectorType;
    cppStructType.m_members["derivedTypes"] = genericVectorType;
    cppStructType.m_members["members"] = genericVectorType;
    m_currentTypes.push_back(cppStructType);

    Lang::Type cppMemberType;
    cppMemberType.m_name = "cpp_struct_member";
    cppMemberType.m_members["setter"] = stringType;
    cppMemberType.m_members["getter"] = stringType;
    cppMemberType.m_members["hasSetter"] = boolType;
    cppMemberType.m_members["struct"] = cppStructType;
    m_currentTypes.push_back(cppMemberType);
}

void SGCore::CodeGen::Generator::addVariablesFromAnnotationsProcessor(SGCore::AnnotationsProcessor& annotationsProcessor) noexcept
{
    // todo: add variable 'structs' of type 'vector'
    m_AST->m_scope["structs"] = std::make_shared<Lang::Type>(*getTypeByName("generic_vector"));

    for(const auto& annotation : annotationsProcessor.getAnnotations())
    {

    }
}

std::string SGCore::CodeGen::Generator::generate(const std::filesystem::path& templateFile) noexcept
{
    std::string templateFileText = Utils::reduce(FileUtils::readFile(templateFile));
    // instead of EOF
    templateFileText += '\n';

    std::string outputString;

    buildAST(templateFileText);
    generateCodeUsingAST(m_AST, outputString);

    std::printf("end\n");

    return outputString;
}

void SGCore::CodeGen::Generator::buildAST(const std::string& templateFileText) noexcept
{
    if(templateFileText.empty())
    {
        m_AST->m_type = Lang::Tokens::K_EOF;

        return;
    }

    std::string currentWord;

    auto currentToken = m_AST;

    for(size_t ci = 0; ci < templateFileText.size(); ++ci)
    {
        const char& curChar = templateFileText[ci];

        if(m_currentCPPCodeToken && !m_skipCodeCopy)
        {
            m_currentCPPCodeToken->m_cppCode += curChar;
        }

        if(curChar == '\n')
        {
            if(m_currentCPPCodeToken)
            {
                currentToken->m_children.push_back(m_currentCPPCodeToken);
            }
            m_currentCPPCodeToken = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_CPP_CODE_LINE);

            m_skipCodeCopy = false;
            analyzeCurrentWordAndCharForTokens(currentToken, currentWord, templateFileText, ci);
            currentWord = "";
            m_isExprStarted = false;

            continue;
        }

        if(curChar == ' ')
        {
            analyzeCurrentWordAndCharForTokens(currentToken, currentWord, templateFileText, ci);
            currentWord = "";
            continue;
        }

        currentWord += curChar;

        analyzeCurrentWordAndCharForTokens(currentToken, currentWord, templateFileText, ci);

        // always in the end of this cycle
        /*if(ci == templateFileText.size() - 1)
        {

        }*/
    }
}

void SGCore::CodeGen::Generator::generateCodeUsingAST(const std::shared_ptr<Lang::ASTToken>& token,
                                                      std::string& outputString) noexcept
{
    for(const auto& child : token->m_children)
    {
        switch(child->m_type)
        {
            case Lang::Tokens::K_STARTEXPR:break;
            case Lang::Tokens::K_ENDEXPR:break;
            case Lang::Tokens::K_FOR:
            {
                generateCodeUsingAST(child, outputString);
                break;
            }
            case Lang::Tokens::K_ENDFOR:break;
            case Lang::Tokens::K_IN:break;
            case Lang::Tokens::K_IF:
            {
                generateCodeUsingAST(child, outputString);
                break;
            }
            case Lang::Tokens::K_ELSE:
            {
                generateCodeUsingAST(child, outputString);
                break;
            }
            case Lang::Tokens::K_ENDIF:break;
            case Lang::Tokens::K_START_PLACEMENT:
            {
                generateCodeUsingAST(child, outputString);
                break;
            }
            case Lang::Tokens::K_END_PLACEMENT:break;
            case Lang::Tokens::K_VAR:break;
            case Lang::Tokens::K_CPP_CODE_LINE:
            {
                outputString += child->m_cppCode;

                break;
            }
            case Lang::Tokens::K_LPAREN:break;
            case Lang::Tokens::K_RPAREN:break;
            case Lang::Tokens::K_LBLOCK:break;
            case Lang::Tokens::K_RBLOCK:break;
            case Lang::Tokens::K_DOT:break;
            case Lang::Tokens::K_UNKNOWN:break;
        }
    }
}

void SGCore::CodeGen::Generator::analyzeCurrentWordAndCharForTokens(std::shared_ptr<Lang::ASTToken>& currentToken,
                                                                    std::string& word,
                                                                    const std::string& text,
                                                                    std::size_t& curCharIdx,
                                                                    bool forceAddAsWord) noexcept
{
    size_t nextCharIdx = curCharIdx + 1;

    const char& curChar = text[curCharIdx];
    char nextChar = '\0';
    if(curCharIdx + 1 < text.size())
    {
        nextChar = text[curCharIdx + 1];
    }

    Lang::Tokens currentWordTokenType = getTokenTypeByName(word);
    Lang::Tokens currentCharTokenType = getTokenTypeByName(std::string() + curChar);

    if(currentWordTokenType == Lang::Tokens::K_STARTEXPR)
    {
        m_currentCPPCodeToken = nullptr;
        m_skipCodeCopy = true;
        m_isExprStarted = true;
        // auto newBranch = addToken(currentToken, Lang::Tokens::K_STARTEXPR);
        // currentToken = newBranch;
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_FOR && std::isspace(curChar))
    {
        if(m_isPlacementStarted)
        {
            // TODO: ERROR: "'for' keyword must be in line where starts expression (line that begins with '##')"
            return;
        }

        currentToken = addToken(currentToken, Lang::Tokens::K_FOR);
        currentToken->m_isExprToken = true;
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_ENDFOR && isSpace(curChar))
    {
        if(m_isPlacementStarted)
        {
            // TODO: ERROR: "'for' keyword must be in line where starts expression (line that begins with '##')"
            return;
        }

        addToken(currentToken, Lang::Tokens::K_ENDFOR)->m_isExprToken = true;
        gotoParent(currentToken);
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_IF && isSpace(curChar))
    {
        if(m_isPlacementStarted)
        {
            // TODO: ERROR: "'for' keyword must be in line where starts expression (line that begins with '##')"
            return;
        }

        currentToken = addToken(currentToken, Lang::Tokens::K_IF);
        currentToken->m_isExprToken = true;
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_ELSE && isSpace(curChar))
    {
        if(m_isPlacementStarted)
        {
            // TODO: ERROR: "'for' keyword must be in line where starts expression (line that begins with '##')"
            return;
        }

        currentToken = addToken(currentToken, Lang::Tokens::K_ELSE);
        currentToken->m_isExprToken = true;
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_ENDIF && isSpace(curChar))
    {
        if(m_isPlacementStarted)
        {
            // TODO: ERROR: "'for' keyword must be in line where starts expression (line that begins with '##')"
            return;
        }

        if(currentToken->m_type == Lang::Tokens::K_ELSE)
        {
            gotoParent(currentToken);
        }
        addToken(currentToken, Lang::Tokens::K_ENDIF)->m_isExprToken = true;
        gotoParent(currentToken);
        word = "";
    }
    else if(m_isExprStarted && currentWordTokenType == Lang::Tokens::K_IN && isSpace(curChar))
    {
        addToken(currentToken, Lang::Tokens::K_IN);
        word = "";
    }
    else if((m_isPlacementStarted || m_isExprStarted) && currentCharTokenType == Lang::Tokens::K_LPAREN)
    {
        //addToken(currentToken, Lang::Tokens::K_LPAREN);
        if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, text, nextCharIdx, true);
        }
        addToken(currentToken, Lang::Tokens::K_LPAREN);
        word = "";
    }
    else if((m_isPlacementStarted || m_isExprStarted) && currentCharTokenType == Lang::Tokens::K_RPAREN)
    {
        if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, text, nextCharIdx, true);
        }
        addToken(currentToken, Lang::Tokens::K_RPAREN);
        word = "";
    }
    else if((m_isPlacementStarted || m_isExprStarted) && currentCharTokenType == Lang::Tokens::K_DOT)
    {
        if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, text, nextCharIdx, true);
        }
        addToken(currentToken, Lang::Tokens::K_DOT);
        word = "";
    }
    else if(getTokenTypeByName(std::string({curChar, nextChar})) == Lang::Tokens::K_START_PLACEMENT)
    {
        m_currentCPPCodeToken->m_cppCode.erase(m_currentCPPCodeToken->m_cppCode.size() - 1);
        currentToken->m_children.push_back(m_currentCPPCodeToken);
        m_currentCPPCodeToken = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_CPP_CODE_LINE);

        ++curCharIdx;
        m_skipCodeCopy = true;
        m_isPlacementStarted = true;
        currentToken = addToken(currentToken, Lang::Tokens::K_START_PLACEMENT);
        word = "";
    }
    else if(currentWordTokenType == Lang::Tokens::K_END_PLACEMENT)
    {
        m_skipCodeCopy = false;
        m_isPlacementStarted = false;
        addToken(currentToken, Lang::Tokens::K_END_PLACEMENT);
        gotoParent(currentToken);
        word = "";
    }
    else
    {
        if((isSpace(curChar) || forceAddAsWord) && (m_isExprStarted || m_isPlacementStarted) && !word.empty())
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

SGCore::CodeGen::Lang::Tokens SGCore::CodeGen::Generator::getTokenTypeByName(const std::string& tokenName) const noexcept
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
    return c == ' ' || c == '\n' || c == '\0';
}

void SGCore::CodeGen::Generator::gotoParent(std::shared_ptr<Lang::ASTToken>& token) const noexcept
{
    auto lockedParent = token->m_parent.lock();
    if(token && lockedParent)
    {
        token = lockedParent;
    }
}

std::optional<SGCore::CodeGen::Lang::Type> SGCore::CodeGen::Generator::getTypeByName(const std::string& typeName) const noexcept
{
    auto it = std::find_if(m_currentTypes.begin(), m_currentTypes.end(), [&typeName](const Lang::Type& t) {
        return t.m_name == typeName;
    });

    if(it == m_currentTypes.end())
    {
        return std::nullopt;
    }

    return *it;
}

bool SGCore::CodeGen::Lang::Type::instanceof(const SGCore::CodeGen::Lang::Type& other) const noexcept
{
    if(m_extends.empty()) return false;

    auto it = std::find_if(m_extends.begin(), m_extends.end(), [&other](const Type& t) {
        return t.m_name == other.m_name;
    });

    if(it == m_extends.end())
    {
        for(const auto& t : m_extends)
        {
            return t.instanceof(other);
        }
    }

    return true;
}

bool SGCore::CodeGen::Lang::Type::instanceof(const std::string& typeName) const noexcept
{
    if(m_extends.empty()) return false;

    auto it = std::find_if(m_extends.begin(), m_extends.end(), [&typeName](const Type& t) {
        return t.m_name == typeName;
    });

    if(it == m_extends.end())
    {
        for(const auto& t : m_extends)
        {
            return t.instanceof(typeName);
        }
    }

    return true;
}

SGCore::CodeGen::Lang::Type* SGCore::CodeGen::Lang::Type::tryGetMember(const std::string& name) noexcept
{
    if(m_members.empty() && m_extends.empty()) return nullptr;

    auto it = m_members.find(name);

    if(it == m_members.end())
    {
        for(auto& t : m_extends)
        {
            return t.tryGetMember(name);
        }
    }

    return &it->second;
}

SGCore::CodeGen::Lang::Function* SGCore::CodeGen::Lang::Type::tryGetFunction(const std::string& name) noexcept
{
    if(m_functions.empty() && m_extends.empty()) return nullptr;

    auto it = m_functions.find(name);

    if(it == m_functions.end())
    {
        for(auto& t : m_extends)
        {
            return t.tryGetFunction(name);
        }
    }

    return &it->second;
}
