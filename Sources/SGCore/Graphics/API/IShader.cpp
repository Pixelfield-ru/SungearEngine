//
// Created by stuka on 13.05.2023.
//

#include "IShader.h"

#include "GPUDeviceInfo.h"
#include "IFrameBuffer.h"
#include "ITexture2D.h"
#include "SGCore/Utils/SGSL/ShaderAnalyzedFile.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

void SGCore::IShader::compile(const SGCore::AssetRef<SGCore::TextFileAsset>& textFileAsset) noexcept
{
    if(!textFileAsset)
    {
        return;
    }

    m_fileAsset = textFileAsset;
    auto shaderAnalyzedFile =
            textFileAsset->getParentAssetManager()->loadAsset<ShaderAnalyzedFile>(textFileAsset->getPath());
    m_shaderAnalyzedFile = shaderAnalyzedFile;

    if(!textFileAsset)
    {
        LOG_E(SGCORE_TAG,
              "Can not compile subpass shader! File asset is nullptr. Please set m_fileAsset before compiling.\n{}", SG_CURRENT_LOCATION_STR);
        return;
    }

    if(shaderAnalyzedFile->getSubShaders().empty())
    {
        LOG_E(SGCORE_TAG,
              "No sub shaders to compile! Shader path: {}\n{}", textFileAsset->getPath().resolved().string(), SG_CURRENT_LOCATION_STR);
        return;
    }

    if(!hasDefine(SGShaderDefineType::SGG_OTHER_DEFINE, "SG_VS_MAX_ATTRIBS_COUNT"))
    {
        addDefine(SGShaderDefineType::SGG_OTHER_DEFINE, ShaderDefine("SG_VS_MAX_ATTRIBS_COUNT", std::to_string(GPUDeviceInfo::getMaxVertexAttribsCount())));
    }

    destroy();
    doCompile();
}

void SGCore::IShader::recompile() noexcept
{
    destroy();
    compile(m_fileAsset.lock());
}

void SGCore::IShader::addDefines(SGShaderDefineType shaderDefineType,
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

void SGCore::IShader::addDefine(SGShaderDefineType shaderDefineType,
                                ShaderDefine shaderDefine)
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    // if define with name shaderDefine already exists then wont add new shader define
    if (std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine)
        != shaderTypedDefines.end())
    {
        return;
    }

    shaderTypedDefines.push_back(std::move(shaderDefine));
}

void SGCore::IShader::removeDefine(SGShaderDefineType shaderDefineType,
                                   const std::string& shaderDefineName)
{
    m_defines[shaderDefineType].remove(ShaderDefine(shaderDefineName, ""));

    if(m_autoRecompile) recompile();
}

void SGCore::IShader::updateDefine(SGShaderDefineType shaderDefineType,
                                   const SGCore::ShaderDefine& shaderDefine)
{
    m_autoRecompile = false;

    removeDefine(shaderDefineType, shaderDefine.m_name);
    addDefine(shaderDefineType, shaderDefine);

    recompile();

    m_autoRecompile = true;
}

void SGCore::IShader::updateDefines(SGShaderDefineType shaderDefineType,
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

void SGCore::IShader::replaceDefines(SGShaderDefineType shaderDefineType,
                                     const std::list<ShaderDefine>& otherDefines) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherDefines.begin(), otherDefines.end());

    if(m_autoRecompile) recompile();
}

void SGCore::IShader::replaceDefines(SGShaderDefineType shaderDefineType,
                                     Ref<IShader> otherShader) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    auto& otherShaderTypedDefines = otherShader->m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherShaderTypedDefines.begin(), otherShaderTypedDefines.end());

    if(m_autoRecompile) recompile();
}

bool SGCore::IShader::hasDefine(SGShaderDefineType shaderDefineType, const std::string& shaderDefineName) const noexcept
{
    auto it =  m_defines.find(shaderDefineType);
    if(it == m_defines.end()) return false;

    if (std::find(it->second.begin(), it->second.end(), ShaderDefine(shaderDefineName, ""))
        != it->second.end())
    {
        return true;
    }

    return false;
}

void SGCore::IShader::clearDefinesOfType(const SGShaderDefineType& shaderDefineType) noexcept
{
    m_defines[shaderDefineType].clear();

    if(m_autoRecompile) recompile();
}

SGCore::IShader& SGCore::IShader::operator=(const SGCore::IShader& other) noexcept
{
    assert(this != std::addressof(other));

    for(const auto& shaderDefinesPair : m_defines)
    {
        replaceDefines(shaderDefinesPair.first, shaderDefinesPair.second);
    }

    return *this;
}

