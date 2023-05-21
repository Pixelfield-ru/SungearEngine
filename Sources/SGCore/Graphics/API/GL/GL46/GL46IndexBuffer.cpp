//
// Created by stuka on 21.05.2023.
//

#include "GL46IndexBuffer.h"
#include "GL46Renderer.h"

#include <iostream>

Core::Graphics::API::GL::GL46::GL46IndexBuffer::~GL46IndexBuffer() noexcept
{
    destroy();
}

Core::Graphics::API::GL::GL46::GL46IndexBuffer* Core::Graphics::API::GL::GL46::GL46IndexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);

    GL46Renderer::getInstance()->checkForErrors();

    return this;
}

Core::Graphics::API::GL::GL46::GL46IndexBuffer* Core::Graphics::API::GL::GL46::GL46IndexBuffer::putData(std::vector<int> data, bool subData) noexcept
{
    m_data = std::move(data);

    int i = m_data[0];
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (m_data.size() * sizeof(GLsizeiptr)), (const void*) &m_data[0], m_usage);

    GL46Renderer::getInstance()->checkForErrors();

    return this;
}

Core::Graphics::API::GL::GL46::GL46IndexBuffer* Core::Graphics::API::GL::GL46::GL46IndexBuffer::bind() noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handler);

    GL46Renderer::getInstance()->checkForErrors();

    return this;
}

void Core::Graphics::API::GL::GL46::GL46IndexBuffer::setUsage(const int& usage) noexcept
{
    m_usage = usage;
}

void Core::Graphics::API::GL::GL46::GL46IndexBuffer::destroy() noexcept
{
    std::cout << "index buffer destroyed" << std::endl;

    glDeleteBuffers(1, &m_handler);

    GL46Renderer::getInstance()->checkForErrors();
}
