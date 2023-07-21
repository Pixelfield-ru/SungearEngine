//
// Created by stuka on 24.05.2023.
//

#include "GLVertexArray.h"

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"
#include "SGCore/Main/CoreMain.h"

Core::Graphics::GLVertexArray::~GLVertexArray() noexcept
{
    destroy();
}

std::shared_ptr<Core::Graphics::IVertexArray> Core::Graphics::GLVertexArray::create() noexcept
{
    destroy();

    glGenVertexArrays(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL4Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

void Core::Graphics::GLVertexArray::destroy() noexcept
{
    if(glIsBuffer(m_handler))
    {
        glDeleteVertexArrays(1, &m_handler);
        std::cout << "deleted buf" << std::endl;
    }

    #ifdef SUNGEAR_DEBUG
    //GL4Renderer::getInstance()->checkForErrors();
    #endif
}

std::shared_ptr<Core::Graphics::IVertexArray> Core::Graphics::GLVertexArray::bind() noexcept
{
    glBindVertexArray(m_handler);

    Core::Main::CoreMain::getRenderer().m_currentBoundVertexArray = this;

    return shared_from_this();
}
