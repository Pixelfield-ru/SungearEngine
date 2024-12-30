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
    for(auto& subShader : m_subShaders)
    {
        subShader.m_code = parentAssetManager->getPackage().readData<std::string>(subShader.m_codeOffsetInPackage,
                                                                                  subShader.m_codeSizeInPackage);
    }
}

SGCore::SGSLESubShader* SGCore::ShaderAnalyzedFile::getSubShaderByType(SGCore::SGSLESubShaderType type) noexcept
{
    for(auto& shader : m_subShaders)
    {
        if(shader.m_type == type)
        {
            return &shader;
        }
    }

    return nullptr;
}

void SGCore::ShaderAnalyzedFile::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                                  SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    m_subPassName = "";
    m_attributes.clear();
    m_subShaders.clear();

    getParentAssetManager()->loadAsset<TextFileAsset>(getPath())->reloadFromDisk();

    doLoad(getPath());
}
