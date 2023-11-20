//
// Created by stuka on 07.07.2023.
//

#include "VkTexture2D.h"

SGCore::VkTexture2D::~VkTexture2D() noexcept
{
    destroy();
}

void SGCore::VkTexture2D::create(std::weak_ptr<Texture2DAsset>) noexcept
{

}

void SGCore::VkTexture2D::destroy() noexcept
{

}

void SGCore::VkTexture2D::onAssetModified() noexcept
{

}

void SGCore::VkTexture2D::onAssetPathChanged() noexcept
{

}

void SGCore::VkTexture2D::onAssetDeleted() noexcept
{

}

void SGCore::VkTexture2D::onAssetRestored() noexcept
{

}

void SGCore::VkTexture2D::bind(const uint8_t& textureUnit) noexcept
{

}

SGCore::VkTexture2D& SGCore::VkTexture2D::operator=(const std::shared_ptr<ITexture2D>& other)
{
    return *this;
}
