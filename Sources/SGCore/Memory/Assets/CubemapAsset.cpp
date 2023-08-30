//
// Created by stuka on 28.08.2023.
//

#include "CubemapAsset.h"
#include "SGCore/Main/CoreMain.h"

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::CubemapAsset::load(const std::string& path)
{
    auto thisShared = shared_from_this();

    m_cubemapTexture = std::shared_ptr<Graphics::ICubemapTexture>(Core::Main::CoreMain::getRenderer().createCubemapTexture());
    m_cubemapTexture->create(std::static_pointer_cast<CubemapAsset>(thisShared));

    m_texture2D = m_cubemapTexture;

    return thisShared;
}

Core::Memory::Assets::CubemapAsset::CubemapAsset(std::shared_ptr<Texture2DAsset> xPositive,
                                                 std::shared_ptr<Texture2DAsset> xNegative,
                                                 std::shared_ptr<Texture2DAsset> yPositive,
                                                 std::shared_ptr<Texture2DAsset> yNegative,
                                                 std::shared_ptr<Texture2DAsset> zPositive,
                                                 std::shared_ptr<Texture2DAsset> zNegative)
{
    m_parts.push_back(xPositive);
    m_parts.push_back(xNegative);

    m_parts.push_back(yPositive);
    m_parts.push_back(yNegative);

    m_parts.push_back(zPositive);
    m_parts.push_back(zNegative);
}

std::list<std::shared_ptr<Core::Memory::Assets::Texture2DAsset>> Core::Memory::Assets::CubemapAsset::getParts() noexcept
{
    return m_parts;
}
