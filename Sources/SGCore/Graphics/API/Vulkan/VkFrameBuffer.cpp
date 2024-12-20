//
// Created by stuka on 27.07.2023.
//

#include "VkFrameBuffer.h"

void SGCore::VkFrameBuffer::bind() const
{

}

void SGCore::VkFrameBuffer::unbind() const
{

}

void SGCore::VkFrameBuffer::create()
{

}

void SGCore::VkFrameBuffer::destroy()
{

}

void
SGCore::VkFrameBuffer::addAttachment(SGFrameBufferAttachmentType,
                                     SGGColorFormat format,
                                     SGGColorInternalFormat internalFormat, const int& mipLevel,
                                     const int& layer)
{

}

glm::vec3 SGCore::VkFrameBuffer::readPixelsFromAttachment(const glm::vec2& mousePos,
                                                          SGFrameBufferAttachmentType attachmentType) const noexcept
{
    return glm::vec3();
}
