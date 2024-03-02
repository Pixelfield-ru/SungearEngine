//
// Created by stuka on 21.05.2023.
//

#include <iostream>

#include "GLIndexBuffer.h"
#include "GLGraphicsTypesCaster.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"

#include "SGCore/Main/CoreMain.h"

SGCore::GLIndexBuffer::~GLIndexBuffer() noexcept
{
    destroy();
}

std::shared_ptr<SGCore::IIndexBuffer> SGCore::GLIndexBuffer::create() noexcept
{
    destroy();

    glGenBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

void SGCore::GLIndexBuffer::destroy() noexcept
{
    glDeleteBuffers(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    //GL46::GL4Renderer::getInstance()->checkForErrors();
    #endif
}

std::shared_ptr<SGCore::IIndexBuffer> SGCore::GLIndexBuffer::putData(const std::vector<std::uint32_t>& data) noexcept
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (data.size() * sizeof(data[0])), &data[0],
                 GLGraphicsTypesCaster::sggBufferUsageToGL(m_usage));

    /*if(CoreMain::getRenderer()->m_currentBoundVertexArray)
    {
        CoreMain::getRenderer()->m_currentBoundVertexArray->m_indicesCount = m_data.size();
    }*/

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

void SGCore::GLIndexBuffer::subData
        (const std::vector<std::uint32_t>& data, const int& offset) noexcept
{
    // m_data.insert(m_data.begin() + offset, data.begin(), data.end());
    
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(data[0]), (GLsizeiptr) (data.size() * sizeof(data[0])),
                    data.data());

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif
}

void SGCore::GLIndexBuffer::subData(std::uint32_t* data, const size_t& elementsCount, const int& offset) noexcept
{
    if(elementsCount == 0) return;
    
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(data[0]), (GLsizeiptr) (elementsCount * sizeof(data[0])),
                    data);
    
    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif
}

std::shared_ptr<SGCore::IIndexBuffer> SGCore::GLIndexBuffer::bind() noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handler);

    // CoreMain::getRenderer().m_currentBoundIndexBuffer = this;

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

std::shared_ptr<SGCore::IIndexBuffer> SGCore::GLIndexBuffer::setUsage(SGGUsage usage) noexcept
{
    m_usage = usage;

    return shared_from_this();
}
