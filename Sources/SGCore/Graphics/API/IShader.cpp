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

    SGSLETranslator sgsleTranslator;
    m_shaderAnalyzedFile = sgsleTranslator.processCode(asset->getPath().string(), asset->getData());

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
                subPassShader->m_subShadersCodes[subShaderType] = m_shaderAnalyzedFile->getSubShaderCode(subPass.m_name, subShaderType);
            }

            for(const auto& sgsleSubShader : subPass.m_subShaders)
            {
                for(const auto& variable : sgsleSubShader.second.m_variables)
                {
                    for(const auto& variableAssignExpr : variable.m_assignExpressions)
                    {
                        Ref<TexturesBlock> newTexBlock;

                        if (variableAssignExpr.m_rvalueFunctionName == "SGGetTexturesFromMaterial")
                        {
                            Ref<TexturesFromMaterialBlock> texBlock = MakeRef<TexturesFromMaterialBlock>();
                            texBlock->m_uniformRawName = variable.m_lValueVarName;
                            texBlock->m_isSingleTextureBlock = !variable.m_isLValueArray;
                            texBlock->m_typeToCollect = sgStandardTextureFromString(
                                    variableAssignExpr.m_rvalueFunctionArgs[0]);

                            newTexBlock = texBlock;

                            subPassShader->addTexturesBlock(texBlock);
                        }
                        else if (variableAssignExpr.m_rvalueFunctionName == "SGGetTextures")
                        {
                            Ref<TexturesFromGlobalStorageBlock> texBlock = MakeRef<TexturesFromGlobalStorageBlock>();
                            texBlock->m_uniformRawName = variable.m_lValueVarName;
                            texBlock->m_isSingleTextureBlock = !variable.m_isLValueArray;
                            for(const auto& arg : variableAssignExpr.m_rvalueFunctionArgs)
                            {
                                texBlock->m_requiredTexturesNames.push_back(arg);
                            }

                            newTexBlock = texBlock;

                            subPassShader->addTexturesBlock(texBlock);
                        }

                        for(const auto& arrIdx : variableAssignExpr.m_arrayIndices)
                        {
                            // std::string uniformName = variable.m_lValueVarName + "[" + std::to_string(arrIdx) + "]";

                            newTexBlock->m_texturesArrayIndices.push_back(arrIdx);
                        }
                    }
                }
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