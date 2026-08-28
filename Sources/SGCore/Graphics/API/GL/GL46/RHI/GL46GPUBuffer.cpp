//
// Created by 8bitniksis on 17.08.2026.
//

#include "GL46GPUBuffer.h"

#include "SGCore/Logger/Logger.h"

SGCore::GL46GPUBuffer::GL46GPUBuffer(const GPUBufferDesc& desc) noexcept
{
    m_desc = desc;
    m_debugName = desc.m_debugName;

    glCreateBuffers(1, &m_handle);

    // immutable storage: host-visible buffers get persistent-mappable write access,
    // device-local ones are only touched through uploads
    const GLbitfield flags = desc.m_access == GPUMemoryAccess::SGG_HOST_VISIBLE
                             ? GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT
                             : GL_DYNAMIC_STORAGE_BIT;
    glNamedBufferStorage(m_handle, static_cast<GLsizeiptr>(desc.m_size), nullptr, flags);

    if(!m_debugName.empty())
    {
        glObjectLabel(GL_BUFFER, m_handle, static_cast<GLsizei>(m_debugName.size()), m_debugName.c_str());
    }
}

SGCore::GL46GPUBuffer::~GL46GPUBuffer()
{
    if(m_mapped) glUnmapNamedBuffer(m_handle);
    if(m_owned) glDeleteBuffers(1, &m_handle);
}

void* SGCore::GL46GPUBuffer::map(std::uint64_t offset, std::uint64_t size) noexcept
{
    if(m_desc.m_access != GPUMemoryAccess::SGG_HOST_VISIBLE)
    {
        SG_LOG_E("GL46GPUBuffer '{}': map() on a device-local buffer; use ICommandList::uploadData.", m_debugName);
        return nullptr;
    }
    if(m_mapped) return nullptr;

    void* pointer = glMapNamedBufferRange(m_handle, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size),
                                          GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
    m_mapped = pointer != nullptr;
    return pointer;
}

void SGCore::GL46GPUBuffer::unmap() noexcept
{
    if(!m_mapped) return;
    glUnmapNamedBuffer(m_handle);
    m_mapped = false;
}

bool SGCore::GL46GPUBuffer::write(const void* data, std::uint64_t size, std::uint64_t offset) noexcept
{
    if(offset + size > m_desc.m_size)
    {
        SG_LOG_E("GL46GPUBuffer '{}': write of {} bytes at {} exceeds size {}.", m_debugName, size, offset, m_desc.m_size);
        return false;
    }
    glNamedBufferSubData(m_handle, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
    return true;
}

SGCore::GL46GPUBuffer::GL46GPUBuffer(GLuint adoptedHandle, std::uint64_t size, std::string debugName) noexcept
{
    m_handle = adoptedHandle;
    m_owned = false;
    m_desc.m_size = size;
    m_desc.m_access = GPUMemoryAccess::SGG_DEVICE_LOCAL;
    m_desc.m_debugName = std::move(debugName);
    m_debugName = m_desc.m_debugName;
}
