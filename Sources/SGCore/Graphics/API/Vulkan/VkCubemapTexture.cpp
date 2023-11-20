//
// Created by stuka on 29.08.2023.
//

#include "VkCubemapTexture.h"

void SGCore::VkCubemapTexture::create(std::weak_ptr<CubemapAsset> cubemapAsset)
{

}

void SGCore::VkCubemapTexture::bind(const uint8_t& textureUnit)
{

}

void SGCore::VkCubemapTexture::destroy()
{

}

SGCore::VkCubemapTexture& SGCore::VkCubemapTexture::operator=(const std::shared_ptr<ITexture2D>& other)
{
	return *this;
}
