#include "GL46FrameBuffer.h"

#include "GL46Renderer.h"
#include "RHI/GL46Device.h"
#include "SGCore/Graphics/API/ITexture2D.h"

SGCore::GL46FrameBuffer::~GL46FrameBuffer() noexcept
{
    GL4FrameBuffer::destroy();
}

SGCore::ICommandList* SGCore::GL46FrameBuffer::commandList() noexcept
{
    return GL46Renderer::getInstance()->getFrameBufferCommandList();
}

void SGCore::GL46FrameBuffer::beginPass() const noexcept
{
    auto* list = commandList();
    if(!list)
    {
        GL4FrameBuffer::bind();
        return;
    }

    RenderPassBeginDesc desc;
    desc.m_frameBuffer = const_cast<GL46FrameBuffer*>(this);
    desc.m_colorAttachments = m_drawAttachments;
    desc.m_colorLoadOp = LoadOp::SGG_LOAD;
    desc.m_depthLoadOp = LoadOp::SGG_LOAD;

    list->begin();
    list->beginRenderPass(desc);
    m_boundThroughRHI = true;
}

void SGCore::GL46FrameBuffer::bind() const
{
    beginPass();
}

void SGCore::GL46FrameBuffer::unbind() const
{
    auto* list = commandList();
    if(!list || !m_boundThroughRHI)
    {
        GL4FrameBuffer::unbind();
        m_boundThroughRHI = false;
        return;
    }
    list->endRenderPass();
    list->end();
    m_boundThroughRHI = false;
}

void SGCore::GL46FrameBuffer::bindAttachmentToDrawIn(const SGFrameBufferAttachmentType& attachmentType)
{
    bindAttachmentsToDrawIn(std::vector<SGFrameBufferAttachmentType> { attachmentType });
}

void SGCore::GL46FrameBuffer::bindAttachmentsToDrawIn(const std::vector<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    m_drawAttachments = attachmentsTypes;
    if(!commandList())
    {
        GL4FrameBuffer::bindAttachmentsToDrawIn(attachmentsTypes);
        return;
    }
    // draw buffers are part of the render pass: re-begin it with the new set (load ops keep contents)
    if(m_boundThroughRHI) beginPass();
    else GL4FrameBuffer::bindAttachmentsToDrawIn(attachmentsTypes);
}

void SGCore::GL46FrameBuffer::bindAttachmentsToDrawIn(const std::set<SGFrameBufferAttachmentType>& attachmentsTypes)
{
    bindAttachmentsToDrawIn(std::vector<SGFrameBufferAttachmentType>(attachmentsTypes.begin(), attachmentsTypes.end()));
}

void SGCore::GL46FrameBuffer::clear()
{
    for(const auto& attachment : m_attachments)
    {
        clearAttachment(attachment.first);
    }
}

void SGCore::GL46FrameBuffer::clearAttachment(const SGFrameBufferAttachmentType& attachmentType)
{
    auto* list = commandList();
    if(!list || !m_boundThroughRHI)
    {
        GL4FrameBuffer::clearAttachment(attachmentType);
        return;
    }

    if(isColorAttachment(attachmentType))
    {
        // glClearBufferfv addresses draw buffers by attachment index; same contract as the legacy path
        const auto index = static_cast<std::uint32_t>(std::to_underlying(attachmentType) -
                                                      std::to_underlying(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0));
        if(const auto attachment = getAttachment(attachmentType))
        {
            list->clearColorAttachment(index, attachment->m_clearColor);
        }
    }
    else if(isDepthAttachment(attachmentType))
    {
        list->clearDepthStencil(1.0f, false, 0);
    }
    else if(isDepthStencilAttachment(attachmentType))
    {
        list->clearDepthStencil(1.0f, true, 0);
    }
}
