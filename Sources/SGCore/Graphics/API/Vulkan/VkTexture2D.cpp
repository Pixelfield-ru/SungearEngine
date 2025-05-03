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

void SGCore::VkTexture2D::subTextureDataOnGAPISide(std::size_t areaWidth, std::size_t areaHeight, std::size_t areaOffsetX, std::size_t areaOffsetY, int dataTypeSize) noexcept
{

}

void SGCore::VkTexture2D::destroy() noexcept
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

glm::vec3 SGCore::VkTexture2D::pickColor(const glm::vec2& inPosition) const noexcept
{
    return glm::vec3();
}
