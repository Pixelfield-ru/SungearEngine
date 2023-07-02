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
        }//second->getTexture2D()->bind(textureTuple.second.first);
    }

    return shared_from_this();
}

/*Core::Memory::Assets::IMaterial::IMaterial() noexcept
{
    m_shader = std::shared_ptr<Core::Graphics::API::IShader>(Core::Main::Core::getRenderer().createShader());
    m_shader->compile(Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/mesh/default_shader.glsl"));
    m_shader->addShaderDefines({
        //Core::Graphics::API::ShaderDefine("FLIP_TEXTURES_Y", "")
    });
}*/

/*
void Core::Memory::Assets::IMaterial::createAsPBR() noexcept
{
    std::string whiteTexPath = "../SGResources/textures/white_texture.png";
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind() noexcept
{
    m_shader->bind();
    for(auto& textureTuple : m_textures)
    {
        textureTuple.second.second->getTexture2D()->bind(textureTuple.second.first);
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::IMaterial::load(const std::string& path)
{
    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::addTexture2D
        (const std::string& name, const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset) noexcept
{
    // if this texture not exists
    if(m_textures.find(name) == m_textures.end())
    {
        m_textures[name].second = texture2DAsset;
        m_shader->addShaderDefines({Graphics::API::ShaderDefine(name + "_DEFINED", "")});
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::findAndAddTexture2D
(const std::string& name, const std::string& path) noexcept
{
    // if this texture not exists
    if(m_textures.find(name) == m_textures.end())
    {
        auto foundTex = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path);

        // current max found unit
        int maxUnit = 0;
        if(!m_textures.empty())
        {
            // finding current maximum texture unit
            for(auto& texPair : m_textures)
            {
                if(texPair.second.first > maxUnit)
                {
                    maxUnit = texPair.second.first;
                }
            }

            maxUnit += 1;
        }

        m_textures[name].first = maxUnit;
        m_textures[name].second = foundTex;
        m_shader->addShaderDefines({ Graphics::API::ShaderDefine(name + "_DEFINED", "") });
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::Texture2DAsset> Core::Memory::Assets::IMaterial::getTexture2D(const std::string& name) noexcept
{
    return m_textures[name].second;
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::setTexture2D
(const std::string_view& name, const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset) noexcept
{
    m_textures[name.data()].second = texture2DAsset;

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial>
Core::Memory::Assets::IMaterial::findAndSetTexture2D(const std::string_view& name, const std::string& path) noexcept
{
    setTexture2D(name, Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path));

    return shared_from_this();
}
*/