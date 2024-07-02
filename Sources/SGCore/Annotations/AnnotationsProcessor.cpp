//
// Created by ilya on 28.06.24.
//
#include "AnnotationsProcessor.h"

#include <SGCore/pch.h>

std::string SGCore::AnnotationsProcessor::Annotation::validateAcceptableArgs(const Annotation& annotationToValidate) const noexcept
{
    decltype(Annotation::m_acceptableArgs) necessaryArgs = m_acceptableArgs;
    
    for(const auto& argPair : annotationToValidate.m_currentArgs)
    {
        auto foundIt = m_acceptableArgs.find(argPair.first);
        
        if(foundIt == m_acceptableArgs.end())
        {
            return fmt::format("Error in annotation '{0}'. Unknown argument: {1}", m_name, argPair.first);
        }
        
        if(foundIt->second.m_requiredValuesCount < argPair.second.m_values.size() && foundIt->second.m_requiredValuesCount != -1)
        {
            return fmt::format("Error in annotation '{0}'. Invalid number of arguments {1}. Requires {2} arguments or less", m_name, argPair.second.m_requiredValuesCount, foundIt->second.m_requiredValuesCount);
        }
        
        necessaryArgs.erase(argPair.first);
    }
    
    std::string necessaryArgsErr;
    
    for(const auto& argPair : necessaryArgs)
    {
        if(!argPair.second.m_isUnnecessary)
        {
            necessaryArgsErr += fmt::format("Error in annotation '{0}'. Argument '{1}' is necessary (missing).\n", m_name, argPair.first);
        }
    }
    
    if(!necessaryArgsErr.empty())
    {
        necessaryArgsErr += annotationToValidate.formArgumentsString();
        
        return necessaryArgsErr;
    }
    
    return "";
}

std::string SGCore::AnnotationsProcessor::Annotation::formArgumentsString() const noexcept
{
    std::string args;
    std::uint32_t curArgIdx = 0;
    for(auto i = m_currentArgs.begin(); i != m_currentArgs.end(); ++i)
    {
        const auto& arg = i;
        
        args += arg->first + " = [";
        for(const auto& val : arg->second.m_values)
        {
            // std::printf("%s", val.c_str());
            args += val;
        }
        if(curArgIdx < m_currentArgs.size() - 1)
        {
            args += "], ";
        }
        else
        {
            args += "]";
        }
        
        ++curArgIdx;
    }
    
    if(m_currentArgs.empty())
    {
        args + " no args";
    }
    
    return args;
}

bool SGCore::AnnotationsProcessor::Member::isAnnotationProvided(const std::string& name) const noexcept
{
    return m_annotations.contains(name);
}

SGCore::AnnotationsProcessor::Member*
SGCore::AnnotationsProcessor::SourceStruct::getMember(const std::string& name) noexcept
{
    auto foundIt = m_members.find({ name });
    if(foundIt != m_members.end())
    {
        return &foundIt->second;
    }
    
    return nullptr;
}

