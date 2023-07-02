#include "IMaterial.h"

#include "SGCore/Main/Core.h"

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind()
{
    m_shader->bind();
    for(auto& types : m_textures)
    {
        for(auto& textures : get<2>(types.second))
        {
            textures.second.second->getTexture2D()->bind(textures.second.first);
        }
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::IMaterial::load(const std::string& path)
{
    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial>
Core::Memory::Assets::IMaterial::findAndAddTexture2D(const SGMaterialTextureType& type, const std::string& path)
{
    auto& texturesWithType = m_textures[type];

    auto& unpackedTexturesWithType = get<2>(texturesWithType);

    if(unpackedTexturesWithType.find(path) == unpackedTexturesWithType.end())
    {
        auto foundTex = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path);

        addTexture2D(type, foundTex);
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::addTexture2D(const SGMaterialTextureType& type,
                                                                         const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset)
{
    auto& texturesWithType = m_textures[type];

    auto& unpackedMaximumTextures = get<0>(texturesWithType);
    auto& unpackedTextureUnitOffset = get<1>(texturesWithType);
    auto& unpackedTexturesWithType = get<2>(texturesWithType);

    // path to texture
    auto texturePath = texture2DAsset->getPath().string();

    // if this texture not exists
    if(unpackedTexturesWithType.find(texturePath) == unpackedTexturesWithType.end())
    {
        // current max found unit
        int maxUnit = unpackedTextureUnitOffset;
        if(!unpackedTexturesWithType.empty())
        {
            // finding current maximum texture unit
            for(auto& texPair : unpackedTexturesWithType)
            {
                if(texPair.second.first > maxUnit)
                {
                    maxUnit = texPair.second.first;
                }
            }

            maxUnit += 1;

        }
        // if there is free space for another texture of this type
        if(maxUnit < maxUnit + unpackedMaximumTextures)
        {
            // this is texture unit to bind for this texture
            unpackedTexturesWithType[texturePath].first = maxUnit;
            unpackedTexturesWithType[texturePath].second = texture2DAsset;

            std::string finalName = sgMaterialTextureTypeToString(type)  + std::to_string(maxUnit) + "_DEFINED";
            SGC_SUCCESS("Final define for material texture: " + finalName);

            m_shader->addShaderDefines({Graphics::API::ShaderDefine(finalName, "")});
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
        get<2>(m_textures[type])[oldTextureAssetPath.data()].second = texture2DAsset;
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
    for(auto& texturesTypes : other.m_textures)
    {
        for(auto& texture : get<2>(texturesTypes.second))
        {
            addTexture2D(texturesTypes.first, texture.second.second);
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