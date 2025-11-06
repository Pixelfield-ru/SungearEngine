//
// Created by stuka on 07.07.2023.
//

#include "VkTexture2D.h"

SGCore::VkTexture2D::~VkTexture2D() noexcept
{
    destroyOnGPU();
}

void SGCore::VkTexture2D::create()
{

}

void SGCore::VkTexture2D::createAsFrameBufferAttachment(IFrameBuffer* parentFrameBuffer,
                                                        SGFrameBufferAttachmentType attachmentType)
{

}

void SGCore::VkTexture2D::subTextureDataOnGAPISide(const std::uint8_t* data, std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY) noexcept
{

}

void SGCore::VkTexture2D::destroyOnGPU() noexcept
{

}

void SGCore::VkTexture2D::bind(const uint8_t& textureUnit) const noexcept
{

}

void* SGCore::VkTexture2D::getTextureNativeHandler() const noexcept
{
    return nullptr;
}

void* SGCore::VkTexture2D::getTextureBufferNativeHandler() const noexcept
{
    return nullptr;
}

SGCore::VkTexture2D& SGCore::VkTexture2D::operator=(const Ref<ITexture2D>& other)
{
    return *this;
}
