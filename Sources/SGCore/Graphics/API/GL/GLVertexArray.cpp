//
// Created by stuka on 24.05.2023.
//

#include "GLVertexArray.h"

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Main/CoreMain.h"

SGCore::GLVertexArray::~GLVertexArray() noexcept
{
    destroy();
}

std::shared_ptr<SGCore::IVertexArray> SGCore::GLVertexArray::create() noexcept
{
    destroy();

    glGenVertexArrays(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
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

std::shared_ptr<SGCore::IVertexArray> SGCore::GLVertexArray::bind() noexcept
{
    glBindVertexArray(m_handler);

    CoreMain::getRenderer().m_currentBoundVertexArray = this;

    return shared_from_this();
}
