//
// Created by stuka on 29.12.2023.
//

#include "IShader.h"

#include <utility>

#include "SGCore/Utils/SGSL/SGSLETranslator.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"

void SGCore::IShader::addSubPassShadersAndCompile(Ref<FileAsset> asset) noexcept
{
    if(!asset) return;

    m_fileAsset = asset;

    std::string assetPath = asset->getPath().string();
    m_shaderAnalyzedFile = AssetManager::loadAsset<ShaderAnalyzedFile>(assetPath);

    for(const auto& subPassIter : m_shaderAnalyzedFile->m_subPasses)
    {
        const auto subPassName = subPassIter.first;
        const auto& subPass = subPassIter.second;

        auto subPassShader = Ref<ISubPassShader>(SGCore::CoreMain::getRenderer()->createShader());

        for(const auto& subShaderIter : subPass.m_subShaders)
        {
            auto subShaderType = subShaderIter.first;

            if(subPass.isSubShaderExists(subShaderType))
            {
                subPassShader->m_subShaders[subShaderType] = m_shaderAnalyzedFile->m_subPasses[subPass.m_name].m_subShaders[subShaderType];
            }
        }

        subPassShader->compile(asset);
        subPassShader->addToGlobalStorage();

        m_subPassesShaders[subPassName] = subPassShader;
    }
}

void SGCore::IShader::setSubPassShader
(const SGCore::Ref<SGCore::IShader>& from, const std::string& subPassName) noexcept
{
    auto foundFrom = from->m_subPassesShaders.find(subPassName);
    if(foundFrom != from->m_subPassesShaders.end())
    {
        m_subPassesShaders[subPassName] = foundFrom->second;
    }
}

SGCore::Ref<SGCore::ISubPassShader> SGCore::IShader::getSubPassShader(const std::string& subPassName) noexcept
{
    auto foundIter = m_subPassesShaders.find(subPassName);
    if(foundIter != m_subPassesShaders.end())
    {
        return foundIter->second;
    }

    return nullptr;
    // return m_subPassesShaders[subPassName];
}

void SGCore::IShader::onAssetModified()
{
    m_subPassesShaders.clear();
    addSubPassShadersAndCompile(m_fileAsset.lock());
}

void SGCore::IShader::onAssetPathChanged()
{
    m_subPassesShaders.clear();
    addSubPassShadersAndCompile(m_fileAsset.lock());
}

void SGCore::IShader::setParentMaterial(const SGCore::Ref<SGCore::IMaterial>& material) noexcept
{
    if(!material) return;

    m_parentMaterial = material;
}

SGCore::Weak<SGCore::IMaterial> SGCore::IShader::getParentMaterial() const noexcept
{
    return m_parentMaterial;
}

void SGCore::IShader::removeAllSubPassShadersByDiskPath(const std::string& path) noexcept
{
    auto curIter = m_subPassesShaders.begin();
    while(curIter != m_subPassesShaders.end())
    {
        if(auto lockedFileAsset = curIter->second->m_fileAsset.lock())
        {
            if(lockedFileAsset->getPath() == path)
            {
                curIter = m_subPassesShaders.erase(curIter);
                continue;
            }
        }

        ++curIter;
    }
}

void SGCore::IShader::removeSubPass(const std::string& subPassName) noexcept
{
    m_subPassesShaders.erase(subPassName);
}