SGCore::AnnotationsProcessor::AnnotationsProcessor() noexcept
{
    {
        Annotation newAnnotation;
        newAnnotation.m_name = "sg_struct";
        
        newAnnotation.m_acceptableArgs["fullName"].m_name = "fullName";
        newAnnotation.m_acceptableArgs["fullName"].m_isUnnecessary = true;
        
        newAnnotation.m_acceptableArgs["type"].m_name = "type";
        newAnnotation.m_acceptableArgs["type"].m_isUnnecessary = true;
        
        newAnnotation.validate = [newAnnotation, this](Annotation& annotation,
                                                           const std::vector<std::string>& words, std::int64_t wordIndex) -> std::string {
            std::string argsAcceptableErr = newAnnotation.validateAcceptableArgs(annotation);
            
            if(!argsAcceptableErr.empty())
            {
                return argsAcceptableErr;
            }
            
            return "";
        };
        
        newAnnotation.acceptVariable = [](Annotation& annotation, const std::string& type, const std::string& name, bool isHigher) -> AnnotationActiveState {
            if((type == "struct" || type == "class") && !isHigher)
            {
                if(annotation.m_currentArgs.find("fullName") == annotation.m_currentArgs.end())
                {
                    auto& arg = annotation.m_currentArgs["fullName"];
                    arg.m_name = "fullName";
                    arg.m_values.push_back(name);
                }
                
                // std::printf("sg_struct acceptation of %s '%s. Annotation addr: %lu'\n", type.c_str(), name.c_str(), reinterpret_cast<size_t>(&annotation));
                
                // DEBUG
                // std::printf("sg_struct accepted %s %s\n", type.c_str(), name.c_str());
                
                return AnnotationActiveState::INACTIVE;
            }
            
            return AnnotationActiveState::STAY_ACTIVE;
        };
        
        m_supportedAnnotations["sg_struct"] = newAnnotation;
    }
    {
        Annotation newAnnotation;
        newAnnotation.m_name = "sg_component";
        
        newAnnotation.validate = [newAnnotation](Annotation& annotation, const std::vector<std::string>& words, std::int64_t wordIndex) -> std::string {
            std::string argsAcceptableErr = newAnnotation.validateAcceptableArgs(annotation);
            
            if(!argsAcceptableErr.empty())
            {
                return argsAcceptableErr;
            }
            
            return "";
        };
        
        m_supportedAnnotations["sg_component"] = newAnnotation;
    }
    
    {
        Annotation newAnnotation;
        newAnnotation.m_name = "sg_member";
        newAnnotation.m_acceptableArgs["parentNamespace"].m_isUnnecessary = true;
        newAnnotation.m_acceptableArgs["parentNamespace"].m_name = "parentNamespace";
        
        newAnnotation.m_acceptableArgs["varName"].m_name = "varName";
        newAnnotation.m_acceptableArgs["varName"].m_isUnnecessary = true;
        
        newAnnotation.acceptVariable = [](Annotation& annotation, const std::string& type, const std::string& name, bool isHigher) -> AnnotationActiveState {
            // std::printf("sg_member acceptation of %s\n", type.c_str());
            if(type == "struct" || type == "class" || type == "namespace")
            {
                if(annotation.m_currentArgs.find("parentNamespace") == annotation.m_currentArgs.end())
                {
                    auto& arg = annotation.m_currentArgs["parentNamespace"];
                    arg.m_name = "parentNamespace";
                    arg.m_values.push_back(name);
                    
                    // DEBUG
                    // std::printf("sg_member accepted %s %s\n", type.c_str(), name.c_str());
                }
                
                return AnnotationActiveState::STAY_ACTIVE;
            }
            else if(type == "variable")
            {
                if(annotation.m_currentArgs.find("varName") == annotation.m_currentArgs.end())
                {
                    auto& arg = annotation.m_currentArgs["varName"];
                    arg.m_name = "varName";
                    arg.m_values.push_back(name);
                }
                
                // DEBUG
                // std::printf("sg_member accepted %s %s\n", type.c_str(), name.c_str());
                
                return AnnotationActiveState::INACTIVE;
            }
            
            return AnnotationActiveState::STAY_ACTIVE;
        };
        
        newAnnotation.validate = [newAnnotation](Annotation& annotation, const std::vector<std::string>& words, std::int64_t wordIndex) -> std::string {
            std::string argsAcceptableErr = newAnnotation.validateAcceptableArgs(annotation);
            
            if(!argsAcceptableErr.empty())
            {
                return argsAcceptableErr;
            }
            
            return "";
        };
        
        m_supportedAnnotations["sg_member"] = newAnnotation;
    }
}

