#include "ITexture2D.h"

#pragma region Getters and setters

std::weak_ptr<Core::Memory::Assets::Texture2DAsset> Core::Graphics::API::ITexture2D::getAsset() noexcept
{
    return m_texture2DAsset;
}

#pragma endregion

