//
// Created by stuka on 24.05.2023.
//

#include "GLVertexArray.h"

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Main/CoreMain.h"

SGCore::GLVertexArray::~GLVertexArray() noexcept
{
    GLVertexArray::destroy();

    static_cast<GLObjectsStorage&>(CoreMain::getRenderer()->storage()).m_vertexArrays.erase(this);
}

void SGCore::GLVertexArray::create() noexcept
{
    destroy();

    glGenVertexArrays(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif
}

void SGCore::GLVertexArray::destroy() noexcept
{
    if(glIsBuffer(m_handler))
    {
        glDeleteVertexArrays(1, &m_handler);
    }

    #ifdef SUNGEAR_DEBUG
    //GL4Renderer::getInstance()->checkForErrors();
    #endif
}

void SGCore::GLVertexArray::bind() noexcept
{
    // SG_ASSERT(glIsBuffer(m_handler), fmt::format("OGL: Can not bind vertex array! Vertex array with handler '{}' is invalid", m_handler).c_str());
    
    glBindVertexArray(m_handler);

    // CoreMain::getRenderer()->m_currentBoundVertexArray = this;
}
