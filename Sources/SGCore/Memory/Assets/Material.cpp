#include "Material.h"

#include "SGCore/Main/Core.h"

Core::Memory::Assets::Material::Material() noexcept
{
    m_shader = std::shared_ptr<Core::Graphics::API::IShader>(Core::Main::Core::getRenderer().createShader());
    m_shader->compile(Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/mesh/default_shader.glsl"));
    m_shader->addShaderDefines({
        //Core::Graphics::API::ShaderDefine("FLIP_TEXTURES_Y", "")
    });
}

void Core::Memory::Assets::Material::createAsPBR() noexcept
{
    std::string whiteTexPath = "../SGResources/textures/white_texture.png";

    findAndAddTexture2D(SGMAT_BASE_COLOR, whiteTexPath);
    findAndAddTexture2D(SGMAT_SPECULAR_COLOR, whiteTexPath);
    findAndAddTexture2D(SGMAT_ROUGHNESS, whiteTexPath);
    findAndAddTexture2D(SGMAT_NORMAL_MAP, whiteTexPath);
    findAndAddTexture2D(SGMAT_PARALLAX_MAP, whiteTexPath);
    findAndAddTexture2D(SGMAT_OCCLUSION, whiteTexPath);
    findAndAddTexture2D(SGMAT_EMISSIVE, whiteTexPath);
}

std::shared_ptr<Core::Memory::Assets::Material> Core::Memory::Assets::Material::bind() noexcept
{
    m_shader->bind();
    for(auto& textureTuple : m_textures)
    {
        textureTuple.second->bind();
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::Material::load(const std::string& path)
{
    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::Material> Core::Memory::Assets::Material::addTexture2D
        (const std::string& name, const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset) noexcept
{
    // if this texture not exists
    if(m_textures.find(name) == m_textures.end())
    {
        m_textures[name] = texture2DAsset->getTexture2D();
        m_shader->addShaderDefines({Graphics::API::ShaderDefine(name + "_DEFINED", "")});
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::Material> Core::Memory::Assets::Material::findAndAddTexture2D
(const std::string& name, const std::string& path) noexcept
{
    // if this texture not exists
    if(m_textures.find(name) == m_textures.end())
    {
        std::shared_ptr<Graphics::API::ITexture2D> foundTex(Core::Main::Core::getRenderer().createTexture2D());
        *foundTex = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path)->getTexture2D();

        // current max found unit
        int maxUnit = 0;
        if(!m_textures.empty())
        {
            for(auto& texPair : m_textures)
            {
                auto& texture = texPair.second;
                if(texture->getUnit() > maxUnit)
                {
                    maxUnit = texture->getUnit();
                }
            }

            foundTex->setUnit(maxUnit + 1);
        }

        m_textures[name] = foundTex;
        m_shader->addShaderDefines({ Graphics::API::ShaderDefine(name + "_DEFINED", "") });
    }

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::API::ITexture2D> Core::Memory::Assets::Material::getTexture2D(const std::string& name) noexcept
{
    return m_textures[name];
}

std::shared_ptr<Core::Memory::Assets::Material> Core::Memory::Assets::Material::setTexture2D
(const std::string_view& name, const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset) noexcept
{
    *getTexture2D(name.data()) = texture2DAsset->getTexture2D();

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::Material>
Core::Memory::Assets::Material::findAndSetTexture2D(const std::string_view& name, const std::string& path) noexcept
{
    setTexture2D(name, Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path));

    return shared_from_this();
}
