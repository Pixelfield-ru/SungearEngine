//
// Created by stuka on 10.09.2024.
//

#include <SGCore/Utils/FileUtils.h>
#include "CodeGeneration.h"

#include "SGCore/Logger/Logger.h"
#include "SGCore/MetaInfo/MetaInfo.h"

SGCore::CodeGen::Generator::Generator()
{
    // creating standard types
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
        m_currentTypes.push_back(uintType);

        Lang::Type intType;
        intType.m_name = "int";
        m_currentTypes.push_back(intType);

        Lang::Function hasMemberFunc;
        hasMemberFunc.m_name = "hasMember";
        hasMemberFunc.m_arguments.push_back({
                                                    .m_name = "name",
                                                    .m_isNecessary = true,
                                                    .m_acceptableType = stringType
                                            });
        hasMemberFunc.m_functor = [hasMemberFunc](const Lang::Type& owner, Lang::Variable* operableVariable, const size_t& curLine,
                                     std::string& outputText, const std::vector<Lang::FunctionArgument>& args) {
            if(args.size() != 1)
            {
                LOG_E(SGCORE_TAG,
                      "Error in CodeGenerator (line: {}): in function 'hasMember': bad count of passed arguments (required: {}, provided: {})",
                      curLine, hasMemberFunc.m_arguments.size(), args.size());
                // outputValue = false;
                return false;
            }

            if(args[0].m_name != "memberName")
            {
                LOG_E(SGCORE_TAG,
                      "Error in CodeGenerator (line: {}): in function 'hasMember': unknown argument '{}'",
                      curLine, args[0].m_name);
                return false;
            }

            if(args[0].m_acceptableType.m_name != "string")
            {
                LOG_E(SGCORE_TAG,
                      "Error in CodeGenerator (line: {}): in function 'hasMember': type of argument '{}' is not string",
                      curLine, args[0].m_name);
                return false;
            }

            const auto& memberNameArg = std::any_cast<std::string>(args[0].m_data);

            return owner.m_members.contains(memberNameArg);
        };

        // adding annotations processor types
        Lang::Type cppStructType;
        cppStructType.m_name = "cpp_struct";
        cppStructType.m_members["fullName"] = stringType;
        cppStructType.m_members["filePath"] = stringType;
        cppStructType.m_members["templates"] = genericVectorType;
        cppStructType.m_members["fullNameWithTemplates"] = stringType;
        cppStructType.m_members["baseTypes"] = genericVectorType;
        cppStructType.m_members["derivedTypes"] = genericVectorType;
        cppStructType.m_members["members"] = genericVectorType;
        cppStructType.m_functions["hasMember"] = hasMemberFunc;
        m_currentTypes.push_back(cppStructType);

        Lang::Type cppMemberType;
        cppMemberType.m_name = "cpp_struct_member";
        cppMemberType.m_members["name"] = stringType;
        cppMemberType.m_members["setter"] = stringType;
        cppMemberType.m_members["getter"] = stringType;
        cppMemberType.m_members["hasSetter"] = boolType;
        cppMemberType.m_members["hasGetter"] = boolType;
        cppMemberType.m_members["struct"] = cppStructType;
        cppMemberType.m_functions["hasMember"] = hasMemberFunc;
        m_currentTypes.push_back(cppMemberType);
    }

    // adding structs from meta info
    m_AST->m_scope["structs"] = std::make_shared<Lang::Variable>(*getTypeByName("generic_vector"));

    size_t currentStructIdx = 0;
    for(auto& metaStruct : MetaInfo::getMeta()["structs"])
    {
        auto newStruct = std::make_shared<Lang::Variable>(*getTypeByName("cpp_struct"));
        (*newStruct)["members"] = Lang::Variable(*getTypeByName("generic_vector"));

        (*newStruct)["fullName"].m_insertedValue = metaStruct["fullName"].getValue();

        auto& structMembers = metaStruct["members"];
        auto& structTemplateArgs = metaStruct["template_args"];

        // doing some actions with templates ==================
        // structFullNameWithTemplates == structName or structFullNameWithTemplates == structName<allTemplateArgs> (if template exists)
        std::string structFullNameWithTemplates = metaStruct["fullName"].getValue();
        const bool hasTemplateArgs = !structTemplateArgs.getChildren().empty();
        if(hasTemplateArgs)
        {
            structFullNameWithTemplates += '<';
        }
        for(const auto& structTemplateArg : structTemplateArgs.getChildren())
        {
            structFullNameWithTemplates += structTemplateArg->first;
        }
        if(hasTemplateArgs)
        {
            structFullNameWithTemplates += '>';
        }
        // =====================================================

        for(auto& memberIt : structMembers.getChildren())
        {
            auto& memberName = memberIt->first;
            auto& member = memberIt->second;

            // getting variables from meta
            const bool hasSetter = member.hasChild("setter");
            const bool hasGetter = member.hasChild("getter");
            const std::string serializableName = member.hasChild("serializableName") ? member["serializableName"].getValue() : memberName;
            const std::string setter = hasSetter ? member["setter"].getValue() : memberName;
            const std::string getter = hasSetter ? member["getter"].getValue() : memberName;

            // creating new generator variable
            (*newStruct)["members"][memberName] = Lang::Variable(*getTypeByName("cpp_struct_member"));
            auto& newGenMember = (*newStruct)["members"][memberName];

            newGenMember["name"].m_insertedValue = memberName;

            newGenMember["hasSetter"].m_insertedValue = hasSetter ? "true" : "false";
            newGenMember["hasGetter"].m_insertedValue = hasGetter ? "true" : "false";

            newGenMember["setter"].m_insertedValue = setter;
            newGenMember["getter"].m_insertedValue = getter;

            newGenMember["serializableName"].m_insertedValue = serializableName;

            newGenMember.setMemberPtr("struct", newStruct);
        }

        (*newStruct)["fullNameWithTemplates"].m_insertedValue = structFullNameWithTemplates;
        (*newStruct)["filePath"].m_insertedValue = metaStruct["filePath"].getValue();

        m_AST->m_scope["structs"]->setMemberPtr(std::to_string(currentStructIdx), newStruct);

        ++currentStructIdx;
        // newStruct.m_members["fullNameWithTemplates"].m_insertedValue = structFullNameWithTemplates;
    }
}