void SGCore::AnnotationsProcessor::processAnnotations(const fs::path& inDirectory, const std::vector<fs::path>& filesToExclude, bool recursively)
{
    std::vector<fs::path> files;
    
    if(recursively)
    {
        for(fs::recursive_directory_iterator i(inDirectory), end; i != end; ++i)
        {
            if(!is_directory(i->path()) && std::find(filesToExclude.begin(), filesToExclude.end(), i->path()) == filesToExclude.end())
            {
                files.emplace_back(i->path());
            }
        }
    }
    
    processAnnotations(files);
}

void SGCore::AnnotationsProcessor::processAnnotations(const std::vector<std::filesystem::path>& files)
{
    auto regexEndIt = std::sregex_iterator();
    
    for(const auto& f : files)
    {
        std::string u8FilePath = Utils::toUTF8<char16_t>(f.u16string());
        if(!fs::exists(f))
        {
            std::printf("File '%s' not exists\n", u8FilePath.c_str());
            continue;
        }
        
        if(f.extension() != ".h" && f.extension() != ".hpp" && f.extension() != ".cpp" && f.extension() != ".cxx")
        {
            // std::printf("Not supported extension of file\n", u8FilePath.c_str());
            continue;
        }
        
        std::string fileText = FileUtils::readFile(u8FilePath);
        
        fileText = prettifyCode(fileText);
        
        std::vector<std::string> lineWords;
        Utils::splitString(fileText, ' ', lineWords);
        
        std::int64_t currentWordIdx = 0;
        std::string currentWord;

        // ==========================================================
        struct NamedVariableDecl
        {
            std::string m_type;
            std::string m_name;
            std::int64_t m_bracesIdx = 0;
        };
        
        std::vector<NamedVariableDecl> namespaces;
        std::int64_t currentOpenedBracesCnt = 0;
        
        std::unordered_map<std::string, Annotation> currentActiveAnnotations;
        
        std::string currentExpression;
        bool skipExprs = false;
        std::int64_t exprsInOneLineCnt = 0;
        
        // ==========================================================
        
        for(std::int64_t currentCharIdx = 0; currentCharIdx < fileText.length(); ++currentCharIdx)
        {
            const auto& c = fileText[currentCharIdx];
            
            if(c == '{')
            {
                ++currentOpenedBracesCnt;
            }
            
            if(c == '}')
            {
                auto nIt = namespaces.begin();
                while(nIt != namespaces.end())
                {
                    if(nIt->m_bracesIdx == currentOpenedBracesCnt)
                    {
                        // DEBUG
                        // std::printf("closed %s: %s\n", nIt->m_type.c_str(), nIt->m_name.c_str());
                        namespaces.erase(nIt);
                        break;
                    }
                    
                    ++nIt;
                }
                
                --currentOpenedBracesCnt;
            }
            
            if(c == ' ')
            {
                if(currentWord == "namespace" || currentWord == "struct" || currentWord == "class")
                {
                    bool isIncompleteType = false;
                    std::string namespaceName;
                    for(auto l0 = currentCharIdx; l0 < fileText.length(); ++l0)
                    {
                        if(fileText[l0] == ';' || fileText[l0] == '{')
                        {
                            if(fileText[l0] == ';')
                            {
                                isIncompleteType = true;
                            }
                            
                            break;
                        }
                        
                        namespaceName += fileText[l0];
                    }
                    
                    namespaceName = Utils::replaceAll<char>(namespaceName, " ", "");
                    
                    // if not incomplete type (maybe forward declaration or friend declaration)
                    if(!isIncompleteType)
                    {
                        
                        std::string finalNamespaceName = !namespaces.empty() ? namespaces.rbegin()->m_name + "::" +
                                                                               namespaceName : namespaceName;
                        
                        // std::printf("namespace: %s\n", finalNamespaceName.c_str());
                        
                        namespaces.emplace_back(currentWord, finalNamespaceName, currentOpenedBracesCnt + 1);
                        
                        auto aIt = currentActiveAnnotations.begin();
                        while(aIt != currentActiveAnnotations.end())
                        {
                            const auto& acceptVariableFunc = m_supportedAnnotations[aIt->first].acceptVariable;
                            if(!acceptVariableFunc)
                            {
                                ++aIt;
                                
                                continue;
                            }
                            
                            AnnotationActiveState result = acceptVariableFunc(aIt->second, currentWord,
                                                                              finalNamespaceName, false);
                            switch(result)
                            {
                                case AnnotationActiveState::INACTIVE:
                                {
                                    m_currentAnnotations.push_back(aIt->second);
                                    m_currentAnnotations.rbegin()->m_filePath = f;
                                    aIt = currentActiveAnnotations.erase(aIt);
                                    
                                    
                                    continue;
                                }
                            }
                            ++aIt;
                        }
                    }
                }
                
                for(const auto& a : m_supportedAnnotations)
                {
                    if(currentWord.contains(a.first))
                    {
                        std::int64_t annotationStartCharIdx = currentCharIdx - currentWord.length();
                        
                        Annotation newAnnotation;
                        newAnnotation.m_name = a.first;
                        
                        bool enableAnnotationValuesRead = false;
                        
                        bool isArray = false;
                        bool isString = false;
                        bool enableValueWriting = false;
                        bool isAppendingChar = true;
                        std::string currentValue;
                        std::string currentArg;
                        
                        // parsing annotation and arguments of annotation
                        for(auto i = annotationStartCharIdx; i < fileText.length(); ++i)
                        {
                            const auto& aC = fileText[i];
                            // std::printf("%c", aC);
                            
                            if(!isString)
                            {
                                if(aC == '(')
                                {
                                    enableAnnotationValuesRead = true;
                                    continue;
                                }
                            }
                            
                            if(enableAnnotationValuesRead)
                            {
                                isAppendingChar = true;
                                
                                if(aC == '=')
                                {
                                    enableValueWriting = !enableValueWriting;
                                    isAppendingChar = false;
                                }
                                
                                if(aC == '"')
                                {
                                    isString = !isString;
                                    if(isString)
                                    {
                                        currentValue = "";
                                    }
                                    isAppendingChar = false;
                                }
                                
                                // new argument
                                if((aC == ',' || aC == ')') && !isArray)
                                {
                                    if(!currentArg.empty())
                                    {
                                        currentValue = Utils::trim(currentValue);
                                        currentArg = Utils::trim(currentArg);
                                        
                                        newAnnotation.m_currentArgs[currentArg].m_name = currentArg;
                                        newAnnotation.m_currentArgs[currentArg].m_values.push_back(currentValue);
                                        
                                        std::printf("annotation: %s, arg: %s, value: %s\n",
                                                    newAnnotation.m_name.c_str(), currentArg.c_str(),
                                                    currentValue.c_str());
                                    }
                                    
                                    if(aC == ')' && !isString)
                                    {
                                        break;
                                    }
                                    
                                    currentArg = "";
                                    currentValue = "";
                                    enableValueWriting = false;
                                    continue;
                                }
                                else if((aC == ',' || aC == ']') && isArray)
                                {
                                    currentValue = Utils::trim(currentValue);
                                    currentArg = Utils::trim(currentArg);
                                    
                                    newAnnotation.m_currentArgs[currentArg].m_name = currentArg;
                                    newAnnotation.m_currentArgs[currentArg].m_values.push_back(currentValue);
                                    
                                    std::printf("annotation: %s, arg: %s, array value: %s\n", newAnnotation.m_name.c_str(), currentArg.c_str(), currentValue.c_str());
                                    
                                    currentValue = "";
                                    
                                    continue;
                                }
                                
                                if(aC == '[')
                                {
                                    isArray = true;
                                    continue;
                                }
                                if(aC == ']')
                                {
                                    isArray = false;
                                    continue;
                                }
                                
                                // writing arg
                                if(isAppendingChar)
                                {
                                    if(!enableValueWriting)
                                    {
                                        currentArg += aC;
                                    }
                                    else
                                    {
                                        currentValue += aC;
                                    }
                                }
                            }
                            
                            if(aC == ')' && !isString)
                            {
                                break;
                            }
                        }
                        
                        if(m_supportedAnnotations[newAnnotation.m_name].validate)
                        {
                            std::string annotationValidationError = m_supportedAnnotations[newAnnotation.m_name].validate(
                                    newAnnotation, lineWords, currentWordIdx);
                            if(!annotationValidationError.empty())
                            {
                                throw std::runtime_error(
                                        annotationValidationError.append("\n\tFile: ").append(u8FilePath));
                            }
                        }
                        
                        const auto& variableAcceptationFunc = m_supportedAnnotations[newAnnotation.m_name].acceptVariable;
                        
                        currentActiveAnnotations[newAnnotation.m_name] = newAnnotation;
                        auto& annot = currentActiveAnnotations[newAnnotation.m_name];
                        if(variableAcceptationFunc)
                        {
                            if(!namespaces.empty())
                            {
                                auto result = variableAcceptationFunc(annot, namespaces.rbegin()->m_type,
                                                                      namespaces.rbegin()->m_name, true);
                                
                                switch(result)
                                {
                                    case AnnotationActiveState::INACTIVE:
                                    {
                                        annot.m_filePath = f;
                                        m_currentAnnotations.push_back(annot);
                                        currentActiveAnnotations.erase(newAnnotation.m_name);
                                        
                                        break;
                                    }
                                }
                            }
                        }
                        
                        break;
                    }
                }
                
                ++currentWordIdx;
                currentWord = "";
                
                currentExpression += ' ';
                
                continue;
            }
            
            if(c == ';' || c == '{' || c == '}')
            {
                ++exprsInOneLineCnt;
                if(exprsInOneLineCnt > 1)
                {
                    skipExprs = true;
                }
                
                if(c == ';' /*|| c == '{'*/)
                {
                    exprsInOneLineCnt = 0;
                    skipExprs = false;
                }
                
                if(skipExprs)
                {
                    // isLastExprHasAssignmentOperator = false;
                    currentExpression = "";
                    continue;
                }
                
                
                std::int64_t bracketsCnt = 0;
                
                if(c == ';')
                {
                    currentExpression += ";";
                }
                
                bool isExpressionValid = std::count(currentExpression.begin(), currentExpression.end(), ' ') + 1 <= currentExpression.length() - 1;
                
                for(std::int64_t k = 0; k < currentExpression.length(); ++k)
                {
                    if(currentExpression[k] == '<' || currentExpression[k] == '(')
                    {
                        ++bracketsCnt;
                    }
                    else if(currentExpression[k] == '>'|| currentExpression[k] == ')')
                    {
                        --bracketsCnt;
                    }
                    
                    // examples:
                    // std::string var = ...;
                    // MyType<OtherType{}> var = ...;
                    // MyType<OtherType{}> var { };
                    // ignoring all in <>
                    if((currentExpression[k] == '{' || currentExpression[k] == '=' || k == currentExpression.length() - 1) && bracketsCnt == 0)
                    {
                        std::string exprCopy = currentExpression;
                        
                        for(auto k0 = k - 1; k0 < exprCopy.length(); ++k0)
                        {
                            if(exprCopy[k0] == ' ' || exprCopy[k0] == ';')
                            {
                                exprCopy.erase(k0);
                            }
                        }
                        
                        std::string variableName;
                        std::string firstWordBeforeVariable;
                        // going back to get variable name
                        for(std::int64_t j = k != currentExpression.length() - 1 ? k - 1 : k; k != 0; --j)
                        {
                            if(std::isspace(exprCopy[j]))
                            {
                                for(std::int64_t l = j - 1; exprCopy[l] != ' '; --l)
                                {
                                    firstWordBeforeVariable += exprCopy[l];
                                }
                                
                                std::reverse(firstWordBeforeVariable.begin(), firstWordBeforeVariable.end());
                                
                                break;
                            }
                            
                            variableName += exprCopy[j];
                        }
                        
                        std::reverse(variableName.begin(), variableName.end());
                        
                        variableName = Utils::reduce(variableName);
                        
                        if(isExpressionValid &&
                            firstWordBeforeVariable != "union" &&
                                firstWordBeforeVariable != "struct" &&
                                firstWordBeforeVariable != "class" &&
                                firstWordBeforeVariable != "namespace" &&
                                !variableName.empty())
                        {
                            // std::printf("FOUND VARIABLE: %s\n", variableName.c_str());
                            
                            auto aIt = currentActiveAnnotations.begin();
                            while(aIt != currentActiveAnnotations.end())
                            {
                                const auto& acceptVariableFunc = m_supportedAnnotations[aIt->first].acceptVariable;
                                
                                if(acceptVariableFunc)
                                {
                                    // variableName.c_str() is necessary
                                    auto acceptResult = acceptVariableFunc(aIt->second, "variable", variableName.c_str(), false);
                                    
                                    switch(acceptResult)
                                    {
                                        case AnnotationActiveState::INACTIVE:
                                        {
                                            m_currentAnnotations.push_back(aIt->second);
                                            m_currentAnnotations.rbegin()->m_filePath = f;
                                            aIt = currentActiveAnnotations.erase(aIt);
                                            continue;
                                        }
                                    }
                                }
                                
                                ++aIt;
                            }
                        }
                        
                        break;
                    }
                }
                
                currentExpression = "";
            }
            else
            {
                currentExpression += c;
            }
            
            currentWord += c;
        }
        
        // std::cout << fileText << std::endl;
    }
}

