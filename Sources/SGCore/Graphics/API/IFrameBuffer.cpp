#include "IFrameBuffer.h"

void SGCore::IFrameBuffer::setWidth(const int& width) noexcept
{
    m_width = width;
}

void SGCore::IFrameBuffer::setHeight(const int& height) noexcept
{
    m_height = height;
}

void SGCore::IFrameBuffer::setSize(const int& width, const int& height) noexcept
{
    m_width = width;
    m_height = height;
}

int SGCore::IFrameBuffer::getWidth() const noexcept
{
    return m_width;
}

int SGCore::IFrameBuffer::getHeight() const noexcept
{
    return m_height;
}
