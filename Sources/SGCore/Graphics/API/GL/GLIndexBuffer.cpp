//
// Created by stuka on 21.05.2023.
//

#include "GLIndexBuffer.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"

#include <iostream>

Core::Graphics::API::GL::GLIndexBuffer::~GLIndexBuffer() noexcept
{
    destroy();
}

Core::Graphics::API::GL::GLIndexBuffer* Core::Graphics::API::GL::GLIndexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return this;
}

void Core::Graphics::API::GL::GLIndexBuffer::destroy() noexcept
{
    #ifdef SUNGEAR_DEBUG
    std::cout << "index buffer destroyed" << std::endl;
    #endif

    glDeleteBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif
}

Core::Graphics::API::GL::GLIndexBuffer* Core::Graphics::API::GL::GLIndexBuffer::putData(std::vector<int> data) noexcept
{
    m_data = std::move(data);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (m_data.size() * sizeof(GLsizeiptr)), (const void*) &m_data[0],
                 castSGGBufferUsageToOGL(m_usage));

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return this;
}

Core::Graphics::API::GL::GLIndexBuffer* Core::Graphics::API::GL::GLIndexBuffer::subData
        (std::vector<int> data, const int& offset) noexcept
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, (GLsizeiptr) (m_data.size() * sizeof(GLsizeiptr)),
                    (const void*) &m_data[0]);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return this;
}

Core::Graphics::API::GL::GLIndexBuffer* Core::Graphics::API::GL::GLIndexBuffer::bind() noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handler);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return this;
}

GLenum Core::Graphics::API::GL::GLIndexBuffer::castSGGBufferUsageToOGL(const SGGBufferUsage& sggBufferUsage) noexcept
{
    GLenum usage = GL_STATIC_DRAW;

    switch(sggBufferUsage)
    {
        case SGG_BUFFER_USAGE_DYNAMIC: usage = GL_DYNAMIC_DRAW; break;
        case SGG_BUFFER_USAGE_STATIC: usage = GL_STATIC_DRAW; break;
    }

    return usage;
}


Core::Graphics::API::GL::GLIndexBuffer* Core::Graphics::API::GL::GLIndexBuffer::setUsage(SGGBufferUsage usage) noexcept
{
    m_usage = usage;

    return this;
}