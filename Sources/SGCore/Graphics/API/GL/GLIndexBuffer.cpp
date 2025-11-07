//
// Created by stuka on 21.05.2023.
//

#include "GLIndexBuffer.h"
#include "GLGraphicsTypesCaster.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"

#include "SGCore/Main/CoreMain.h"

SGCore::GLIndexBuffer::~GLIndexBuffer() noexcept
{
    destroy();
}

void SGCore::GLIndexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);
}

void SGCore::GLIndexBuffer::create(const size_t& byteSize) noexcept
{
    destroy();
    
    glGenBuffers(1, &m_handler);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(byteSize), nullptr,
                 GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));
}

void SGCore::GLIndexBuffer::destroy() noexcept
{
    glDeleteBuffers(1, &m_handler);
}

void SGCore::GLIndexBuffer::putData(const std::vector<std::uint32_t>& data) noexcept
{
    if(data.empty()) return;

    m_data.resize(data.size());
    std::memcpy(m_data.data(), data.data(), data.size() * sizeof(data[0]));

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLuint64) (data.size() * sizeof(data[0])), &data[0],
                 GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));
}

void SGCore::GLIndexBuffer::subData
        (const std::vector<std::uint32_t>& data, const int& offset) noexcept
{
    if(data.empty() || (offset + data.size()) > std::ssize(m_data)) return;

    std::memcpy(m_data.data() + offset, data.data(), data.size() * sizeof(data[0]));
    
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(data[0]), (GLuint64) (data.size() * sizeof(data[0])),
                    data.data());
}

void SGCore::GLIndexBuffer::subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) noexcept
{
    if(elementsCount == 0 || (offset + elementsCount) > std::ssize(m_data)) return;

    std::memcpy(m_data.data() + offset, data, elementsCount * sizeof(data[0]));

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(data[0]), (GLuint64) (elementsCount * sizeof(data[0])),
                    data);
}

void SGCore::GLIndexBuffer::bind() noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handler);

    // CoreMain::getRenderer().m_currentBoundIndexBuffer = this;
}

void SGCore::GLIndexBuffer::setUsage(SGGUsage usage) noexcept
{
    m_usage = usage;
}
