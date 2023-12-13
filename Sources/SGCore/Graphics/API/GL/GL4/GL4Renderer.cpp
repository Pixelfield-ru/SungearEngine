#include "GL4Renderer.h"

#include "SGCore/Main/CoreMain.h"

#include <glm/gtc/type_ptr.hpp>

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Rendering/Skybox.h"

#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/ECS/Rendering/Gizmos/IGizmo.h"

#include "SGCore/Graphics/API/GL/DeviceGLInfo.h"

void SGCore::GL4Renderer::init() noexcept
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

    DeviceGLInfo::prepareInfo();

    if(!confirmSupport())
    {
        CoreMain::getWindow().setShouldClose(true);
    }

    setDepthTestingEnabled(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_STENCIL_TEST);

    /*glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GREATER, 0.2);*/

    /*glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);*/

    // -------------------------------------

    // TODO: make defines for uniforms names

    m_viewMatricesBuffer = Ref<GL4UniformBuffer>(createUniformBuffer());
    m_viewMatricesBuffer->m_blockName = "CameraData";
    m_viewMatricesBuffer->putUniforms({
                                              IShaderUniform("camera.projectionMatrix", SGGDataType::SGG_MAT4),
                                              IShaderUniform("camera.viewMatrix", SGGDataType::SGG_MAT4),
                                              IShaderUniform("camera.spaceMatrix", SGGDataType::SGG_MAT4),
                                              IShaderUniform("camera.position", SGGDataType::SGG_FLOAT3),
                                              IShaderUniform("camera.rotation", SGGDataType::SGG_FLOAT3),
                                              IShaderUniform("camera.scale", SGGDataType::SGG_FLOAT3)
                                        });
    m_viewMatricesBuffer->setLayoutLocation(1);
    m_viewMatricesBuffer->prepare();

    m_programDataBuffer = Ref<GL4UniformBuffer>(createUniformBuffer());
    m_programDataBuffer->m_blockName = "ProgramData";
    m_programDataBuffer->putUniforms({
                                             IShaderUniform("windowSize", SGGDataType::SGG_FLOAT2),
                                             IShaderUniform("currentTime", SGGDataType::SGG_FLOAT)
                                       });
    m_programDataBuffer->setLayoutLocation(3);
    m_programDataBuffer->prepare();
}

bool SGCore::GL4Renderer::confirmSupport() noexcept
{
    // because gl version always has this pattern
    int glMajorVersion = DeviceGLInfo::getVersion()[0] - '0';
    int glMinorVersion = DeviceGLInfo::getVersion()[2] - '0';

    if(glMajorVersion < 4)
    {
        SGCF_ERROR("OpengGL 4.0 is not supported!", SG_LOG_CURRENT_SESSION_FILE);

        return false;
    }

    return true;
}

void SGCore::GL4Renderer::checkForErrors(const std::source_location& location) noexcept
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

void SGCore::GL4Renderer::printInfo() noexcept
{
    SGF_INFO("OpenGL supporting extensions: ", SG_GL_SUPPORTING_EXTENSIONS_FILE);

    GLint extensionsNum = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsNum);

    for(int i = 0; i < extensionsNum; i++)
    {
        SGF_INFO(std::string(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i))), SG_GL_SUPPORTING_EXTENSIONS_FILE);
    }
}

