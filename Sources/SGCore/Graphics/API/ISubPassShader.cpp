//
// Created by stuka on 13.05.2023.
//

#include "ISubPassShader.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"
#include "IFrameBuffer.h"
#include "ITexture2D.h"

void SGCore::ISubPassShader::recompile() noexcept
{
    destroy();
    compile(m_fileAsset.lock());
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

void SGCore::ISubPassShader::addTexturesBlock(const SGCore::Ref<SGCore::TexturesBlock>& block) noexcept
{
    if(std::find(m_texturesBlocks.begin(), m_texturesBlocks.end(), block) ==
       m_texturesBlocks.end())
    {
        m_texturesBlocks.push_back(block);
        block->setParentShader(shared_from_this());
    }
}

void SGCore::ISubPassShader::removeTexturesBlock(const Ref<TexturesBlock>& block) noexcept
{
    m_texturesBlocks.erase(
            std::remove(m_texturesBlocks.begin(), m_texturesBlocks.end(), block),
            m_texturesBlocks.end());
}

void SGCore::ISubPassShader::clearTexturesBlocks() noexcept
{
    m_texturesBlocks.clear();
}

void SGCore::ISubPassShader::addTexture(const Ref<ITexture2D>& texture2D) noexcept
{
    for(auto& texturesBlock : m_texturesBlocks)
    {
        texturesBlock->addTexture(texture2D);
    }
}

void SGCore::ISubPassShader::addTexture
(const SGCore::Ref<SGCore::ITexture2D>& texture2D, SGTextureType textureType) noexcept
{
    for(auto& texturesBlock : m_texturesBlocks)
    {
        texturesBlock->addTexture(texture2D, textureType);
    }
}

void SGCore::ISubPassShader::removeTexture(const SGCore::Ref<SGCore::ITexture2D>& texture2D) noexcept
{
    for(auto& texturesBlock : m_texturesBlocks)
    {
        texturesBlock->removeTexture(texture2D);
    }
}

void SGCore::ISubPassShader::collectTexturesFromMaterial(const SGCore::Ref<SGCore::IMaterial>& material) noexcept
{
    for(auto& texturesBlock : m_texturesBlocks)
    {
        texturesBlock->collectTexturesFromMaterial(material);
    }
}

void SGCore::ISubPassShader::onTexturesCountChanged() noexcept
{
    bind();

    // todo: make pass all textures to shader!!

    std::uint8_t texBlock = 0;

    std::uint8_t curTexture = 0;

    for(const auto& texturesBlock : m_texturesBlocks)
    {
        if(texturesBlock->m_isSingleTextureBlock)
        {
            useTextureBlock(texturesBlock->m_uniformName, texBlock);

            ++curTexture;
            ++texBlock;
        }
        else
        {
            for(std::uint8_t i = 0; i < texturesBlock->getTextures().size(); ++i)
            {
                useTextureBlock(texturesBlock->m_uniformName,
                                texBlock);

                ++curTexture;
                ++texBlock;
            }
        }

        useInteger(texturesBlock->m_uniformRawName + "_CURRENT_COUNT", curTexture);

        curTexture = 0;
    }
}

void SGCore::ISubPassShader::bindTexturesBlocks() const noexcept
{
    std::uint8_t curTexBlock = 0;
    for(const auto& texturesBlock : m_texturesBlocks)
    {
        auto curIter = texturesBlock->getTextures().begin();
        while(curIter != texturesBlock->getTextures().end())
        {
            if(auto lockedTexture = curIter->lock())
            {
                lockedTexture->bind(curTexBlock);

                ++curIter;
                ++curTexBlock;
            }
            else
            {
                curIter = texturesBlock->eraseTexture(curIter);
            }
        }
    }
}
