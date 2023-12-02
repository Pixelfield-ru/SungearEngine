//
// Created by stuka on 27.07.2023.
//

#include "VkFrameBuffer.h"

std::shared_ptr<SGCore::IFrameBuffer> SGCore::VkFrameBuffer::bind()
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::VkFrameBuffer::unbind()
{
    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::VkFrameBuffer::create()
{
    return shared_from_this();
}

void SGCore::VkFrameBuffer::destroy()
{

}

std::shared_ptr<SGCore::IFrameBuffer>
SGCore::VkFrameBuffer::addAttachment(SGFrameBufferAttachmentType,
                                     SGGColorFormat format,
                                     SGGColorInternalFormat internalFormat, const int& mipLevel,
                                     const int& layer)
{
    return shared_from_this();
}

void SGCore::VkFrameBuffer::getAttachmentsCount(uint16_t& depthAttachmentsCount,
                                                        uint16_t& depthStencilAttachmentsCount,
                                                        uint16_t& colorAttachmentsCount,
                                                        uint16_t& renderAttachmentsCount) const noexcept
{

}
