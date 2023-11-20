#include "ITexture2D.h"

#pragma region Getters and setters

std::weak_ptr<SGCore::Texture2DAsset> SGCore::ITexture2D::getAsset() noexcept
{
    return m_texture2DAsset;
}

#pragma endregion

