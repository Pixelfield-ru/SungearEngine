//
// Created by stuka on 29.12.2023.
//

#include "IShader.h"

#include <utility>

#include "SGCore/Utils/SGSL/GLSLShadersPreprocessor.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"

void SGCore::IShader::addSubPassShadersAndCompile(Ref<FileAsset> asset) noexcept
{
    if(!asset) return;

    m_fileAsset = asset;

    GLSLShadersPreprocessor preprocessor;
    m_shaderAnalyzedFile = GLSLShadersPreprocessor::processCode(asset->getPath().string(), asset->getData(), preprocessor);

    for(const auto& subPassIter : m_shaderAnalyzedFile.m_subPasses)
    {
        const auto subPassName = subPassIter.first;
        const auto& subPass = subPassIter.second;

        auto subPassShader = Ref<ISubPassShader>(SGCore::CoreMain::getRenderer()->createShader());

        for(const auto& subShaderIter : subPass.m_subShaders)
        {
            auto subShaderType = subShaderIter.first;

            if(subPass.isSubShaderExists(subShaderType))
            {
                subPassShader->m_subShadersCodes[subShaderType] = m_shaderAnalyzedFile.getSubShaderCode(subPass.m_name, subShaderType);
            }
        }

        for(const auto& variable : subPass.m_variables)
        {
            if(variable.m_rsideFunction == "SGGetTexturesFromMaterial")
            {
                Ref<TexturesFromMaterialBlock> fromMaterialBlock = MakeRef<TexturesFromMaterialBlock>();
                fromMaterialBlock->m_uniformName = variable.m_name;
                fromMaterialBlock->m_isSingleTextureBlock = !variable.m_isArray;
                fromMaterialBlock->m_typeToCollect = sgStandardTextureFromString(variable.m_rsideFunctionArgs[0]);

                subPassShader->addTexturesBlock(fromMaterialBlock);
            }
            else if(variable.m_rsideFunction == "SGGetTextures")
            {
                Ref<TexturesFromGlobalStorageBlock> texturesFromGlobalStorageListener = MakeRef<TexturesFromGlobalStorageBlock>();
                texturesFromGlobalStorageListener->m_uniformName = variable.m_name;
                texturesFromGlobalStorageListener->m_isSingleTextureBlock = !variable.m_isArray;
                for(const auto& arg : variable.m_rsideFunctionArgs)
                {
                    texturesFromGlobalStorageListener->m_requiredTexturesNames.push_back(arg);
                }

                subPassShader->addTexturesBlock(texturesFromGlobalStorageListener);
            }
        }

        if(auto lockedMaterial = m_parentMaterial.lock())
        {
            subPassShader->collectTexturesFromMaterial(lockedMaterial);
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

    for(auto& subPassShaderPair : m_subPassesShaders)
    {
        auto& subPassShader = subPassShaderPair.second;

        subPassShader->clearTexturesBlocksOfType<TexturesFromMaterialBlock>();

        subPassShader->collectTexturesFromMaterial(material);
    }
}

SGCore::Weak<SGCore::IMaterial> SGCore::IShader::getParentMaterial() const noexcept
{
    return m_parentMaterial;
}

void SGCore::IShader::collectTextureFromMaterial(const Ref<ITexture2D>& texture, SGTextureType textureType) noexcept
{
    for(auto& subPassShaderPair : m_subPassesShaders)
    {
        auto& subPassShader = subPassShaderPair.second;

        subPassShader->addTexture(texture, textureType);
    }
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