std::string SGCore::AnnotationsProcessor::prettifyCode(const std::string& code) noexcept
{
    std::string outputStr;
    
    std::vector<std::string> lines;
    std::vector<std::string> lineWords;
    
    Utils::splitString(code, '\n', lines);
    
    bool isOneLineComment = false;
    bool isMultilineComment = false;
    
    for(std::uint64_t lI = 0; lI < lines.size(); ++lI)
    {
        const std::string& line = lines[lI];
        
        Utils::splitString(line, ' ', lineWords);
        
        for(std::uint64_t wI = 0; wI < lineWords.size(); ++wI)
        {
            const std::string& word = lineWords[wI];
            
            if(word.contains("//"))
            {
                isOneLineComment = true;
            }
            
            if(word.contains("/*"))
            {
                isMultilineComment = true;
            }
            
            auto foundMultilineCloseBracket = word.find("*/");
            if(foundMultilineCloseBracket != std::string::npos)
            {
                isMultilineComment = false;
            }
            
            if(isOneLineComment || isMultilineComment)
            {
                continue;
            }
            
            // NOW WE CAN PARSING
            
            if(foundMultilineCloseBracket != std::string::npos)
            {
                outputStr += word.substr(foundMultilineCloseBracket + 2, word.length() - (foundMultilineCloseBracket + 2));
            }
            else
            {
                outputStr += word + " ";
            }
        }
        
        lineWords.clear();
        
        // outputStr += "\n";
        
        if(isOneLineComment)
        {
            isOneLineComment = false;
            continue;
        }
        
        isOneLineComment = false;
    }
    
    outputStr = Utils::reduce(outputStr);
    
    return outputStr;
}

std::string SGCore::AnnotationsProcessor::stringifyAnnotations() const noexcept
{
    std::string str;
    
    for(const auto& annotation : m_currentAnnotations)
    {
        str += fmt::format("Annotation '{0}' has arguments '{1}'\n", annotation.m_name, annotation.formArgumentsString());
    }
    
    return str;
}

