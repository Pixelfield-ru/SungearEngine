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
#include "SGUtils/UniqueName.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IMaterial;
    class ITexture2D;

    // todo: make read and draw bindings
    class IFrameBuffer : public UniqueNameWrapper, public std::enable_shared_from_this<IFrameBuffer>
    {
    public:
        glm::vec4 m_bgColor { 0.0, 0.0, 0.0, 1.0 };

        virtual Ref<IFrameBuffer> bindAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                                 const std::uint8_t& textureBlock) { return shared_from_this(); };

        virtual Ref<IFrameBuffer> bindAttachmentToRead(const SGFrameBufferAttachmentType& attachmentType) { return shared_from_this(); }
        virtual Ref<IFrameBuffer> bindAttachmentToDraw(const SGFrameBufferAttachmentType& attachmentType) { return shared_from_this(); }

        virtual Ref<IFrameBuffer> bindAttachmentsToRead(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) { return shared_from_this(); }
        virtual Ref<IFrameBuffer> bindAttachmentsToDraw(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) { return shared_from_this(); }
        virtual Ref<IFrameBuffer> bindAttachmentsToDraw(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes) { return shared_from_this(); }

        virtual Ref<IFrameBuffer> unbindAttachmentToRead() { return shared_from_this(); }
        virtual Ref<IFrameBuffer> unbindAttachmentToDraw() { return shared_from_this(); }

        virtual Ref<IFrameBuffer> bind() = 0;
        virtual Ref<IFrameBuffer> unbind() = 0;

        virtual Ref<IFrameBuffer> create() = 0;
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
                                                            std::uint8_t multisamplingSamplesCount) { return nullptr; };

        std::shared_ptr<IFrameBuffer> setWidth(const int& width) noexcept;
        std::shared_ptr<IFrameBuffer> setHeight(const int& height) noexcept;

        std::shared_ptr<IFrameBuffer> setSize(const int& width, const int& height) noexcept;

        int getWidth() const noexcept;
        int getHeight() const noexcept;

    protected:
        std::unordered_map<SGFrameBufferAttachmentType, Ref<ITexture2D>> m_attachments;

        int m_width = 0;
        int m_height = 0;
    };
}

#endif // SUNGEARENGINE_IFRAMEBUFFER_H
