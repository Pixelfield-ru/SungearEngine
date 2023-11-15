#include "IFrameBuffer.h"

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::IFrameBuffer::setWidth(const int& width) noexcept
{
    m_width = width;

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::IFrameBuffer::setHeight(const int& height) noexcept
{
    m_height = height;

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IFrameBuffer> Core::Graphics::IFrameBuffer::setSize(const int& width, const int& height) noexcept
{
    m_width = width;
    m_height = height;

    return shared_from_this();
}

int Core::Graphics::IFrameBuffer::getWidth() const noexcept
{
    return m_width;
}

int Core::Graphics::IFrameBuffer::getHeight() const noexcept
{
    return m_height;
}
