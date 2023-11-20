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
