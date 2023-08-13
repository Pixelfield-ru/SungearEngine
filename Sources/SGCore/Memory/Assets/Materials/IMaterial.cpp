#include "IMaterial.h"

#include "SGCore/Main/CoreMain.h"
#include <glad/glad.h>

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind()
{
    return bind(m_shader);
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

    m_shader->setAssetModifiedChecking(false);

    m_shader->removeShaderDefine(maxTextureBlockDefine);
    m_shader->addShaderDefines({
        Graphics::ShaderDefine(maxTextureBlockDefine, std::to_string(maxTextures)),
        });

    m_shader->setAssetModifiedChecking(true);
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
        m_shader->useMaterialTexture(typedTexture.second);
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::Texture2DAsset>
Core::Memory::Assets::IMaterial::findAndAddTexture2D(const SGMaterialTextureType& type,
                                                     const std::string& path,
                                                     const SGTextureType& textureType)
{
    auto& texturesWithType = m_blocks[type];

    auto& typedTextures = texturesWithType.m_textures;

    if(typedTextures.find(path) == typedTextures.end())
    {
        auto foundTex =
                Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path, textureType);

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

            m_shader->addShaderDefines({
                Graphics::ShaderDefine(newTextureFinalDefine, "")
            });
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
(std::shared_ptr<Graphics::IShader> otherShader)
{
    if(m_shader)
    {
        otherShader->replaceDefines(m_shader);
    }

    m_shader = otherShader;
}

std::shared_ptr<Core::Graphics::IShader> Core::Memory::Assets::IMaterial::getShader() const noexcept
{
    return m_shader;
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
