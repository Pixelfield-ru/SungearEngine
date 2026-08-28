//
// Created by stuka on 24.04.2023.
//

#ifndef SUNGEARENGINE_IFRAMEBUFFER_H
#define SUNGEARENGINE_IFRAMEBUFFER_H

#include <cstdint>
#include <set>
#include <unordered_map>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "AttachmentReadback.h"
#include "GraphicsDataTypes.h"
#include "SGCore/Utils/Unique/UniqueName.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IMaterial;
    class ITexture2D;


    // todo: make read and draw bindings
    class SGCORE_EXPORT IFrameBuffer : public UniqueNameWrapper, public std::enable_shared_from_this<IFrameBuffer>
    {
    public:
        virtual ~IFrameBuffer() = default;

        glm::vec4 m_bgColor { 0.0, 0.0, 0.0, 1.0 };

        int m_viewportWidth = 0;
        int m_viewportHeight = 0;
        int m_viewportPosX = 0;
        int m_viewportPosY = 0;

        virtual void bindAttachment(const SGFrameBufferAttachmentType& attachmentType,
                                    const std::uint8_t& textureBlock) { };

        virtual void bindAttachmentToReadFrom(const SGFrameBufferAttachmentType& attachmentType) { }
        virtual void bindAttachmentToDrawIn(const SGFrameBufferAttachmentType& attachmentType) { }

        virtual void bindAttachmentsToReadFrom(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) { }
        virtual void bindAttachmentsToDrawIn(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) { }
        virtual void bindAttachmentsToDrawIn(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes) { }

        virtual void unbindAttachmentToReadFrom() { }
        virtual void unbindAttachmentToDrawIn() { }

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void useStates() const noexcept { };

        virtual void create() = 0;
        virtual void destroy() = 0;

        virtual void clear() { };

        /**
         * CALL THIS ONLY AFTER BINDING DRAW ATTACHMENT WITH TYPE \p attachmentType !!!
         * @param attachmentType
         */
        virtual void clearAttachment(const SGFrameBufferAttachmentType& attachmentType) { };
        
        virtual void addAttachment(SGFrameBufferAttachmentType attachmentType,
                                   SGGColorFormat format,
                                   SGGColorInternalFormat internalFormat,
                                   SGGDataType dataType,
                                   const int& mipLevel,
                                   const int& layer) = 0;
        
        virtual void addAttachment(SGFrameBufferAttachmentType attachmentType,
                                   SGGColorFormat format,
                                   SGGColorInternalFormat internalFormat,
                                   SGGDataType dataType,
                                   const int& mipLevel,
                                   const int& layer,
                                   bool useMultisampling,
                                   std::uint8_t multisamplingSamplesCount)
        {};

        virtual void attachAttachment(const Ref<ITexture2D>& otherAttachment) noexcept = 0;

        virtual void removeAttachment(SGFrameBufferAttachmentType attachmentType) noexcept = 0;

        bool hasAttachment(SGFrameBufferAttachmentType attachmentType) const noexcept;

        void setWidth(const int& width) noexcept;
        void setHeight(const int& height) noexcept;

        void setSize(const int& width, const int& height) noexcept;

        int getWidth() const noexcept;
        int getHeight() const noexcept;

        [[nodiscard]] virtual glm::vec3 readPixelsFromAttachment(const glm::vec2& mousePos, SGFrameBufferAttachmentType attachmentType) const noexcept = 0;

        /// Backend object handle (GL: framebuffer name; 0 = default framebuffer).
        [[nodiscard]] virtual std::uintptr_t getNativeHandle() const noexcept { return 0; }

        /**
         * Reads the whole color attachment back to CPU in the attachment's own format
         * (m_format / m_dataType of the attachment texture), tightly packed, rows ordered the way
         * the current graphics API stores them (bottom row first for OpenGL). Callers convert to
         * whatever they need using \p out.m_format / \p out.m_dataType.
         * Intended for tests and screenshots, not for per-frame use.
         * @return false if the backend does not support readback or the attachment is not a color attachment.
         */
        [[nodiscard]] virtual bool readAttachmentPixels(SGFrameBufferAttachmentType attachmentType,
                                                        AttachmentReadback& out) const noexcept
        {
            return false;
        }

        const auto& getAttachments() const noexcept
        {
            return m_attachments;
        }
        
        Ref<ITexture2D> getAttachment(SGFrameBufferAttachmentType attachmentType) const noexcept;

    protected:
        std::unordered_map<SGFrameBufferAttachmentType, Ref<ITexture2D>> m_attachments;

        int m_width = 0;
        int m_height = 0;
    };
}

#endif // SUNGEARENGINE_IFRAMEBUFFER_H
