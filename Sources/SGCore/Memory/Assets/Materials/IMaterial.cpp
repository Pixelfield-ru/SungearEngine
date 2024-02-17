#include "IMaterial.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IShader.h"

void SGCore::IMaterial::createShader() noexcept
{
    m_shader = MakeRef<IShader>();
    
    m_shader->setParentMaterial(shared_from_this());
}

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

    // todo:
    if(m_shader)
    {
        m_shader->collectTextureFromMaterial(foundTex, textureType);
    }

    return foundTex;
}

void
SGCore::IMaterial::addTexture2D(const SGTextureType& textureType, const SGCore::Ref<SGCore::ITexture2D>& tex)
{
    m_textures[textureType].push_back(tex);

    // todo:
    if(m_shader)
    {
        m_shader->collectTextureFromMaterial(tex, textureType);
    }
}

void SGCore::IMaterial::copyTextures(const std::shared_ptr<IMaterial>& to) const noexcept
{
    to->m_textures = m_textures;

    if(to->m_shader)
    {
        for(const auto& texIter : to->m_textures)
        {
            for(const auto& tex : texIter.second)
            {
                to->m_shader->collectTextureFromMaterial(tex, texIter.first);
            }
        }
    }
}

void SGCore::IMaterial::setShader(const SGCore::Ref<SGCore::IShader>& shader) noexcept
{
    m_shader = shader;

    for(const auto& subPassShaderPair : m_shader->getSubPassesShaders())
    {
        const auto& subPassShader = subPassShaderPair.second;

        subPassShader->useVectorf("materialDiffuseCol",
                                  m_diffuseColor
        );
        subPassShader->useVectorf("materialSpecularCol",
                                  m_specularColor
        );
        subPassShader->useVectorf("materialAmbientCol",
                                  m_ambientColor
        );
        subPassShader->useVectorf("materialEmissionCol",
                                  m_emissionColor
        );
        subPassShader->useVectorf("materialTransparentCol",
                                  m_transparentColor
        );
        subPassShader->useFloat("materialShininess",
                                m_shininess
        );
        subPassShader->useFloat("materialMetallicFactor",
                                m_metallicFactor
        );
        subPassShader->useFloat("materialRoughnessFactor",
                                m_roughnessFactor
        );
    }
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
        for(const auto& subPassShaderPair : m_shader->getSubPassesShaders())
        {
            const auto& subPassShader = subPassShaderPair.second;

            subPassShader->bind();

            subPassShader->useVectorf("materialDiffuseCol",
                                      m_diffuseColor
            );
        }
    }
}

void SGCore::IMaterial::setSpecularColor(const glm::vec4& col) noexcept
{
    m_specularColor = col;

    if(m_shader)
    {
        for(const auto& subPassShaderPair : m_shader->getSubPassesShaders())
        {
            const auto& subPassShader = subPassShaderPair.second;

            subPassShader->bind();

            subPassShader->useVectorf("materialSpecularCol",
                                      m_specularColor
            );
        }
    }
}

void SGCore::IMaterial::setAmbientColor(const glm::vec4& col) noexcept
{
    m_ambientColor = col;

    if(m_shader)
    {
        for(const auto& subPassShaderPair: m_shader->getSubPassesShaders())
        {
            const auto& subPassShader = subPassShaderPair.second;

            subPassShader->bind();

            subPassShader->useVectorf("materialAmbientCol",
                                      m_ambientColor
            );
        }
    }
}

void SGCore::IMaterial::setEmissionColor(const glm::vec4& col) noexcept
{
    m_emissionColor = col;

    if(m_shader)
    {
        for(const auto& subPassShaderPair : m_shader->getSubPassesShaders())
        {
            const auto& subPassShader = subPassShaderPair.second;

            subPassShader->bind();

            subPassShader->useVectorf("materialEmissionCol",
                                      m_emissionColor
            );
        }
    }
}

void SGCore::IMaterial::setTransparentColor(const glm::vec4& col) noexcept
{
    m_transparentColor = col;

    if(m_shader)
    {
        for(const auto& subPassShaderPair : m_shader->getSubPassesShaders())
        {
            const auto& subPassShader = subPassShaderPair.second;

            subPassShader->bind();

            subPassShader->useVectorf("materialTransparentCol",
                                      m_transparentColor
            );
        }
    }
}

void SGCore::IMaterial::setShininess(const float& shininess) noexcept
{
    m_shininess = shininess;

    if(m_shader)
    {
        for(const auto& subPassShaderPair : m_shader->getSubPassesShaders())
        {
            const auto& subPassShader = subPassShaderPair.second;

            subPassShader->bind();

            subPassShader->useFloat("materialShininess",
                                    m_shininess
            );
        }
    }
}

void SGCore::IMaterial::setMetallicFactor(const float& metallicFactor) noexcept
{
    m_metallicFactor = metallicFactor;

    if(m_shader)
    {
        for(const auto& subPassShaderPair : m_shader->getSubPassesShaders())
        {
            const auto& subPassShader = subPassShaderPair.second;

            subPassShader->bind();

            subPassShader->useFloat("materialMetallicFactor",
                                    m_metallicFactor
            );
        }
    }
}

void SGCore::IMaterial::setRoughnessFactor(const float& roughnessFactor) noexcept
{
    m_roughnessFactor = roughnessFactor;

    if(m_shader)
    {
        for(const auto& subPassShaderPair : m_shader->getSubPassesShaders())
        {
            const auto& subPassShader = subPassShaderPair.second;

            subPassShader->bind();

            subPassShader->useFloat("materialRoughnessFactor",
                                    m_roughnessFactor
            );
        }
    }
}

SGCore::IMaterial& SGCore::IMaterial::operator=
        (const SGCore::IMaterial& other) noexcept
{
    assert(this != std::addressof(other));

    other.copyTextures(shared_from_this());

    return *this;
}
