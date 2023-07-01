//
// Created by stuka on 24.05.2023.
//

#include "GLVertexArray.h"

#include "SGCore/Graphics/API/GL/GL46/GL46Renderer.h"

Core::Graphics::API::GL::GLVertexArray::~GLVertexArray() noexcept
{
    destroy();
}

std::shared_ptr<Core::Graphics::API::IVertexArray> Core::Graphics::API::GL::GLVertexArray::create() noexcept
{
    destroy();

    glGenVertexArrays(1, &m_handler);

    #ifdef SUNGEAR_DEBUG
    GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif

    return shared_from_this();
}

void Core::Graphics::API::GL::GLVertexArray::destroy() noexcept
{
    if(glIsBuffer(m_handler))
    {
        glDeleteVertexArrays(1, &m_handler);
        std::cout << "deleted buf" << std::endl;
    }

    #ifdef SUNGEAR_DEBUG
    //GL46::GL46Renderer::getInstance()->checkForErrors();
    #endif
}

std::shared_ptr<Core::Graphics::API::IVertexArray> Core::Graphics::API::GL::GLVertexArray::bind() noexcept
{
    glBindVertexArray(m_handler);

    Core::Main::Core::getRenderer().m_currentBoundVertexArray = this;

    return shared_from_this();
}
