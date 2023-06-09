//
// Created by stuka on 21.05.2023.
//

#include <iostream>

#include "GLIndexBuffer.h"
#include "GLGraphicsTypesCaster.h"
#include "GL46/GL46Renderer.h"

Core::Graphics::API::GL::GLIndexBuffer::~GLIndexBuffer() noexcept
{
    destroy();
}

std::shared_ptr<Core::Graphics::API::IIndexBuffer> Core::Graphics::API::GL::GLIndexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

void Core::Graphics::API::GL::GLIndexBuffer::destroy() noexcept
{
    glDeleteBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif
}

std::shared_ptr<Core::Graphics::API::IIndexBuffer> Core::Graphics::API::GL::GLIndexBuffer::putData(std::vector<int> data) noexcept
{
    m_data = std::move(data);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (m_data.size() * sizeof(GLsizeiptr)), (const void*) &m_data[0],
                 GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::API::IIndexBuffer> Core::Graphics::API::GL::GLIndexBuffer::subData
        (std::vector<int> data, const int& offset) noexcept
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, (GLsizeiptr) (m_data.size() * sizeof(GLsizeiptr)),
                    (const void*) &m_data[0]);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::API::IIndexBuffer> Core::Graphics::API::GL::GLIndexBuffer::bind() noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handler);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::API::IIndexBuffer> Core::Graphics::API::GL::GLIndexBuffer::setUsage(SGGUsage usage) noexcept
{
    m_usage = usage;

    return shared_from_this();
}