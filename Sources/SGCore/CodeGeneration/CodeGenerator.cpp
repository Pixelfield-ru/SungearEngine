//
// Created by stuka on 10.09.2024.
//

#include <SGCore/Utils/FileUtils.h>
#include "CodeGenerator.h"

#include "SGCore/Logger/Logger.h"

SGCore::CodeGen::Generator::Generator()
{
    // creating standard types
    {
        Lang::Type iterableType;
        iterableType.m_name = "iterable";
        m_currentTypes.push_back(iterableType);

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

        Lang::Type genericMapType;
        genericMapType.m_name = "generic_map";
        genericMapType.m_extends.push_back(iterableType);

        Lang::Function placeFunc;
        placeFunc.m_name = "place";
        placeFunc.m_arguments.push_back({
                                                .m_name = "separator",
                                                .m_isNecessary = true,
                                                .m_acceptableType = stringType
                                        });
        placeFunc.m_functor = [placeFunc](const Lang::Type& owner, Lang::Variable* operableVariable,
                                          const size_t& curLine,
                                          std::string& outputText,
                                          const std::vector<Lang::FunctionArgument>& args) -> std::string {
            if(args.size() != 1)
            {
                LOG_E(SGCORE_TAG,
                      "Error in CodeGenerator (line: {}): in function 'place': bad count of passed arguments (required: {}, provided: {})",
                      curLine, placeFunc.m_arguments.size(), args.size());
                // outputValue = false;
                return "";
            }

            if(args[0].m_name != "separator")
            {
                LOG_E(SGCORE_TAG,
                      "Error in CodeGenerator (line: {}): in function 'place': unknown argument '{}'",
                      curLine, args[0].m_name);
                return "";
            }

            if(args[0].m_acceptableType.m_name != "string")
            {
                LOG_E(SGCORE_TAG,
                      "Error in CodeGenerator (line: {}): in function 'place': type of argument '{}' is not string",
                      curLine, args[0].m_name);
                return "";
            }

            const auto& separatorArg = std::any_cast<std::string>(args[0].m_data);

            std::string outputString;

            size_t elemIdx = 0;
            for(const auto& elem: operableVariable->getMembers())
            {
                if(elem.second->m_isBuiltin)
                {
                    ++elemIdx;
                    continue;
                }

                outputString += elem.second->m_insertedValue;
                if(elemIdx < operableVariable->getMembers().size() - 1)
                {
                    outputString += separatorArg;
                }

                ++elemIdx;
            }

            return outputString;
        };

        genericMapType.m_functions["place"] = placeFunc;

        Lang::Function isEmptyFunc;
        isEmptyFunc.m_name = "empty";
        isEmptyFunc.m_functor = [](const Lang::Type& owner, Lang::Variable* operableVariable, const size_t& curLine,
                                   std::string& outputText, const std::vector<Lang::FunctionArgument>& args) {
            return operableVariable->getMembers().empty();
        };

        genericMapType.m_functions["empty"] = isEmptyFunc;

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

            if(args[0].m_name != "name")
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

            return operableVariable->getMembers().contains(memberNameArg);
        };

        genericMapType.m_functions["hasMember"] = hasMemberFunc;

        m_currentTypes.push_back(genericMapType);

        // =====================================================

        // adding annotations processor types
        Lang::Type cppStructType;
        cppStructType.m_name = "cpp_struct";
        /*cppStructType.m_members["fullName"] = stringType;
        cppStructType.m_members["filePath"] = stringType;
        cppStructType.m_members["templateDecl"] = stringType;
        cppStructType.m_members["fullNameWithTemplate"] = stringType;
        cppStructType.m_members["baseTypes"] = genericMapType;
        cppStructType.m_members["derivedTypes"] = genericMapType;
        cppStructType.m_members["members"] = genericMapType;*/
        cppStructType.m_functions["hasMember"] = hasMemberFunc;
        m_currentTypes.push_back(cppStructType);

        Lang::Type cppMemberType;
        cppMemberType.m_name = "cpp_struct_member";
        /*cppMemberType.m_members["name"] = stringType;
        cppMemberType.m_members["setter"] = stringType;
        cppMemberType.m_members["getter"] = stringType;
        cppMemberType.m_members["hasSetter"] = boolType;
        cppMemberType.m_members["hasGetter"] = boolType;
        cppMemberType.m_members["struct"] = cppStructType;*/
        cppMemberType.m_functions["hasMember"] = hasMemberFunc;
        m_currentTypes.push_back(cppMemberType);
    }

    addBuiltinVariables();
}

