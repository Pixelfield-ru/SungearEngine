#include "IMaterial.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IShader.h"

void SGCore::IMaterial::load(const std::string& path)
{

}

std::shared_ptr<SGCore::ITexture2D>
SGCore::IMaterial::findAndAddTexture2D(const SGTextureType& textureType,
                                                     const std::string& path)
{
    auto foundTex =
            AssetManager::loadAsset<ITexture2D>(path);

    m_textures[textureType].push_back(foundTex);

    return foundTex;
}

void
SGCore::IMaterial::addTexture2D(const SGTextureType& textureType, const SGCore::Ref<SGCore::ITexture2D>& tex)
{
    m_textures[textureType].push_back(tex);
}

void SGCore::IMaterial::copyTextures(const std::shared_ptr<IMaterial>& to) const noexcept
{
    to->m_textures = m_textures;
}

void SGCore::IMaterial::setDiffuseColor(const glm::vec4& col) noexcept
{
    m_diffuseColor = col;
}

void SGCore::IMaterial::setSpecularColor(const glm::vec4& col) noexcept
{
    m_specularColor = col;
}

void SGCore::IMaterial::setAmbientColor(const glm::vec4& col) noexcept
{
    m_ambientColor = col;
}

void SGCore::IMaterial::setEmissionColor(const glm::vec4& col) noexcept
{
    m_emissionColor = col;
}

void SGCore::IMaterial::setTransparentColor(const glm::vec4& col) noexcept
{
    m_transparentColor = col;
}

void SGCore::IMaterial::setShininess(const float& shininess) noexcept
{
    m_shininess = shininess;
}

void SGCore::IMaterial::setMetallicFactor(const float& metallicFactor) noexcept
{
    m_metallicFactor = metallicFactor;
}

void SGCore::IMaterial::setRoughnessFactor(const float& roughnessFactor) noexcept
{
    m_roughnessFactor = roughnessFactor;
}

SGCore::IMaterial& SGCore::IMaterial::operator=
        (const SGCore::IMaterial& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    other.copyTextures(shared_from_this());

    return *this;
}
