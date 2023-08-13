//
// Created by stuka on 13.05.2023.
//

#include "IShader.h"

void Core::Graphics::IShader::addShaderDefines(const std::vector<ShaderDefine>& shaderDefines)
{
    for(auto& shaderDefine : shaderDefines)
    {
        if(std::find(m_defines.begin(), m_defines.end(), shaderDefine) != m_defines.end()) return;

        m_defines.push_back(shaderDefine);
    }

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::removeShaderDefine(const Core::Graphics::ShaderDefine& shaderDefine)
{
    m_defines.remove(shaderDefine);

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::removeShaderDefine(const std::string& shaderDefineName)
{
    m_defines.remove(ShaderDefine(shaderDefineName, ""));

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::onAssetModified()
{
    compile(m_fileAsset.lock());
}

void Core::Graphics::IShader::onAssetPathChanged()
{
    compile(m_fileAsset.lock());
}

void Core::Graphics::IShader::replaceDefines(const std::list<ShaderDefine>& otherDefines) noexcept
{
    m_defines.clear();
    m_defines.insert(m_defines.end(), otherDefines.begin(), otherDefines.end());

    onAssetModified();
}

void Core::Graphics::IShader::replaceDefines(std::shared_ptr<IShader> otherShader) noexcept
{
    m_defines.clear();
    m_defines.insert(m_defines.end(), otherShader->m_defines.begin(), otherShader->m_defines.end());

    onAssetModified();
}

Core::Graphics::IShader& Core::Graphics::IShader::operator=(const Core::Graphics::IShader& other) noexcept
{
    assert(this != std::addressof(other));

    replaceDefines(other.m_defines);

    m_fileAsset = other.m_fileAsset;

    return *this;
}
