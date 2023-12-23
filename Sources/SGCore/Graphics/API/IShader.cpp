//
// Created by stuka on 13.05.2023.
//

#include "IShader.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"

void SGCore::ShaderTexturesFromGlobalStorageBlock::addTexture(const SGCore::Ref<SGCore::ITexture2D>& texture2D) noexcept
{
    // if texture with this name requires
    if (std::find(m_requiredTexturesNames.begin(), m_requiredTexturesNames.end(), texture2D->getName()) !=
        m_requiredTexturesNames.end())
    {
        m_requiredTexturesNames.remove(texture2D->getName());

        m_textures.push_back(texture2D);

        if(auto lockedShader = m_parentShader.lock())
        {
            lockedShader->onTexturesCountChanged();
        }
    }
}

void SGCore::ShaderTexturesFromGlobalStorageBlock::removeTexture
(const SGCore::Ref<SGCore::ITexture2D>& texture2D) noexcept
{
    size_t removedCnt = m_textures.remove_if([&texture2D](auto otherTexture2D) {
        return !(otherTexture2D.owner_before(texture2D) || texture2D.owner_before(otherTexture2D));
    });

    if(removedCnt > 0)
    {
        m_requiredTexturesNames.push_back(texture2D->getName());

        if(auto lockedShader = m_parentShader.lock())
        {
            lockedShader->onTexturesCountChanged();
        }
    }
}

void SGCore::ShaderTexturesFromGlobalStorageBlock::clearTextures() noexcept
{
    auto currIter = m_textures.begin();
    while(currIter != m_textures.end())
    {
        m_requiredTexturesNames.push_back(currIter->lock()->getName());
        currIter = m_textures.erase(currIter);
    }

    if(auto lockedShader = m_parentShader.lock())
    {
        lockedShader->onTexturesCountChanged();
    }
}

bool SGCore::ShaderTexturesFromGlobalStorageBlock::operator==(
        const SGCore::ShaderTexturesFromGlobalStorageBlock& other) const noexcept
{
    return other.m_uniformName == m_uniformName;
}

bool SGCore::ShaderTexturesFromGlobalStorageBlock::operator!=(
        const SGCore::ShaderTexturesFromGlobalStorageBlock& other) const noexcept
{
    return !(*this == other);
}

// ================================================================
// ================================================================
// ================================================================

void SGCore::IShader::updateFrameBufferAttachmentsCount(const Ref<IFrameBuffer>& frameBuffer,
                                                        const std::string& frameBufferNameInShader) noexcept
{
    std::uint16_t depthAttachmentsCount = frameBuffer->getDepthAttachmentsCount();
    std::uint16_t depthStencilAttachmentsCount = frameBuffer->getDepthStencilAttachmentsCount();
    std::uint16_t colorAttachmentsCount = frameBuffer->getColorAttachmentsCount();
    std::uint16_t renderAttachmentsCount = frameBuffer->getRenderAttachmentsCount();

    // todo: names below are constant. mb make as defines
    useInteger(frameBufferNameInShader + "_depthAttachmentsCount", depthAttachmentsCount);
    useInteger(frameBufferNameInShader + "_depthStencilAttachmentsCount", depthStencilAttachmentsCount);
    useInteger(frameBufferNameInShader + "_colorAttachmentsCount", colorAttachmentsCount);
    useInteger(frameBufferNameInShader + "_renderAttachmentsCount", renderAttachmentsCount);
}

