//
// Created by stuka on 13.05.2023.
//

#include "ISubPassShader.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"
#include "IFrameBuffer.h"
#include "ITexture2D.h"
#include "SGCore/Utils/SGSL/SGSLESubShader.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

void SGCore::ISubPassShader::recompile() noexcept
{
    destroy();
    compile(m_subPassName);
}

void SGCore::ISubPassShader::addDefines(const SGShaderDefineType& shaderDefineType,
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

    if(m_autoRecompile) recompile();
}

void SGCore::ISubPassShader::emplaceDefines(const SGShaderDefineType& shaderDefineType,
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

    if(m_autoRecompile) recompile();
}

void SGCore::ISubPassShader::addDefine(const SGShaderDefineType& shaderDefineType,
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

void SGCore::ISubPassShader::emplaceDefine(const SGShaderDefineType& shaderDefineType,
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

void SGCore::ISubPassShader::removeDefine(const SGShaderDefineType& shaderDefineType,
                                          const ShaderDefine& shaderDefine)
{
    m_defines[shaderDefineType].remove(shaderDefine);

    if(m_autoRecompile) recompile();
}

void SGCore::ISubPassShader::removeDefine(const SGShaderDefineType& shaderDefineType,
                                          const std::string& shaderDefineName)
{
    m_defines[shaderDefineType].remove(ShaderDefine(shaderDefineName, ""));

    if(m_autoRecompile) recompile();
}

void SGCore::ISubPassShader::updateDefine(const SGShaderDefineType& shaderDefineType,
                                          const SGCore::ShaderDefine& shaderDefine)
{
    m_autoRecompile = false;

    removeDefine(shaderDefineType, shaderDefine.m_name);
    addDefine(shaderDefineType, shaderDefine);

    recompile();

    m_autoRecompile = true;
}

void SGCore::ISubPassShader::emplaceUpdateDefine(const SGShaderDefineType& shaderDefineType,
                                                 SGCore::ShaderDefine&& shaderDefine)
{
    m_autoRecompile = false;

    removeDefine(shaderDefineType, shaderDefine.m_name);
    emplaceDefine(shaderDefineType, std::move(shaderDefine));

    recompile();

    m_autoRecompile = true;
}

void SGCore::ISubPassShader::updateDefines(const SGShaderDefineType& shaderDefineType,
                                           const std::vector<ShaderDefine>& shaderDefines)
{
    m_autoRecompile = false;
    for(auto& shaderDefine : shaderDefines)
    {
        removeDefine(shaderDefineType, shaderDefine.m_name);
        addDefine(shaderDefineType, shaderDefine);
    }
    recompile();
    m_autoRecompile = true;
}

void SGCore::ISubPassShader::emplaceUpdateDefines(const SGShaderDefineType& shaderDefineType,
                                                  std::vector<ShaderDefine>& shaderDefines)
{
    m_autoRecompile = false;
    for(auto& shaderDefine : shaderDefines)
    {
        removeDefine(shaderDefineType, shaderDefine.m_name);
        emplaceDefine(shaderDefineType, std::move(shaderDefine));
    }
    recompile();
    m_autoRecompile = true;
}

void SGCore::ISubPassShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                            const std::list<ShaderDefine>& otherDefines) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherDefines.begin(), otherDefines.end());

    if(m_autoRecompile) recompile();
}

void SGCore::ISubPassShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                            Ref<ISubPassShader> otherShader) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    auto& otherShaderTypedDefines = otherShader->m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherShaderTypedDefines.begin(), otherShaderTypedDefines.end());

    if(m_autoRecompile) recompile();
}

void SGCore::ISubPassShader::clearDefinesOfType(const SGShaderDefineType& shaderDefineType) noexcept
{
    m_defines[shaderDefineType].clear();

    if(m_autoRecompile) recompile();
}

SGCore::ISubPassShader& SGCore::ISubPassShader::operator=(const SGCore::ISubPassShader& other) noexcept
{
    assert(this != std::addressof(other));

    for(const auto& shaderDefinesPair : m_defines)
    {
        replaceDefines(shaderDefinesPair.first, shaderDefinesPair.second);
    }

    return *this;
}

void SGCore::ISubPassShader::addToGlobalStorage() noexcept
{
    GPUObjectsStorage::addShader(shared_from_this());
}

size_t SGCore::ISubPassShader::bindMaterialTextures(const SGCore::Ref<SGCore::IMaterial>& material) noexcept
{
    size_t offset = 0;
    
    std::string preallocUniformName;
    preallocUniformName.resize(48);
    
    for(const auto& texPair : material->getTextures())
    {
        preallocUniformName = sgStandardTextureTypeNameToStandardUniformName(texPair.first);
        size_t firstSize = preallocUniformName.size();
        preallocUniformName += "[0]";
        
        if(!isUniformExists(preallocUniformName)) continue;
        
        preallocUniformName.erase(preallocUniformName.size() - 3, 3);
        
        size_t arrayIdx = 0;
        
        for(const auto& tex : texPair.second)
        {
            preallocUniformName += '[';
            preallocUniformName += std::to_string(arrayIdx);
            preallocUniformName += ']';
            
            // out of uniform samplers array bounds
            if(!isUniformExists(preallocUniformName)) break;
            
            useTextureBlock(preallocUniformName, offset);
            tex->bind(offset);
            
            ++arrayIdx;
            ++offset;
        }
        
        size_t secondSize = preallocUniformName.size();
        
        size_t difference = secondSize - firstSize;
        
        preallocUniformName.erase(preallocUniformName.size() - difference, difference);
        preallocUniformName += "_CURRENT_COUNT";
        
        useInteger(preallocUniformName, arrayIdx);
    }
    
    return offset;
}

void SGCore::ISubPassShader::unbindMaterialTextures(const SGCore::Ref<SGCore::IMaterial>& material) noexcept
{
    std::string preallocUniformName;
    preallocUniformName.resize(48);
    
    for(const auto& texPair : material->getTextures())
    {
        preallocUniformName = sgStandardTextureTypeNameToStandardUniformName(texPair.first);
        preallocUniformName += "_CURRENT_COUNT";
        
        useInteger(preallocUniformName, 0);
    }
}

size_t SGCore::ISubPassShader::bindTextureBindings(const size_t& samplersOffset) noexcept
{
    size_t offset = samplersOffset;
    
    auto it = m_textureBindings.begin();
    while(it != m_textureBindings.end())
    {
        if(auto lockedTexture = it->m_texture.lock())
        {
            lockedTexture->bind(offset);
            useTextureBlock(it->m_bindingName, offset);
            
            ++it;
            ++offset;
        }
        else
        {
            it = m_textureBindings.erase(it);
        }
    }
    
    return offset;
}

void SGCore::ISubPassShader::addTextureBinding
(const std::string& bindingName, const SGCore::Ref<SGCore::ITexture2D>& texture) noexcept
{
    ShaderTextureBinding shaderTextureBinding;
    shaderTextureBinding.m_bindingName = bindingName;
    shaderTextureBinding.m_texture = texture;
    m_textureBindings.push_back(shaderTextureBinding);
    // m_textureBindings.emplace_back(std::move(shaderTextureBinding));
}

void SGCore::ISubPassShader::removeTextureBinding(const std::string& bindingName) noexcept
{
    std::erase_if(m_textureBindings, [&bindingName](const ShaderTextureBinding& shaderTextureBinding) {
        return shaderTextureBinding.m_bindingName == bindingName;
    });
}
