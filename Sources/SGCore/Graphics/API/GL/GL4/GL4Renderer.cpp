#include "GL4Renderer.h"

#include "SGCore/Main/CoreMain.h"

#include <glm/gtc/type_ptr.hpp>

#include <thread>

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Rendering/CameraComponent.h"
#include "SGCore/ECS/Rendering/ShadowsCasterComponent.h"

void Core::Graphics::GL4Renderer::init() noexcept
{
    SGCF_INFO("-----------------------------------", SG_LOG_CURRENT_SESSION_FILE);
    SGCF_INFO("GLRenderer initializing...", SG_LOG_CURRENT_SESSION_FILE);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SGCF_ERROR("Failed to initialize GLRenderer.", SG_LOG_CURRENT_SESSION_FILE);
    }
    else
    {
        SGCF_INFO("GLRenderer initialized!", SG_LOG_CURRENT_SESSION_FILE);
    }

    printInfo();
    SGCF_INFO("-----------------------------------", SG_LOG_CURRENT_SESSION_FILE);

    // -------------------------------------

    if(!confirmSupport())
    {
        Core::Main::CoreMain::getWindow().setShouldClose(true);
    }

    glEnable(GL_DEPTH_TEST);
    /*glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);*/

    glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GREATER, 0.2);

    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);*/

    // -------------------------------------

    m_modelMatricesBuffer = std::shared_ptr<GL46UniformBuffer>(createUniformBuffer());
    m_modelMatricesBuffer->m_blockName = "ObjectMatrices";
    m_modelMatricesBuffer->putUniforms({Core::Graphics::IShaderUniform("objectModelMatrix", SGGDataType::SGG_MAT4)});
    m_modelMatricesBuffer->putData<float>({ });
    m_modelMatricesBuffer->setLayoutLocation(0);
    m_modelMatricesBuffer->prepare();

    m_viewMatricesBuffer = std::shared_ptr<GL46UniformBuffer>(createUniformBuffer());
    m_viewMatricesBuffer->m_blockName = "ViewMatrices";
    m_viewMatricesBuffer->putUniforms({
                                                Core::Graphics::IShaderUniform("projectionMatrix", SGGDataType::SGG_MAT4),
                                                Core::Graphics::IShaderUniform("viewMatrix", SGGDataType::SGG_MAT4)
                                        });
    m_viewMatricesBuffer->putData<float>({ });
    m_viewMatricesBuffer->putData<float>({ });
    m_viewMatricesBuffer->setLayoutLocation(1);
    m_viewMatricesBuffer->prepare();
}

bool Core::Graphics::GL4Renderer::confirmSupport() noexcept
{
    std::string glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    if(!glVersion.starts_with("4.0"))
    {
        SGCF_ERROR("OpengGL 4.0 is not supported!", SG_LOG_CURRENT_SESSION_FILE);

        return false;
    }

    return true;
}

void Core::Graphics::GL4Renderer::checkForErrors(const std::source_location& location) noexcept
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

void Core::Graphics::GL4Renderer::printInfo() noexcept
{
    SGCF_INFO("GLRenderer info:", SG_LOG_CURRENT_SESSION_FILE);
    SGCF_INFO("OpenGL version is " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))), SG_LOG_CURRENT_SESSION_FILE);
    SGF_INFO("Supporting extensions: ", SG_GL_SUPPORTING_EXTENSIONS_FILE);

    GLint extensionsNum = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsNum);

    for(int i = 0; i < extensionsNum; i++)
    {
        SGF_INFO(std::string(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i))), SG_GL_SUPPORTING_EXTENSIONS_FILE);
    }
}

void Core::Graphics::GL4Renderer::renderFrame(const glm::ivec2& windowSize)
{
    glViewport(0, 0, windowSize.x, windowSize.y);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 0, 1, 1);
}

// TODO: just test. delete
void Core::Graphics::GL4Renderer::renderMesh(
        const std::shared_ptr<ECS::CameraComponent>& cameraComponent,
        const std::shared_ptr<ECS::TransformComponent>& transformComponent,
        const std::shared_ptr<ECS::MeshComponent>& meshComponent)
{
    if(!meshComponent->m_mesh) return;

    meshComponent->m_mesh->m_material->bind();
    meshComponent->m_mesh->getVertexArray()->bind();

    m_modelMatricesBuffer->bind();
    m_viewMatricesBuffer->bind();

    meshComponent->m_mesh->m_material->m_shader->useUniformBuffer(m_modelMatricesBuffer);
    meshComponent->m_mesh->m_material->m_shader->useUniformBuffer(m_viewMatricesBuffer);

    m_modelMatricesBuffer->subData("objectModelMatrix", glm::value_ptr(transformComponent->m_modelMatrix), 16);

    m_viewMatricesBuffer->subData("viewMatrix", glm::value_ptr(cameraComponent->m_viewMatrix), 16);
    m_viewMatricesBuffer->subData("projectionMatrix", glm::value_ptr(cameraComponent->m_projectionMatrix), 16);

    glDrawElements(GL_TRIANGLES, meshComponent->m_mesh->getVertexArray()->m_indicesCount, GL_UNSIGNED_INT, nullptr);
}

