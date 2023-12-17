//
// Created by stuka on 24.04.2023.
//

#ifndef SUNGEARENGINE_IFRAMEBUFFER_H
#define SUNGEARENGINE_IFRAMEBUFFER_H

#include <iostream>
#include <memory>
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <initializer_list>
#include <set>
#include <unordered_map>

#include "GraphicsDataTypes.h"
#include "IFrameBufferAttachment.h"

namespace SGCore
{
    struct MaterialTexturesBlock;
    class IMaterial;

    struct MarkedFrameBufferAttachmentsBlock;

    // todo: make read and draw bindings
    class IFrameBuffer : public std::enable_shared_from_this<IFrameBuffer>
    {
    public:
        glm::vec4 m_bgColor { 0.0, 0.0, 0.0, 1.0 };

        virtual std::shared_ptr<IFrameBuffer> bindAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                                             const std::uint8_t& textureBlock) { return shared_from_this(); };

        virtual std::shared_ptr<IFrameBuffer> bindAttachmentToRead(const SGFrameBufferAttachmentType& attachmentType) { return shared_from_this(); }
        virtual std::shared_ptr<IFrameBuffer> bindAttachmentToDraw(const SGFrameBufferAttachmentType& attachmentType) { return shared_from_this(); }

        virtual std::shared_ptr<IFrameBuffer> bindAttachmentsToRead(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) { return shared_from_this(); }
        virtual std::shared_ptr<IFrameBuffer> bindAttachmentsToDraw(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) { return shared_from_this(); }
        virtual std::shared_ptr<IFrameBuffer> bindAttachmentsToDraw(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes) { return shared_from_this(); }

        virtual std::shared_ptr<IFrameBuffer> unbindAttachmentToRead() { return shared_from_this(); }
        virtual std::shared_ptr<IFrameBuffer> unbindAttachmentToDraw() { return shared_from_this(); }

        virtual std::shared_ptr<IFrameBuffer> bind() = 0;
        virtual std::shared_ptr<IFrameBuffer> unbind() = 0;

        virtual std::shared_ptr<IFrameBuffer> create() = 0;
        virtual void destroy() = 0;

        virtual std::shared_ptr<IFrameBuffer> clear() { return shared_from_this(); };

        virtual std::shared_ptr<IFrameBuffer> addAttachment(SGFrameBufferAttachmentType attachmentType,
                                                            SGGColorFormat format,
                                                            SGGColorInternalFormat internalFormat,
                                                            const int& mipLevel,
                                                            const int& layer) = 0;

        virtual std::shared_ptr<IFrameBuffer> addAttachment(SGFrameBufferAttachmentType attachmentType,
                                                            SGGColorFormat format,
                                                            SGGColorInternalFormat internalFormat,
                                                            const int& mipLevel,
                                                            const int& layer,
                                                            bool useMultisampling,
                                                            std::uint8_t multisamplingSamplesCount) { };

        std::shared_ptr<IFrameBuffer> setWidth(const int& width) noexcept;
        std::shared_ptr<IFrameBuffer> setHeight(const int& height) noexcept;

        std::shared_ptr<IFrameBuffer> setSize(const int& width, const int& height) noexcept;

        int getWidth() const noexcept;
        int getHeight() const noexcept;

        std::uint16_t getDepthAttachmentsCount() const noexcept;
        std::uint16_t getDepthStencilAttachmentsCount() const noexcept;
        std::uint16_t getColorAttachmentsCount() const noexcept;
        std::uint16_t getRenderAttachmentsCount() const noexcept;

        const auto& getDepthAttachments() const noexcept
        {
            return m_depthAttachments;
        }

        const auto& getDepthStencilAttachments() const noexcept
        {
            return m_depthStencilAttachments;
        }

        const auto& getColorAttachments() const noexcept
        {
            return m_colorAttachments;
        }

        const auto& getRenderAttachments() const noexcept
        {
            return m_renderAttachments;
        }

    protected:
         std::unordered_map<SGFrameBufferAttachmentType, IFrameBufferAttachment> m_depthAttachments;
         std::unordered_map<SGFrameBufferAttachmentType, IFrameBufferAttachment> m_depthStencilAttachments;
         std::unordered_map<SGFrameBufferAttachmentType, IFrameBufferAttachment> m_colorAttachments;
         std::unordered_map<SGFrameBufferAttachmentType, IFrameBufferAttachment> m_renderAttachments;

        int m_width = 0;
        int m_height = 0;
    };
}

#endif // SUNGEARENGINE_IFRAMEBUFFER_H
