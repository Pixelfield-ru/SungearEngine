//
// Created by stuka on 13.12.2024.
//

#include "SGSLETranslator.h"
#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"

void SGCore::SGSLETranslator::processCode(const std::filesystem::path& path, const std::string& code,
                                          const Ref <ShaderAnalyzedFile>& toAnalyzedFile) noexcept
{
    const std::string preprocessedCode = preprocessorPass(code);
    std::cout << preprocessedCode << std::endl;
}

std::string SGCore::SGSLETranslator::preprocessorPass(const std::string& code) noexcept
{
    std::string outputCode;

    size_t lineIdx = 0;

    std::string currentWord;

    std::string commentToken;
    std::string uncommentToken;

    for(size_t i = 0; i < code.size(); ++i)
    {
        const auto& c = code[i];

        currentWord += c;

        // processing comments ====================================
        if(c == '*')
        {
            if(!isValidCommentToken(commentToken))
            {
                commentToken += c;
            }
            else
            {
                uncommentToken += c;
            }
        }

        if(c == '/')
        {
            if(!isValidCommentToken(commentToken))
            {
                commentToken += c;
            }
            else
            {
                uncommentToken += c;
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
            ++lineIdx;
            continue;
        }

        if(isValidCommentToken(commentToken))
        {
            ++lineIdx;
            continue;
        }
        // ======================================================


        if(c == ' ' || c == '\n')
        {
            if(currentWord == "#include")
            {
                auto includedFilePair = readLine(i + 1, code);
                std::filesystem::path includedFilePath = Utils::getRealPath(includedFilePair.first);
                if(!std::filesystem::exists(includedFilePath))
                {
                    const auto includedFilePathAsString = Utils::toUTF8(includedFilePath.u16string());

                    includedFilePath =
                            Utils::toUTF8(findRealIncludePath(std::filesystem::path(
                                                                      includedFilePathAsString.begin() + 1,
                                                                      includedFilePathAsString.end() - 1
                                                              )
                            ).u16string());

                    if(!m_includedPaths.contains(includedFilePath))
                    {
                        outputCode += FileUtils::readFile(includedFilePath);

                        m_includedPaths.insert(includedFilePath);
                    }

                }

                i += includedFilePair.second;
            }

            currentWord = "";
        }

        outputCode += c;

        if(c == '\n')
        {
            ++lineIdx;
            continue;
        }
    }

    /*while(std::getline(codeStream, line))
    {
        std::vector<std::string> words;
        SGCore::Utils::splitString(line, ' ', words);

        if()
    }*/

    return outputCode;
}

void SGCore::SGSLETranslator::includeDirectory(const std::filesystem::path& dirPath) noexcept
{
    s_includedDirectories.push_back(dirPath);
}

bool SGCore::SGSLETranslator::isValidCommentToken(const std::string& commentToken) noexcept
{
    return commentToken == "//" || commentToken == "/*";
}

std::pair<std::string, size_t> SGCore::SGSLETranslator::readLine(const size_t& startIdx, const std::string& code) noexcept
{
    std::string outputLine;
    size_t i = startIdx;
    for(;; ++i)
    {
        const auto& c = code[i];

        if(c == '\n' || c == '\0')
        {
            break;
        }

        outputLine += c;
    }

    return { outputLine, i };
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