void SGCore::GL4Renderer::prepareFrame(const glm::ivec2& windowSize)
{
    glViewport(0, 0, windowSize.x, windowSize.y);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SGCore::GL4Renderer::prepareUniformBuffers(const Ref<IRenderingComponent>& renderingComponent,
                                                        const Ref<Transform>& transformComponent)
{
    // double t0 = glfwGetTime();

    m_viewMatricesBuffer->bind();
    m_programDataBuffer->bind();

    if(renderingComponent)
    {
        m_viewMatricesBuffer->subData("camera.spaceMatrix",
                                      glm::value_ptr(renderingComponent->m_spaceMatrix), 16);
        m_viewMatricesBuffer->subData("camera.projectionMatrix",
                                      glm::value_ptr(renderingComponent->m_projectionMatrix), 16);
        m_viewMatricesBuffer->subData("camera.viewMatrix",
                                      glm::value_ptr(renderingComponent->m_viewMatrix), 16);
    }
    if(transformComponent)
    {
        m_viewMatricesBuffer->subData<float>("camera.position",
                                      glm::value_ptr(transformComponent->m_position), 3);
    }

    int windowWidth;
    int windowHeight;

    CoreMain::getWindow().getSize(windowWidth, windowHeight);
    // todo: перенести обновление в класс окна
    m_programDataBuffer->subData("windowSize", { windowWidth, windowHeight });
    m_programDataBuffer->subData("currentTime", { (float) glfwGetTime() });

    // double t1 = glfwGetTime();

    // 0.004 ms average
    // std::cout << "time for subdata: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void SGCore::GL4Renderer::renderMeshData(const Ref<IMeshData>& meshData,
                                                 const MeshDataRenderInfo& meshDataRenderInfo)
{
    if(meshDataRenderInfo.m_enableFacesCulling)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GLGraphicsTypesCaster::sggFaceTypeToGL(meshDataRenderInfo.m_facesCullingFaceType));
        glFrontFace(GLGraphicsTypesCaster::sggPolygonsOrderToGL(
                meshDataRenderInfo.m_facesCullingPolygonsOrder)
        );
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    if(meshData->getVertexArray())
    {
        meshData->getVertexArray()->bind();
    }

    auto drawMode = GLGraphicsTypesCaster::sggDrawModeToGL(meshDataRenderInfo.m_drawMode);

    if(drawMode == GL_LINES)
    {
        glLineWidth(meshDataRenderInfo.m_linesWidth);
    }

    if(drawMode == GL_POINTS)
    {
        glPointSize(meshDataRenderInfo.m_pointsSize);
    }

    if(!meshData->m_useIndices)
    {
        glDrawArrays(GL_LINES, 0, meshData->m_positions.size() / 3);
    }
    else
    {
        glDrawElements(drawMode, meshData->m_indices.size(),
                       GL_UNSIGNED_INT, nullptr);
    }
}

SGCore::GL46Shader* SGCore::GL4Renderer::createShader()
{
    auto* shader = new GL46Shader;
    shader->m_version = "400 core";

    return shader;
}

SGCore::GL46Shader* SGCore::GL4Renderer::createShader(const std::string& path)
{
    auto* shader = createShader();

    shader->compile(
            AssetManager::loadAsset<FileAsset>(path)
    );

    return shader;
}

SGCore::GLVertexArray* SGCore::GL4Renderer::createVertexArray()
{
    return new GLVertexArray;
}

SGCore::GLVertexBuffer* SGCore::GL4Renderer::createVertexBuffer()
{
    return new GLVertexBuffer;
}

SGCore::GLVertexBufferLayout* SGCore::GL4Renderer::createVertexBufferLayout()
{
    return new GLVertexBufferLayout;
}

SGCore::GLIndexBuffer* SGCore::GL4Renderer::createIndexBuffer()
{
    return new GLIndexBuffer;
}

SGCore::GL4Texture2D* SGCore::GL4Renderer::createTexture2D()
{
    return new GL4Texture2D;
}

SGCore::GL4CubemapTexture* SGCore::GL4Renderer::createCubemapTexture()
{
    return new GL4CubemapTexture;
}

SGCore::GL4UniformBuffer* SGCore::GL4Renderer::createUniformBuffer()
{
    return new GL4UniformBuffer;
}

SGCore::GL4FrameBuffer* SGCore::GL4Renderer::createFrameBuffer()
{
    return new GL4FrameBuffer;
}

SGCore::GL3MeshData* SGCore::GL4Renderer::createMeshData()
{
    return new GL3MeshData;
}

void SGCore::GL4Renderer::setDepthTestingEnabled(const bool& enabled) const noexcept
{
    if(enabled)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

const SGCore::Ref<SGCore::GL4Renderer>& SGCore::GL4Renderer::getInstance() noexcept
{
    static Ref<GL4Renderer> s_instancePointer(new GL4Renderer);
    s_instancePointer->m_apiType = SG_API_TYPE_GL4;

    return s_instancePointer;
}
