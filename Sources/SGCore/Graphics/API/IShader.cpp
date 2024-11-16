//
// Created by stuka on 29.12.2023.
//

#include "IShader.h"

#include "SGCore/Utils/SGSL/SGSLETranslator.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"
#include "ISubPassShader.h"

void SGCore::IShader::compile(AssetRef<TextFileAsset> fromFile) noexcept
{
    if(!fromFile) return;

    m_subPassesShaders.clear();

    m_fileAsset = fromFile;

    m_shaderAnalyzedFile = AssetManager::getInstance()->loadAsset<ShaderAnalyzedFile>(fromFile->getPath());

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
        
        subPassShader->m_fileAsset = fromFile;
        subPassShader->compile(subPassName);
        subPassShader->addToGlobalStorage();

        m_subPassesShaders.push_back(subPassShader);
    }
}

void SGCore::IShader::setSubPassShader
(const std::string& subPassName, const IShader* from) noexcept
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

SGCore::Ref<SGCore::ISubPassShader> SGCore::IShader::getSubPassShader(const std::string& subPassName) const noexcept
{
    auto it = std::find_if(m_subPassesShaders.begin(), m_subPassesShaders.end(), [&subPassName](const Ref<ISubPassShader>& subPassShader) {
        return subPassShader->m_subPassName == subPassName;
    });
    
    return it != m_subPassesShaders.end() ? *it : nullptr;
}

/*void SGCore::IShader::onAssetModified()
{
    m_subPassesShaders.clear();
    addSubPassShadersAndCompile(m_fileAsset.lock());
}

void SGCore::IShader::onAssetPathChanged()
{
    m_subPassesShaders.clear();
    addSubPassShadersAndCompile(m_fileAsset.lock());
}*/

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

void SGCore::IShader::doLoad(const std::filesystem::path& path)
{
    m_fileAsset = getParentAssetManager()->loadAsset<TextFileAsset>(path);
}

void SGCore::IShader::doLazyLoad()
{
    compile(m_fileAsset.lock());
}

void SGCore::IShader::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    // nothing to do...
}

SGCore::AssetRef<SGCore::TextFileAsset> SGCore::IShader::getFile() const noexcept
{
    return m_fileAsset.lock();
}

void SGCore::IShader::resolveMemberAssetsReferences(SGCore::AssetManager* parentAssetManager) noexcept
{
    parentAssetManager->resolveAssetReference(m_shaderAnalyzedFile);
    parentAssetManager->resolveWeakAssetReference(m_fileAsset);
}
