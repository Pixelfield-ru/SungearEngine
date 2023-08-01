//
// Created by stuka on 27.07.2023.
//

#include "VkFrameBuffer.h"

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::VkFrameBuffer::bind()
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::VkFrameBuffer::unbind()
{
    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::VkFrameBuffer::create()
{
    return shared_from_this();
}

void Core::Graphics::VkFrameBuffer::destroy()
{

}

std::shared_ptr<Core::Graphics::IFrameBuffer>
Core::Graphics::VkFrameBuffer::addAttachment(const SGFrameBufferAttachmentType&, const std::string& name,
                                             const SGGColorFormat& format,
                                             const SGGColorInternalFormat& internalFormat, const int& mipLevel,
                                             const int& layer)
{
    return shared_from_this();
}