void SGCore::CodeGen::Generator::addBuiltinVariables() noexcept
{
    // adding structs from meta info
    m_AST->m_scope["structs"] = std::make_shared<Lang::Variable>(*getTypeByName("generic_map"));

    size_t currentStructIdx = 0;
    for(auto& metaStruct : MetaInfo::getMeta()["structs"])
    {
        std::shared_ptr<Lang::Variable> newStruct;
        // if base type struct was already created then taking existing struct
        if(m_AST->m_scope["structs"]->hasMember(metaStruct["fullName"].getValue()))
        {
            newStruct = m_AST->m_scope["structs"]->getMember(metaStruct["fullName"].getValue());
        }
        else // if base type struct was not created then create base type struct
        {
            newStruct = std::make_shared<Lang::Variable>(*getTypeByName("cpp_struct"));
        }

        // adding all variables of meta
        addVariableFields(*newStruct, metaStruct);

        // adding some other builtin members of struct ==============================================

        // adding baseTypes of struct ==========
        auto& structExtends = metaStruct["baseTypes"];

        for(const auto& baseType : structExtends.getChildren())
        {
            (*newStruct)["baseTypes"][baseType->first].m_insertedValue = baseType->first;
            // if base type struct was not created then create base type struct
            if(!m_AST->m_scope["structs"]->hasMember(baseType->first))
            {
                auto newBaseStruct = std::make_shared<Lang::Variable>(*getTypeByName("cpp_struct"));

                (*newBaseStruct)["derivedTypes"] = Lang::Variable(*getTypeByName("generic_map"));
                (*newBaseStruct)["derivedTypes"][metaStruct["fullName"].getValue()].m_insertedValue = metaStruct["fullName"].getValue();

                m_AST->m_scope["structs"]->setMemberPtr(baseType->first, newBaseStruct);
            }
            else // if base type struct was already created then taking existing struct
            {
                auto& newBaseStruct = (*m_AST->m_scope["structs"])[baseType->first];

                newBaseStruct["derivedTypes"] = Lang::Variable(*getTypeByName("generic_map"));
                newBaseStruct["derivedTypes"][metaStruct["fullName"].getValue()].m_insertedValue = metaStruct["fullName"].getValue();
            }
        }
        // =====================================

        auto& structTemplateArgs = metaStruct["template_args"];

        // doing some actions with templates ==================
        // structFullNameWithTemplates == structName or structFullNameWithTemplates == structName<allTemplateArgs> (if template exists)
        std::string structFullNameWithTemplates = metaStruct["fullName"].getValue();
        std::string structTemplateDecl;
        const bool hasTemplateArgs = !structTemplateArgs.getChildren().empty();
        if(hasTemplateArgs)
        {
            structFullNameWithTemplates += '<';
        }
        for(size_t i = 0; i < structTemplateArgs.getChildren().size(); ++i)
        {
            const auto& structTemplateArg = structTemplateArgs.getChildren()[i];
            structFullNameWithTemplates += structTemplateArg->first;
            structTemplateDecl += structTemplateArg->second.getValue() + " " + structTemplateArg->first;

            if(i < structTemplateArgs.getChildren().size() - 1)
            {
                structFullNameWithTemplates += ", ";
                structTemplateDecl += ", ";
            }
        }
        if(hasTemplateArgs)
        {
            structFullNameWithTemplates += '>';
        }
        // =====================================================

        (*newStruct)["fullNameWithTemplate"].m_insertedValue = structFullNameWithTemplates;
        if(!structTemplateDecl.empty())
        {
            (*newStruct)["templateDecl"].m_insertedValue = structTemplateDecl;
        }

        m_AST->m_scope["structs"]->setMemberPtr(metaStruct["fullName"].getValue(), newStruct);

        ++currentStructIdx;
        // newStruct.m_members["fullNameWithTemplates"].m_insertedValue = structFullNameWithTemplates;
    }
}

