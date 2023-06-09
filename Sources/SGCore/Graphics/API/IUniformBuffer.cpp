#include "IUniformBuffer.h"

#include "SGCore/Logging/Log.h"

Core::Graphics::API::IUniformBuffer::~IUniformBuffer()
{
    delete m_buffer;
}

Core::Graphics::API::IUniformBuffer::IUniformBuffer(std::list<IShaderUniform> uniforms)
{
    putUniforms(std::move(uniforms));
}

void Core::Graphics::API::IUniformBuffer::putUniforms
(std::list<IShaderUniform> uniforms) noexcept
{
    m_uniforms = std::move(uniforms);

    // first iteration - finding total size of buffer
    for(auto& uniform: m_uniforms)
    {
        m_bufferSize += getSGGDataTypeSizeInBytes(uniform.m_dataType);
    }

    // allocating buffer
    m_buffer = static_cast<char*>(malloc(m_bufferSize));

    SGCF_INFO("uniforms buffer size: " + std::to_string(m_bufferSize), SG_LOG_CURRENT_SESSION_FILE);

    m_begin.current = reinterpret_cast<UniformsDataIterator*>(m_buffer);

    m_end.current = m_begin.current;
    // second iteration. setup iterators
    for(auto& uniform : m_uniforms)
    {
        std::uint16_t typeSize = getSGGDataTypeSizeInBytes(uniform.m_dataType);
        m_begin.next = reinterpret_cast<UniformsDataIterator*>(reinterpret_cast<char*>(m_begin.current) + typeSize);

        auto* previousIter = m_begin.previous;
        m_begin.current = m_begin.next;
        m_begin.previous = previousIter;
    }

    std::swap(m_begin.current, m_end.current);
    m_current = m_begin;
}

Core::Graphics::API::UniformsDataIterator Core::Graphics::API::UniformsDataIterator::operator++(int) noexcept
{
    current = next;
    return *current;
}
