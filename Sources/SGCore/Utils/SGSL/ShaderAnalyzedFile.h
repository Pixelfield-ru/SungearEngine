//
// Created by ilya on 10.02.24.
//

#ifndef SGSLETRANSLATOR_SHADERANALYZEDFILE_H
#define SGSLETRANSLATOR_SHADERANALYZEDFILE_H

#include <string>
#include <unordered_map>
#include <memory>
#include "SGSLESubPass.h"

namespace SGCore
{
    struct ShaderAnalyzedFile
    {
        friend class SGSLETranslator;
        
        std::string m_path;
        std::string m_globalCode;
        
        std::unordered_map<std::string, SGSLESubPass> m_subPasses;
        std::vector<std::shared_ptr<ShaderAnalyzedFile>> m_includedFiles;
        
        void includeFile(const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept
        {
            m_globalCode += analyzedFile->m_globalCode + "\n";
            
            for(const auto& subPassPair : analyzedFile->m_subPasses)
            {
                const auto& subPassName = subPassPair.first;
                const auto& otherSubPass = subPassPair.second;
                
                auto& thisSubPass = m_subPasses[subPassName];
                
                thisSubPass.m_globalCode += otherSubPass.m_globalCode + "\n";
                
                for(const auto& subShaderPair : otherSubPass.m_subShaders)
                {
                    auto subShaderType = subShaderPair.first;
                    const auto& subShader = subShaderPair.second;
                    
                    auto& thisSubShader = thisSubPass.m_subShaders[subShaderType];
                    
                    thisSubShader.m_code = subShader.m_code + "\n" + thisSubShader.m_code;
                }
            }
            
            // m_linesTotalCount += analyzedFile.m_linesTotalCount;
            m_includedFiles.push_back(analyzedFile);
        }
        
        SGSLESubShader& addOrGetSubShader(const std::string& subPassName, SGSLESubShaderType subShaderType)
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
        
        std::string getSubShaderCode(const std::string& subPassName, SGSLESubShaderType subShaderType) noexcept
        {
            return m_subPasses[subPassName].m_subShaders[subShaderType].m_code;
        }
        
        std::string getAllCode() const noexcept
        {
            std::string allCode;
            
            for(const auto& subPassIter : m_subPasses)
            {
                allCode += "// SubPass: " + subPassIter.first + "\n";
                
                for(const auto& subShaderIter : subPassIter.second.m_subShaders)
                {
                    allCode += "// SubShader: " + sgsleSubShaderTypeToString(subShaderIter.first) + "\n";
                    
                    allCode += subShaderIter.second.m_code;
                    
                    allCode += "\n";
                }
            }
            
            return allCode;
        }
    };
}

#endif //SGSLETRANSLATOR_SHADERANALYZEDFILE_H
