//
// Created by stuka on 13.12.2024.
//

#include "SGSLETranslator.h"
#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"

void SGCore::SGSLETranslator::processCode(const std::filesystem::path& path, const std::string& code,
                                          const Ref<ShaderAnalyzedFile>& toAnalyzedFile) noexcept
{
    const std::string preprocessedCode = preprocessorPass(path, code);
    // std::cout << preprocessedCode << std::endl;

    translateCode(path, preprocessedCode, toAnalyzedFile);

    std::string savePath = Utils::toUTF8(path.u16string());

    std::replace(savePath.begin(), savePath.end(), '/', '_');
    std::replace(savePath.begin(), savePath.end(), '\\', '_');
    std::replace(savePath.begin(), savePath.end(), '.', '_');
    std::replace(savePath.begin(), savePath.end(), ':', '_');

    for(auto& s : toAnalyzedFile->getSubShaders())
    {
        std::string subShaderType;
        switch(s.getType())
        {
            case SST_NONE: subShaderType = "none"; break;
            case SST_VERTEX: subShaderType = "vertex"; break;
            case SST_FRAGMENT: subShaderType = "fragment"; break;
            case SST_GEOMETRY: subShaderType = "geometry"; break;
            case SST_COMPUTE: subShaderType = "compute"; break;
            case SST_TESS_CONTROL: subShaderType = "tesselation-control"; break;
            case SST_TESS_EVALUATION: subShaderType = "tesselation-evalution"; break;
        }

        FileUtils::writeToFile(std::filesystem::path("SGSLETranslatorDebug") / (savePath + "_" + subShaderType + ".shader"), s.getCode(), false, true);
    }
}

std::string SGCore::SGSLETranslator::preprocessorPass(const std::filesystem::path& path, const std::string& code) noexcept
{
    std::string outputCode;

    size_t lineIdx = 0;

    std::string commentToken;
    std::string uncommentToken;

    for(size_t i = 0; i < code.size(); ++i)
    {
        const auto& c = code[i];
        char prevC = ' ';
        if(((std::int64_t) i) - 1 >= 0)
        {
            prevC = code[i - 1];
        }

        // processing comments ====================================
        if(c == '/')
        {
            if(i + 1 < code.size())
            {
                if(code[i + 1] == '/')
                {
                    commentToken = "//";
                    ++i;
                }
                else if(code[i + 1] == '*')
                {
                    commentToken = "/*";
                    ++i;
                }
            }
        }

        if(c == '*')
        {
            if(i + 1 < code.size())
            {
                if(code[i + 1] == '/')
                {
                    uncommentToken = "*/";
                    ++i;
                }
            }
        }

        if(commentToken == "//" && c == '\n')
        {
            commentToken = "";
            ++lineIdx;
            continue;
        }

        if(commentToken == "/*" && uncommentToken == "*/")
        {
            commentToken = "";
            uncommentToken = "";
            ++lineIdx;
            continue;
        }

        if(isValidCommentToken(commentToken))
        {
            ++lineIdx;
            continue;
        }
        // ======================================================

        // if we have next word
        if(prevC == ' ' || prevC == '\n')
        {
            auto wordPair = readWord(i, code);
            if(wordPair.first == "#include")
            {
                auto includedFilePair = readExpr(i + wordPair.second, code, "\"", "\"");
                std::filesystem::path includedFilePath = Utils::getRealPath(includedFilePair.first);
                if(!std::filesystem::exists(includedFilePath))
                {
                    includedFilePath =
                            Utils::toUTF8(findRealIncludePath(std::filesystem::path(
                                                                      includedFilePair.first.begin(),
                                                                      includedFilePair.first.end()
                                                              )
                            ).u16string());

                    // finally trying to include file near the current file
                    if(!std::filesystem::exists(includedFilePath))
                    {
                        includedFilePath = SGCore::Utils::getRealPath(Utils::toUTF8((path.parent_path() / includedFilePair.first).u16string()));
                    }
                }

                if(!m_includedPaths.contains(includedFilePath))
                {
                    if(std::filesystem::exists(includedFilePath))
                    {
                        LOG_I(SGCORE_TAG, "Including path: '{}'", Utils::toUTF8(includedFilePath.u16string()));

                        outputCode += preprocessorPass(includedFilePath, FileUtils::readFile(includedFilePath));

                        m_includedPaths.insert(includedFilePath);
                    }
                    else
                    {
                        LOG_E(SGCORE_TAG, "SGSLE PREPROCESSING: In file '{}': error including file by path '{}': this file does not exist!",
                              Utils::toUTF8(path.u16string()),
                              Utils::toUTF8(includedFilePath.u16string()));
                    }
                }

                i += wordPair.second + includedFilePair.second;
                continue;
            }
        }

        outputCode += c;

        if(c == '\n')
        {
            ++lineIdx;
            // continue;
        }
    }

    return outputCode;
}

