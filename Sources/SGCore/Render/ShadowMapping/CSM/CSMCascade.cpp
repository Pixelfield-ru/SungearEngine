//
// Created by stuka on 10.07.2025.
//

#include "CSMCascade.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"

void SGCore::CSMCascade::createFrameBuffer() noexcept
{
    m_frameBuffer = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer());
    m_frameBuffer->setSize(m_frameBufferSize.x, m_frameBufferSize.y);
    m_frameBuffer->create();
    m_frameBuffer->bind();
    m_frameBuffer->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                 SGGColorFormat::SGG_DEPTH_COMPONENT,
                                 SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F,
                                 0,
                                 0);
}
