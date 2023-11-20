//
// Created by stuka on 28.08.2023.
//

#include "CubemapAsset.h"
#include "SGCore/Main/CoreMain.h"

std::shared_ptr<SGCore::IAsset> SGCore::CubemapAsset::load(const std::string& path)
{
    auto thisShared = shared_from_this();

    m_texture2D = std::shared_ptr<ICubemapTexture>(CoreMain::getRenderer().createCubemapTexture());
    std::static_pointer_cast<ICubemapTexture>(m_texture2D)
            ->create(std::static_pointer_cast<CubemapAsset>(thisShared));

    return thisShared;
}

SGCore::CubemapAsset::CubemapAsset(std::shared_ptr<Texture2DAsset> xPositive,
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

std::list<std::shared_ptr<SGCore::Texture2DAsset>> SGCore::CubemapAsset::getParts() noexcept
{
    return m_parts;
}
