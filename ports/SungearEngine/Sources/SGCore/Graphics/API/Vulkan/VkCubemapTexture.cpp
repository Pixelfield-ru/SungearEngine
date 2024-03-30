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

void SGCore::VkCubemapTexture::bind(const uint8_t& textureUnit)
{

}

void SGCore::VkCubemapTexture::destroy()
{

}

SGCore::VkCubemapTexture& SGCore::VkCubemapTexture::operator=(const Ref<ITexture2D>& other)
{
	return *this;
}
