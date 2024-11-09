//
// Created by ilya on 16.02.24.
//
#include "ShaderAnalyzedFile.h"

#include "SGSLETranslator.h"
#include "SGCore/Utils/FileUtils.h"

void SGCore::ShaderAnalyzedFile::doLoad(const std::filesystem::path& path)
{
    SGSLETranslator translator;
    
    translator.processCode(path, SGCore::FileUtils::readFile(path), shared_from_this());
}

void SGCore::ShaderAnalyzedFile::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    for(const auto& subPassPair : m_subPasses)
    {
        for(const auto& subShaderPair : subPassPair.second.m_subShaders)
        {
            if(!subShaderPair.second) continue;

            subShaderPair.second->doLoadFromBinaryFile(parentAssetManager);
        }
    }
}
