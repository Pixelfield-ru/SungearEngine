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

std::shared_ptr<SGCore::IVertexBuffer> SGCore::GLVertexBuffer::putData
(const std::vector<float>& data) noexcept
{
    if(data.empty()) return shared_from_this();

    m_data.insert(m_data.end(), data.begin(), data.end());

    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (m_data.size() * sizeof(m_data[0])), &m_data[0],
                 GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

void SGCore::GLVertexBuffer::subData
(const std::vector<float>& data, const size_t& offset) noexcept
{
    if(data.empty()) return;

    // m_data.insert(m_data.begin() + offset, data.begin(), data.end());

    glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(data[0]), (GLsizeiptr) (data.size() * sizeof(data[0])),
                    &data[0]);

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
