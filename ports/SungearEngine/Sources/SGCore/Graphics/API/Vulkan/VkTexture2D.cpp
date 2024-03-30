//
// Created by stuka on 07.07.2023.
//

#include "VkTexture2D.h"

SGCore::VkTexture2D::~VkTexture2D() noexcept
{
    destroy();
}

void SGCore::VkTexture2D::create()
{

}

void SGCore::VkTexture2D::createAsFrameBufferAttachment(const SGCore::Ref<SGCore::IFrameBuffer>& parentFrameBuffer,
                                                        SGFrameBufferAttachmentType attachmentType)
{

}

void SGCore::VkTexture2D::subTextureDataOnGAPISide(const size_t& bytesCount, const size_t& bytesOffset) noexcept
{

}

void SGCore::VkTexture2D::destroy() noexcept
{

}

void SGCore::VkTexture2D::bind(const uint8_t& textureUnit) noexcept
{

}

SGCore::VkTexture2D& SGCore::VkTexture2D::operator=(const Ref<ITexture2D>& other)
{
    return *this;
}