/*void SGCore::CodeGen::Generator::addVariablesFromAnnotationsProcessor(SGCore::AnnotationsProcessor& annotationsProcessor) noexcept
{
    currentStructIdx = 0;
    for(auto& s : detectedStructs)
    {
        for(auto& member : s.second.m_members)
        {
            member.second.m_members["struct"] = s.second;
        }

        m_AST->m_scope["structs"]->m_members[std::to_string(currentStructIdx)] = s.second;
        ++currentStructIdx;
    }
}*/

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

        // checking for comment
        if(curChar == '\\')
        {
            if(ci + 1 < templateFileText.size())
            {
                if(templateFileText[ci + 1] == '\\')
                {
                    m_currentCommentType = CommentType::ONE_LINE;
                }
                else if(templateFileText[ci + 1] == '*')
                {
                    m_currentCommentType = CommentType::MULTILINE;
                }
            }

            if(ci - 1 > 0)
            {
                if(templateFileText[ci - 1] == '*')
                {
                    m_currentCommentType = CommentType::NO_COMMENT;
                    continue;
                }
            }
        }

        if(curChar == '\n')
        {
            if(m_currentCommentType == CommentType::ONE_LINE)
            {
                m_currentCommentType = CommentType::NO_COMMENT;
            }
        }

        // skipping characters if there is comment lines (comments: \\ - one line, \* - start multiline comment, *\ - end multiline comment)
        if(m_currentCommentType != CommentType::NO_COMMENT)
        {
            continue;
        }

        if(m_currentCPPCodeToken && !m_skipCodeCopy)
        {
            m_currentCPPCodeToken->m_cppCode += curChar;
        }

        if(m_writeCharSeq)
        {
            m_currentCharSeqToken->m_cppCode += curChar;
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
    size_t currentChildTokenIdx = 0;
    for(const auto& child : token->m_children)
    {
        /*switch(child->m_type)
        {
            case Lang::Tokens::K_VAR: break;
            // saving m_currentUsedVariable if dot is after variable in template file (appeal to a member)
            case Lang::Tokens::K_DOT: break;
            default:
            {
                if(m_currentUsedVariable)
                {
                    // placing variable
                    outputString += m_currentUsedVariable->m_insertedValue;
                }

                m_currentUsedVariable = nullptr;

                break;
            }
        }*/

        switch(child->m_type)
        {
            case Lang::Tokens::K_STARTEXPR:break;
            case Lang::Tokens::K_ENDEXPR:break;
            case Lang::Tokens::K_FOR:
            {
                // getting last token in variable tokens sequence (example: var0.var1.var2)
                VariableFindResult tokenAndVariableToForIn = getLastVariable(child, 2);

                // getting variable that we will use to bind to every element in iterable variable
                auto bindableVariableToken = child->m_children[0];
                if(tokenAndVariableToForIn.m_token->m_type != Lang::Tokens::K_VAR)
                {
                    // todo: make error that token is not variable

                    LOG_E(SGCORE_TAG, "Error in CodeGenerator: (for cycle) trying to iterate not variable type.")
                    ++currentChildTokenIdx;
                    continue;
                }

                if(bindableVariableToken->m_type != Lang::Tokens::K_VAR)
                {
                    // todo: make error that token is not variable

                    LOG_E(SGCORE_TAG, "Error in CodeGenerator: (for cycle) trying to bind not variable type.")
                    ++currentChildTokenIdx;
                    continue;
                }

                auto newBindableVariable = std::make_shared<Lang::Variable>();
                token->m_scope[bindableVariableToken->m_name] = newBindableVariable;

                if(!tokenAndVariableToForIn.m_variable)
                {
                    // todo: make error that variable does not exist

                    LOG_E(SGCORE_TAG, "Error in CodeGenerator: (for cycle) trying to iterate variable '{}' that does not exist.", tokenAndVariableToForIn.m_token->m_name)
                    ++currentChildTokenIdx;
                    continue;
                }

                if(!tokenAndVariableToForIn.m_variable->getTypeInfo().instanceof("iterable"))
                {
                    // todo: make error that variable is not instance of iterable

                    LOG_E(SGCORE_TAG, "Error in CodeGenerator: trying to iterate variable '{}' that is not instance of 'iterable'.", tokenAndVariableToForIn.m_token->m_name)
                    ++currentChildTokenIdx;
                    continue;
                }

                LOG_I(SGCORE_TAG, "CodeGenerator: iterating variable '{}'...", tokenAndVariableToForIn.m_token->m_name)

                size_t iterationsCount = tokenAndVariableToForIn.m_variable->membersCount();

                size_t curElemIdx = 0;
                for(const auto& elem : tokenAndVariableToForIn.m_variable->getMembers())
                {
                    token->m_scope[bindableVariableToken->m_name] = elem.second;

                    LOG_I(SGCORE_TAG, "CodeGenerator: iterating variable '{}'. Iteration '{}'", tokenAndVariableToForIn.m_token->m_name, curElemIdx)
                    generateCodeUsingAST(child, outputString);

                    ++curElemIdx;
                }

                break;
            }
            case Lang::Tokens::K_ENDFOR:break;
            case Lang::Tokens::K_IN: break;
            case Lang::Tokens::K_IF:
            {
                // getting last variable in sequence of variables
                VariableFindResult tokenAndVar = getLastVariable(child, 0);

                if(tokenAndVar.m_token->m_type != Lang::Tokens::K_VAR)
                {
                    LOG_E(SGCORE_TAG, "Error in CodeGenerator: (if branch) trying to test variable that is not variable type (maybe keyword).")
                    ++currentChildTokenIdx;
                    continue;
                }

                std::any funcOutput;

                if(tokenAndVar.m_variable)
                {
                    if(tokenAndVar.m_variable->getTypeInfo().m_name != "bool")
                    {
                        LOG_E(SGCORE_TAG,
                              "Error in CodeGenerator: (if branch) trying to test variable that is not boolean type.")
                        ++currentChildTokenIdx;
                        continue;
                    }
                }
                else if(tokenAndVar.m_function)
                {
                    bool lparenFound = false;
                    bool rparenFound = false;

                    std::vector<Lang::FunctionArgument> funcArguments;

                    // analyzing next token
                    for(size_t j = tokenAndVar.m_tokenIndex; j < child->m_children.size(); ++j)
                    {
                        const auto& curFuncToken = child->m_children[j];

                        if(curFuncToken->m_type == Lang::Tokens::K_LPAREN)
                        {
                            lparenFound = true;
                            continue;
                        }
                        if(curFuncToken->m_type == Lang::Tokens::K_RPAREN)
                        {
                            rparenFound = true;
                            continue;
                        }
                    }

                    if(!lparenFound || !rparenFound)
                    {
                        LOG_E(SGCORE_TAG, "Error in CodeGenerator: in function '{}': parens mismatch.", tokenAndVar.m_function->m_name);
                    }

                    if(auto lockedParentLockedVar = tokenAndVar.m_function->m_parentVariable.lock())
                    {
                        funcOutput = tokenAndVar.m_function->m_functor(lockedParentLockedVar->getTypeInfo(),
                                                                       lockedParentLockedVar.get(), 0, outputString,
                                                                       funcArguments);

                        if(!funcOutput.has_value() || funcOutput.type() != typeid(bool))
                        {
                            LOG_E(SGCORE_TAG, "Error in CodeGenerator: return type of function '{}' is not 'bool' type.",
                                  tokenAndVar.m_function->m_name);
                            continue;
                        }
                    }
                }
                else
                {
                    ++currentChildTokenIdx;
                    continue;
                }

                std::shared_ptr<Lang::ASTToken> elseToken;

                // finding 'else' branch
                for(size_t i = 0; i < child->m_children.size(); ++i)
                {
                    const auto& childOfChild = child->m_children[i];

                    if(childOfChild->m_type == Lang::Tokens::K_ELSE)
                    {
                        if(i != child->m_children.size() - 2)
                        {
                            LOG_E(SGCORE_TAG, "Error in CodeGenerator: (if branch) 'else' branch is not last in 'if'-'else if' sequence.")
                            break;
                        }

                        // else branch was successfully found and 'else' branch is last in 'if'-'else if' sequence
                        elseToken = childOfChild;
                        break;
                    }
                }

                if((tokenAndVar.m_variable && tokenAndVar.m_variable->m_insertedValue == "true") && std::any_cast<bool>(funcOutput))
                {
                    generateCodeUsingAST(child, outputString);
                }
                else
                {
                    if(elseToken)
                    {
                        generateCodeUsingAST(elseToken, outputString);
                    }
                }

                break;
            }
            case Lang::Tokens::K_ELSE: break; // nothing to do
            case Lang::Tokens::K_ENDIF:break; // nothing to do
            case Lang::Tokens::K_START_PLACEMENT:
            {
                VariableFindResult tokenAndVar = getLastVariable(child, 0);

                if(tokenAndVar.m_variable)
                {
                    outputString += tokenAndVar.m_variable->m_insertedValue;
                }
                // generateCodeUsingAST(child, outputString);
                break;
            }
            case Lang::Tokens::K_END_PLACEMENT:break;
            case Lang::Tokens::K_VAR:
            {
                /*if(m_currentUsedVariable)
                {
                    m_currentUsedVariable = &m_currentUsedVariable->m_members[child->m_name];
                }
                else
                {
                    auto foundVar = child->getScopeVariable(child->m_name);
                    if(!foundVar)
                    {
                        // todo: make error

                        ++currentChildTokenIdx;
                        continue;
                    }

                    m_currentUsedVariable = foundVar.get();
                }*/

                break;
            }
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

        ++currentChildTokenIdx;
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

    if(currentWordTokenType == Lang::Tokens::K_STARTEXPR && !m_writeCharSeq)
    {
        m_currentCPPCodeToken = nullptr;
        m_skipCodeCopy = true;
        m_isExprStarted = true;
        // auto newBranch = addToken(currentToken, Lang::Tokens::K_STARTEXPR);
        // currentToken = newBranch;
        word = "";
    }
    else if(m_isExprStarted && !m_writeCharSeq && currentWordTokenType == Lang::Tokens::K_FOR && std::isspace(curChar))
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
    else if(m_isExprStarted && !m_writeCharSeq && currentWordTokenType == Lang::Tokens::K_ENDFOR && isSpace(curChar))
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
    else if(m_isExprStarted && !m_writeCharSeq && currentWordTokenType == Lang::Tokens::K_IF && isSpace(curChar))
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
    else if(m_isExprStarted && !m_writeCharSeq && currentWordTokenType == Lang::Tokens::K_ELSE && isSpace(curChar))
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
    else if(m_isExprStarted && !m_writeCharSeq && currentWordTokenType == Lang::Tokens::K_ENDIF && isSpace(curChar))
    {
        if(m_isPlacementStarted)
        {
            // TODO: ERROR: "'for' keyword must be in line where starts expression (line that begins with '##')"
            return;
        }

        if(currentToken->m_type == Lang::Tokens::K_ELSE && !m_writeCharSeq)
        {
            gotoParent(currentToken);
        }
        addToken(currentToken, Lang::Tokens::K_ENDIF)->m_isExprToken = true;
        gotoParent(currentToken);
        word = "";
    }
    else if(m_isExprStarted && !m_writeCharSeq && currentWordTokenType == Lang::Tokens::K_IN && isSpace(curChar))
    {
        addToken(currentToken, Lang::Tokens::K_IN);
        word = "";
    }
    else if((m_isPlacementStarted || m_isExprStarted) && !m_writeCharSeq && currentCharTokenType == Lang::Tokens::K_LPAREN)
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
    else if((m_isPlacementStarted || m_isExprStarted) && !m_writeCharSeq && currentCharTokenType == Lang::Tokens::K_RPAREN)
    {
        if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, text, nextCharIdx, true);
        }
        addToken(currentToken, Lang::Tokens::K_RPAREN);
        word = "";
    }
    else if((m_isPlacementStarted || m_isExprStarted) && !m_writeCharSeq && currentCharTokenType == Lang::Tokens::K_DOT)
    {
        if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, text, nextCharIdx, true);
        }
        addToken(currentToken, Lang::Tokens::K_DOT);
        word = "";
    }
    else if((m_isPlacementStarted || m_isExprStarted) && !m_writeCharSeq && currentCharTokenType == Lang::Tokens::K_COLON)
    {
        if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, text, nextCharIdx, true);
        }
        addToken(currentToken, Lang::Tokens::K_COLON);
        word = "";
    }
    else if((m_isPlacementStarted || m_isExprStarted) && currentCharTokenType == Lang::Tokens::K_QUOTE)
    {
        /*if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, text, nextCharIdx, true);
        }*/

        if(!m_writeCharSeq)
        {
            m_currentCharSeqToken = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_CHAR_SEQ);
        }

        // if we are writing char seq now then adding current char seq token to AST
        if(m_writeCharSeq)
        {
            // removing last character (because it is redundant character: it can be character of K_QUOTE)
            m_currentCharSeqToken->m_cppCode.erase(m_currentCharSeqToken->m_cppCode.size() - 1);

            currentToken->m_children.push_back(m_currentCharSeqToken);
        }

        m_writeCharSeq = !m_writeCharSeq;
        word = "";
    }
    else if(getTokenTypeByName(std::string({curChar, nextChar})) == Lang::Tokens::K_START_PLACEMENT && !m_writeCharSeq)
    {
        // removing last character (because it is redundant character: it can be character of next line)
        m_currentCPPCodeToken->m_cppCode.erase(m_currentCPPCodeToken->m_cppCode.size() - 1);
        currentToken->m_children.push_back(m_currentCPPCodeToken);
        m_currentCPPCodeToken = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_CPP_CODE_LINE);

        ++curCharIdx;
        m_skipCodeCopy = true;
        m_isPlacementStarted = true;
        currentToken = addToken(currentToken, Lang::Tokens::K_START_PLACEMENT);
        word = "";
    }
    else if(currentWordTokenType == Lang::Tokens::K_END_PLACEMENT && !m_writeCharSeq)
    {
        m_skipCodeCopy = false;
        m_isPlacementStarted = false;
        addToken(currentToken, Lang::Tokens::K_END_PLACEMENT);
        gotoParent(currentToken);
        word = "";
    }
    else
    {
        if((isSpace(curChar) || forceAddAsWord) && (m_isExprStarted || m_isPlacementStarted) && !m_writeCharSeq && !word.empty())
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
    // copying scope
    newToken->m_scope.insert(toToken->m_scope.begin(), toToken->m_scope.end());
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

SGCore::CodeGen::Generator::VariableFindResult
SGCore::CodeGen::Generator::getLastVariable(const std::shared_ptr<Lang::ASTToken>& from,
                                            const size_t& begin) noexcept
{
    VariableFindResult out;
    bool isDotWas = false;

    auto& outToken = out.m_token;
    auto& outVariable = out.m_variable;
    auto& outFunc = out.m_function;

    for(size_t i = begin; i < from->m_children.size(); ++i)
    {
        const auto& child = from->m_children[i];
        if(child->m_type == Lang::Tokens::K_VAR)
        {
            auto lastToken = outToken;
            outToken = child;
            // getting variable from scope because it is first iteration
            if(i == begin)
            {
                outVariable = from->getScopeVariable(child->m_name);
            }
            else // iteration continues (accessing to member variables)
            {
                if(outVariable)
                {
                    auto foundMember = outVariable->getMember(child->m_name);
                    auto foundFunction = outVariable->getTypeInfo().tryGetFunction(child->m_name);
                    if(!foundMember && !foundFunction)
                    {
                        LOG_E(SGCORE_TAG, "Error in CodeGenerator: member or function '{}' does not exist in '{}' type (variable: '{}').",
                              child->m_name,
                              outVariable->getTypeInfo().m_name,
                              lastToken->m_name)
                    }
                    else
                    {
                        if(foundMember)
                        {
                            // if member with this name was found then assigning new variable pointer to out.second
                            outVariable = foundMember;
                            // function and member with the same names can not exist
                            outFunc = std::nullopt;
                        }
                        else
                        {
                            // if member with this name was found then assigning found function to outFunc
                            outFunc = foundFunction;
                            outFunc->m_parentVariable = outVariable;
                            // function and member with the same names can not exist
                            outVariable = nullptr;
                        }
                    }
                }
            }

            isDotWas = false;
        }
        else if(child->m_type == Lang::Tokens::K_DOT)
        {
            isDotWas = true;
        }
        else
        {
            out.m_tokenIndex = i;
            break;
        }
    }

    if(isDotWas)
    {
        LOG_E(SGCORE_TAG, "Error in CodeGenerator: unknown member variable ''. (DOT token found but VAR token was not found after DOT)")
        outToken = nullptr;
        outVariable = nullptr;
    }

    return out;
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

SGCore::CodeGen::Lang::Variable::Variable(const SGCore::CodeGen::Lang::Type& withType) noexcept
{
    m_typeInfo = withType;

    for(const auto& member : withType.m_members)
    {
        m_members[member.first] = std::make_shared<Variable>(member.second);
    }

    for(const auto& type : withType.m_extends)
    {
        m_inheritanceMap[type.m_name] = std::make_shared<Variable>(type);
    }
}

const SGCore::CodeGen::Lang::Type& SGCore::CodeGen::Lang::Variable::getTypeInfo() const noexcept
{
    return m_typeInfo;
}

std::shared_ptr<SGCore::CodeGen::Lang::Variable> SGCore::CodeGen::Lang::Variable::getMember(const std::string& name) noexcept
{
    if(m_members.empty() && m_inheritanceMap.empty()) return nullptr;

    auto it = m_members.find(name);

    if(it == m_members.end())
    {
        for(auto& t : m_inheritanceMap)
        {
            return t.second->getMember(name);
        }

        return nullptr;
    }

    return it->second;
}

std::optional<SGCore::CodeGen::Lang::Function> SGCore::CodeGen::Lang::Type::tryGetFunction(const std::string& name) const noexcept
{
    if(m_functions.empty() && m_extends.empty()) return std::nullopt;

    auto it = m_functions.find(name);

    if(it == m_functions.end())
    {
        for(auto& t : m_extends)
        {
            return t.tryGetFunction(name);
        }

        return std::nullopt;
    }

    return it->second;
}

SGCore::CodeGen::Lang::Variable& SGCore::CodeGen::Lang::Variable::operator[](const std::string& memberName) noexcept
{
    if(hasMember(memberName))
    {
        return *m_members[memberName];
    }
    else
    {
        auto newMember = std::make_shared<Variable>();
        newMember->m_name = memberName;

        m_members[memberName] = newMember;

        return *newMember;
    }
}

bool SGCore::CodeGen::Lang::Variable::hasMember(const std::string& memberName) const noexcept
{
    return m_members.contains(memberName);
}

size_t SGCore::CodeGen::Lang::Variable::membersCount() const noexcept
{
    return m_members.size();
}

const std::unordered_map<std::string, std::shared_ptr<SGCore::CodeGen::Lang::Variable>>&
SGCore::CodeGen::Lang::Variable::getMembers() const noexcept
{
    return m_members;
}

void SGCore::CodeGen::Lang::Variable::setMemberPtr(const std::string& memberName, const std::shared_ptr<Variable>& value) noexcept
{
    m_members[memberName] = value;
    value->m_name = memberName;
}

std::shared_ptr<SGCore::CodeGen::Lang::Variable> SGCore::CodeGen::Lang::ASTToken::getScopeVariable(const std::string& variableName) const noexcept
{
    if(m_scope.empty()) return nullptr;

    auto var = m_scope.find(variableName);

    if(var == m_scope.end())
    {
        if(auto lockedParent = m_parent.lock())
        {
            return lockedParent->getScopeVariable(variableName);
        }
        else
        {
            return nullptr;
        }
    }

    return var->second;
}
