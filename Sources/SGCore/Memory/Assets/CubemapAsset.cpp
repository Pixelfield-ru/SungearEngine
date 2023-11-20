//
// Created by stuka on 28.08.2023.
//

#include "CubemapAsset.h"
#include "SGCore/Main/CoreMain.h"

SGCore::Ref<SGCore::IAsset> SGCore::CubemapAsset::load(const std::string& path)
{
    auto thisShared = shared_from_this();

    m_texture2D = Ref<ICubemapTexture>(CoreMain::getRenderer().createCubemapTexture());
    std::static_pointer_cast<ICubemapTexture>(m_texture2D)
            ->create(std::static_pointer_cast<CubemapAsset>(thisShared));

    return thisShared;
}

SGCore::CubemapAsset::CubemapAsset(Ref<Texture2DAsset> xPositive,
                                                 Ref<Texture2DAsset> xNegative,
                                                 Ref<Texture2DAsset> yPositive,
                                                 Ref<Texture2DAsset> yNegative,
                                                 Ref<Texture2DAsset> zPositive,
                                                 Ref<Texture2DAsset> zNegative)
{
    m_parts.push_back(xPositive);
    m_parts.push_back(xNegative);

    m_parts.push_back(yPositive);
    m_parts.push_back(yNegative);

    m_parts.push_back(zPositive);
    m_parts.push_back(zNegative);
}

std::list<SGCore::Ref<SGCore::Texture2DAsset>> SGCore::CubemapAsset::getParts() noexcept
{
    return m_parts;
}
