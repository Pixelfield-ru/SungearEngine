//
// Created by ilya on 07.02.24.
//

#include <sstream>
#include <filesystem>

#include "SGSLETranslator.h"
#include "SGSLEUtils.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Utils/FileUtils.h"

void
SGCore::SGSLETranslator::processCode(const std::filesystem::path& path, const std::string& code,
                                     SGCore::SGSLETranslator& translator, bool isRootShader,
                                     const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept
{
    // std::cout << "loading shader: " << path << std::endl;

    if(isRootShader)
    {
        // translator.m_config.m_outputDebugDirectoryPath += "/" + replacedPath;
    }
    
    std::string correctedCode = sgsleCodeCorrector(code);
    sgslePreprocessor(Utils::toUTF8(path.u16string()), correctedCode, analyzedFile);
    sgsleMainProcessor(analyzedFile, translator);
    
    if(translator.m_config.m_useOutputDebug && isRootShader)
    {
        std::string replacedPath = Utils::toUTF8(SGCore::Utils::replaceAll<char16_t>(path.u16string(), u"/", u"_"));
        replacedPath = SGCore::Utils::replaceAll<char>(replacedPath, "\\", "_");
        replacedPath = SGCore::Utils::replaceAll<char>(replacedPath, ":", "_");
        replacedPath = SGCore::Utils::replaceAll<char>(replacedPath, ".", "_");

        // todo: make for hlsl and other
        SGCore::FileUtils::writeToFile(translator.m_config.m_outputDebugDirectoryPath + "/" + replacedPath + ".glsl",
                                        analyzedFile->getAllCode(), false, true);
    }
}

void
SGCore::SGSLETranslator::processCode(const std::filesystem::path& path, const std::string& code, SGSLETranslator& translator, const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept
{
    processCode(path, code, translator, true, analyzedFile);
}

void
SGCore::SGSLETranslator::processCode(const std::filesystem::path& path, const std::string& code, const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept
{
    processCode(path, code, *this, analyzedFile);
}

std::string SGCore::SGSLETranslator::sgsleCodeCorrector(const std::string& code)
{
    // EOL is end-of-line
    // WO - without
    
    enum CommentMode
    {
        NO_COMMENT,
        SINGLE_LINE_COMMENT,
        MULTILINE_COMMENT
    };
    
    std::string outputStr;
    
    std::string currentLine;
    
    size_t currentCharOfStrIdxWOSpaces = 0;
    
    bool preProcDirDeclared = false;
    
    char lastChar = '\0';
    
    CommentMode commentMode = CommentMode::NO_COMMENT;
    
    // new line if met symbol '{' or '}'
    // new line by the EOL character if first character of string '#'
    // else collecting characters to line while not met ';'
    for(char c : code)
    {
        if(lastChar == '*' && c == '/' && !preProcDirDeclared)
        {
            commentMode = CommentMode::NO_COMMENT;
            currentCharOfStrIdxWOSpaces = 0;
            continue;
        }
        
        if(lastChar == '/' && c == '/' && !preProcDirDeclared)
        {
            commentMode = CommentMode::SINGLE_LINE_COMMENT;
            currentLine.erase(currentLine.end() - 1);
            continue;
        }
        
        if(commentMode == CommentMode::SINGLE_LINE_COMMENT && c == '\n')
        {
            currentCharOfStrIdxWOSpaces = 0;
            commentMode = CommentMode::NO_COMMENT;
            continue;
        }
        
        if(commentMode == CommentMode::NO_COMMENT)
        {
            if(lastChar == '/' && c == '*' && !preProcDirDeclared)
            {
                commentMode = CommentMode::MULTILINE_COMMENT;
                currentLine.erase(currentLine.end() - 1);
                continue;
            }
            
            if(c == '#' && currentCharOfStrIdxWOSpaces == 0)
            {
                preProcDirDeclared = true;
            }
            
            if(c == '\n' && preProcDirDeclared)
            {
                currentCharOfStrIdxWOSpaces = 0;
                preProcDirDeclared = false;
                currentLine += c;
                outputStr += SGCore::Utils::reduce(currentLine);
                currentLine = "";
                
                continue;
            }
            
            if(c == ';' && !preProcDirDeclared)
            {
                currentCharOfStrIdxWOSpaces = 0;
                currentLine += c;
                currentLine += '\n';
                outputStr += SGCore::Utils::reduce(currentLine);
                currentLine = "";
                
                continue;
            }
            
            if((c == '{' || c == '}') && !preProcDirDeclared)
            {
                currentLine += c;
                currentLine += '\n';
                
                outputStr += SGCore::Utils::reduce(currentLine);
                currentLine = "";
                
                currentCharOfStrIdxWOSpaces = 0;
                
                continue;
            }
            
            if(c != '\n')
            {
                currentLine += c;
            } else
            {
                currentLine += ' ';
            }
            
            if(c != ' ' && c != '\n')
            {
                ++currentCharOfStrIdxWOSpaces;
            }
        }
        
        lastChar = c;
    }
    
    return outputStr;
}

void
SGCore::SGSLETranslator::sgslePreProcessor(const std::string& path, const std::string& code,
                                           SGSLETranslator& translator,
                                           const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept
{
    std::stringstream codeStream(code);
    
    std::string line;
    
    size_t lineIdx = 0;
    
    while(std::getline(codeStream, line))
    {
        std::vector<std::string> words;
        SGCore::Utils::splitString(line, ' ', words);
        
        bool append = true;
        
        if(lineIdx == 0)
        {
            line = "// code from shader by path: '" + path + "'\n" + line;
        }
        
        if(words.size() >= 2 && words[0] == "#sg_pragma" && words[1] == "once")
        {
            translator.m_includedFiles.insert(path);
            
            append = false;
        }
        
        if(!words.empty() && words[0] == "#sg_include")
        {
            std::string includedFilePath = SGCore::Utils::toString(words.begin() + 1, words.end());
            std::string finalIncludedFilePath = std::filesystem::path(path).parent_path().string() + "/" +
                                                std::string(includedFilePath.begin() + 1, includedFilePath.end() - 1);
            
            
            // std::string f = finalIncludedFilePath;
            finalIncludedFilePath = SGCore::Utils::getRealPath(finalIncludedFilePath);
            if(!std::filesystem::exists(finalIncludedFilePath))
            {
                finalIncludedFilePath = SGCore::Utils::toUTF8(findRealIncludePath(std::string(includedFilePath.begin() + 1, includedFilePath.end() - 1)).u16string());

                // trying to include file using relative to executable file path
                /*finalIncludedFilePath = std::filesystem::current_path().string() + "/" +
                                        std::string(includedFilePath.begin() + 1, includedFilePath.end() - 1);
                // f = finalIncludedFilePath;
                finalIncludedFilePath = SGCore::Utils::getRealPath(finalIncludedFilePath);*/
                
                //std::cout << "exec path: " << std::filesystem::current_path().string() << ", f: " << f << ", realpath: " << finalIncludedFilePath << std::endl;
            }
            
            bool alreadyIncluded = false;
            for(const auto& p : translator.m_includedFiles)
            {
                std::filesystem::path rp(p);
                if(rp == std::filesystem::path(finalIncludedFilePath))
                {
                    alreadyIncluded = true;
                    break;
                }
            }
            
            if(!alreadyIncluded)
            {
                auto includedAnalyzedFile = MakeRef<ShaderAnalyzedFile>();
                
                translator.processCode(finalIncludedFilePath, SGCore::FileUtils::readFile(finalIncludedFilePath),
                            translator, false, includedAnalyzedFile);
                
                analyzedFile->includeFile(includedAnalyzedFile);
            }
            
            append = false;
        }
        
        std::smatch sgslFuncWithArgsCallRegexMatch;
        
        if(std::regex_search(line, sgslFuncWithArgsCallRegexMatch, m_sgslFuncWithArgsCallRegex))
        {
            std::smatch blockArgsMatch;
            
            std::string blockName = sgslFuncWithArgsCallRegexMatch[1];
            std::string argsString = sgslFuncWithArgsCallRegexMatch[2];
            
            std::vector<std::string> blockArgs;
            
            std::sregex_iterator iter(argsString.begin(), argsString.end(), m_funcArgsDividerRegex);
            
            while(iter != m_regexIterEnd)
            {
                std::smatch match = *iter;
                blockArgs.push_back(match[0].str());
                ++iter;
            }
            
            if(blockName == "SGSubPass")
            {
                for(const auto& subPassName : blockArgs)
                {
                    SGSLESubPass* subPass = &analyzedFile->m_subPasses[subPassName];
                    subPass->m_name = subPassName;
                    subPass->m_onBlockEnd = [&translator, &subPass]() {
                        translator.m_currentSubPasses.erase(
                                std::remove(translator.m_currentSubPasses.begin(), translator.m_currentSubPasses.end(),
                                            subPass), translator.m_currentSubPasses.end());
                    };
                    translator.m_currentSubPasses.push_back(subPass);
                }
                append = false;
            }
            else if(blockName == "SGSubShader")
            {
                for(const auto& subPass : translator.m_currentSubPasses)
                {
                    for(const auto& subShaderName : blockArgs)
                    {
                        SGSLESubShaderType shaderType = stringToSGSLESubShaderType(subShaderName);
                        
                        SGSLESubShader* subShader = &*analyzedFile->addOrGetSubShader(subPass->m_name, shaderType);
                        
                        subShader->m_type = shaderType;
                        subShader->m_onBlockEnd = [&translator, &subShader, &subPass]() {
                            translator.m_currentSubShaders.erase(std::remove(translator.m_currentSubShaders.begin(),
                                                                             translator.m_currentSubShaders.end(),
                                                                             std::make_pair(subPass, subShader)),
                                                                 translator.m_currentSubShaders.end());
                        };
                        
                        translator.m_currentSubShaders.emplace_back(subPass, subShader);
                    }
                }
                append = false;
            }
        }
        
        if(line.ends_with("{"))
        {
            for(auto* subPass : translator.m_currentSubPasses)
            {
                ++subPass->m_openedBracketsCount;
            }
            
            for(auto& subShader : translator.m_currentSubShaders)
            {
                ++subShader.second->m_openedBracketsCount;
            }
        }
        else if(line.ends_with("}"))
        {
            auto subPassesIt = translator.m_currentSubPasses.begin();
            while(subPassesIt != translator.m_currentSubPasses.end())
            {
                --(*subPassesIt)->m_openedBracketsCount;
                
                if((*subPassesIt)->m_openedBracketsCount == 0)
                {
                    subPassesIt = translator.m_currentSubPasses.erase(subPassesIt);
                    append = false;
                } else
                {
                    ++subPassesIt;
                }
            }
            
            auto subShadersIt = translator.m_currentSubShaders.begin();
            while(subShadersIt != translator.m_currentSubShaders.end())
            {
                --(*subShadersIt).second->m_openedBracketsCount;
                
                if((*subShadersIt).second->m_openedBracketsCount == 0)
                {
                    subShadersIt = translator.m_currentSubShaders.erase(subShadersIt);
                    append = false;
                } else
                {
                    ++subShadersIt;
                }
            }
        }
        
        if(append)
        {
            if(!translator.m_currentSubPasses.empty())
            {
                for(auto& subPass : translator.m_currentSubPasses)
                {
                    size_t currentSubPassSubShadersCnt = 0;
                    for(auto& subShaderPair : translator.m_currentSubShaders)
                    {
                        if(subShaderPair.first->m_name == subPass->m_name)
                        {
                            analyzedFile->addOrGetSubShader(subPass->m_name, subShaderPair.second->m_type)->m_code +=
                                    line + "\n";
                            ++currentSubPassSubShadersCnt;
                        }
                    }
                    
                    if(currentSubPassSubShadersCnt == 0)
                    {
                        subPass->m_globalCode += line + "\n";
                    }
                }
            }
            else
            {
                analyzedFile->m_globalCode += line + "\n";
            }
            
            ++lineIdx;
        }
    }
}

void
SGCore::SGSLETranslator::sgslePreprocessor(const std::string& path, const std::string& code, const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept
{
    sgslePreProcessor(path, code, *this, analyzedFile);
}

void
SGCore::SGSLETranslator::sgsleMainProcessor(const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile, SGSLETranslator& translator) noexcept
{
    for(auto& subPass : analyzedFile->m_subPasses)
    {
        for(auto& subShaderIter : subPass.second.m_subShaders)
        {
            Ref<SGSLESubShader> subShader = subShaderIter.second;
            
            std::vector<std::string> lines;
            SGCore::Utils::splitString(subShader->m_code, '\n', lines);
            
            subShader->m_code = "";

            size_t curLineIdx = 0;
            for(auto& line : lines)
            {
                std::vector<std::string> splittedBySpaceLine;
                SGCore::Utils::splitString(line, ' ', splittedBySpaceLine);
                
                std::vector<std::string> assignExprSplitted;
                SGCore::Utils::splitString(line, '=', assignExprSplitted);
                
                // members calls processing
                std::smatch variableFuncCallRegexMatch;
                if(std::regex_search(line, variableFuncCallRegexMatch, m_sgslVariableFuncCallRegex))
                {
                    std::string variableName = variableFuncCallRegexMatch[1];
                    std::string variableFunction = variableFuncCallRegexMatch[2];
                    
                    std::string fullMatch = variableName + "." + variableFunction;
                    
                    // todo:
                    if(variableFunction == "sg_length()")
                    {
                        line = SGCore::Utils::replaceAll(line, fullMatch, variableName + "_CURRENT_COUNT");
                    }
                }
                
                if(splittedBySpaceLine.size() >= 2)
                {
                    if(splittedBySpaceLine[0] == "struct")
                    {
                        SGSLEStruct sgsleStruct;
                        sgsleStruct.m_name = SGCore::Utils::replaceAll<char>(splittedBySpaceLine[1], "{", "");
                        
                        for(size_t i = 2; i < splittedBySpaceLine.size(); i += 2)
                        {
                            if(splittedBySpaceLine[i] == "extends")
                            {
                                std::string clearBaseName = SGCore::Utils::replaceAll<char>(splittedBySpaceLine[i + 1], "{",
                                                                                             "");
                                
                                line = SGCore::Utils::replaceAll<char>(line, "extends " + clearBaseName, "");
                                line = SGCore::Utils::reduce(line);
                                
                                SGSLEStruct* baseStruct = subShader->tryGetStruct(clearBaseName);
                                
                                if(!baseStruct)
                                {
                                    // todo: print error that is base struct is not declared
                                    continue;
                                }
                                
                                sgsleStruct.m_bases.push_back(clearBaseName);
                                
                                line += "\n// base struct '" + baseStruct->m_name + "' variables: \n";
                                for(auto& variable : baseStruct->m_variables)
                                {
                                    if(std::find(sgsleStruct.m_variables.begin(), sgsleStruct.m_variables.end(), variable) != sgsleStruct.m_variables.end()) continue;
                                    sgsleStruct.m_variables.push_back(variable);
                                    line += variable + '\n';
                                }
                                // line = std::string(line.begin(), line.end() - 1);
                            }
                        }
                        
                        for(size_t i = curLineIdx + 1; !lines[i].ends_with("}"); ++i)
                        {
                            sgsleStruct.m_variables.push_back(lines[i]);
                        }
                        
                        subShader->m_structs.push_back(sgsleStruct);
                    }
                    
                    // processing declare
                    // todo:
                    if(splittedBySpaceLine[0] == "SGSampler2D" || splittedBySpaceLine[0] == "SGSamplerCube")
                    {
                        std::smatch variableDeclRegexMatch;
                        
                        std::string variableType = splittedBySpaceLine[0];
                        
                        SGSLEVariable variable;
                        
                        variable.m_lValueVarType = splittedBySpaceLine[0];
                        
                        // variables can be only array
                        if(std::regex_search(line, variableDeclRegexMatch, m_arrayVariableDeclRegex))
                        {
                            std::string variableName = variableDeclRegexMatch[1];
                            std::string variableArraySize = variableDeclRegexMatch[2];
                            
                            variable.m_lValueVarName = variableName;
                            variable.m_isLValueArray = true;
                            
                            if(variableArraySize.empty())
                            {
                                // todo: print error
                                continue;
                            }
                            
                            variable.m_lValueArraySize = variableArraySize.empty() ? 0 : std::atoi(variableArraySize.c_str());
                        }
                        
                        subShader->m_variables.push_back(variable);
                        
                        std::string defineName = "__" + variable.m_lValueVarName + "_MAX_COUNT__";
                        
                        line = "";
                        line += "#define " + defineName + " " + std::to_string(variable.m_lValueArraySize) + "\n";
                        line += "uniform " + SGSLEUtils::sgsleTypeToGLSL(variableType) + " " + variable.m_lValueVarName + "[" + defineName + "];\n";
                        line += "uniform int " + variable.m_lValueVarName + "_CURRENT_COUNT;\n";
                        // continue;
                    }
                }
                
                // processing assign
                if(assignExprSplitted.size() >= 2)
                {
                    std::string& lSide = assignExprSplitted[0];
                    std::string& rSide = assignExprSplitted[1];
                    
                    bool isSGFuncCall = false;
                    
                    std::string sgFuncName;
                    std::vector<std::string> sgFuncArgs;
                    
                    std::smatch sgFuncCallRegexMatch;
                    if(std::regex_search(rSide, sgFuncCallRegexMatch, m_rSideSGFuncCallRegex))
                    {
                        sgFuncName = sgFuncCallRegexMatch[1];
                        if(sgFuncName == "SGGetTextures" || sgFuncName == "SGGetTexturesFromMaterial")
                        {
                            isSGFuncCall = true;
                            std::string concatenatedArgs = sgFuncCallRegexMatch[2];
                            
                            std::smatch sgFuncArgsDividerRegexMatch;
                            if(std::regex_search(concatenatedArgs, sgFuncArgsDividerRegexMatch, m_funcArgsDividerRegex))
                            {
                                for(const auto& arg : sgFuncArgsDividerRegexMatch)
                                {
                                    sgFuncArgs.push_back(arg);
                                }
                            }
                        }
                    }
                    
                    if(isSGFuncCall)
                    {
                        std::smatch variableAssignRegexMatch;
                        if(std::regex_search(lSide, variableAssignRegexMatch, m_lSideOfAssignExprRegex0))
                        {
                            std::string variableName = variableAssignRegexMatch[1];
                            
                            SGSLEVariable* subShaderVar = subShader->tryGetVariable(variableName);
                            
                            if(!subShaderVar)
                            {
                                // todo: print error that is variable is not declared
                                continue;
                            }
                            
                            SGSLEAssignExpression assignExpression;
                            
                            assignExpression.m_rvalueFunctionName = sgFuncName;
                            assignExpression.m_rvalueFunctionArgs = sgFuncArgs;
                            
                            size_t startIdx = std::atoi(variableAssignRegexMatch[2].str().c_str());
                            if(variableAssignRegexMatch.size() == 4 && !variableAssignRegexMatch[3].str().empty())
                            {
                                size_t endIdx = std::atoi(variableAssignRegexMatch[3].str().c_str());
                                
                                if(endIdx < startIdx)
                                {
                                    std::cout << "err" << std::endl;
                                    // todo: print error
                                    continue;
                                }
                                
                                if(endIdx > subShaderVar->m_lValueArraySize)
                                {
                                    // todo: print error
                                    continue;
                                }
                                
                                for(size_t i = startIdx; i <= endIdx; ++i)
                                {
                                    assignExpression.m_arrayIndices.push_back(i);
                                }
                            }
                            else
                            {
                                assignExpression.m_arrayIndices.push_back(startIdx);
                            }
                            
                            subShaderVar->m_assignExpressions.push_back(assignExpression);
                        }
                        
                        continue;
                    }
                }

                subShader->m_code += line + '\n';
                
                ++curLineIdx;
            }
            
            if(translator.m_config.m_useOutputDebug)
            {
                sgsleMakeSubShaderCodePretty(subShader);
            }
        }
    }
}

void SGCore::SGSLETranslator::sgsleMakeSubShaderCodePretty(const Ref<SGCore::SGSLESubShader>& subShader) const noexcept
{
    std::stringstream codeStream(subShader->m_code);
    subShader->m_code = "";
    
    char lastLineChar = ' ';
    int currentIntent = 0;
    std::string line;
    
    while(std::getline(codeStream, line))
    {
        if(lastLineChar == '{')
        {
            ++currentIntent;
        }
        else if(line.ends_with('}'))
        {
            --currentIntent;
        }

        if(!line.empty())
        {
            lastLineChar = *line.rbegin();
        }
        else
        {
            lastLineChar = ' ';
        }
        
        line = currentIntent <= 0 ? line : std::string(currentIntent, '\t') + line;
        
        subShader->m_code += line + '\n';
    }
}

void SGCore::SGSLETranslator::includeDirectory(const std::filesystem::path& dirPath) noexcept
{
    s_includedDirectories.push_back(dirPath);
}

std::filesystem::path
SGCore::SGSLETranslator::findRealIncludePath(const std::filesystem::path& originalIncludePath) noexcept
{
    std::filesystem::path finalIncludedFilePath = "unknown";

    for(const auto& includedDir : s_includedDirectories)
    {
        finalIncludedFilePath = includedDir / originalIncludePath;
        finalIncludedFilePath = SGCore::Utils::getRealPath(SGCore::Utils::toUTF8(finalIncludedFilePath.u16string()));
        if(std::filesystem::exists(finalIncludedFilePath)) break;
    }

    if(finalIncludedFilePath == "unknown")
    {
        LOG_E(SGCORE_TAG, "SGSLE PROCESSOR: can not find real include path for path '{}'. Check that you include all needed directories using function 'includeDirectory'.",
              SGCore::Utils::toUTF8(originalIncludePath.u16string()));
        return originalIncludePath;
    }

    return finalIncludedFilePath;
}