void SGCore::IShader::addDefines(const SGShaderDefineType& shaderDefineType,
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

void SGCore::IShader::emplaceDefines(const SGShaderDefineType& shaderDefineType,
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

void SGCore::IShader::addDefine(const SGShaderDefineType& shaderDefineType,
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

void SGCore::IShader::emplaceDefine(const SGShaderDefineType& shaderDefineType,
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

void SGCore::IShader::removeDefine(const SGShaderDefineType& shaderDefineType,
                                           const ShaderDefine& shaderDefine)
{
    m_defines[shaderDefineType].remove(shaderDefine);

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::removeDefine(const SGShaderDefineType& shaderDefineType,
                                           const std::string& shaderDefineName)
{
    m_defines[shaderDefineType].remove(ShaderDefine(shaderDefineName, ""));

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::updateDefine(const SGShaderDefineType& shaderDefineType,
                                           const SGCore::ShaderDefine& shaderDefine)
{
    setAssetModifiedChecking(false);

    removeDefine(shaderDefineType, shaderDefine.m_name);
    addDefine(shaderDefineType, shaderDefine);

    setAssetModifiedChecking(true);
}

void SGCore::IShader::emplaceUpdateDefine(const SGShaderDefineType& shaderDefineType,
                                                  SGCore::ShaderDefine&& shaderDefine)
{
    setAssetModifiedChecking(false);

    removeDefine(shaderDefineType, shaderDefine.m_name);
    emplaceDefine(shaderDefineType, std::move(shaderDefine));

    setAssetModifiedChecking(true);
}

void SGCore::IShader::updateDefines(const SGShaderDefineType& shaderDefineType,
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

void SGCore::IShader::emplaceUpdateDefines(const SGShaderDefineType& shaderDefineType,
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

void SGCore::IShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                             const std::list<ShaderDefine>& otherDefines) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherDefines.begin(), otherDefines.end());

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                             Ref<IShader> otherShader) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    auto& otherShaderTypedDefines = otherShader->m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherShaderTypedDefines.begin(), otherShaderTypedDefines.end());

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::clearDefinesOfType(const SGShaderDefineType& shaderDefineType) noexcept
{
    m_defines[shaderDefineType].clear();

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::onAssetModified()
{
    compile(m_fileAsset.lock());
}

void SGCore::IShader::onAssetPathChanged()
{
    compile(m_fileAsset.lock());
}

SGCore::IShader& SGCore::IShader::operator=(const SGCore::IShader& other) noexcept
{
    assert(this != std::addressof(other));

    for(const auto& shaderDefinesPair : m_defines)
    {
        replaceDefines(shaderDefinesPair.first, shaderDefinesPair.second);
    }

    m_fileAsset = other.m_fileAsset;

    return *this;
}

SGCore::Ref<SGCore::IShader> SGCore::IShader::addToGlobalStorage() noexcept
{
    auto thisShared = shared_from_this();

    GPUObjectsStorage::addShader(thisShared);

    return thisShared;
}

void SGCore::IShader::removeTexturesFromGlobalStorageBlock(
        const SGCore::ShaderTexturesFromGlobalStorageBlock& block) noexcept
{
    std::remove(m_texturesFromGlobalStorageBlocks.begin(), m_texturesFromGlobalStorageBlocks.end(), block);
}

void SGCore::IShader::clearTexturesFromGlobalStorageBlocks() noexcept
{
    m_texturesFromGlobalStorageBlocks.clear();
}

void SGCore::IShader::onTexturesCountChanged() noexcept
{
    // todo: make pass all textures to shader!!

    std::uint8_t texBlock = 0;

    std::uint8_t curTexture = 0;

    for(const auto& texturesFromGlobalStorageBlock : m_texturesFromGlobalStorageBlocks)
    {
        if(texturesFromGlobalStorageBlock.m_isSingleTextureBlock)
        {
            useTextureBlock(texturesFromGlobalStorageBlock.m_uniformName, texBlock);

            ++texBlock;
        }
        else
        {
            for(std::uint8_t i = 0; i < texturesFromGlobalStorageBlock.getTextures().size(); ++i)
            {
                useTextureBlock(texturesFromGlobalStorageBlock.m_uniformName + "[" + std::to_string(curTexture) + "]",
                                texBlock);

                ++curTexture;
                ++texBlock;
            }
        }

        useInteger(texturesFromGlobalStorageBlock.m_uniformName + "_COUNT", curTexture);

        curTexture = 0;
    }
}
