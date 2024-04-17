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
        
        virtual void bindAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                    const std::uint8_t& textureBlock) { };

        virtual void bindAttachmentToReadFrom(const SGFrameBufferAttachmentType& attachmentType) { }
        virtual void bindAttachmentToDrawIn(const SGFrameBufferAttachmentType& attachmentType) { }

        virtual void bindAttachmentsToReadFrom(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) { }
        virtual void bindAttachmentsToDrawIn(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) { }
        virtual void bindAttachmentsToDrawIn(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes) { }

        virtual void unbindAttachmentToReadFrom() { }
        virtual void unbindAttachmentToDrawIn() { }

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void create() = 0;
        virtual void destroy() = 0;

        virtual void clear() { };

        virtual void addAttachment(SGFrameBufferAttachmentType attachmentType,
                                                            SGGColorFormat format,
                                                            SGGColorInternalFormat internalFormat,
                                                            const int& mipLevel,
                                                            const int& layer) = 0;

        virtual void addAttachment(SGFrameBufferAttachmentType attachmentType,
                                                            SGGColorFormat format,
                                                            SGGColorInternalFormat internalFormat,
                                                            const int& mipLevel,
                                                            const int& layer,
                                                            bool useMultisampling,
                                                            std::uint8_t multisamplingSamplesCount) { };

        void setWidth(const int& width) noexcept;
        void setHeight(const int& height) noexcept;

        void setSize(const int& width, const int& height) noexcept;

        int getWidth() const noexcept;
        int getHeight() const noexcept;
        
        const auto& getAttachments() const noexcept
        {
            return m_attachments;
        }

    protected:
        std::unordered_map<SGFrameBufferAttachmentType, Ref<ITexture2D>> m_attachments;

        int m_width = 0;
        int m_height = 0;
    };
}

#endif // SUNGEARENGINE_IFRAMEBUFFER_H
