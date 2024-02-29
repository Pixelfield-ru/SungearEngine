//
// Created by stuka on 23.05.2023.
//

#include "GLVertexBuffer.h"
#include "GLGraphicsTypesCaster.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"

SGCore::GLVertexBuffer::~GLVertexBuffer() noexcept
{
    destroy();
}

std::shared_ptr<SGCore::IVertexBuffer> SGCore::GLVertexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

void SGCore::GLVertexBuffer::destroy() noexcept
{
    glDeleteBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    //GL46::GL4Renderer::getInstance()->checkForErrors();
    #endif
}

void SGCore::GLVertexBuffer::subDataOnGAPISide(const void* data, const size_t& bytesCount,
                                               const size_t& bytesOffset,
                                               bool isPutData) noexcept
{
    if(isPutData)
    {
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) bytesCount, data,
                     GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, (GLsizeiptr) bytesOffset, (GLsizeiptr) bytesCount, data);
    }
    
    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif
}

std::shared_ptr<SGCore::IVertexBuffer> SGCore::GLVertexBuffer::bind() noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handler);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<SGCore::IVertexBuffer> SGCore::GLVertexBuffer::setUsage(SGGUsage usage) noexcept
{
    m_usage = usage;

    return shared_from_this();
}
