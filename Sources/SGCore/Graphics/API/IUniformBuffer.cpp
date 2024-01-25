#include "IUniformBuffer.h"

#include "SGUtils/MemoryUtils.h"

SGCore::IUniformBuffer::~IUniformBuffer()
{
    SG_DELETE(m_buffer)
}

std::uint16_t SGCore::IUniformBuffer::getLayoutLocation() const noexcept
{
    return m_layoutLocation;
}

bool SGCore::IUniformBuffer::isBindPerUniform() const noexcept
{
    return false;
}
