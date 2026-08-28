//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GL46FRAMEBUFFER_H
#define SUNGEARENGINE_GL46FRAMEBUFFER_H

#include <vector>

#include "SGCore/Graphics/API/GL/GL4/GL4FrameBuffer.h"
#include "SGCore/Graphics/RHI/ICommandList.h"

namespace SGCore
{
    class ICommandList;

    /**
     * GL46 framebuffer: the legacy IFrameBuffer calls the passes make (bind / draw-buffer
     * selection / clear / unbind) are executed as RHI render-pass operations —
     * ICommandList::beginRenderPass / clear* / endRenderPass — so the RHI owns render targets
     * on GL46 while the passes are still written against the legacy interface. Changing the
     * draw buffers while bound re-begins the pass with load ops (the shape Vulkan requires).
     */
    class GL46FrameBuffer : public GL4FrameBuffer
    {
        friend class GL46Renderer;

    public:
        ~GL46FrameBuffer() noexcept override;

        void bind() const override;
        void unbind() const override;

        void bindAttachmentToDrawIn(const SGFrameBufferAttachmentType& attachmentType) override;
        void bindAttachmentsToDrawIn(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes) override;
        void bindAttachmentsToDrawIn(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes) override;

        void clear() override;
        void clearAttachment(const SGFrameBufferAttachmentType& attachmentType) override;

    private:
        /// Draw buffers requested by the pass; part of the render pass description.
        mutable std::vector<SGFrameBufferAttachmentType> m_drawAttachments;
        mutable bool m_boundThroughRHI { };

        [[nodiscard]] static ICommandList* commandList() noexcept;
        void beginPass() const noexcept;
    };
}

#endif //SUNGEARENGINE_GL46FRAMEBUFFER_H
