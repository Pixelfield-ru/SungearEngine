#include "GLRenderer.h"

void Core::Graphics::API::OpenGL::GLRenderer::init(const Main::Window& wnd) noexcept
{
    Core::Logging::c_printf(Core::Logging::SG_INFO, "GLRenderer initializing...");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Core::Logging::c_printf(Core::Logging::SG_INFO, "Failed to initialize GLRenderer!");
    }
    else
    {
        Core::Logging::c_printf(Core::Logging::SG_INFO, "GLRenderer initialized!");
    }

    print_info();
}

void Core::Graphics::API::OpenGL::GLRenderer::check_for_errors() noexcept
{
    int err_code = glGetError();

    std::string err_str;

    switch(err_code)
    {
        case GL_INVALID_ENUM: err_str = "GL_INVALID_ENUM: enumeration parameter is not a legal enumeration for that function."; break;
        case GL_INVALID_VALUE: err_str = "GL_INVALID_VALUE: value parameter is not a legal value for that function."; break;
        case GL_INVALID_OPERATION: err_str = "GL_INVALID_OPERATION: set of state for a command is not legal for the parameters given to that command."; break;
        case GL_STACK_OVERFLOW: err_str = "GL_STACK_OVERFLOW: stack pushing operation cannot be done because it would overflow the limit of that stack's size."; break;
        case GL_STACK_UNDERFLOW: err_str = "GL_STACK_UNDERFLOW: stack popping operation cannot be done because the stack is already at its lowest point."; break;
        case GL_OUT_OF_MEMORY: err_str = "GL_OUT_OF_MEMORY: performing an operation that can allocate memory, and the memory cannot be allocated."; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: err_str = "GL_INVALID_FRAMEBUFFER_OPERATION: doing anything that would attempt to read from or write/render to a framebuffer that is not complete."; break;
        case GL_CONTEXT_LOST: err_str = "GL_CONTEXT_LOST: the OpenGL context has been lost, due to a graphics card reset."; break;
        default: err_str = "Unknown error"; break;
    };

    Core::Logging::c_printf(Core::Logging::MessageType::SG_ERROR, "OpenGL error (code: %i): %s", err_code, err_str.c_str());
}

void Core::Graphics::API::OpenGL::GLRenderer::print_info() noexcept
{
    Core::Logging::c_printf(Core::Logging::SG_INFO, "-----------------------------------");
    Core::Logging::c_printf(Core::Logging::SG_INFO, "GLRenderer info:");
    Core::Logging::c_printf(Core::Logging::SG_INFO, "OpenGL version: %s", glGetString(GL_VERSION));
    Core::Logging::c_printf(Core::Logging::SG_INFO, "-----------------------------------");
}

void Core::Graphics::API::OpenGL::GLRenderer::start_loop()
{
    while(!Core::Main::Core::get_window().should_close())
    {
        glBegin(GL_TRIANGLES);

        glVertex2f(0, 0);
        glVertex2f(0, 0.5);
        glVertex2f(0.5, 0);

        glEnd();

        sg_call_frame_post_render_callback();

        Core::Main::Core::get_window().proceed_frame();
    }
}