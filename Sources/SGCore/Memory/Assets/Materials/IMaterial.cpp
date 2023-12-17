#include "IMaterial.h"
#include "SGCore/Main/CoreMain.h"

std::shared_ptr<SGCore::IMaterial>
SGCore::IMaterial::bind(const std::shared_ptr<IShader>& shader)
{
    std::uint8_t curTexBlock = 0;
    std::uint16_t currentTextureOfType = 0;
    std::string texturesTypeStr;

    for(std::uint16_t texType = SGTextureType::SGTP_EMISSIVE; texType <= SGTextureType::SGTP_SHADOW_MAP2D; ++texType)
    {
        auto texturesType = (SGTextureType) texType;

        texturesTypeStr = sgStandardTextureTypeToString(texturesType);

        const auto& foundTypedTextures = m_textures.find(texturesType);

        if(foundTypedTextures != m_textures.cend())
        {
            for(const auto& texture : foundTypedTextures->second)
            {
                texture->bind(curTexBlock);
                shader->useTextureBlock(texturesTypeStr + "[" + std::to_string(currentTextureOfType) + "]",
                                        curTexBlock
                );

                ++curTexBlock;
                ++currentTextureOfType;
            }
        }

        shader->useTextureBlock(texturesTypeStr + "_COUNT", currentTextureOfType);

        currentTextureOfType = 0;
    }

    shader->useVectorf("materialDiffuseCol",
                       m_diffuseColor
    );
    shader->useVectorf("materialSpecularCol",
                       m_specularColor
    );
    shader->useVectorf("materialAmbientCol",
                       m_ambientColor
    );
    shader->useVectorf("materialEmissionCol",
                       m_emissionColor
    );
    shader->useVectorf("materialTransparentCol",
                       m_transparentColor
    );
    shader->useFloat("materialShininess",
                     m_shininess
    );
    shader->useFloat("materialMetallicFactor",
                     m_metallicFactor
    );
    shader->useFloat("materialRoughnessFactor",
                     m_roughnessFactor
    );

    return shared_from_this();
}

std::shared_ptr<SGCore::IAsset> SGCore::IMaterial::load(const std::string& path)
{
    return shared_from_this();
}

std::shared_ptr<SGCore::Texture2DAsset>
SGCore::IMaterial::findAndAddTexture2D(const SGTextureType& textureType,
                                                     const std::string& path)
{
    auto foundTex =
            AssetManager::loadAsset<Texture2DAsset>(path);

    m_textures[textureType].push_back(foundTex->m_texture2D);

    return foundTex;
}

void SGCore::IMaterial::copyTextures(const std::shared_ptr<IMaterial>& to) const noexcept
{
    to->m_textures = m_textures;
}

SGCore::IMaterial&
SGCore::IMaterial::operator=(const SGCore::IMaterial& other) noexcept
{
    assert(this != std::addressof(other));

    other.copyTextures(shared_from_this());

    return *this;
}
