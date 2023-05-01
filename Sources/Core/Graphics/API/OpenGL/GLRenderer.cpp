#include "GLRenderer.h"
#include <thread>
#include <limits>

#define SIGN_F(num, denum) ((exp(num) - exp (denum)) > max_exp) &&  (mantissa(num) >= mantissa(denum))

void Core::Graphics::API::OpenGL::GLRenderer::init(const Main::Window& wnd) noexcept
{
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "GLRenderer initializing...");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Core::Logging::consolePrintf(Core::Logging::SG_INFO, "Failed to initialize GLRenderer!");
    }
    else
    {
        Core::Logging::consolePrintf(Core::Logging::SG_INFO, "GLRenderer initialized!");
    }

    printInfo();
}

void Core::Graphics::API::OpenGL::GLRenderer::checkForErrors() noexcept
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

    Core::Logging::consolePrintf(Core::Logging::MessageType::SG_ERROR, "OpenGL error (code: %i): %s", errCode,
                                 errStr.c_str());
}

void Core::Graphics::API::OpenGL::GLRenderer::printInfo() noexcept
{
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "-----------------------------------");
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "GLRenderer info:");
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "OpenGL version: %s", glGetString(GL_VERSION));
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "-----------------------------------");
}

void Core::Graphics::API::OpenGL::GLRenderer::startLoop()
{
    // далее кринж
    double posx {}, posy {};

    while(!Core::Main::Core::getWindow().shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        int viewportWidth, viewportHeight;
        Core::Main::Core::getWindow().getSize(viewportWidth, viewportHeight);
        glViewport(0, 0, viewportWidth, viewportHeight);

        glBegin(GL_TRIANGLES);

        glVertex2f(0 + posx, 0 + posy);
        glVertex2f(0 + posx, 0.5f + posy);
        glVertex2f(0.5f + posx, 0 + posy);

        glEnd();

        if(mainInputListener->keyboardKeyDown(KEY_W))
        {
            posy += 0.01f;
        }
        if(mainInputListener->keyboardKeyDown(KEY_S))
        {
            posy -= 0.01f;
        }
        if(mainInputListener->keyboardKeyDown(KEY_D))
        {
            posx += 0.01f;
        }
        if(mainInputListener->keyboardKeyDown(KEY_A))
        {
            posx -= 0.01f;
        }

        sgCallFramePostRenderCallback();

        Core::Main::Core::getWindow().proceedFrame();
    }
}