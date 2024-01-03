//
// Created by stuka on 26.12.2023.
//

#ifndef SGSLINTERPRETER_SHADERANALYZEDFILE_H
#define SGSLINTERPRETER_SHADERANALYZEDFILE_H

#include <string>

#include "SGSLSubPass.h"

namespace SGCore
{
    struct ShaderAnalyzedFile
    {
        friend class GLSLShadersPreprocessor;

        std::string m_path;
        std::string m_globalCode;
        size_t m_linesRawCount = 0;

        std::unordered_map<std::string, SGSLSubPass> m_subPasses;
        std::vector<ShaderAnalyzedFile> m_includedFiles;

        std::vector<SGSLBracketsListener*> m_bracketsListeners;

        void includeFile(const ShaderAnalyzedFile& analyzedFile) noexcept
        {
            m_globalCode += analyzedFile.m_globalCode + "\n";

            for(const auto& subPassPair : analyzedFile.m_subPasses)
            {
                const auto& subPassName = subPassPair.first;
                const auto& otherSubPass = subPassPair.second;

                auto& thisSubPass = m_subPasses[subPassName];

                for(const auto& subShaderPair : otherSubPass.m_subShaders)
                {
                    auto subShaderType = subShaderPair.first;
                    const auto& subShader = subShaderPair.second;

                    auto& thisSubShader = thisSubPass.m_subShaders[subShaderType];

                    thisSubShader.m_code = subShader.m_code + "\n" + thisSubShader.m_code;
                }
            }

            m_linesTotalCount += analyzedFile.m_linesTotalCount;
            m_includedFiles.push_back(analyzedFile);
        }

        SGSLSubShader& addOrGetSubShader(const std::string& subPassName, SGSLSubShaderType subShaderType)
        {
            auto& subPass = m_subPasses[subPassName];
            if(subPass.m_subShaders.find(subShaderType) == subPass.m_subShaders.end())
            {
                auto& newSubShader = subPass.m_subShaders[subShaderType];
                newSubShader.m_code = m_globalCode + "\n" + subPass.m_globalCode;

                return newSubShader;
            }
            else
            {
                return subPass.m_subShaders[subShaderType];
            }
        }

        bool operator==(const ShaderAnalyzedFile& other) const noexcept
        {
            return m_path == other.m_path;
        }

        bool operator!=(const ShaderAnalyzedFile& other) const noexcept
        {
            return !(*this == other);
        }

        [[nodiscard]] auto getLinesTotalCount() const noexcept
        {
            return m_linesTotalCount;
        }

        [[nodiscard]] auto getExternalLinesCount() const noexcept
        {
            return m_linesTotalCount - m_linesRawCount;
        }

        std::string getSubShaderCode(const std::string& subPassName, SGSLSubShaderType subShaderType)
        {
            return m_subPasses[subPassName].m_subShaders[subShaderType].m_code;
        }

    private:
        bool m_currentIfDefDirectivePassed = true;

        std::vector<SGSLSubPass*> m_subPassesToAppendCode;
        SGSLSubShaderType m_subShaderTypeToAppendCode = SGSLSubShaderType::SST_NONE;
        size_t m_linesTotalCount = 0;
    };
}

#endif //SGSLINTERPRETER_SHADERANALYZEDFILE_H
