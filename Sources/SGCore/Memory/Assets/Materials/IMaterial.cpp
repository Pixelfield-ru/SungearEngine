#include "IMaterial.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/MarkedShader.h"

std::shared_ptr<Core::Memory::Assets::IMaterial>
Core::Memory::Assets::IMaterial::bind(const std::shared_ptr<Graphics::IShader>& shader,
                                      const std::unordered_map<SGTextureType,
                                              Graphics::MarkedTexturesBlock>& markedTexturesBlocks)
{
    std::uint8_t currentTexBlockOfType = 0;

    for(const auto& markedTextureBlock : markedTexturesBlocks)
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
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind
(const std::shared_ptr<Graphics::MarkedShader>& markedShader)
{
    return bind(markedShader->m_shader, markedShader->getTexturesBlocks());
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind(
        const std::unordered_map<SGTextureType, Graphics::MarkedTexturesBlock>& markedTexturesBlocks)
{
    return bind(m_customShader, markedTexturesBlocks);
}

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::IMaterial::load(const std::string& path)
{
    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::Texture2DAsset>
Core::Memory::Assets::IMaterial::findAndAddTexture2D(const SGTextureType& textureType,
                                                     const std::string& path)
{
    auto foundTex =
            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path);

    m_textures.emplace_back(textureType, foundTex->m_texture2D);

    return foundTex;
}

void Core::Memory::Assets::IMaterial::copyTextures(const std::shared_ptr<IMaterial>& to) const noexcept
{
    // adding all textures
    for(auto& texture : m_textures)
    {
        to->m_textures.push_back(texture);
    }
}

Core::Memory::Assets::IMaterial&
Core::Memory::Assets::IMaterial::operator=(const Core::Memory::Assets::IMaterial& other) noexcept
{
    assert(this != std::addressof(other));

    m_customShader = other.m_customShader;

    other.copyTextures(shared_from_this());

    return *this;
}
