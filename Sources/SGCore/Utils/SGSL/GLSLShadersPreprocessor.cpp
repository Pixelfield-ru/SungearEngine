//
// Created by stuka on 29.12.2023.
//

#include "GLSLShadersPreprocessor.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/FileAsset.h"

SGCore::ShaderAnalyzedFile SGCore::GLSLShadersPreprocessor::processCode(const std::string& shaderFilePath,
                                                                        const std::string_view& rawCode,
                                                                        GLSLShadersPreprocessor& preprocessor) noexcept
{
    ShaderAnalyzedFile shaderAnalyzedFile;
    shaderAnalyzedFile.m_path = shaderFilePath;

    std::filesystem::path shaderFSPath(shaderFilePath);

    std::istringstream codeStream(rawCode.data());

    std::string line;

    size_t linesRawCount = 1;

    char curChar;
    size_t curCharCount = 0;

    std::vector<std::string> lineWords;

    bool useSemicolonDelim = true;

    bool isMultiLineComment = false;
    bool commented = false;

    size_t ifdefsCount = 0;
    size_t endifsCount = 0;
    size_t inactiveIfDefIdx = 0;

    while(curCharCount != rawCode.size())
    {
        curChar = rawCode[curCharCount];

        if(curChar == '\n' && useSemicolonDelim) { }
        else
        {
            if(!commented) line += curChar;
        }

        if(curChar == '\n')
        {
            ++linesRawCount;

            if(!isMultiLineComment && commented)
            {
                commented = false;
                line += "\n";
            }
        }

        if(curChar == '#')
        {
            useSemicolonDelim = false;
        }

        if(curChar == '{' || curChar == '}')
        {
            useSemicolonDelim = false;
        }

        bool endOfFile = curCharCount == rawCode.size() - 1;

        if((useSemicolonDelim && curChar == ';') || endOfFile)
        {
            line = Utils::replaceAll(line, "//", "");
            line = Utils::replaceAll(line, "/*", "");

            // without
            std::string lineWOSpaces = Utils::replaceAll(line, " ", "");

            if(shaderAnalyzedFile.m_currentIfDefDirectivePassed)
            {
                checkAndCompileSGSLExpr(line, linesRawCount, shaderAnalyzedFile, preprocessor);
                if(!lineWOSpaces.starts_with("#"))
                {
                    appendLine(line + "\n", shaderAnalyzedFile);
                }
            }

            line = "";

            useSemicolonDelim = true;
        }
        else if((!useSemicolonDelim && curChar == '\n') || endOfFile)
        { // todo: make subpasses and subshader process here
            line = Utils::replaceAll(line, "//", "");
            line = Utils::replaceAll(line, "/*", "");

            // without
            std::string lineWOSpaces = Utils::replaceAll(line, " ", "");

            if(lineWOSpaces.starts_with("SGUniformsDeclaration{") && shaderAnalyzedFile.m_currentIfDefDirectivePassed)
            {
                preprocessor.m_sgslUniformsDeclaration = true;

                line = Utils::replaceAll(line, "SGUniformsDeclaration", "");
                line = Utils::replaceFirst(line, "{", "");
            }

            // SGSL BLOCKS DECL
            static std::smatch sgslSubBlocksMatch;
            if(std::regex_search(lineWOSpaces, sgslSubBlocksMatch, m_sgslSubPassesDeclRegex))
            {
                static std::smatch argsMatch;

                std::string blockName = sgslSubBlocksMatch[1];
                std::string argsString = sgslSubBlocksMatch[2];

                std::vector<std::string> blockArgs;

                if(blockName == "SGSubPass" || blockName == "SGSubShader")
                {
                    std::sregex_iterator iter(argsString.begin(), argsString.end(), m_nonStringArgsDividerRegex);

                    while(iter != end)
                    {
                        std::smatch match = *iter;
                        blockArgs.push_back(match[0].str());
                        ++iter;
                    }

                    line = "";
                }

                if(blockName == "SGSubShader")
                {
                    if(blockArgs.size() != 1)
                    {
                        SGCF_ERROR(
                                "SGSLInterpreter: Error: required 1 argument but " + std::to_string(blockArgs.size()) +
                                " arguments was provided. Line: "
                                + std::to_string(linesRawCount) + ". Path: " + shaderFilePath,
                                SG_LOG_CURRENT_SESSION_FILE);
                    }
                    else
                    {
                        if(shaderAnalyzedFile.m_subPassesToAppendCode.empty())
                        {
                            SGCF_ERROR("SGSLInterpreter: Error: the subshader '" + blockArgs[0] +
                                       "' block was started, but the subpass was not started. Line: "
                                       + std::to_string(linesRawCount) + ". Path: " + shaderFilePath,
                                       SG_LOG_CURRENT_SESSION_FILE);
                        }

                        SGSLSubShaderType shaderType = SGSLSubShaderType::SST_NONE;

                        if(blockArgs[0] == "Vertex")
                        {
                            shaderType = SGSLSubShaderType::SST_VERTEX;
                        }
                        else if(blockArgs[0] == "Fragment")
                        {
                            shaderType = SGSLSubShaderType::SST_FRAGMENT;
                        }
                        else if(blockArgs[0] == "Geometry")
                        {
                            shaderType = SGSLSubShaderType::SST_GEOMETRY;
                        }
                        else if(blockArgs[0] == "Compute")
                        {
                            shaderType = SGSLSubShaderType::SST_COMPUTE;
                        }
                        else if(blockArgs[0] == "TessControl")
                        {
                            shaderType = SGSLSubShaderType::SST_TESS_CONTROL;
                        }
                        else if(blockArgs[0] == "TessEvaluation")
                        {
                            shaderType = SGSLSubShaderType::SST_TESS_EVALUATION;
                        }
                        else
                        {
                            SGCF_ERROR("SGSLInterpreter: Error: unknown subshader type '" + blockArgs[0]
                                       + "'. Line: "
                                       + std::to_string(linesRawCount) + ". Path: " + shaderFilePath,
                                       SG_LOG_CURRENT_SESSION_FILE);
                        }

                        shaderAnalyzedFile.m_subShaderTypeToAppendCode = shaderType;

                        for(auto& subPass : shaderAnalyzedFile.m_subPassesToAppendCode)
                        {
                            auto* subShader = &(shaderAnalyzedFile.addOrGetSubShader(subPass->m_name, shaderType));
                            subShader->m_type = shaderType;
                            subShader->m_onBlockEnd = [&shaderAnalyzedFile]() { shaderAnalyzedFile.m_subShaderTypeToAppendCode = SGSLSubShaderType::SST_NONE; };
                            shaderAnalyzedFile.m_bracketsListeners.push_back(subShader);
                        }
                    }
                }
                else if(blockName == "SGSubPass")
                {
                    shaderAnalyzedFile.m_subPassesToAppendCode.clear();

                    for(const auto& arg : blockArgs)
                    {
                        auto* subPass = &shaderAnalyzedFile.m_subPasses[arg];
                        shaderAnalyzedFile.m_subPassesToAppendCode.push_back(subPass);

                        subPass->m_onBlockEnd = [subPass, &shaderAnalyzedFile]()
                        {
                            shaderAnalyzedFile.m_subPassesToAppendCode.erase(
                                    std::remove(shaderAnalyzedFile.m_subPassesToAppendCode.begin(),
                                                shaderAnalyzedFile.m_subPassesToAppendCode.end(), subPass
                                    ), shaderAnalyzedFile.m_subPassesToAppendCode.end());
                        };
                        subPass->m_name = arg;
                        shaderAnalyzedFile.m_bracketsListeners.push_back(subPass);
                    }
                }
            }

            checkAndCompileSGSLExpr(line, linesRawCount, shaderAnalyzedFile, preprocessor);

            if(lineWOSpaces.contains("{"))
            {
                for(auto* bracketsListener : shaderAnalyzedFile.m_bracketsListeners)
                {
                    ++bracketsListener->m_openedBracketsCount;
                }
            }

            if(lineWOSpaces.contains("}"))
            {
                if(preprocessor.m_sgslUniformsDeclaration)
                {
                    line = Utils::replaceFirst(line, "}", "");
                }
                preprocessor.m_sgslUniformsDeclaration = false;

                auto bracketsListenersIter = shaderAnalyzedFile.m_bracketsListeners.begin();
                while(bracketsListenersIter != shaderAnalyzedFile.m_bracketsListeners.end())
                {
                    auto& bracketsListener = *bracketsListenersIter;
                    --bracketsListener->m_openedBracketsCount;
                    if(bracketsListener->m_openedBracketsCount == 0)
                    {
                        bracketsListener->m_onBlockEnd();
                        bracketsListenersIter = shaderAnalyzedFile.m_bracketsListeners.erase(bracketsListenersIter);
                        line = Utils::replaceFirst(line, "}", "");
                    }
                    else
                    {
                        ++bracketsListenersIter;
                    }
                }
            }

            std::vector<std::string> words;
            Utils::splitString(line, ' ', words);

            words.erase(std::remove_if(words.begin(), words.end(), [](const std::string& word) { return word.empty(); }), words.end());

            if(!words.empty())
            {
                const auto& firstWord = Utils::replaceAll(words[0], "\n", "");

                if(firstWord == "#sg_include")
                {
                    if(!shaderAnalyzedFile.m_currentIfDefDirectivePassed) break;

                    const auto& includePath = Utils::replaceAll(words[1], "\n", "");

                    if(includePath.starts_with("\"") && includePath.ends_with("\""))
                    {
                        std::string includedShaderPath = shaderFSPath.parent_path().string() + "/" +
                                                         std::string(includePath.begin() + 1,
                                                                     includePath.end() - 1
                                                         );

                        // std::string includedShaderText = readFile(includedShaderPath);

                        // loading code of included shader
                        auto includedShaderFile =
                                AssetManager::loadAsset<FileAsset>(includedShaderPath);

                        shaderAnalyzedFile.includeFile(processCode(includedShaderPath, includedShaderFile->getData(), preprocessor));
                    }
                }
                else if(firstWord == "#sg_define")
                {
                    preprocessor.m_pushedDefines.insert(words[1]);
                }
                else if(firstWord == "#sg_ifndef")
                {
                    if(shaderAnalyzedFile.m_currentIfDefDirectivePassed)
                    {
                        shaderAnalyzedFile.m_currentIfDefDirectivePassed = !preprocessor.m_pushedDefines.contains(words[1]);
                        if(!shaderAnalyzedFile.m_currentIfDefDirectivePassed)
                        {
                            inactiveIfDefIdx = ifdefsCount;
                        }
                    }
                    ++ifdefsCount;
                }
                else if(firstWord == "#sg_ifdef")
                {
                    if(shaderAnalyzedFile.m_currentIfDefDirectivePassed)
                    {
                        shaderAnalyzedFile.m_currentIfDefDirectivePassed = preprocessor.m_pushedDefines.contains(words[1]);
                        if(!shaderAnalyzedFile.m_currentIfDefDirectivePassed)
                        {
                            inactiveIfDefIdx = ifdefsCount;
                        }
                    }
                    ++ifdefsCount;
                }
                else if(firstWord.contains("#sg_endif"))
                {
                    ++endifsCount;
                    if((ifdefsCount - inactiveIfDefIdx) == endifsCount)
                    {
                        shaderAnalyzedFile.m_currentIfDefDirectivePassed = true;
                    }
                }
                else
                {
                    if(shaderAnalyzedFile.m_currentIfDefDirectivePassed) appendLine(line, shaderAnalyzedFile);
                }
            }

            line = "";

            useSemicolonDelim = true;
        }
        else if(useSemicolonDelim && curChar == '\n')
        {
            line += " ";
        }

        if(curCharCount - 1 >= 0)
        {
            if(rawCode[curCharCount - 1] == '/' && curChar == '/')
            {
                isMultiLineComment = false;
                commented = true;
            }
            else if(rawCode[curCharCount - 1] == '/' && curChar == '*')
            {
                isMultiLineComment = true;
                commented = true;
            }
        }
        if(curCharCount + 1 < rawCode.size())
        {
            if(rawCode[curCharCount - 1] == '*' && curChar == '/')
            {
                isMultiLineComment = false;
                commented = false;
            }
        }

        ++curCharCount;
    }

    shaderAnalyzedFile.m_linesRawCount = linesRawCount;

    std::cout << "lines cnt: " << std::to_string(linesRawCount) << std::endl;

    shaderAnalyzedFile.m_subPassesToAppendCode.clear();
    shaderAnalyzedFile.m_subShaderTypeToAppendCode = SGSLSubShaderType::SST_NONE;

    shaderAnalyzedFile.m_bracketsListeners.clear();

    return shaderAnalyzedFile;
}

