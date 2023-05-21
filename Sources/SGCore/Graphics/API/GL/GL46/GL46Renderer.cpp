#include "GL46Renderer.h"

#include <thread>

#include "GL46Shader.h"
#include "GL46IndexBuffer.h"

const std::shared_ptr<Core::Graphics::API::GL::GL46::GL46Renderer>& Core::Graphics::API::GL::GL46::GL46Renderer::getInstance() noexcept
{
    static auto* s_nakedInstancePointer = new GL46Renderer();
    static std::shared_ptr<GL46Renderer> s_instancePointer(s_nakedInstancePointer);

    return s_instancePointer;
}

void Core::Graphics::API::GL::GL46::GL46Renderer::init(const Main::Window& wnd) noexcept
{
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "-----------------------------------");
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "GL46Renderer initializing...");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Core::Logging::consolePrintf(Core::Logging::SG_INFO, "Failed to initialize GL46Renderer!");
    }
    else
    {
        Core::Logging::consolePrintf(Core::Logging::SG_INFO, "GL46Renderer initialized!");
    }

    printInfo();
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "-----------------------------------");
}

void Core::Graphics::API::GL::GL46::GL46Renderer::checkForErrors(std::source_location location) noexcept
{
    int errCode = glGetError();

    std::string errStr;

    switch(errCode)
    {
        case GL_INVALID_ENUM: errStr = "GL_INVALID_ENUM: enumeration parameter is not a legal enumeration for that function."; break;
        case GL_INVALID_VALUE: errStr = "GL_INVALID_VALUE: value parameter is not a legal value for that function."; break;
        case GL_INVALID_OPERATION: errStr = "GL_INVALID_OPERATION: set of state for a command is not legal for the parameters given to that command."; break;
        case GL_STACK_OVERFLOW: errStr = "GL_STACK_OVERFLOW: stack pushing operation cannot be done because it would overflow the limit of that stack's size."; break;
        case GL_STACK_UNDERFLOW: errStr = "GL_STACK_UNDERFLOW: stack popping operation cannot be done because the stack is already at its lowest point."; break;
        case GL_OUT_OF_MEMORY: errStr = "GL_OUT_OF_MEMORY: performing an operation that can allocate memory, and the memory cannot be allocated."; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: errStr = "GL_INVALID_FRAMEBUFFER_OPERATION: doing anything that would attempt to read from or write/render to a framebuffer that is not complete."; break;
        case GL_CONTEXT_LOST: errStr = "GL_CONTEXT_LOST: the OpenGL context has been lost, due to a graphics card reset."; break;
        default: errStr = "Unknown error"; break;
    };

    if(errCode != 0)
    {
        Core::Logging::consolePrintf(Core::Logging::MessageType::SG_ERROR,
                                     "OpenGL error (code: " + std::to_string(errCode) + "): " + errStr, location
        );
    }
}

void Core::Graphics::API::GL::GL46::GL46Renderer::printInfo() noexcept
{
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "GL46Renderer info:");
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "OpenGL version is " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
}

void Core::Graphics::API::GL::GL46::GL46Renderer::renderFrame()
{
    // TODO: for test. delete.
    glClear(GL_COLOR_BUFFER_BIT);
    int viewportWidth, viewportHeight;
    Core::Main::Core::getWindow().getSize(viewportWidth, viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);

    glBegin(GL_TRIANGLES);

    glVertex2f(0, 0);
    glVertex2f(0, 0.5f);
    glVertex2f(0.5f, 0);

    glEnd();
}

Core::Graphics::API::Shader* Core::Graphics::API::GL::GL46::GL46Renderer::createShader()
{
    return new GL46Shader;
}

Core::Graphics::API::IIndexBuffer* Core::Graphics::API::GL::GL46::GL46Renderer::createIndexBuffer()
{
    return new GL46IndexBuffer;
}