void Core::Graphics::GL4Renderer::renderMesh(const std::shared_ptr<ECS::ShadowsCasterComponent>& shadowsCasterComponent,
                                             const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                                             const std::shared_ptr<ECS::MeshComponent>& meshComponent)
{
    if(!meshComponent->m_mesh) return;

    // TODO: MAKE CHECKING FOR ALREADY BIND FRAMEBUFFERS, VAOs, VBOs e.t.c. (for not to bind every time the same buffer)
    shadowsCasterComponent->m_frameBuffer->bind();
    ECS::ShadowsCasterComponent::getObjectsShader()->bind();
    meshComponent->m_mesh->getVertexArray()->bind();

    m_modelMatricesBuffer->bind();
    m_viewMatricesBuffer->bind();

    ECS::ShadowsCasterComponent::getObjectsShader()->useUniformBuffer(m_modelMatricesBuffer);
    ECS::ShadowsCasterComponent::getObjectsShader()->useUniformBuffer(m_viewMatricesBuffer);

    m_modelMatricesBuffer->subData("objectModelMatrix", glm::value_ptr(transformComponent->m_modelMatrix), 16);

    m_viewMatricesBuffer->subData("viewMatrix", glm::value_ptr(shadowsCasterComponent->m_viewMatrix), 16);
    m_viewMatricesBuffer->subData("projectionMatrix", glm::value_ptr(shadowsCasterComponent->m_projectionMatrix), 16);

    glDrawElements(GL_TRIANGLES, meshComponent->m_mesh->getVertexArray()->m_indicesCount, GL_UNSIGNED_INT, nullptr);

    shadowsCasterComponent->m_frameBuffer->unbind();
}

Core::Graphics::GL46Shader* Core::Graphics::GL4Renderer::createShader()
{
    return new GL46Shader;
}

Core::Graphics::GL46Shader* Core::Graphics::GL4Renderer::createPBRShader()
{
    auto* shader = new GL46Shader;
    shader->m_version = "400";
    shader->compile(
            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>(SG_GLSL4_PBR_SHADER_PATH)
    );

    return shader;
}

Core::Graphics::GL46Shader* Core::Graphics::GL4Renderer::createOnlyGeometryShader()
{
    auto* shader = new GL46Shader;
    shader->m_version = "400";
    shader->compile(
            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>(SG_GLSL4_ONLY_GEOM_SHADER_PATH)
    );

    return shader;
}

Core::Graphics::GLVertexArray* Core::Graphics::GL4Renderer::createVertexArray()
{
    return new GLVertexArray;
}

Core::Graphics::GLVertexBuffer* Core::Graphics::GL4Renderer::createVertexBuffer()
{
    return new GLVertexBuffer;
}

Core::Graphics::GLVertexBufferLayout* Core::Graphics::GL4Renderer::createVertexBufferLayout()
{
    return new GLVertexBufferLayout;
}

Core::Graphics::GLIndexBuffer* Core::Graphics::GL4Renderer::createIndexBuffer()
{
    return new GLIndexBuffer;
}

Core::Graphics::ITexture2D* Core::Graphics::GL4Renderer::createTexture2D()
{
    return new GL4Texture2D;
}

Core::Graphics::GL46UniformBuffer* Core::Graphics::GL4Renderer::createUniformBuffer()
{
    return new GL46UniformBuffer;
}

Core::Graphics::GL4FrameBuffer* Core::Graphics::GL4Renderer::createFrameBuffer()
{
    return new GL4FrameBuffer;
}

Core::Graphics::GL3Mesh* Core::Graphics::GL4Renderer::createMesh()
{
    return new GL3Mesh;
}

Core::Memory::Assets::IMaterial* Core::Graphics::GL4Renderer::createMaterial()
{
    auto* mat = new Memory::Assets::IMaterial;

    mat->m_shader = std::shared_ptr<Core::Graphics::IShader>(createPBRShader());

    // adding block decls
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_EMISSIVE,
                             1, 0);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION,
                             1, 1);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_AMBIENT,
                             1, 2);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS,
                             1, 3);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_DIFFUSE,
                             1, 4);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_DISPLACEMENT,
                             1, 5);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_HEIGHT,
                             1, 6);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_NORMALS,
                             1, 7);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_BASE_COLOR,
                             1, 8);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_CLEARCOAT,
                             1, 9);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_EMISSION_COLOR,
                             1, 10);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_LIGHTMAP,
                             1, 11);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_METALNESS,
                             1, 12);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_NORMAL_CAMERA,
                             1, 13);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_OPACITY,
                             1, 14);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_REFLECTION,
                             1, 15);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SHEEN,
                             1, 16);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SHININESS,
                             1, 17);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SPECULAR,
                             1, 18);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_TRANSMISSION,
                             1, 19);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SHADOW_MAP,
                             5, 20);

    return mat;
}

const std::shared_ptr<Core::Graphics::GL4Renderer>& Core::Graphics::GL4Renderer::getInstance() noexcept
{
    static std::shared_ptr<GL4Renderer> s_instancePointer(new GL4Renderer);
    s_instancePointer->m_apiType = APIType::OPENGL;

    return s_instancePointer;
}
