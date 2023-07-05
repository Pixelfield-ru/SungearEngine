#include "GL46Renderer.h"
#include "SGCore/Graphics/API/GL/GL3/GL3Mesh.h"
#include "glm/gtc/type_ptr.hpp"

#include <thread>

const std::shared_ptr<Core::Graphics::GL::GL46Renderer>& Core::Graphics::GL::GL46Renderer::getInstance() noexcept
{
    static auto* s_nakedInstancePointer = new GL46Renderer;
    static std::shared_ptr<GL46Renderer> s_instancePointer(s_nakedInstancePointer);

    return s_instancePointer;
}

void Core::Graphics::GL::GL46Renderer::init() noexcept
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

    // -------------------------------------

    glEnable(GL_DEPTH_TEST);
    /*glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);*/

    glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GREATER, 0.2);

    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);*/

    // -------------------------------------

    m_modelMatricesBuffer = std::shared_ptr<IUniformBuffer>(Core::Main::Core::getRenderer().createUniformBuffer());
    m_modelMatricesBuffer->putUniforms({Core::Graphics::IShaderUniform("objectModelMatrix", SGGDataType::SGG_MAT4)});
    m_modelMatricesBuffer->putData<float>({ });
    m_modelMatricesBuffer->setLayoutLocation(0);
    m_modelMatricesBuffer->prepare();

    m_cameraMatricesBuffer = std::shared_ptr<IUniformBuffer>(Core::Main::Core::getRenderer().createUniformBuffer());
    m_cameraMatricesBuffer->putUniforms({
                                                Core::Graphics::IShaderUniform("cameraProjectionMatrix", SGGDataType::SGG_MAT4),
                                                Core::Graphics::IShaderUniform("cameraViewMatrix", SGGDataType::SGG_MAT4)
                                        });
    m_cameraMatricesBuffer->putData<float>({ });
    m_cameraMatricesBuffer->putData<float>({ });
    m_cameraMatricesBuffer->setLayoutLocation(1);
    m_cameraMatricesBuffer->prepare();
}

void Core::Graphics::GL::GL46Renderer::checkForErrors(std::source_location location) noexcept
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

void Core::Graphics::GL::GL46Renderer::printInfo() noexcept
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

void Core::Graphics::GL::GL46Renderer::renderFrame(const glm::ivec2& windowSize)
{
    glViewport(0, 0, windowSize.x, windowSize.y);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);
}

// TODO: just test. delete
void Core::Graphics::GL::GL46Renderer::renderMesh(
        const std::shared_ptr<ECS::CameraComponent>& cameraComponent,
        const std::shared_ptr<ECS::TransformComponent>& transformComponent,
        const std::shared_ptr<ECS::MeshComponent>& meshComponent)
{
    if(!meshComponent->m_mesh) return;

    m_modelMatricesBuffer->bind();
    m_cameraMatricesBuffer->bind();

    m_modelMatricesBuffer->subData("objectModelMatrix", glm::value_ptr(transformComponent->m_modelMatrix), 16);

    m_cameraMatricesBuffer->subData("cameraViewMatrix", glm::value_ptr(cameraComponent->m_viewMatrix), 16);
    m_cameraMatricesBuffer->subData("cameraProjectionMatrix", glm::value_ptr(cameraComponent->m_projectionMatrix), 16);

    meshComponent->m_mesh->m_material->bind();
    meshComponent->m_mesh->getVertexArray()->bind();

    glDrawElements(GL_TRIANGLES, meshComponent->m_mesh->getVertexArray()->m_indicesCount, GL_UNSIGNED_INT, nullptr);

    //std::cout << "rendered object with mesh " <<  meshComponent->m_mesh->m_name << std::endl;
}

Core::Graphics::GL::GL46Shader* Core::Graphics::GL::GL46Renderer::createShader()
{
    return new GL46Shader;
}

Core::Graphics::GL::GLVertexArray* Core::Graphics::GL::GL46Renderer::createVertexArray()
{
    return new GLVertexArray;
}

Core::Graphics::GL::GLVertexBuffer* Core::Graphics::GL::GL46Renderer::createVertexBuffer()
{
    return new GLVertexBuffer;
}

Core::Graphics::GL::GLVertexBufferLayout* Core::Graphics::GL::GL46Renderer::createVertexBufferLayout()
{
    return new GLVertexBufferLayout;
}

Core::Graphics::GL::GLIndexBuffer* Core::Graphics::GL::GL46Renderer::createIndexBuffer()
{
    return new GLIndexBuffer;
}

Core::Graphics::GL::GL46Texture2D* Core::Graphics::GL::GL46Renderer::createTexture2D()
{
    return new GL46Texture2D;
}

Core::Graphics::IUniformBuffer* Core::Graphics::GL::GL46Renderer::createUniformBuffer()
{
    return new GL46UniformBuffer;
}

Core::ImportedScene::IMesh* Core::Graphics::GL::GL46Renderer::createMesh()
{
    return new GL3Mesh;
}
