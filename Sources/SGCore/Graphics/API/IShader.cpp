//
// Created by stuka on 13.05.2023.
//

#include "IShader.h"

void Core::Graphics::IShader::addShaderDefines(const SGShaderDefineType& shaderDefineType,
                                               const std::vector<ShaderDefine>& shaderDefines)
{
    for(auto& shaderDefine : shaderDefines)
    {
        auto& shaderTypedDefines = m_defines[shaderDefineType];
        if(std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine) != shaderTypedDefines.end()) return;

        shaderTypedDefines.push_back(shaderDefine);
    }

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::removeShaderDefine(const SGShaderDefineType& shaderDefineType,
                                                 const Core::Graphics::ShaderDefine& shaderDefine)
{
    m_defines[shaderDefineType].remove(shaderDefine);

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::removeShaderDefine(const SGShaderDefineType& shaderDefineType,
                                                 const std::string& shaderDefineName)
{
    m_defines[shaderDefineType].remove(ShaderDefine(shaderDefineName, ""));

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                             const std::list<ShaderDefine>& otherDefines) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherDefines.begin(), otherDefines.end());

    onAssetModified();
}

void Core::Graphics::IShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                             std::shared_ptr<IShader> otherShader) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    auto& otherShaderTypedDefines = otherShader->m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherShaderTypedDefines.begin(), otherShaderTypedDefines.end());

    onAssetModified();
}

void Core::Graphics::IShader::clearDefinesOfType(const SGShaderDefineType& shaderDefineType) noexcept
{
    m_defines[shaderDefineType].clear();
}

void Core::Graphics::IShader::onAssetModified()
{
    compile(m_fileAsset.lock());
}

void Core::Graphics::IShader::onAssetPathChanged()
{
    compile(m_fileAsset.lock());
}

Core::Graphics::IShader& Core::Graphics::IShader::operator=(const Core::Graphics::IShader& other) noexcept
{
    assert(this != std::addressof(other));

    for(const auto& shaderDefinesPair : m_defines)
    {
        replaceDefines(shaderDefinesPair.first, shaderDefinesPair.second);
    }

    m_fileAsset = other.m_fileAsset;

    return *this;
}
