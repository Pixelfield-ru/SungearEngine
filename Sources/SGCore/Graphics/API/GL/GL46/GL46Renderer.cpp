#include "GL46Renderer.h"

#include <thread>

const std::shared_ptr<Core::Graphics::API::GL::GL46::GL46Renderer>& Core::Graphics::API::GL::GL46::GL46Renderer::getInstance() noexcept
{
    static auto* s_nakedInstancePointer = new GL46Renderer;
    static std::shared_ptr<GL46Renderer> s_instancePointer(s_nakedInstancePointer);

    return s_instancePointer;
}

void Core::Graphics::API::GL::GL46::GL46Renderer::init() noexcept
{
    SGC_INFO("-----------------------------------");
    SGC_INFO("GL46Renderer initializing...");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SGC_ERROR("Failed to initialize GL46Renderer!");
    }
    else
    {
        SGC_INFO("GL46Renderer initialized!");
    }

    printInfo();
    SGC_INFO("-----------------------------------");
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
        SGC_ERROR_SL("OpenGL error (code: " + std::to_string(errCode) + "): " + errStr, location);
    }
}

void Core::Graphics::API::GL::GL46::GL46Renderer::printInfo() noexcept
{
    SGC_INFO("GL46Renderer info:");
    SGC_INFO("OpenGL version is " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    SGF_INFO("Supporting extensions: ", SG_GL_SUPPORTING_EXTENSIONS_FILE);

    GLint extensionsNum = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsNum);

    for(int i = 0; i < extensionsNum; i++)
    {
        SGF_INFO(std::string(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i))), SG_GL_SUPPORTING_EXTENSIONS_FILE);
    }
}

void Core::Graphics::API::GL::GL46::GL46Renderer::renderFrame(const glm::ivec2& windowSize)
{
    glViewport(0, 0, windowSize.x, windowSize.y);
}

// TODO: just test. delete
void Core::Graphics::API::GL::GL46::GL46Renderer::renderMesh(ITexture2D* texture2D,
                                                             IShader* shader,
                                                             IVertexArray* vertexArray)
{
    texture2D->bind();
    shader->bind();
    vertexArray->bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

Core::Graphics::API::GL::GL46::GL46Shader* Core::Graphics::API::GL::GL46::GL46Renderer::createShader()
{
    return new GL46Shader;
}

Core::Graphics::API::GL::GLVertexArray* Core::Graphics::API::GL::GL46::GL46Renderer::createVertexArray()
{
    return new GLVertexArray;
}

Core::Graphics::API::GL::GLVertexBuffer* Core::Graphics::API::GL::GL46::GL46Renderer::createVertexBuffer()
{
    return new GLVertexBuffer;
}

Core::Graphics::API::GL::GLVertexBufferLayout* Core::Graphics::API::GL::GL46::GL46Renderer::createVertexBufferLayout()
{
    return new GLVertexBufferLayout;
}

Core::Graphics::API::GL::GLIndexBuffer* Core::Graphics::API::GL::GL46::GL46Renderer::createIndexBuffer()
{
    return new GLIndexBuffer;
}

Core::Graphics::API::GL::GL46::GL46Texture2D* Core::Graphics::API::GL::GL46::GL46Renderer::createTexture2D()
{
    return new GL46Texture2D;
}
