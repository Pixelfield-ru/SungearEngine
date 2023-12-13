#include "IMaterial.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/ShaderMarkup.h"

std::shared_ptr<SGCore::IMaterial>
SGCore::IMaterial::bind(const std::shared_ptr<IShader>& shader)
{
    std::uint8_t currentTexBlockOfType = 0;

    /*for(const auto& markedTextureBlock : shaderMarkup.m_texturesBlocks)
    {
        std::string texBlockTypeStr = markedTextureBlock.second.m_name;

        for(const auto& texture : m_textures)
        {
            if(texture.m_type == markedTextureBlock.second.m_type)
            {
                texture.m_texture->bind(markedTextureBlock.second.m_offset + currentTexBlockOfType);

                currentTexBlockOfType++;
            }
        }

        if(markedTextureBlock.second.m_autoSamplersCount)
        {
            shader->useInteger(texBlockTypeStr + "_COUNT", currentTexBlockOfType);
        }

        currentTexBlockOfType = 0;
    }*/

    std::uint16_t currentTexBlock = 0;



    if(shader->m_bindFrameBuffers)
    {
        if(shader->m_bindEveryFrameBufferAttachmentsSeparately)
        {
            for()
        }
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

    m_textures.emplace_back(textureType, foundTex->m_texture2D);

    return foundTex;
}

void SGCore::IMaterial::copyTextures(const std::shared_ptr<IMaterial>& to) const noexcept
{
    // adding all textures
    for(auto& texture : m_textures)
    {
        to->m_textures.push_back(texture);
    }
}

SGCore::IMaterial&
SGCore::IMaterial::operator=(const SGCore::IMaterial& other) noexcept
{
    assert(this != std::addressof(other));

    other.copyTextures(shared_from_this());

    return *this;
}