void SGCore::CodeGen::Generator::addVariableFields
        (Lang::Variable& var, SGCore::MetaInfo::Meta& meta)
{
    for(auto& p : meta.getChildren())
    {
        auto& childMeta = p->second;

        if(childMeta.getChildren().empty())
        {
            auto& varMember = var[childMeta.getName()];
            varMember = Lang::Variable(*getTypeByName("generic_map"));
            varMember.m_insertedValue = childMeta.getValue();
            varMember["name"].m_insertedValue = childMeta.getName();
            varMember["name"].m_isBuiltin = true;
        }
        else
        {
            auto& varMember = var[childMeta.getName()];
            varMember = Lang::Variable(*getTypeByName("generic_map"));
            varMember["name"].m_insertedValue = childMeta.getName();
            varMember["name"].m_isBuiltin = true;
            addVariableFields(varMember, childMeta);
        }
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
    // std::string templateFileText = Utils::reduce(FileUtils::readFile(templateFile));
    std::string templateFileText = FileUtils::readFile(templateFile);
    // instead of EOF
    templateFileText += '\n';

    std::string outputString;

    buildAST(templateFileText);
    generateCodeUsingAST(m_AST, outputString);

    std::printf("end\n");

    // clearing tmp variables
    m_isExprStarted = false;
    m_isPlacementStarted = false;
    m_currentCommentType = CommentType::NO_COMMENT;

    m_skipCodeCopy = false;
    m_writeCharSeq = false;

    m_currentUsedVariable = nullptr;

    m_AST = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_FILESTART);
    m_currentCPPCodeToken = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_CPP_CODE_LINE);
    m_currentCharSeqToken = std::make_shared<Lang::ASTToken>(Lang::Tokens::K_CHAR_SEQ);

    addBuiltinVariables();

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
    for(size_t childIdx = 0; childIdx < token->m_children.size(); ++childIdx)
    {
        const auto& child = token->m_children[childIdx];

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
                    continue;
                }

                if(bindableVariableToken->m_type != Lang::Tokens::K_VAR)
                {
                    // todo: make error that token is not variable

                    LOG_E(SGCORE_TAG, "Error in CodeGenerator: (for cycle) trying to bind not variable type.")
                    continue;
                }

                auto newBindableVariable = std::make_shared<Lang::Variable>();
                token->m_scope[bindableVariableToken->m_name] = newBindableVariable;

                if(!tokenAndVariableToForIn.m_variable)
                {
                    // todo: make error that variable does not exist

                    LOG_E(SGCORE_TAG, "Error in CodeGenerator: (for cycle) trying to iterate variable '{}' that does not exist.", tokenAndVariableToForIn.m_token->m_name)
                    continue;
                }

                if(!tokenAndVariableToForIn.m_variable->getTypeInfo().instanceof("iterable"))
                {
                    // todo: make error that variable is not instance of iterable

                    LOG_E(SGCORE_TAG, "Error in CodeGenerator: trying to iterate variable '{}' that is not instance of 'iterable'.", tokenAndVariableToForIn.m_token->m_name)
                    continue;
                }

                LOG_I(SGCORE_TAG, "CodeGenerator: iterating variable '{}'...", tokenAndVariableToForIn.m_token->m_name)

                size_t iterationsCount = tokenAndVariableToForIn.m_variable->membersCount();

                size_t curElemIdx = 0;
                for(const auto& elem : tokenAndVariableToForIn.m_variable->getMembers())
                {
                    if(elem.second->m_isBuiltin) continue;

                    token->m_scope[bindableVariableToken->m_name] = elem.second;

                    LOG_I(SGCORE_TAG, "CodeGenerator: iterating variable '{}'. Iteration '{}'. Current member: '{}'", tokenAndVariableToForIn.m_token->m_name, curElemIdx, elem.second->m_name);
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
                    continue;
                }

                std::any funcOutput;

                if(tokenAndVar.m_variable)
                {
                    if(tokenAndVar.m_variable->getTypeInfo().m_name != "bool")
                    {
                        LOG_E(SGCORE_TAG,
                              "Error in CodeGenerator: (if branch) trying to test variable that is not boolean type.")
                        continue;
                    }
                }
                else if(tokenAndVar.m_function)
                {
                    funcOutput = processFunctionTokensAndCallFunc(child, tokenAndVar.m_tokenIndex,
                                                                  *tokenAndVar.m_function, outputString);

                    if(!std::any_cast<bool>(&funcOutput))
                    {
                        LOG_E(SGCORE_TAG, "Error in CodeGenerator: return type of function '{}' is not 'bool' type.",
                              tokenAndVar.m_function->m_name);

                        continue;
                    }
                }
                else
                {
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

                bool isNotSignFound = false;
                // if 'not' sign found. reversing values
                if(token->m_children[childIdx - 1]->m_type == Lang::Tokens::K_NOT)
                {
                    isNotSignFound = true;
                }

                const bool& funcOutputVal = funcOutput.has_value() && std::any_cast<bool>(funcOutput);

                if((tokenAndVar.m_variable &&
                    (tokenAndVar.m_variable->m_insertedValue == "true" ||
                     (isNotSignFound && tokenAndVar.m_variable->m_insertedValue == "false"))) ||

                   (funcOutput.has_value() &&
                    (funcOutputVal ||
                    (isNotSignFound && !funcOutputVal))))
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
                else if(tokenAndVar.m_function)
                {
                    std::any funcOutput = processFunctionTokensAndCallFunc(child, tokenAndVar.m_tokenIndex, *tokenAndVar.m_function, outputString);
                    if(!std::any_cast<std::string>(&funcOutput))
                    {
                        LOG_E(SGCORE_TAG, "Error in CodeGenerator: return type of function '{}' is not 'string' type.", tokenAndVar.m_function->m_name);
                        continue;
                    }

                    outputString += std::any_cast<std::string>(funcOutput);
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
    else if((m_isPlacementStarted || m_isExprStarted) && !m_writeCharSeq && currentCharTokenType == Lang::Tokens::K_NOT)
    {
        /*if(currentWordTokenType != currentCharTokenType)
        {
            std::string tmpWord = std::string(word.begin(), word.end() - 1);
            analyzeCurrentWordAndCharForTokens(currentToken, tmpWord, text, nextCharIdx, true);
        }*/
        addToken(currentToken, Lang::Tokens::K_NOT);
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

std::any SGCore::CodeGen::Generator::processFunctionTokensAndCallFunc(const std::shared_ptr<Lang::ASTToken>& inToken,
                                                                      const size_t& tokenOffset,
                                                                      SGCore::CodeGen::Lang::Function& callableFunction,
                                                                      std::string& outputString) const noexcept
{
    std::any funcOutput;

    bool lparenFound = false;
    bool rparenFound = false;

    std::vector<Lang::FunctionArgument> funcArguments;
    std::shared_ptr<Lang::ASTToken> currentArgToken;

    // analyzing next token
    for(size_t j = tokenOffset; j < inToken->m_children.size(); ++j)
    {
        const auto& curFuncToken = inToken->m_children[j];
        std::shared_ptr<Lang::ASTToken> nextFuncToken;
        if(j + 1 < inToken->m_children.size())
        {
            nextFuncToken = inToken->m_children[j + 1];
        }

        if(curFuncToken->m_type == Lang::Tokens::K_LPAREN)
        {
            lparenFound = true;
            continue;
        }
        if(curFuncToken->m_type == Lang::Tokens::K_RPAREN)
        {
            rparenFound = true;
            break; // arguments acceptance ending
        }

        if((curFuncToken->m_type == Lang::Tokens::K_VAR ||
            curFuncToken->m_type == Lang::Tokens::K_CHAR_SEQ) &&
           currentArgToken)
        {
            Lang::FunctionArgument funcArg;
            funcArg.m_name = currentArgToken->m_name;

            if(curFuncToken->m_type == Lang::Tokens::K_CHAR_SEQ)
            {
                funcArg.m_acceptableType = *getTypeByName("string");
                funcArg.m_data = curFuncToken->m_cppCode;
            }
            else
            {
                auto scopeVariable = inToken->getScopeVariable(curFuncToken->m_cppCode);
                if(!scopeVariable)
                {
                    LOG_E(SGCORE_TAG,
                          "Error in CodeGenerator: unknown scope variable '{}' in function argument '{}'.",
                          curFuncToken->m_name, currentArgToken->m_name);
                    continue;
                }

                funcArg.m_acceptableType = scopeVariable->getTypeInfo();
                funcArg.m_data = scopeVariable->m_insertedValue;
            }

            funcArguments.push_back(funcArg);

            currentArgToken = nullptr;
        }

        if(curFuncToken->m_type == Lang::Tokens::K_VAR)
        {
            if(nextFuncToken && nextFuncToken->m_type == Lang::Tokens::K_COLON)
            {
                currentArgToken = curFuncToken;
            }
            else
            {
                if(currentArgToken)
                {
                    LOG_E(SGCORE_TAG, "Error in CodeGenerator (in function call): unknown token with name '{}'.", curFuncToken->m_name);
                }
            }
        }
    }

    if(!lparenFound || !rparenFound)
    {
        LOG_E(SGCORE_TAG, "Error in CodeGenerator: in function '{}': parens mismatch.", callableFunction.m_name);
    }

    if(auto lockedParentLockedVar = callableFunction.m_parentVariable.lock())
    {
        funcOutput = callableFunction.m_functor(lockedParentLockedVar->getTypeInfo(),
                                                       lockedParentLockedVar.get(), 0, outputString,
                                                       funcArguments);
    }

    return funcOutput;
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

void SGCore::CodeGen::Lang::Variable::removeMember(const std::string& memberName) noexcept
{
    m_members.erase(memberName);
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
