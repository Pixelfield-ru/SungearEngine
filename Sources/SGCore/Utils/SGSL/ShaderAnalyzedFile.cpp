//
// Created by ilya on 16.02.24.
//
#include "ShaderAnalyzedFile.h"

#include "SGSLETranslator.h"
#include "SGCore/Utils/FileUtils.h"

void SGCore::ShaderAnalyzedFile::doLoad(const InterpolatedPath& path)
{
    SGSLETranslator translator;
    
    translator.processCode(path.resolved(), SGCore::FileUtils::readFile(path.resolved()), shared_from_this());
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
