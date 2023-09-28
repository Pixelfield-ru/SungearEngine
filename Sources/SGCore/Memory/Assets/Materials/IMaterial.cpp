#include "IMaterial.h"

#include "SGCore/Main/CoreMain.h"
#include <glad/glad.h>

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind()
{
    return bind(m_currentShader);
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind
(const std::shared_ptr<Graphics::IShader>& otherShader)
{
    otherShader->bind();

    for(const auto& block : m_blocks)
    {
        for(const auto& typedTexture : block.second.m_textures)
        {
            const auto& textureUnit = typedTexture.second.m_textureUnit;
            if(typedTexture.second.m_textureAsset)
            {
                const auto& texture = typedTexture.second.m_textureAsset->getTexture2D();
                if(texture) texture->bind(textureUnit);
            }

            otherShader->useMaterialTexture(typedTexture.second);
        }
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::IMaterial::load(const std::string& path)
{
    return shared_from_this();
}

void Core::Memory::Assets::IMaterial::addBlockDeclaration
(const SGMaterialTextureType& blockType, const uint8_t& maxTextures, const uint8_t& blockOffset)
{
    m_blocks[blockType].m_maximumTextures = maxTextures;
    m_blocks[blockType].m_texturesUnitOffset = blockOffset;

    auto typeName = sgMaterialTextureTypeToString(blockType);
    auto maxTextureBlockDefine = typeName + "_MAX_TEXTURES_NUM";

    SGC_SUCCESS("Final define for this type textures number: " + maxTextureBlockDefine);

    for(auto& shaderPair : m_shaders)
    {
        auto& shader = shaderPair.second;

        shader->setAssetModifiedChecking(false);

        shader->removeShaderDefine(SGShaderDefineType::SGG_MATERIAL_TEXTURES_BLOCK_DEFINE,
                                   maxTextureBlockDefine);
        shader->addShaderDefines(SGShaderDefineType::SGG_MATERIAL_TEXTURES_BLOCK_DEFINE,
                                 {
                                         Graphics::ShaderDefine(maxTextureBlockDefine,
                                                                std::to_string(maxTextures)),
                                 });

        shader->setAssetModifiedChecking(true);
    }
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bindBlock(const SGMaterialTextureType& blockType)
{
    for(const auto& typedTexture : m_blocks[blockType].m_textures)
    {
        const auto& textureUnit = typedTexture.second.m_textureUnit;
        if(typedTexture.second.m_textureAsset)
        {
            const auto& texture = typedTexture.second.m_textureAsset->getTexture2D();
            if(texture) texture->bind(textureUnit);
        }
        m_currentShader->useMaterialTexture(typedTexture.second);
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::Texture2DAsset>
Core::Memory::Assets::IMaterial::findAndAddTexture2D(const SGMaterialTextureType& type,
                                                     const std::string& path)
{
    auto& texturesWithType = m_blocks[type];

    auto& typedTextures = texturesWithType.m_textures;

    if(typedTextures.find(path) == typedTextures.end())
    {
        auto foundTex =
                Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path);

        addTexture2D(type, foundTex);

        return foundTex;
    }

    return nullptr;
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::addTexture2D
(const SGMaterialTextureType& type,
 const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset)
{
    auto& texturesWithType = m_blocks[type];

    auto& maximumTextures = texturesWithType.m_maximumTextures;
    auto& texturesUnitOffset = texturesWithType.m_texturesUnitOffset;
    auto& typedTextures = texturesWithType.m_textures;

    // path to texture
    auto texturePath = texture2DAsset->getPath().string();

    std::uint8_t freeUnit = 0;

    // if this texture not exists
    if(typedTextures.find(texturePath) == typedTextures.end())
    {
        // current max found unit
        freeUnit = texturesUnitOffset;
        if(!typedTextures.empty())
        {
            // finding current maximum texture unit
            for(auto& texPair : typedTextures)
            {
                if(texPair.second.m_textureUnit > freeUnit)
                {
                    freeUnit = texPair.second.m_textureUnit;
                }
            }

            freeUnit += 1;

        }
        // if there is free space for another texture of this type
        if(freeUnit < freeUnit + maximumTextures)
        {
            // this is texture unit to bind for this texture
            typedTextures[texturePath].m_textureUnit = freeUnit;
            typedTextures[texturePath].m_textureAsset = texture2DAsset;

            auto typeName = sgMaterialTextureTypeToString(type);
            auto finalName = typeName + std::to_string(freeUnit);
            auto newTextureFinalDefine = finalName +  + "_DEFINED";
            SGC_SUCCESS("Final define for new material texture: " + newTextureFinalDefine);

            typedTextures[texturePath].m_nameInShader = finalName;
            //texture2DAsset->m_name = finalName;

            for(auto& shaderPair : m_shaders)
            {
                auto& shader = shaderPair.second;
                shader->addShaderDefines(SGShaderDefineType::SGG_MATERIAL_TEXTURES_BLOCK_DEFINE,
                                         {
                                                 Graphics::ShaderDefine(newTextureFinalDefine, "")
                                         });
            }
        }
        else // there is no free texture units for texture
        {
            SGC_WARNING("There are not enough texture blocks to add a new texture of type '" +
                                sgMaterialTextureTypeToString(type) + "' to the material '" + m_name +
                                "'. The texture will be added to the list of not added textures");

            m_notUsedTextures[type].push_back(texture2DAsset);
        }
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial>
Core::Memory::Assets::IMaterial::setTexture2D(const SGMaterialTextureType& type, const std::string_view& oldTextureAssetPath,
                                              const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset)
{
    if(oldTextureAssetPath != texture2DAsset->getPath())
    {
        m_blocks[type].m_textures[oldTextureAssetPath.data()].m_textureAsset = texture2DAsset;
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::findAndSetTexture2D
(const SGMaterialTextureType& type,
 const std::string& oldTextureAssetPath,
 const std::string& newTextureAssetPath)
{
    if(oldTextureAssetPath != newTextureAssetPath)
    {
        setTexture2D(type, oldTextureAssetPath,
                     Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(newTextureAssetPath));
    }

    return shared_from_this();
}

void Core::Memory::Assets::IMaterial::setShader
(const std::string_view& name,
 const std::shared_ptr<Graphics::IShader>& otherShader)
{
    // setting to false so as not to compile otherShader after each defines change
    otherShader->setAssetModifiedChecking(false);

    otherShader->clearDefinesOfType(SGShaderDefineType::SGG_MATERIAL_TEXTURES_BLOCK_DEFINE);

    const auto& foundShaderPair = m_shaders.find(name.data());
    const bool shaderFound = foundShaderPair != m_shaders.end();
    if(shaderFound)
    {
        otherShader->replaceDefines(SGShaderDefineType::SGG_MATERIAL_TEXTURES_BLOCK_DEFINE,
                                    m_shaders[name.data()]);
    }

    if(foundShaderPair->second == m_currentShader)
    {
        m_currentShader = otherShader;
    }

    // adding blocks decls and textures in blocks decls to shader if shader haven't been in m_shaders
    if(!shaderFound)
    {
        for(const auto& blockPair : m_blocks)
        {
            auto typeName = sgMaterialTextureTypeToString(blockPair.first);
            auto maxTextureBlockDefine = typeName + "_MAX_TEXTURES_NUM";

            otherShader->addShaderDefines(SGShaderDefineType::SGG_MATERIAL_TEXTURES_BLOCK_DEFINE,
                                          { Graphics::ShaderDefine(maxTextureBlockDefine,
                                                                   std::to_string(blockPair.second.m_maximumTextures)) });

            for(const auto& blockTexture : blockPair.second.m_textures)
            {
                otherShader->addShaderDefines(SGShaderDefineType::SGG_MATERIAL_TEXTURES_BLOCK_DEFINE,
                                              { Graphics::ShaderDefine(maxTextureBlockDefine,
                                                                       blockTexture.second.m_nameInShader + "_DEFINED") });
            }
        }
    }

    // set to true (means calling function onAssetModified)
    otherShader->setAssetModifiedChecking(true);

    m_shaders[name.data()] = otherShader;
}

std::shared_ptr<Core::Graphics::IShader> Core::Memory::Assets::IMaterial::getShader(const std::string_view& name) noexcept
{
    return m_shaders.find(name.data()) != m_shaders.end() ? m_shaders[name.data()] : nullptr;
}

void Core::Memory::Assets::IMaterial::setCurrentShader(const std::string_view& name) noexcept
{
    if(m_shaders.find(name.data()) != m_shaders.end())
    {
        m_currentShader = m_shaders[name.data()];
    }
}

std::shared_ptr<Core::Graphics::IShader> Core::Memory::Assets::IMaterial::getCurrentShader() const noexcept
{
    return m_currentShader;
}

Core::Memory::Assets::IMaterial&
Core::Memory::Assets::IMaterial::operator=(const Core::Memory::Assets::IMaterial& other) noexcept
{
    assert(this != std::addressof(other));

    // adding all used textures
    for(auto& texturesTypes : other.m_blocks)
    {
        for(auto& texture : texturesTypes.second.m_textures)
        {
            addTexture2D(texturesTypes.first, texture.second.m_textureAsset);
        }
    }

    // adding all not used textures
    for(auto& texturesTypes : other.m_notUsedTextures)
    {
        for(auto& texture : texturesTypes.second)
        {
            addTexture2D(texturesTypes.first, texture);
        }
    }

    return *this;
}
