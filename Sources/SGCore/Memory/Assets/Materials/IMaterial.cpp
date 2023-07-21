#include "IMaterial.h"

#include "SGCore/Main/CoreMain.h"

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind()
{
    m_shader->bind();
    for(const auto& block : m_blocks)
    {
        for(const auto& typedTexture : block.second.m_textures)
        {
            const auto& texture = typedTexture.second.m_textureAsset->getTexture2D();
            const auto& textureUnit = typedTexture.second.m_textureUnit;
            texture->bind(textureUnit);
            m_shader->useMaterialTexture(typedTexture.second);
        }
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::IMaterial::load(const std::string& path)
{
    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::Texture2DAsset>
Core::Memory::Assets::IMaterial::findAndAddTexture2D(const SGMaterialTextureType& type, const std::string& path)
{
    auto& texturesWithType = m_blocks[type];

    auto& typedTextures = texturesWithType.m_textures;

    if(typedTextures.find(path) == typedTextures.end())
    {
        auto foundTex = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path);

        addTexture2D(type, foundTex);

        return foundTex;
    }

    return nullptr;
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::addTexture2D(const SGMaterialTextureType& type,
                                                                         const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset)
{
    auto& texturesWithType = m_blocks[type];

    auto& maximumTextures = texturesWithType.m_maximumTextures;
    auto& texturesUnitOffset = texturesWithType.m_texturesUnitOffset;
    auto& typedTextures = texturesWithType.m_textures;

    // path to texture
    auto texturePath = texture2DAsset->getPath().string();

    // if this texture not exists
    if(typedTextures.find(texturePath) == typedTextures.end())
    {
        // current max found unit
        m_maxUnit = texturesUnitOffset;
        if(!typedTextures.empty())
        {
            // finding current maximum texture unit
            for(auto& texPair : typedTextures)
            {
                if(texPair.second.m_textureUnit > m_maxUnit)
                {
                    m_maxUnit = texPair.second.m_textureUnit;
                }
            }

            m_maxUnit += 1;

        }
        // if there is free space for another texture of this type
        if(m_maxUnit < m_maxUnit + maximumTextures)
        {
            // this is texture unit to bind for this texture
            typedTextures[texturePath].m_textureUnit = m_maxUnit;
            typedTextures[texturePath].m_textureAsset = texture2DAsset;

            std::string finalName = sgMaterialTextureTypeToString(type)  + std::to_string(m_maxUnit);
            std::string finalDefine = finalName +  + "_DEFINED";
            SGC_SUCCESS("Final define for material texture: " + finalDefine);

            typedTextures[texturePath].m_nameInShader = finalName;
            //texture2DAsset->m_name = finalName;

            m_shader->addShaderDefines({Graphics::ShaderDefine(finalDefine, "")});
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

std::shared_ptr<Core::Memory::Assets::IMaterial>
Core::Memory::Assets::IMaterial::findAndSetTexture2D(const SGMaterialTextureType& type, const std::string& oldTextureAssetPath, const std::string& newTextureAssetPath)
{
    if(oldTextureAssetPath != newTextureAssetPath)
    {
        setTexture2D(type, oldTextureAssetPath,
                     Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(newTextureAssetPath));
    }

    return shared_from_this();
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