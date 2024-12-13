//
// Created by ilya on 16.02.24.
//
#include "ShaderAnalyzedFile.h"

#include "SGCore/Memory/Assets/TextFileAsset.h"
#include "SGSLETranslator.h"
#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Memory/AssetManager.h"

void SGCore::ShaderAnalyzedFile::doLoad(const InterpolatedPath& path)
{
    auto fileAsset = getParentAssetManager()->loadAsset<TextFileAsset>(path);

    SGSLETranslator translator;

    translator.processCode(path.resolved(), fileAsset->getData(), shared_from_this());
}

void SGCore::ShaderAnalyzedFile::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    for(auto& subShaderPair : m_subShaders)
    {
        auto& subShader = subShaderPair.second;
        subShader.m_code = parentAssetManager->getPackage().readData<std::string>(subShader.m_codeOffsetInPackage,
                                                                                  subShader.m_codeSizeInPackage);
    }
}