void SGCore::SGSLETranslator::translateCode(const std::filesystem::path& path, const std::string& code,
                                            const SGCore::Ref<SGCore::ShaderAnalyzedFile>& toAnalyzedFile) noexcept
{
    SGSLESubShader* currentSubShader = nullptr;

    std::string outputCode;

    // adding subshaders of all types to analyzed file to add global code in subshaders
    for(int i = SGSLESubShaderType::SST_VERTEX; i <= SGSLESubShaderType::SST_TESS_EVALUATION; ++i)
    {
        if(!toAnalyzedFile->getSubShaderByType((SGSLESubShaderType) i))
        {
            SGSLESubShader subShader;
            subShader.m_type = (SGSLESubShaderType) i;
            toAnalyzedFile->m_subShaders.push_back(subShader);
        }
    }

    // subshaders that were met in file by path
    std::unordered_set<SGSLESubShaderType> metSubShaders;

    for(size_t i = 0; i < code.size(); ++i)
    {
        const auto& c = code[i];
        char prevC = ' ';
        if(((std::int64_t) i) - 1 >= 0)
        {
            prevC = code[i - 1];
        }

        // if we have next word
        if(prevC == ' ' || prevC == '\n')
        {
            SGSLESubShaderType curSubShaderType = SGSLESubShaderType::SST_NONE;
            bool isValidKeyWordFound = false;

            auto wordPair = readWord(i, code);
            const auto& word = wordPair.first;
            const auto& wordOffset = wordPair.second;
            // trying to process shader type directives
            {
                if(word == "#vertex")
                {
                    curSubShaderType = SGSLESubShaderType::SST_VERTEX;
                }
                else if(word == "#fragment")
                {
                    curSubShaderType = SGSLESubShaderType::SST_FRAGMENT;
                }
                else if(word == "#geometry")
                {
                    curSubShaderType = SGSLESubShaderType::SST_GEOMETRY;
                }
                else if(word == "#compute")
                {
                    curSubShaderType = SGSLESubShaderType::SST_COMPUTE;
                }
                else if(word == "#tess_control")
                {
                    curSubShaderType = SGSLESubShaderType::SST_TESS_CONTROL;
                }
                else if(word == "#tess_eval")
                {
                    curSubShaderType = SGSLESubShaderType::SST_TESS_EVALUATION;
                }
                else if(word == "#end")
                {
                    currentSubShader = nullptr;

                    isValidKeyWordFound = true;
                }

                if(curSubShaderType != SGSLESubShaderType::SST_NONE)
                {
                    if(currentSubShader)
                    {
                        LOG_E(SGCORE_TAG, "The declaration of a new SubShader was met (decl: '{}'),"
                                          " but the previous SubShader was not completed with the #end directive. Translation aborted.", word);

                        break;
                    }

                    metSubShaders.insert(curSubShaderType);

                    currentSubShader = toAnalyzedFile->getSubShaderByType(curSubShaderType);
                    if(!currentSubShader)
                    {
                        toAnalyzedFile->m_subShaders.emplace_back();
                        currentSubShader = &*toAnalyzedFile->m_subShaders.rbegin();
                        currentSubShader->m_type = curSubShaderType;
                    }

                    isValidKeyWordFound = true;
                }
            }

            if(word == "#subpass")
            {
                auto subPassNameFilePair = readExpr(i + wordOffset, code, "[", "]");
                toAnalyzedFile->m_subPassName = subPassNameFilePair.first;

                i += subPassNameFilePair.second;

                isValidKeyWordFound = true;
            }

            if(word == "#attribute")
            {
                auto attribNamePair = readExpr(i + wordOffset, code, "[", "]");
                auto attribValuePair = readExpr(i + wordOffset + attribNamePair.second, code, "[", "]");

                toAnalyzedFile->m_attributes[attribNamePair.first] = attribValuePair.first;

                i += attribNamePair.second + attribValuePair.second;

                isValidKeyWordFound = true;
            }

            if(isValidKeyWordFound)
            {
                i += wordOffset;
                continue;
            }
        }

        // if we have current bind subshader then adding code to this subshader
        if(currentSubShader)
        {
            currentSubShader->m_code += c;
        }
        else
        {
            // else we are adding code to all subshaders because it is global code
            for(auto& subShader : toAnalyzedFile->m_subShaders)
            {
                subShader.m_code += c;
            }
        }
    }

    // finally removing all subshaders from analyzed file that were not met in file by path
    {
        auto it = toAnalyzedFile->m_subShaders.begin();
        while(it != toAnalyzedFile->m_subShaders.end())
        {
            if(!metSubShaders.contains(it->m_type))
            {
                it = toAnalyzedFile->m_subShaders.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    if(toAnalyzedFile->m_subPassName.empty())
    {
        LOG_E(SGCORE_TAG, "SGSLE ERROR: #subpass is required for shader by path '{}'.", Utils::toUTF8(path.u16string()));
    }
    else
    {
        LOG_I(SGCORE_TAG, "SGSLE: Translated shader by path '{}'. Shader is using subpass '{}'.",
              Utils::toUTF8(path.u16string()), toAnalyzedFile->m_subPassName);
    }
}

void SGCore::SGSLETranslator::includeDirectory(const std::filesystem::path& dirPath) noexcept
{
    s_includedDirectories.push_back(dirPath);
}

bool SGCore::SGSLETranslator::isValidCommentToken(const std::string& commentToken) noexcept
{
    return commentToken == "//" || commentToken == "/*";
}

std::pair<std::string, size_t> SGCore::SGSLETranslator::readExpr(const size_t& startIdx, const std::string& code,
                                                                 const std::string& exprFenceBegin,
                                                                 const std::string& exprFenceEnd) noexcept
{
    std::string outputLine;
    size_t i = startIdx;
    bool isExprStarted = false;
    std::string curWord;

    const size_t maxFenceSize =
            exprFenceBegin.size() > exprFenceEnd.size() ? exprFenceBegin.size() : exprFenceEnd.size();

    for(; i < code.size(); ++i)
    {
        const auto& c = code[i];

        curWord = "";

        bool foundExprStart = false;
        for(size_t j = i; j < i + maxFenceSize && j < code.size(); ++j)
        {
            curWord += code[j];
            if(curWord == exprFenceBegin && !isExprStarted)
            {
                i += curWord.size() - 1;

                curWord = "";

                isExprStarted = true;
                foundExprStart = true;

                break;
            }
            else if(curWord == exprFenceEnd && isExprStarted)
            {
                return { outputLine, i - startIdx };
            }
        }

        if(foundExprStart)
        {
            continue;
        }

        if(isExprStarted)
        {
            outputLine += c;
        }
    }

    return { outputLine, i - startIdx };
}

std::pair<std::string, size_t>
SGCore::SGSLETranslator::readWord(const size_t& startIdx, const std::string& code) noexcept
{
    std::string outputLine;
    size_t i = startIdx;
    for(;; ++i)
    {
        const auto& c = code[i];

        if(c == '\n' || c == '\0' || c == ' ')
        {
            break;
        }

        outputLine += c;
    }

    return { outputLine, i - startIdx };
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
