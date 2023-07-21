#include "GL4Renderer.h"

#include "SGCore/Main/CoreMain.h"

#include "glm/gtc/type_ptr.hpp"

#include <thread>

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

    m_cameraMatricesBuffer = std::shared_ptr<GL46UniformBuffer>(createUniformBuffer());
    m_cameraMatricesBuffer->m_blockName = "CameraMatrices";
    m_cameraMatricesBuffer->putUniforms({
                                                Core::Graphics::IShaderUniform("cameraProjectionMatrix", SGGDataType::SGG_MAT4),
                                                Core::Graphics::IShaderUniform("cameraViewMatrix", SGGDataType::SGG_MAT4)
                                        });
    m_cameraMatricesBuffer->putData<float>({ });
    m_cameraMatricesBuffer->putData<float>({ });
    m_cameraMatricesBuffer->setLayoutLocation(1);
    m_cameraMatricesBuffer->prepare();
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
    m_cameraMatricesBuffer->bind();

    meshComponent->m_mesh->m_material->m_shader->useUniformBuffer(m_modelMatricesBuffer);
    meshComponent->m_mesh->m_material->m_shader->useUniformBuffer(m_cameraMatricesBuffer);

    m_modelMatricesBuffer->subData("objectModelMatrix", glm::value_ptr(transformComponent->m_modelMatrix), 16);

    m_cameraMatricesBuffer->subData("cameraViewMatrix", glm::value_ptr(cameraComponent->m_viewMatrix), 16);
    m_cameraMatricesBuffer->subData("cameraProjectionMatrix", glm::value_ptr(cameraComponent->m_projectionMatrix), 16);

    glDrawElements(GL_TRIANGLES, meshComponent->m_mesh->getVertexArray()->m_indicesCount, GL_UNSIGNED_INT, nullptr);

    //std::cout << "rendered object with mesh " <<  meshComponent->m_mesh->m_name << std::endl;
}

Core::Graphics::GL46Shader* Core::Graphics::GL4Renderer::createShader()
{
    return new GL46Shader;
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

Core::Graphics::GL3Mesh* Core::Graphics::GL4Renderer::createMesh()
{
    return new GL3Mesh;
}

Core::Memory::Assets::IMaterial *Core::Graphics::GL4Renderer::createMaterial()
{
    auto* mat = new Memory::Assets::IMaterial;

    mat->m_shader = std::shared_ptr<Core::Graphics::IShader>(createShader());
    mat->m_shader->m_version = "400";
    mat->m_shader->compile(Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>(
            "../SGResources/shaders/glsl4/pbr/default_shader.glsl")
            );
    mat->m_shader->addShaderDefines({
                                       //Core::Graphics::API::ShaderDefine("FLIP_TEXTURES_Y", "")
                               });

    // setting maximum textures of different types
    mat->getTextures()[SGMaterialTextureType::SGTP_EMISSIVE].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_AMBIENT].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_DIFFUSE].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_DISPLACEMENT].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_HEIGHT].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_NORMALS].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_BASE_COLOR].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_CLEARCOAT].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_EMISSION_COLOR].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_LIGHTMAP].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_METALNESS].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_NORMAL_CAMERA].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_OPACITY].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_REFLECTION].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_SHEEN].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_SHININESS].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_SPECULAR].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_TRANSMISSION].m_maximumTextures = 1;

    // setting offsets
    mat->getTextures()[SGMaterialTextureType::SGTP_EMISSIVE].m_texturesUnitOffset = 0;
    mat->getTextures()[SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION].m_texturesUnitOffset = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_AMBIENT].m_texturesUnitOffset = 2;
    mat->getTextures()[SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS].m_texturesUnitOffset = 3;
    mat->getTextures()[SGMaterialTextureType::SGTP_DIFFUSE].m_texturesUnitOffset = 4;
    mat->getTextures()[SGMaterialTextureType::SGTP_DISPLACEMENT].m_texturesUnitOffset = 5;
    mat->getTextures()[SGMaterialTextureType::SGTP_HEIGHT].m_texturesUnitOffset = 6;
    mat->getTextures()[SGMaterialTextureType::SGTP_NORMALS].m_texturesUnitOffset = 7;
    mat->getTextures()[SGMaterialTextureType::SGTP_BASE_COLOR].m_texturesUnitOffset = 8;
    mat->getTextures()[SGMaterialTextureType::SGTP_CLEARCOAT].m_texturesUnitOffset = 9;
    mat->getTextures()[SGMaterialTextureType::SGTP_EMISSION_COLOR].m_texturesUnitOffset = 10;
    mat->getTextures()[SGMaterialTextureType::SGTP_LIGHTMAP].m_texturesUnitOffset = 11;
    mat->getTextures()[SGMaterialTextureType::SGTP_METALNESS].m_texturesUnitOffset = 12;
    mat->getTextures()[SGMaterialTextureType::SGTP_NORMAL_CAMERA].m_texturesUnitOffset = 13;
    mat->getTextures()[SGMaterialTextureType::SGTP_OPACITY].m_texturesUnitOffset = 14;
    mat->getTextures()[SGMaterialTextureType::SGTP_REFLECTION].m_texturesUnitOffset = 15;
    mat->getTextures()[SGMaterialTextureType::SGTP_SHEEN].m_texturesUnitOffset = 16;
    mat->getTextures()[SGMaterialTextureType::SGTP_SHININESS].m_texturesUnitOffset = 17;
    mat->getTextures()[SGMaterialTextureType::SGTP_SPECULAR].m_texturesUnitOffset = 18;
    mat->getTextures()[SGMaterialTextureType::SGTP_TRANSMISSION].m_texturesUnitOffset = 19;

    return mat;
}

const std::shared_ptr<Core::Graphics::GL4Renderer>& Core::Graphics::GL4Renderer::getInstance() noexcept
{
    static std::shared_ptr<GL4Renderer> s_instancePointer(new GL4Renderer);
    s_instancePointer->m_apiType = APIType::OPENGL;

    return s_instancePointer;
}
