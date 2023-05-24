//
// Created by stuka on 24.05.2023.
//

#include "GLVertexArray.h"

#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"

Core::Graphics::API::GL::GLVertexArray::~GLVertexArray() noexcept
{
    destroy();
}

Core::Graphics::API::GL::GLVertexArray* Core::Graphics::API::GL::GLVertexArray::create() noexcept
{
    destroy();

    glGenVertexArrays(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return this;
}

void Core::Graphics::API::GL::GLVertexArray::destroy() noexcept
{
    glDeleteVertexArrays(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif
}

Core::Graphics::API::GL::GLVertexArray* Core::Graphics::API::GL::GLVertexArray::bind() noexcept
{
    glBindVertexArray(m_handler);

    return this;
}