void SGCore::GLSLShadersPreprocessor::checkAndCompileSGSLExpr(std::string& line, const size_t& currentLineIdx,
                                                              ShaderAnalyzedFile& shaderAnalyzedFile,
                                                              GLSLShadersPreprocessor& preprocessor)
{
    static std::smatch assignExprMatch;
    static std::smatch sgslTypeFuncCallMatch;
    static std::smatch varNameMatch;
    static std::smatch arraySizeMatch;

    if(std::regex_search(line, assignExprMatch, m_assignExprsRegex) && preprocessor.m_sgslUniformsDeclaration)
    {
        std::string typeName;
        std::string variableNameWithArrSize;
        std::string variableName;
        size_t arraySize = 1;
        bool isArraySizeGiven = false;
        bool isArray = false;
        std::string funcName;
        std::vector<std::string> funcArgs;

        typeName = assignExprMatch[1];
        variableNameWithArrSize = assignExprMatch[2].str();
        if(std::regex_search(variableNameWithArrSize, arraySizeMatch, m_arraySizeRegex))
        {
            arraySize = std::stoi(arraySizeMatch[0]);
            isArraySizeGiven = true;
            isArray = true;
        }
        if(variableNameWithArrSize.ends_with("[]"))
        {
            isArray = true;
        }
        if(std::regex_search(variableNameWithArrSize, varNameMatch, m_variableNameRegex))
        {
            variableName = varNameMatch[0];
        }
        funcName = assignExprMatch[4];

        std::string assignExprMatchStr = assignExprMatch[5].str();
        std::sregex_iterator iter(assignExprMatchStr.begin(), assignExprMatchStr.end(),
                                  m_stringArgsDividerRegex
        );

        while(iter != end)
        {
            std::smatch match = *iter;
            funcArgs.push_back(match[1].str());
            ++iter;
        }

        if(std::find_if(preprocessor.m_declaredVariables.begin(), preprocessor.m_declaredVariables.end(),
                        [&variableName](const SGSLVariable& var)
                        { return var.m_name == variableName; }
        ) != preprocessor.m_declaredVariables.end())
        {
            SGCF_ERROR("SGSLInterpreter: Redeclaration of variable '" + variableName + "'. Line: "
                       + std::to_string(currentLineIdx) + ". Path: " + shaderAnalyzedFile.m_path,
                       SG_LOG_CURRENT_SESSION_FILE);
        }

        SGSLVariable sgslVariable;
        sgslVariable.m_name = variableName;
        sgslVariable.m_count = arraySize;
        sgslVariable.m_type = typeName;
        sgslVariable.m_isArray = isArray;
        sgslVariable.m_rsideFunction = funcName;
        sgslVariable.m_rsideFunctionArgs = funcArgs;

        for(auto& subPass: shaderAnalyzedFile.m_subPassesToAppendCode)
        {
            subPass->m_variables.insert(sgslVariable);
        }

        preprocessor.m_declaredVariables.insert(sgslVariable);

        if(isArraySizeGiven && arraySize < funcArgs.size())
        {
            SGCF_ERROR("SGSLInterpreter: Array size is less then return size! Line: "
                       + std::to_string(currentLineIdx) + ". Path: " + shaderAnalyzedFile.m_path,
                       SG_LOG_CURRENT_SESSION_FILE);
        }
        else if(!isArraySizeGiven && m_arraySizeNotRequiredFunctions.contains(funcName))
        {
            arraySize = funcArgs.size();
        }
        if(!isArraySizeGiven && m_arraySizeRequiredFunctions.contains(funcName))
        {
            SGCF_ERROR("SGSLInterpreter: You must give size for lvalue array for rvalue function '"
                       + funcName + "'. Line: " + std::to_string(currentLineIdx) + ". Path: "
                       + shaderAnalyzedFile.m_path, SG_LOG_CURRENT_SESSION_FILE);
        }

        // is glsl shader

        std::string glslSamplerType;
        if(sgslVariable.m_type == "SGUSampler2D")
        {
            glslSamplerType = "sampler2D";
        }
        else if(sgslVariable.m_type == "SGUSamplerCube")
        {
            glslSamplerType = "samplerCube";
        }

        if(arraySize == 1 && !isArray)
        {
            line = "uniform " + glslSamplerType + " " + variableName + ";\n";
        }
        else
        {
            line = "uniform int " + variableName + "_COUNT;\n";
            line +=
                    "uniform " + glslSamplerType + " " + variableName + "[" + std::to_string(arraySize) + "];\n";
        }
    }
    else if(std::regex_search(line, sgslTypeFuncCallMatch, m_sgslTypeFuncCallRegex))
    {
        std::string fullMatch = sgslTypeFuncCallMatch[0];
        std::string varName = sgslTypeFuncCallMatch[1];
        std::string funcName = sgslTypeFuncCallMatch[2];

        if(std::find_if(preprocessor.m_declaredVariables.begin(), preprocessor.m_declaredVariables.end(),
                        [&varName](const SGSLVariable& var)
                        { return var.m_name == varName; }
        ) == preprocessor.m_declaredVariables.end())
        {
            SGCF_ERROR("SGSLInterpreter: Variable '" + varName + "' Does not exist. Line: "
                       + std::to_string(currentLineIdx) + ". Path: " + shaderAnalyzedFile.m_path,
                       SG_LOG_CURRENT_SESSION_FILE);
        }

        std::string replaceExpr;

        if(funcName == "length()")
        {
            replaceExpr = varName + "_COUNT";
        }

        line = Utils::replaceFirst(line, varName + "." + funcName, replaceExpr);
    }
}

void SGCore::GLSLShadersPreprocessor::appendLine(const std::string& line, ShaderAnalyzedFile& shaderAnalyzedFile)
{
    if(shaderAnalyzedFile.m_subPassesToAppendCode.empty())
    {
        shaderAnalyzedFile.m_globalCode += line;
    }
    else
    {
        if(shaderAnalyzedFile.m_subShaderTypeToAppendCode == SGSLSubShaderType::SST_NONE)
        {
            for(auto& subPass: shaderAnalyzedFile.m_subPassesToAppendCode)
            {
                subPass->m_globalCode += line;
            }
        }
        else
        {
            for(auto& subPass: shaderAnalyzedFile.m_subPassesToAppendCode)
            {
                subPass->m_subShaders[shaderAnalyzedFile.m_subShaderTypeToAppendCode].m_code += line;
            }
        }
    }
}
