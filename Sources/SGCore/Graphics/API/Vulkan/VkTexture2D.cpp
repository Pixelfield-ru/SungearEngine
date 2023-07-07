//
// Created by stuka on 07.07.2023.
//

#include "VkTexture2D.h"

Core::Graphics::VkTexture2D::~VkTexture2D() noexcept
{
    destroy();
}

void Core::Graphics::VkTexture2D::create(std::weak_ptr<Memory::Assets::Texture2DAsset>) noexcept
{

}

void Core::Graphics::VkTexture2D::destroy() noexcept
{

}

void Core::Graphics::VkTexture2D::onAssetModified() noexcept
{

}

void Core::Graphics::VkTexture2D::onAssetPathChanged() noexcept
{

}

void Core::Graphics::VkTexture2D::onAssetDeleted() noexcept
{

}

void Core::Graphics::VkTexture2D::onAssetRestored() noexcept
{

}

void Core::Graphics::VkTexture2D::bind(const uint8_t& textureUnit) noexcept
{

}

Core::Graphics::VkTexture2D& Core::Graphics::VkTexture2D::operator=(const std::shared_ptr<ITexture2D>& other)
{

}
