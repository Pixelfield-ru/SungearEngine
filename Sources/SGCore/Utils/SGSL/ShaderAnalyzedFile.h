//
// Created by ilya on 10.02.24.
//

#ifndef SGSLETRANSLATOR_SHADERANALYZEDFILE_H
#define SGSLETRANSLATOR_SHADERANALYZEDFILE_H

#include <SGCore/pch.h>

#include "SGCore/Memory/Assets/IAsset.h"
#include "SGSLESubPass.h"

namespace SGCore
{
    struct ShaderAnalyzedFile : public IAsset, public std::enable_shared_from_this<ShaderAnalyzedFile>
    {
        static inline size_t asset_type_id = StaticTypeID<ShaderAnalyzedFile>::setID(4);

        friend class SGSLETranslator;
        friend class AssetManager;

        std::string m_globalCode;
        
        std::unordered_map<std::string, SGSLESubPass> m_subPasses;
        std::vector<std::shared_ptr<ShaderAnalyzedFile>> m_includedFiles;
        
        void includeFile(const std::shared_ptr<ShaderAnalyzedFile>& analyzedFile) noexcept
        {
            m_globalCode += analyzedFile->m_globalCode + "\n";
            
            for(const auto& subPassPair : analyzedFile->m_subPasses)
            {
                const auto& otherSubPassName = subPassPair.first;
                const auto& otherSubPass = subPassPair.second;
                
                auto& thisSubPass = m_subPasses[otherSubPassName];
                
                thisSubPass.m_globalCode += otherSubPass.m_globalCode + "\n";
                
                for(const auto& otherSubShaderPair : otherSubPass.m_subShaders)
                {
                    auto otherSubShaderType = otherSubShaderPair.first;
                    const auto& otherSubShader = otherSubShaderPair.second;
                    
                    Ref<SGSLESubShader> thisSubShader = addOrGetSubShader(otherSubPassName, otherSubShaderType);
                    
                    thisSubShader->m_code = otherSubShader->m_code + "\n" + thisSubShader->m_code;
                }
            }
            
            // m_linesTotalCount += analyzedFile.m_linesTotalCount;
            m_includedFiles.push_back(analyzedFile);
        }
        
        Ref<SGSLESubShader> addOrGetSubShader(const std::string& subPassName, SGSLESubShaderType subShaderType)
        {
            auto& subPass = m_subPasses[subPassName];
            if(subPass.m_subShaders.find(subShaderType) == subPass.m_subShaders.end())
            {
                Ref<SGSLESubShader> newSubShader = MakeRef<SGSLESubShader>();
                subPass.m_subShaders[subShaderType] = newSubShader;
                newSubShader->m_code = m_globalCode + "\n" + subPass.m_globalCode;
                
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
            return m_subPasses[subPassName].m_subShaders[subShaderType]->m_code;
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
                    
                    allCode += subShaderIter.second->m_code;
                    
                    allCode += "\n";
                }
            }
            
            return allCode;
        }
        
    protected:
        void doLoad(const std::filesystem::path& path) final;

        // todo: impl
        void serializeToPackage(AssetsPackage::AssetSection& currentAssetSection, bool isDataSerializing) override;
    };
}

#endif //SGSLETRANSLATOR_SHADERANALYZEDFILE_H
