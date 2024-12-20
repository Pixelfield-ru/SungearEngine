//
// Created by stuka on 29.08.2023.
//

#include "VkCubemapTexture.h"

void SGCore::VkCubemapTexture::create()
{

}

void SGCore::VkCubemapTexture::createAsFrameBufferAttachment(const SGCore::Ref<SGCore::IFrameBuffer>& parentFrameBuffer,
                                                             SGFrameBufferAttachmentType attachmentType)
{

}

void SGCore::VkCubemapTexture::subTextureBufferDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset)
{
    ITexture2D::subTextureBufferDataOnGAPISide(bytesCount, bytesOffset);
}

void SGCore::VkCubemapTexture::subTextureDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset)
{

}

void SGCore::VkCubemapTexture::bind(const uint8_t& textureUnit) const
{

}

void SGCore::VkCubemapTexture::destroy()
{

}

void* SGCore::VkCubemapTexture::getTextureNativeHandler() const noexcept
{
    return nullptr;
}

void* SGCore::VkCubemapTexture::getTextureBufferNativeHandler() const noexcept
{
    return nullptr;
}

glm::vec3 SGCore::VkCubemapTexture::pickColor(const glm::vec2& inPosition) const noexcept
{
    return glm::vec3();
}

SGCore::VkCubemapTexture& SGCore::VkCubemapTexture::operator=(const Ref<ITexture2D>& other)
{
	return *this;
}
