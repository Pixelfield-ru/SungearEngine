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

void SGCore::IShader::addSubPassShadersAndCompile(Ref<TextFileAsset> asset) noexcept
{
    if(!asset) return;

    m_fileAsset = asset;

    std::string assetPath = asset->getPath().string();
    m_shaderAnalyzedFile = AssetManager::getInstance()->loadAsset<ShaderAnalyzedFile>(assetPath);

    for(const auto& subPassIter : m_shaderAnalyzedFile->m_subPasses)
    {
        const auto subPassName = subPassIter.first;
        const auto& subPass = subPassIter.second;

        auto subPassShader = Ref<ISubPassShader>(CoreMain::getRenderer()->createSubPassShader());

        for(const auto& subShaderIter : subPass.m_subShaders)
        {
            auto subShaderType = subShaderIter.first;

            if(subPass.isSubShaderExists(subShaderType))
            {
                subPassShader->m_subShaders[subShaderType] = m_shaderAnalyzedFile->m_subPasses[subPass.m_name].m_subShaders[subShaderType];
            }
        }
        
        subPassShader->m_fileAsset = asset;
        subPassShader->compile(subPassName);
        subPassShader->addToGlobalStorage();

        if(!getSubPassShader(subPassName))
        {
            m_subPassesShaders.push_back(subPassShader);
        }
        else
        {
            setSubPassShader(subPassName, subPassShader);
        }
    }
}

void SGCore::IShader::setSubPassShader
(const std::string& subPassName, const SGCore::Ref<SGCore::IShader>& from) noexcept
{
    auto foundFrom = from->getSubPassShader(subPassName);
    if(foundFrom)
    {
        setSubPassShader(subPassName, foundFrom);
    }
}

void SGCore::IShader::setSubPassShader
(const std::string& subPassName, const SGCore::Ref<SGCore::ISubPassShader>& subPassShader) noexcept
{
    for(auto& s : m_subPassesShaders)
    {
        if(s->m_subPassName == subPassName)
        {
            s = subPassShader;
            break;
        }
    }
}

SGCore::Ref<SGCore::ISubPassShader> SGCore::IShader::getSubPassShader(const std::string& subPassName) noexcept
{
    auto it = std::find_if(m_subPassesShaders.begin(), m_subPassesShaders.end(), [&subPassName](const Ref<ISubPassShader>& subPassShader) {
        return subPassShader->m_subPassName == subPassName;
    });
    
    return it != m_subPassesShaders.end() ? *it : nullptr;
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

void SGCore::IShader::removeAllSubPassShadersByDiskPath(const std::string& path) noexcept
{
    std::erase_if(m_subPassesShaders, [&path](const Ref<ISubPassShader>& subPassShader) {
        auto lockedFileAsset = subPassShader->m_fileAsset.lock();
        if(!lockedFileAsset) return false;
        
        return path == lockedFileAsset->getPath();
    });
}

void SGCore::IShader::removeSubPass(const std::string& subPassName) noexcept
{
    std::erase_if(m_subPassesShaders, [&subPassName](const Ref<ISubPassShader>& subPassShader) {
        return subPassName == subPassShader->m_subPassName;
    });
}
