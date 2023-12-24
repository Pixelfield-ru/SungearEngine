#include "IMaterial.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IShader.h"

SGCore::IMaterial::IMaterial() noexcept
{
    auto& shadersPaths = *SGSingleton::getSharedPtrInstance<ShadersPaths>();

    m_shader = Ref<IShader>(
            CoreMain::getRenderer().createShader(
                    shadersPaths["PBR"]["DefaultShader"]
            )
    )->addToGlobalStorage();
}

std::shared_ptr<SGCore::IMaterial>
SGCore::IMaterial::bind(const std::shared_ptr<IShader>& shader)
{
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

SGCore::Ref<SGCore::IMaterial> SGCore::IMaterial::bind()
{
    if(m_shader)
    {
        m_shader->bind();
    }

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

    // todo:
    if(m_shader)
    {
        m_shader->collectTexturesFromMaterial(shared_from_this());
    }

    return foundTex;
}

void SGCore::IMaterial::copyTextures(const std::shared_ptr<IMaterial>& to) const noexcept
{
    to->m_textures = m_textures;
}

void SGCore::IMaterial::setShader(const SGCore::Ref<SGCore::IShader>& shader) noexcept
{
    m_shader = shader;

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
}

SGCore::Ref<SGCore::IShader> SGCore::IMaterial::getShader() const noexcept
{
    return m_shader;
}

void SGCore::IMaterial::setDiffuseColor(const glm::vec4& col) noexcept
{
    m_diffuseColor = col;

    if(m_shader)
    {
        m_shader->bind();

        m_shader->useVectorf("materialDiffuseCol",
                             m_diffuseColor
        );
    }
}

void SGCore::IMaterial::setSpecularColor(const glm::vec4& col) noexcept
{
    m_specularColor = col;

    if(m_shader)
    {
        m_shader->bind();

        m_shader->useVectorf("materialSpecularCol",
                             m_specularColor
        );
    }
}

void SGCore::IMaterial::setAmbientColor(const glm::vec4& col) noexcept
{
    m_ambientColor = col;

    if(m_shader)
    {
        m_shader->bind();

        m_shader->useVectorf("materialAmbientCol",
                             m_ambientColor
        );
    }
}

void SGCore::IMaterial::setEmissionColor(const glm::vec4& col) noexcept
{
    m_emissionColor = col;

    if(m_shader)
    {
        m_shader->bind();

        m_shader->useVectorf("materialEmissionCol",
                             m_emissionColor
        );
    }
}

void SGCore::IMaterial::setTransparentColor(const glm::vec4& col) noexcept
{
    m_transparentColor = col;

    if(m_shader)
    {
        m_shader->bind();

        m_shader->useVectorf("materialTransparentCol",
                             m_transparentColor
        );
    }
}

void SGCore::IMaterial::setShininess(const float& shininess) noexcept
{
    m_shininess = shininess;

    if(m_shader)
    {
        m_shader->bind();

        m_shader->useFloat("materialShininess",
                           m_shininess
        );
    }
}

void SGCore::IMaterial::setMetallicFactor(const float& metallicFactor) noexcept
{
    m_metallicFactor = metallicFactor;

    if(m_shader)
    {
        m_shader->bind();

        m_shader->useFloat("materialMetallicFactor",
                           m_metallicFactor
        );
    }
}

void SGCore::IMaterial::setRoughnessFactor(const float& roughnessFactor) noexcept
{
    m_roughnessFactor = roughnessFactor;

    if(m_shader)
    {
        m_shader->bind();

        m_shader->useFloat("materialRoughnessFactor",
                           m_roughnessFactor
        );
    }
}

SGCore::IMaterial& SGCore::IMaterial::operator=
        (const SGCore::IMaterial& other) noexcept
{
    assert(this != std::addressof(other));

    other.copyTextures(shared_from_this());

    return *this;
}

