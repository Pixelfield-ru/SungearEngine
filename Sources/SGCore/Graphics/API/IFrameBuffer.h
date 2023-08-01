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
    // todo: make read and draw bindings
    class IFrameBuffer : public std::enable_shared_from_this<IFrameBuffer>
    {
    public:
        // name of attachment to read
        std::string m_readAttachmentName;
        // name of attachment to draw
        std::string m_drawAttachmentName;

        virtual std::shared_ptr<IFrameBuffer> bindAttachment(const std::string& attachmentName,
                                                             const std::uint8_t& textureBlock) { };

        virtual std::shared_ptr<IFrameBuffer> bindAttachmentToRead() { }
        virtual std::shared_ptr<IFrameBuffer> bindAttachmentToDraw() { }

        virtual std::shared_ptr<IFrameBuffer> unbindAttachmentToRead() { }
        virtual std::shared_ptr<IFrameBuffer> unbindAttachmentToDraw() { }

        virtual std::shared_ptr<IFrameBuffer> bind() = 0;
        virtual std::shared_ptr<IFrameBuffer> unbind() = 0;

        virtual std::shared_ptr<IFrameBuffer> create() = 0;
        virtual void destroy() = 0;

        virtual std::shared_ptr<IFrameBuffer> clear() { };

        virtual std::shared_ptr<IFrameBuffer> addAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                                            const std::string& name,
                                                            const SGGColorFormat& format,
                                                            const SGGColorInternalFormat& internalFormat,
                                                            const int& mipLevel,
                                                            const int& layer) = 0;

        std::shared_ptr<IFrameBuffer> setWidth(int&& width) noexcept
        {
            m_width = width;

            return shared_from_this();
        }

        std::shared_ptr<IFrameBuffer> setHeight(int&& height) noexcept
        {
            m_height = height;

            return shared_from_this();
        }

        std::shared_ptr<IFrameBuffer> setSize(int&& width, int&& height) noexcept
        {
            m_width = width;
            m_height = height;

            return shared_from_this();
        }

        int getWidth() const noexcept
        {
            return m_width;
        }

        int getHeight() const noexcept
        {
            return m_height;
        }

    protected:
        int m_width = 0;
        int m_height = 0;
    };
}

#endif //NATIVECORE_IFRAMEBUFFER_H
