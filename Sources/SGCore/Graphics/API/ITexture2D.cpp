#include "ITexture2D.h"

#pragma region Getters and setters

std::shared_ptr<Core::Graphics::API::ITexture2D> Core::Graphics::API::ITexture2D::setUnit(const uint8_t& unit)
{
    m_unit = unit;

    return shared_from_this();
}

#pragma endregion

std::uint8_t Core::Graphics::API::ITexture2D::getUnit() noexcept
{
    return m_unit;
}

std::weak_ptr<Core::Memory::Assets::Texture2DAsset> Core::Graphics::API::ITexture2D::getAsset() noexcept
{
    return m_texture2DAsset;
}

