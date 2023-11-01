//
// Created by stuka on 13.05.2023.
//

#include "IShader.h"

void Core::Graphics::IShader::addDefines(const SGShaderDefineType& shaderDefineType,
                                         const std::vector<ShaderDefine>& shaderDefines)
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    for(auto& shaderDefine : shaderDefines)
    {
        // if define with name shaderDefine already exists then wont add new shader define
        if(std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine)
            != shaderTypedDefines.end())
        {
            return;
        }

        shaderTypedDefines.push_back(shaderDefine);
    }

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::emplaceDefines(const SGShaderDefineType& shaderDefineType,
                                             std::vector<ShaderDefine>& shaderDefines)
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    for(auto& shaderDefine : shaderDefines)
    {
        // if define with name shaderDefine already exists then wont add new shader define
        if(std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine)
           != shaderTypedDefines.end())
        {
            return;
        }

        shaderTypedDefines.emplace_back(std::move(shaderDefine));
    }

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::addDefine(const SGShaderDefineType& shaderDefineType,
                                        const ShaderDefine& shaderDefine)
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    // if define with name shaderDefine already exists then wont add new shader define
    if (std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine)
        != shaderTypedDefines.end())
    {
        return;
    }

    shaderTypedDefines.push_back(shaderDefine);
}

void Core::Graphics::IShader::emplaceDefine(const SGShaderDefineType& shaderDefineType,
                                            ShaderDefine&& shaderDefine)
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    // if define with name shaderDefine already exists then wont add new shader define
    if (std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine)
        != shaderTypedDefines.end())
    {
        return;
    }

    shaderTypedDefines.emplace_back(std::move(shaderDefine));
}

void Core::Graphics::IShader::removeDefine(const SGShaderDefineType& shaderDefineType,
                                           const ShaderDefine& shaderDefine)
{
    m_defines[shaderDefineType].remove(shaderDefine);

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::removeDefine(const SGShaderDefineType& shaderDefineType,
                                           const std::string& shaderDefineName)
{
    m_defines[shaderDefineType].remove(ShaderDefine(shaderDefineName, ""));

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::updateDefine(const SGShaderDefineType& shaderDefineType,
                                           const Core::Graphics::ShaderDefine& shaderDefine)
{
    setAssetModifiedChecking(false);

    removeDefine(shaderDefineType, shaderDefine.m_name);
    addDefine(shaderDefineType, shaderDefine);

    setAssetModifiedChecking(true);
}

void Core::Graphics::IShader::emplaceUpdateDefine(const SGShaderDefineType& shaderDefineType,
                                                  Core::Graphics::ShaderDefine&& shaderDefine)
{
    setAssetModifiedChecking(false);

    removeDefine(shaderDefineType, shaderDefine.m_name);
    emplaceDefine(shaderDefineType, std::move(shaderDefine));

    setAssetModifiedChecking(true);
}

void Core::Graphics::IShader::updateDefines(const SGShaderDefineType& shaderDefineType,
                                            const std::vector<ShaderDefine>& shaderDefines)
{
    setAssetModifiedChecking(false);
    for(auto& shaderDefine : shaderDefines)
    {
        removeDefine(shaderDefineType, shaderDefine.m_name);
        addDefine(shaderDefineType, shaderDefine);
    }
    setAssetModifiedChecking(true);
}

void Core::Graphics::IShader::emplaceUpdateDefines(const SGShaderDefineType& shaderDefineType,
                                                   std::vector<ShaderDefine>& shaderDefines)
{
    setAssetModifiedChecking(false);
    for(auto& shaderDefine : shaderDefines)
    {
        removeDefine(shaderDefineType, shaderDefine.m_name);
        emplaceDefine(shaderDefineType, std::move(shaderDefine));
    }
    setAssetModifiedChecking(true);
}

void Core::Graphics::IShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                             const std::list<ShaderDefine>& otherDefines) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherDefines.begin(), otherDefines.end());

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                             std::shared_ptr<IShader> otherShader) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    auto& otherShaderTypedDefines = otherShader->m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherShaderTypedDefines.begin(), otherShaderTypedDefines.end());

    if(m_assetModifiedChecking) onAssetModified();
}

void Core::Graphics::IShader::clearDefinesOfType(const SGShaderDefineType& shaderDefineType) noexcept
{
    m_defines[shaderDefineType].clear();

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
