//
// Created by stuka on 21.05.2023.
//

#include <iostream>

#include "GLIndexBuffer.h"
#include "GLGraphicsTypesCaster.h"
#include "GL46/GL46Renderer.h"

#include "SGCore/Main/CoreMain.h"

Core::Graphics::GL::GLIndexBuffer::~GLIndexBuffer() noexcept
{
    destroy();
}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::GL::GLIndexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

void Core::Graphics::GL::GLIndexBuffer::destroy() noexcept
{
    glDeleteBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    //GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif
}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::GL::GLIndexBuffer::putData(std::vector<std::uint32_t> data) noexcept
{
    m_data = std::move(data);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (m_data.size() * sizeof(m_data[0])), &m_data[0],
                 GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));

    if(Core::Main::CoreMain::getRenderer().m_currentBoundVertexArray)
    {
        Core::Main::CoreMain::getRenderer().m_currentBoundVertexArray->m_indicesCount = m_data.size();
    }

    #ifdef SUNGEAR_DEBUG
    GL::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::GL::GLIndexBuffer::subData
        (std::vector<std::uint32_t> data, const int& offset) noexcept
{
    // TODO: make sub data in m_data

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, (GLsizeiptr) (m_data.size() * sizeof(GLsizeiptr)),
                    &m_data[0]);

    #ifdef SUNGEAR_DEBUG
    GL::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::GL::GLIndexBuffer::bind() noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handler);

    Core::Main::CoreMain::getRenderer().m_currentBoundIndexBuffer = this;

    #ifdef SUNGEAR_DEBUG
    GL::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IIndexBuffer> Core::Graphics::GL::GLIndexBuffer::setUsage(SGGUsage usage) noexcept
{
    m_usage = usage;

    return shared_from_this();
}