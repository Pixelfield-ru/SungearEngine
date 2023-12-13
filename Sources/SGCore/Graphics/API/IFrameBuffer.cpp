#include "IFrameBuffer.h"

std::shared_ptr<SGCore::IFrameBuffer> SGCore::IFrameBuffer::setWidth(const int& width) noexcept
{
    m_width = width;

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::IFrameBuffer::setHeight(const int& height) noexcept
{
    m_height = height;

    return shared_from_this();
}

std::shared_ptr<SGCore::IFrameBuffer> SGCore::IFrameBuffer::setSize(const int& width, const int& height) noexcept
{
    m_width = width;
    m_height = height;

    return shared_from_this();
}

int SGCore::IFrameBuffer::getWidth() const noexcept
{
    return m_width;
}

int SGCore::IFrameBuffer::getHeight() const noexcept
{
    return m_height;
}

void SGCore::IFrameBuffer::getAttachmentsCount(uint16_t& depthAttachmentsCount,
                                               uint16_t& depthStencilAttachmentsCount,
                                               uint16_t& colorAttachmentsCount,
                                               uint16_t& renderAttachmentsCount) const noexcept
{
    depthAttachmentsCount = m_depthAttachmentsCount;
    depthStencilAttachmentsCount = m_depthStencilAttachmentsCount;
    colorAttachmentsCount = m_colorAttachmentsCount;
    renderAttachmentsCount = m_renderAttachmentsCount;
}

std::uint16_t SGCore::IFrameBuffer::getDepthAttachmentsCount() const noexcept
{
    return m_depthAttachmentsCount;
}

std::uint16_t SGCore::IFrameBuffer::getDepthStencilAttachmentsCount() const noexcept
{
    return m_depthStencilAttachmentsCount;
}

std::uint16_t SGCore::IFrameBuffer::getColorAttachmentsCount() const noexcept
{
    return m_colorAttachmentsCount;
}

std::uint16_t SGCore::IFrameBuffer::getRenderAttachmentsCount() const noexcept
{
    return m_renderAttachmentsCount;
}
