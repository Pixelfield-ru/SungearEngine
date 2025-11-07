//
// Created by stuka on 23.05.2023.
//

#include "GLVertexBuffer.h"
#include "GLGraphicsTypesCaster.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"

SGCore::GLVertexBuffer::~GLVertexBuffer() noexcept
{
    GLVertexBuffer::destroy();
}

void SGCore::GLVertexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);
}

void SGCore::GLVertexBuffer::create(const size_t& byteSize) noexcept
{
    destroy();

    m_data.resize(byteSize);

    glGenBuffers(1, &m_handler);
    bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLuint64>(byteSize), nullptr,
                 GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));
}

void SGCore::GLVertexBuffer::destroy() noexcept
{
    glDeleteBuffers(1, &m_handler);
}

void SGCore::GLVertexBuffer::subDataOnGAPISide(const void* data, const size_t& bytesCount,
                                               const size_t& bytesOffset,
                                               bool isPutData) noexcept
{
    if(bytesCount == 0) return;

    if(isPutData)
    {
        glBufferData(GL_ARRAY_BUFFER, (GLuint64) bytesCount, data,
                     GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, (GLuint64) bytesOffset, (GLuint64) bytesCount, data);
    }
}

void SGCore::GLVertexBuffer::bind() noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handler);
}

void SGCore::GLVertexBuffer::setUsage(SGGUsage usage) noexcept
{
    m_usage = usage;
}
