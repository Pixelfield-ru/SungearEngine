//
// Created by stuka on 24.04.2023.
//

#ifndef SUNGEARENGINE_IFRAMEBUFFER_H
#define SUNGEARENGINE_IFRAMEBUFFER_H

#include <iostream>
#include <memory>
#include <list>

#include "GraphicsDataTypes.h"
#include "IFrameBufferAttachment.h"

namespace Core::Memory::Assets
{
    struct MaterialTexturesBlock;
    class IMaterial;
}

namespace Core::Graphics
{
    struct MarkedFrameBufferAttachmentsBlock;

    // todo: make read and draw bindings
    class IFrameBuffer : public std::enable_shared_from_this<IFrameBuffer>
    {
    public:
        // type of attachment to read
        SGFrameBufferAttachmentType m_readAttachmentType = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0;
        // type of attachment to draw
        SGFrameBufferAttachmentType m_drawAttachmentType = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0;

        /**
         * Binds all frame buffer attachments based on the layout of the blocks of the material.
         * @param material - The material from which the markup will be taken.
         * @param frameBufferIndex - Framebuffer index in the material.
         * @return This.
         */
        virtual std::shared_ptr<IFrameBuffer> bindAttachments(const MarkedFrameBufferAttachmentsBlock& markedFrameBufferAttachmentsBlock)
        { return shared_from_this(); }

        virtual std::shared_ptr<IFrameBuffer> bindAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                                             const std::uint8_t& textureBlock) { return shared_from_this(); };

        virtual std::shared_ptr<IFrameBuffer> bindAttachmentToRead() { return shared_from_this(); }
        virtual std::shared_ptr<IFrameBuffer> bindAttachmentToDraw() { return shared_from_this(); }

        virtual std::shared_ptr<IFrameBuffer> unbindAttachmentToRead() { return shared_from_this(); }
        virtual std::shared_ptr<IFrameBuffer> unbindAttachmentToDraw() { return shared_from_this(); }

        virtual std::shared_ptr<IFrameBuffer> bind() = 0;
        virtual std::shared_ptr<IFrameBuffer> unbind() = 0;

        virtual std::shared_ptr<IFrameBuffer> create() = 0;
        virtual void destroy() = 0;

        virtual std::shared_ptr<IFrameBuffer> clear() { return shared_from_this(); };

        virtual std::shared_ptr<IFrameBuffer> addAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                                            const SGGColorFormat& format,
                                                            const SGGColorInternalFormat& internalFormat,
                                                            const int& mipLevel,
                                                            const int& layer) = 0;

        std::shared_ptr<IFrameBuffer> setWidth(const int& width) noexcept
        {
            m_width = width;

            return shared_from_this();
        }

        std::shared_ptr<IFrameBuffer> setHeight(const int& height) noexcept
        {
            m_height = height;

            return shared_from_this();
        }

        std::shared_ptr<IFrameBuffer> setSize(const int& width, const int& height) noexcept
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

#endif // SUNGEARENGINE_IFRAMEBUFFER_H
