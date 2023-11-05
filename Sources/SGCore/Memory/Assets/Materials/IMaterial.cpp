#include "IMaterial.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/MarkedShader.h"

std::shared_ptr<Core::Memory::Assets::IMaterial>
Core::Memory::Assets::IMaterial::bind(const std::shared_ptr<Graphics::IShader>& shader,
                                      const std::map<SGTextureType,
                                              Graphics::MarkedTexturesBlock>& markedTexturesBlocks)
{
    std::uint8_t currentTexBlockOfType = 0;

    for(const auto& markedTextureBlock : markedTexturesBlocks)
    {
        std::string texBlockTypeStr = sgTextureTypeToString(markedTextureBlock.first);

        for(const auto& texture : m_textures)
        {
            if(texture.m_type == markedTextureBlock.first)
            {
                texture.m_texture->bind(markedTextureBlock.second.m_texturesUnitOffset + currentTexBlockOfType);

                currentTexBlockOfType++;
            }
        }

        // if not in framebuffers area
        if(markedTextureBlock.first < SGTextureType::SGTP_SHADOW_MAP)
        {
            shader->useInteger(texBlockTypeStr + "Samplers_COUNT", currentTexBlockOfType);
        }

        currentTexBlockOfType = 0;
    }

    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind
(const std::shared_ptr<Graphics::MarkedShader>& markedShader)
{
    return bind(markedShader->m_shader, markedShader->getBlocks());
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::IMaterial::bind(
        const std::map<SGTextureType, Graphics::MarkedTexturesBlock>& markedTexturesBlocks)
{
    return bind(m_customShader, markedTexturesBlocks);
}

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::IMaterial::load(const std::string& path)
{
    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::Texture2DAsset>
Core::Memory::Assets::IMaterial::findAndAddTexture2D(const SGTextureType& type,
                                                     const std::string& path)
{
    auto foundTex =
            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path);

    m_textures.emplace_back(type, foundTex->m_texture2D);

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
