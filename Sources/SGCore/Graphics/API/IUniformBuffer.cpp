#include "IUniformBuffer.h"

#include "SGCore/Logging/Log.h"
#include "SGCore/Memory/Utils.h"

Core::Graphics::API::IUniformBuffer::~IUniformBuffer()
{
    SG_DELETE(m_buffer);
}

Core::Graphics::API::IUniformBuffer::IUniformBuffer(const std::list<IShaderUniform>& uniforms)
{
    putUniforms(uniforms);
}

void Core::Graphics::API::IUniformBuffer::putUniforms
(const std::list<IShaderUniform>& uniforms) noexcept
{
    m_uniforms.insert(m_uniforms.end(), uniforms.begin(), uniforms.end());

    std::uint64_t lastBufferSize = m_bufferSize;
    m_bufferSize = 0;
    // first iteration - finding total size of buffer
    for(auto& uniform: m_uniforms)
    {
        m_bufferSize += getSGGDataTypeAlignedSizeInBytes(uniform.m_dataType);
    }

    //TODO: (possible) to add more memory allocation so as not to allocate frequently. allow only when there is not enough space

    // if buffer is not allocated (first put)
    if(!m_buffer)
    {
        // allocating buffer
        m_buffer = static_cast<char*>(malloc(m_bufferSize));

        char* curPtr = m_buffer;
        for(auto& uniform : m_uniforms)
        {
            m_bufferLayout.push_back(curPtr);
            curPtr += getSGGDataTypeAlignedSizeInBytes(uniform.m_dataType);
        }

        m_currentLayoutPosition = m_bufferLayout.begin();
    }
    else // if buffer already allocated (second etc. allocations)
    {
        char* newBuffer = static_cast<char*>(malloc(m_bufferSize));
        // copy last buffer to new buffer
        memcpy(newBuffer, m_buffer, lastBufferSize);
        // delete last buffer
        delete m_buffer;
        m_buffer = newBuffer;

        m_bufferLayout.clear();

        // updating buffer layout
        char* curPtr = m_buffer;
        for(auto & m_uniform : m_uniforms)
        {
            m_bufferLayout.push_back(curPtr);
            curPtr += getSGGDataTypeAlignedSizeInBytes(m_uniform.m_dataType);
        }

        // setting layout position to new buffer layout begin
        m_currentLayoutPosition = m_bufferLayout.begin();

        // and then just increment m_currentLayoutPosition until we reach the last maximum filled uniforms in the buffer
        for(int i = 0; i < m_filledUniformsNum; i++, m_currentLayoutPosition++) { }
    }

    SGCF_INFO("uniforms buffer size: " + std::to_string(m_bufferSize), SG_LOG_CURRENT_SESSION_FILE);
}

std::uint16_t Core::Graphics::API::IUniformBuffer::getLayoutLocation() const noexcept
{
    return m_layoutLocation;
}
