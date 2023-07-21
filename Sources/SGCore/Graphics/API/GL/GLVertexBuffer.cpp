//
// Created by stuka on 23.05.2023.
//

#include "GLVertexBuffer.h"
#include "GLGraphicsTypesCaster.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"

Core::Graphics::GLVertexBuffer::~GLVertexBuffer() noexcept
{
    destroy();
}

std::shared_ptr<Core::Graphics::IVertexBuffer> Core::Graphics::GLVertexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

void Core::Graphics::GLVertexBuffer::destroy() noexcept
{
    glDeleteBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    //GL46::GL4Renderer::getInstance()->checkForErrors();
    #endif
}

std::shared_ptr<Core::Graphics::IVertexBuffer> Core::Graphics::GLVertexBuffer::putData
(std::vector<float> data) noexcept
{
    m_data = std::move(data);

    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (m_data.size() * sizeof(m_data[0])), &m_data[0],
                 GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IVertexBuffer> Core::Graphics::GLVertexBuffer::subData
(std::vector<float> data, const int& offset) noexcept
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, (GLsizeiptr) (m_data.size() * sizeof(m_data[0])),
                    &m_data[0]);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IVertexBuffer> Core::Graphics::GLVertexBuffer::bind() noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handler);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IVertexBuffer> Core::Graphics::GLVertexBuffer::setUsage(SGGUsage usage) noexcept
{
    m_usage = usage;

    return shared_from_this();
}
