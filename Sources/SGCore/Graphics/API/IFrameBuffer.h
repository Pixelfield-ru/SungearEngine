//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_IFRAMEBUFFER_H
#define NATIVECORE_IFRAMEBUFFER_H

#include <iostream>
#include <memory>
#include <list>

#include "GraphicsDataTypes.h"
#include "IFrameBufferAttachment.h"

namespace Core::Graphics
{
    class IFrameBuffer : public std::enable_shared_from_this<IFrameBuffer>
    {
    public:
        virtual std::shared_ptr<IFrameBuffer> bind() = 0;
        virtual std::shared_ptr<IFrameBuffer> unbind() = 0;

        virtual std::shared_ptr<IFrameBuffer> create() = 0;
        virtual void destroy() = 0;

        virtual std::shared_ptr<IFrameBuffer> addAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                                            const std::string& name,
                                                            const int& width, const int& height,
                                                            const SGGColorFormat& format,
                                                            const SGGColorInternalFormat& internalFormat,
                                                            const int& mipLevel,
                                                            const int& layer) = 0;
    };
}

#endif //NATIVECORE_IFRAMEBUFFER_H
