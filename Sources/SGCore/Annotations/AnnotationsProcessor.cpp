//
// Created by ilya on 28.06.24.
//
#include "AnnotationsProcessor.h"

#include <SGCore/pch.h>

#include "SGCore/Scene/Serializer.h"

namespace fs = std::filesystem;

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
        
        if((foundIt->second.m_requiredValuesCount < argPair.second.m_values.size() || foundIt->second.m_requiredValuesCount > argPair.second.m_values.size()) && foundIt->second.m_requiredValuesCount != -1)
        {
            return fmt::format("Error in annotation '{0}'. Invalid number of values {1}. Argument '{2}' requires {3} values. Annotation: {4}",
                               m_name, argPair.second.m_values.size(), argPair.first, foundIt->second.m_requiredValuesCount, annotationToValidate.formArgumentsString());
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
        std::uint32_t curValIdx = 0;
        for(const auto& val : arg->second.m_values)
        {
            auto* asStr = std::any_cast<std::string>(&val);
            auto* asSubAnnotation = std::any_cast<Annotation>(&val);
            
            if(asStr)
            {
                if(curValIdx < arg->second.m_values.size() - 1)
                {
                    args += *asStr + ", ";
                }
                else
                {
                    args += *asStr;
                }
            }
            else if(asSubAnnotation)
            {
                args += "\n\twith sub-annotation " + asSubAnnotation->formArgumentsString();
            }
            
            ++curValIdx;
        }
        if(curArgIdx < m_currentArgs.size() - 1)
        {
            args += "], ";
        }
        else
        {
            args += "]";
            args += (!m_anonymousArgs.empty() ? ",\n" : "");
        }
        
        ++curArgIdx;
    }
    
    for(const auto& anonymousArg : m_anonymousArgs)
    {
        const auto& arg = anonymousArg;
        
        std::uint32_t curValIdx = 0;
        for(const auto& val : anonymousArg.m_values)
        {
            auto* asStr = std::any_cast<std::string>(&val);
            auto* asSubAnnotation = std::any_cast<Annotation>(&val);
            
            if(asStr)
            {
                if(curValIdx < arg.m_values.size() - 1)
                {
                    args += *asStr + ", ";
                }
                else
                {
                    args += *asStr;
                }
            }
            else if(asSubAnnotation)
            {
                args += " \twith sub-annotation '" + asSubAnnotation->formArgumentsString() + "',\n";
            }
            
            ++curValIdx;
        }
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
        Annotation sgFunctionAnnotation;
        sgFunctionAnnotation.m_name = "sg_function";

        sgFunctionAnnotation.m_acceptableArgs["name"].m_name = "name";

        sgFunctionAnnotation.m_acceptableArgs["getterFor"].m_name = "getterFor";
        sgFunctionAnnotation.m_acceptableArgs["getterFor"].m_isUnnecessary = true;
        sgFunctionAnnotation.m_acceptableArgs["setterFor"].m_name = "setterFor";
        sgFunctionAnnotation.m_acceptableArgs["setterFor"].m_isUnnecessary = true;

        sgFunctionAnnotation.m_acceptableArgs["parentNamespace"].m_name = "parentNamespace";
        sgFunctionAnnotation.m_acceptableArgs["parentNamespace"].m_isUnnecessary = true;

        sgFunctionAnnotation.validate = [sgFunctionAnnotation, this](Annotation& annotation,
                                                                                       const std::vector<std::string>& words,
                                                                                       std::int64_t wordIndex) -> std::string {
            std::string argsAcceptableErr = sgFunctionAnnotation.validateAcceptableArgs(annotation);
            
            if(!argsAcceptableErr.empty())
            {
                return argsAcceptableErr;
            }
            
            return "";
        };

        sgFunctionAnnotation.acceptVariable = [](Annotation& annotation, const std::string& type, const std::string& name, bool isHigher) -> AnnotationActiveState {
            if(type == "struct" || type == "class" || type == "namespace")
            {
                // if not specified explicitly
                if(annotation.m_currentArgs.find("parentNamespace") == annotation.m_currentArgs.end())
                {
                    auto& arg = annotation.m_currentArgs["parentNamespace"];
                    arg.m_name = "parentNamespace";
                    arg.m_values.push_back(name);

                    // DEBUG
                    // std::printf("sg_member accepted %s %s\n", type.c_str(), name.c_str());
                }

                return AnnotationActiveState::INACTIVE;
            }

            return AnnotationActiveState::STAY_ACTIVE;
        };

        m_supportedAnnotations["sg_function"] = sgFunctionAnnotation;
    }
    
    {
        Annotation newAnnotation;
        newAnnotation.m_name = "sg_struct";
        
        newAnnotation.m_acceptableArgs["fullName"].m_name = "fullName";
        newAnnotation.m_acceptableArgs["fullName"].m_isUnnecessary = true;
        newAnnotation.m_acceptableArgs["fullName"].m_requiredValuesCount = 2;

        newAnnotation.m_acceptableArgs["template"].m_name = "template";
        newAnnotation.m_acceptableArgs["template"].m_isUnnecessary = true;
        newAnnotation.m_acceptableArgs["template"].m_requiredValuesCount = -1;

        newAnnotation.m_acceptableArgs["extends"].m_name = "extends";
        newAnnotation.m_acceptableArgs["extends"].m_isUnnecessary = true;
        newAnnotation.m_acceptableArgs["extends"].m_requiredValuesCount = -1;

        // type of struct or class
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
            if((type == "struct" || type == "class" || type == "namespace") && !isHigher)
            {
                // if not specified explicitly
                if(annotation.m_currentArgs.find("fullName") == annotation.m_currentArgs.end())
                {
                    auto& arg = annotation.m_currentArgs["fullName"];
                    arg.m_name = "fullName";
                    arg.m_values.push_back(name);
                    // adding type (class or structure)
                    arg.m_values.push_back(type);
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
        
        newAnnotation.m_acceptableArgs["serializableName"].m_name = "serializableName";
        newAnnotation.m_acceptableArgs["serializableName"].m_isUnnecessary = true;
        
        newAnnotation.acceptVariable = [](Annotation& annotation, const std::string& type, const std::string& name, bool isHigher) -> AnnotationActiveState {
            // std::printf("sg_member acceptation of %s\n", type.c_str());
            if(type == "struct" || type == "class" || type == "namespace")
            {
                // if not specified explicitly
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
                // if not specified explicitly
                if(annotation.m_currentArgs.find("varName") == annotation.m_currentArgs.end())
                {
                    auto& arg = annotation.m_currentArgs["varName"];
                    arg.m_name = "varName";
                    arg.m_values.push_back(name);
                }
                
                // if not specified explicitly
                if(annotation.m_currentArgs.find("serializableName") == annotation.m_currentArgs.end())
                {
                    auto& arg = annotation.m_currentArgs["serializableName"];
                    arg.m_name = "serializableName";
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
        
        std::unordered_map<std::string, std::vector<Annotation>> currentActiveAnnotations;
        
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
                        if(fileText[l0] == ';' || fileText[l0] == '{' || fileText[l0] == ':')
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

                            auto annotIt = aIt->second.begin();
                            while(annotIt != aIt->second.end())
                            {
                                AnnotationActiveState result = acceptVariableFunc(*annotIt, currentWord,
                                                                                  finalNamespaceName, false);
                                switch (result)
                                {
                                    case AnnotationActiveState::INACTIVE:
                                    {
                                        annotIt->m_filePath = f;
                                        m_currentAnnotations.push_back(*annotIt);
                                        annotIt = aIt->second.erase(annotIt);
                                        // aIt = currentActiveAnnotations.erase(aIt);

                                        continue;
                                    }
                                }

                                ++annotIt;
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
                        
                        // parsing annotation and arguments of annotation
                        parseAnnotationArgument(newAnnotation, annotationStartCharIdx, fileText, lineWords, currentWordIdx);
                        
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
                        
                        currentActiveAnnotations[newAnnotation.m_name].push_back(newAnnotation);
                        auto& annotations = currentActiveAnnotations[newAnnotation.m_name];
                        if(variableAcceptationFunc)
                        {
                            auto annotIt = annotations.begin();
                            while(annotIt != annotations.end())
                            {
                                auto result = variableAcceptationFunc(*annotIt,
                                                                      !namespaces.empty() ? namespaces.rbegin()->m_type
                                                                                          : "namespace",
                                                                      !namespaces.empty() ? namespaces.rbegin()->m_name
                                                                                          : "", true);

                                switch (result)
                                {
                                    case AnnotationActiveState::INACTIVE:
                                    {
                                        annotIt->m_filePath = f;
                                        m_currentAnnotations.push_back(*annotIt);
                                        annotIt = annotations.erase(annotIt);
                                        // currentActiveAnnotations.erase(newAnnotation.m_name);

                                        continue;
                                    }
                                }

                                ++annotIt;
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
                        for(std::int64_t j = (k != exprCopy.length() - 1 ? k - 1 : k); j != 0; --j)
                        {
                            if(std::isspace(exprCopy[j]))
                            {
                                for(std::int64_t l = std::min((j == 0 ? j : j - 1), std::ssize(exprCopy)); exprCopy[l] != ' ' && l != 0; --l)
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
                                    auto annotIt = aIt->second.begin();
                                    while(annotIt != aIt->second.end())
                                    {
                                        // variableName.c_str() is necessary
                                        auto acceptResult = acceptVariableFunc(*annotIt, "variable",
                                                                               variableName.c_str(), false);

                                        switch (acceptResult)
                                        {
                                            case AnnotationActiveState::INACTIVE:
                                            {
                                                annotIt->m_filePath = f;
                                                m_currentAnnotations.push_back(*annotIt);
                                                annotIt = aIt->second.erase(annotIt);
                                                // aIt = currentActiveAnnotations.erase(aIt);
                                                continue;
                                            }
                                        }

                                        ++annotIt;
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

const std::vector<SGCore::AnnotationsProcessor::Annotation>& SGCore::AnnotationsProcessor::getAnnotations() const noexcept
{
    return m_currentAnnotations;
}

std::int64_t
SGCore::AnnotationsProcessor::parseAnnotationArgument(SGCore::AnnotationsProcessor::Annotation& toAnnotation,
                                                      const int64_t& charIdx, const std::string& fileText,
                                                      const std::vector<std::string>& words,
                                                      const std::int64_t& wordIdx) noexcept
{
    bool enableAnnotationValuesRead = false;
    bool isArray = false;
    bool isString = false;
    bool enableValueWriting = false;
    bool isAppendingChar = true;
    std::string currentValue;
    std::string currentArg;
    
    auto curWordIdx = wordIdx;
    // auto curCharIdx = charIdx;
    
    std::int8_t roundBracketsCnt = 1;
    
    for(auto i = charIdx; i < fileText.length(); ++i)
    {
        
        const auto& aC = fileText[i];
        // std::cout << aC;
        // std::printf("%c", aC);
        
        if(aC == ' ')
        {
            ++curWordIdx;
        }
        
        if(aC == '"')
        {
            // enableValueWriting = !enableValueWriting;
            isString = !isString;
            if(isString)
            {
                currentValue = "";
            }
            isAppendingChar = false;
            continue;
        }
        
        if(!isString)
        {
            if(aC == '(')
            {
                if(roundBracketsCnt > 1)
                {
                    std::cout << "arg name: " << currentArg << std::endl;
                    
                    std::string annotationName = "anonymousAnnotation";
                    
                    Annotation newAnnotation;
                    newAnnotation.m_name = annotationName;
                    
                    i = parseAnnotationArgument(newAnnotation, i, fileText, words, curWordIdx);
                    
                    auto validateAnnotationFunc = m_supportedAnnotations[annotationName].validate;
                    if(validateAnnotationFunc)
                    {
                        validateAnnotationFunc(newAnnotation, words, curWordIdx);
                    }
                    
                    if(!newAnnotation.m_currentArgs.empty())
                    {
                        toAnnotation.m_currentArgs[Utils::trim(currentArg)].m_values.emplace_back(newAnnotation);
                        enableAnnotationValuesRead = false;
                    }
                    // enableAnnotationValuesRead = true;
                    continue;
                }
                ++roundBracketsCnt;
                enableAnnotationValuesRead = true;
                continue;
            }

            /*if(aC == ')')
            {
                break;
            }*/
            
            if(aC == ')')
            {
                if(!currentArg.empty())
                {
                    currentValue = Utils::trim(currentValue);
                    currentArg = Utils::trim(currentArg);
                    
                    toAnnotation.m_currentArgs[currentArg].m_name = currentArg;
                    toAnnotation.m_currentArgs[currentArg].m_values.emplace_back(currentValue);
                    
                    std::printf("annotation: %s (addr: %lu), arg: %s, value: %s\n",
                                toAnnotation.m_name.c_str(), reinterpret_cast<unsigned long>(&toAnnotation), currentArg.c_str(),
                                currentValue.c_str());
                }
                
                return i;
            }
        }
        
        if(enableAnnotationValuesRead)
        {
            isAppendingChar = true;
            
            if(aC == '=')
            {
                enableValueWriting = true;
                isAppendingChar = false;
            }
            
            // new argument
            if((aC == ',' || aC == ')' || aC == ']') && !isString)
            {
                if(!currentArg.empty())
                {
                    currentValue = Utils::trim(currentValue);
                    currentArg = Utils::trim(currentArg);
                    
                    toAnnotation.m_currentArgs[currentArg].m_name = currentArg;
                    toAnnotation.m_currentArgs[currentArg].m_values.push_back(currentValue);
                    
                    std::printf("annotation: %s (addr: %lu), arg: %s, value: %s\n",
                                toAnnotation.m_name.c_str(), reinterpret_cast<unsigned long>(&toAnnotation), currentArg.c_str(),
                                currentValue.c_str());
                    
                    if(!isArray || aC == ']' || aC == ')')
                    {
                        currentArg = "";
                        currentValue = "";
                        enableValueWriting = false;
                    }
                }
                
                if(aC == ']')
                {
                    isArray = false;
                }
                
                if(aC == ')')
                {
                    return i;
                }
                
                continue;
            }
            
            if(!isString && aC == '[')
            {
                isArray = true;
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
    }
    
    return charIdx;
}

void SGCore::AnnotationsProcessor::saveToFile(const std::filesystem::path& filePath) const noexcept
{
    rapidjson::Document document;
    document.SetObject();

    Serializer::serialize(document, document, "annotationsProcessor", *this);

    rapidjson::StringBuffer stringBuffer;
    stringBuffer.Clear();
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
    document.Accept(writer);

    FileUtils::writeToFile(filePath, stringBuffer.GetString(), false, true);
}

void SGCore::AnnotationsProcessor::loadFromFile(const std::filesystem::path& filePath) const noexcept
{
    rapidjson::Document document;
    document.Parse(FileUtils::readFile(filePath).c_str());

    // Serializer::deserialize<AnnotationsProcessor>(document, "annotationsProcessor");
}

std::unordered_map<std::string, SGCore::AnnotationsProcessor::Annotation>& SGCore::AnnotationsProcessor::getSupportedAnnotations() noexcept
{
    return m_supportedAnnotations;
}

const std::unordered_map<std::string, SGCore::AnnotationsProcessor::Annotation>&
SGCore::AnnotationsProcessor::getSupportedAnnotations() const noexcept
{
    return m_supportedAnnotations;
}