size_t SGCore::IShader::bindMaterialTextures(const SGCore::AssetRef<SGCore::IMaterial>& material, std::uint8_t samplersOffset) noexcept
{
    size_t offset = samplersOffset;

    std::string preallocUniformName;
    preallocUniformName.resize(64);

    constexpr auto texturesTypesCount = std::to_underlying(SGTextureSlot::SGTT_COUNT);

    for(int i = 0; i < texturesTypesCount; ++i)
    {
        const auto& texturesOfType = material->getTextures()[i];

        const std::string& textureUniform = sgStandardTextureTypeNameToStandardUniformName((SGTextureSlot) i);
        const std::string& texturesCountUniform = sgStandardTextureTypeNameToStandardUniformName((SGTextureSlot) i) + "_CURRENT_COUNT";
        
        size_t arrayIdx = 0;
        
        for(const auto& tex : texturesOfType)
        {
            if(!tex) continue;

            preallocUniformName = textureUniform;
            
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

        if(arrayIdx == 0) continue;
        
        useInteger(texturesCountUniform, arrayIdx);
    }
    
    return offset;
}

size_t SGCore::IShader::bindTextures(const std::vector<AssetRef<ITexture2D>>& textures,
                                     SGTextureSlot slotType,
                                     std::uint8_t samplersOffset) noexcept
{
    static const std::string textureUniform = sgStandardTextureTypeNameToStandardUniformName(slotType);
    static const std::string texturesCountUniform = textureUniform + "_CURRENT_COUNT";

    size_t texUnitOffset = 0;

    for(size_t i = 0; i < textures.size(); ++i)
    {
        const auto& texture = textures[i];
        useTextureBlock(textureUniform + '[' + std::to_string(i) + ']', samplersOffset);
        texture->bind(texUnitOffset);

        ++samplersOffset;
    }

    useInteger(texturesCountUniform, textures.size());

    return samplersOffset;
}

size_t SGCore::IShader::bindTextureBindings(const size_t& samplersOffset) noexcept
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

void SGCore::IShader::addTextureBinding
(const std::string& bindingName, const SGCore::AssetRef<SGCore::ITexture2D>& texture) noexcept
{
    ShaderTextureBinding shaderTextureBinding;
    shaderTextureBinding.m_bindingName = bindingName;
    shaderTextureBinding.m_texture = texture;
    m_textureBindings.push_back(shaderTextureBinding);
    // m_textureBindings.emplace_back(std::move(shaderTextureBinding));
}

void SGCore::IShader::removeTextureBinding(const std::string& bindingName) noexcept
{
    std::erase_if(m_textureBindings, [&bindingName](const ShaderTextureBinding& shaderTextureBinding) {
        return shaderTextureBinding.m_bindingName == bindingName;
    });
}

void SGCore::IShader::doLoad(const SGCore::InterpolatedPath& path)
{
    m_fileAsset = getParentAssetManager()->loadAsset<TextFileAsset>(path);
    m_shaderAnalyzedFile = getParentAssetManager()->loadAsset<ShaderAnalyzedFile>(path);
}

void SGCore::IShader::doLazyLoad()
{
    compile(m_fileAsset.lock());
}

void SGCore::IShader::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    // nothing to do...
}

void SGCore::IShader::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    for(auto& binding : m_textureBindings)
    {
        AssetManager::resolveWeakAssetReference(updatedAssetManager, binding.m_texture);
    }

    // todo: ????
    AssetManager::resolveWeakAssetReference(updatedAssetManager, m_shaderAnalyzedFile);
    AssetManager::resolveWeakAssetReference(updatedAssetManager, m_fileAsset);

    recompile();
}

SGCore::AssetRef<SGCore::ShaderAnalyzedFile> SGCore::IShader::getAnalyzedFile() const noexcept
{
    return m_shaderAnalyzedFile.lock();
}

SGCore::AssetRef<SGCore::TextFileAsset> SGCore::IShader::getFile() const noexcept
{
    return m_fileAsset.lock();
}

const std::vector<SGCore::ShaderTextureBinding>& SGCore::IShader::getTextureBindings() const noexcept
{
    return m_textureBindings;
}

void SGCore::IShader::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                       SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    auto lockedFileAsset = m_fileAsset.lock();
    if(lockedFileAsset)
    {
        lockedFileAsset->reloadFromDisk(loadPolicy, lazyLoadInThread);
    }

    auto lockedAnalyzedFile = m_shaderAnalyzedFile.lock();
    if(lockedAnalyzedFile)
    {
        lockedAnalyzedFile->reloadFromDisk(loadPolicy, lazyLoadInThread);
    }
}
