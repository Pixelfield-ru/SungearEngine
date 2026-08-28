//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKFRAMEBUFFER_H
#define SUNGEARENGINE_VKFRAMEBUFFER_H

#include <vector>

#include "SGCore/Graphics/API/IFrameBuffer.h"

namespace SGCore
{
    class ICommandList;

    /// Legacy IFrameBuffer facade on Vulkan: a bag of VkTexture2D attachments. bind()/unbind() open
    /// and close a dynamic-rendering pass on the renderer's framebuffer command list and submit it,
    /// so legacy passes keep their bind -> draw -> unbind flow (as GL46FrameBuffer does over the RHI).
    class VkFrameBuffer : public IFrameBuffer
    {
    public:
        ~VkFrameBuffer() override;

        void bindAttachment(const SGFrameBufferAttachmentType& attachmentType, const std::uint8_t& textureBlock) override;

        void bindAttachmentToDrawIn(const SGFrameBufferAttachmentType& attachmentType) override;
        void bindAttachmentsToDrawIn(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) override;
        void bindAttachmentsToDrawIn(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes) override;

        void bind() const override;
        void unbind() const override;

        void create() override;
        void destroy() override;

        void clear() override;
        void clearAttachment(const SGFrameBufferAttachmentType& attachmentType) override;

        void addAttachment(SGFrameBufferAttachmentType attachmentType,
                           SGGColorFormat format,
                           SGGColorInternalFormat internalFormat,
                           SGGDataType dataType,
                           const int& mipLevel,
                           const int& layer) override;

        void addAttachment(SGFrameBufferAttachmentType attachmentType,
                           SGGColorFormat format,
                           SGGColorInternalFormat internalFormat,
                           SGGDataType dataType,
                           const int& mipLevel,
                           const int& layer,
                           bool useMultisampling,
                           std::uint8_t multisamplingSamplesCount) override;

        void attachAttachment(const Ref<ITexture2D>& otherAttachment) noexcept final;
        void removeAttachment(SGFrameBufferAttachmentType attachmentType) noexcept final;

        [[nodiscard]] glm::vec3 readPixelsFromAttachment(const glm::vec2& mousePos, SGFrameBufferAttachmentType attachmentType) const noexcept final;

        [[nodiscard]] std::uintptr_t getNativeHandle() const noexcept override { return reinterpret_cast<std::uintptr_t>(this); }

        [[nodiscard]] bool readAttachmentPixels(SGFrameBufferAttachmentType attachmentType, AttachmentReadback& out) const noexcept override;

    private:
        [[nodiscard]] static ICommandList* commandList() noexcept;
        void beginPass() const noexcept;
        /// Clears an attachment outside of a render pass (immediate).
        void clearAttachmentImmediate(const SGFrameBufferAttachmentType& attachmentType) const noexcept;

        std::vector<SGFrameBufferAttachmentType> m_drawAttachments;
        mutable bool m_boundThroughRHI { };
    };
}

#endif //SUNGEARENGINE_VKFRAMEBUFFER_H
