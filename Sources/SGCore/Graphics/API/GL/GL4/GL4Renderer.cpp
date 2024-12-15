#include "GL4Renderer.h"

#include "SGCore/Main/CoreMain.h"

#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Transformations/TransformBase.h"

#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"

#include "SGCore/Graphics/API/GL/DeviceGLInfo.h"
#include "SGCore/Memory/AssetManager.h"

void SGCore::GL4Renderer::init() noexcept
{
    LOG_I("SGCore", "-----------------------------------");
    LOG_I("SGCore", "GLRenderer initializing...");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_I("SGCore", "Failed to initialize GLRenderer.");
    }
    else
    {
        LOG_I("SGCore", "GLRenderer initialized!");
    }

    printInfo();
    LOG_I("SGCore", "-----------------------------------");

    // -------------------------------------
    
    DeviceGLInfo::init();

    if(!confirmSupport())
    {
        CoreMain::getWindow().setShouldClose(true);
    }

    setDepthTestingEnabled(true);
    glEnable(GL_BLEND);
    // glBlendFunc(GL_ONE, GL_ZERO);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
                                              IShaderUniform("camera.projectionSpaceMatrix", SGGDataType::SGG_MAT4),
                                              IShaderUniform("camera.orthographicSpaceMatrix", SGGDataType::SGG_MAT4),
                                              IShaderUniform("camera.orthographicMatrix", SGGDataType::SGG_MAT4),
                                              IShaderUniform("camera.projectionMatrix", SGGDataType::SGG_MAT4),
                                              IShaderUniform("camera.viewMatrix", SGGDataType::SGG_MAT4),
                                              IShaderUniform("camera.position", SGGDataType::SGG_FLOAT3),
                                              IShaderUniform("camera.p0", SGGDataType::SGG_FLOAT),
                                              IShaderUniform("camera.rotation", SGGDataType::SGG_FLOAT3),
                                              IShaderUniform("camera.p1", SGGDataType::SGG_FLOAT),
                                              IShaderUniform("camera.scale", SGGDataType::SGG_FLOAT3),
                                              IShaderUniform("camera.p2", SGGDataType::SGG_FLOAT)
                                        });
    m_viewMatricesBuffer->setLayoutLocation(1);
    m_viewMatricesBuffer->prepare();

    m_programDataBuffer = Ref<GL4UniformBuffer>(createUniformBuffer());
    m_programDataBuffer->m_blockName = "ProgramDataBlock";
    m_programDataBuffer->putUniforms({
                                             IShaderUniform("programData.windowSize", SGGDataType::SGG_FLOAT2),
                                             IShaderUniform("programData.primaryMonitorSize", SGGDataType::SGG_FLOAT2),
                                             IShaderUniform("programData.p0", SGGDataType::SGG_FLOAT),
                                             IShaderUniform("programData.currentTime", SGGDataType::SGG_FLOAT)
                                       });
    m_programDataBuffer->setLayoutLocation(2);
    m_programDataBuffer->prepare();
}

bool SGCore::GL4Renderer::confirmSupport() noexcept
{
    // because gl version always has this pattern
    int glMajorVersion = DeviceGLInfo::getVersion()[0] - '0';
    int glMinorVersion = DeviceGLInfo::getVersion()[2] - '0';

    if(glMajorVersion < 4)
    {
        LOG_E(SGCORE_TAG, "OpengGL 4.0 is not supported!\n{}", SG_CURRENT_LOCATION_STR);

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
        LOG_E(SGCORE_TAG, "OpenGL error (code: {}): {}", errCode, errStr);
    }
}

void SGCore::GL4Renderer::printInfo() noexcept
{

}

void SGCore::GL4Renderer::prepareFrame(const glm::ivec2& windowSize)
{
    glViewport(0, 0, windowSize.x, windowSize.y);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SGCore::GL4Renderer::prepareUniformBuffers(const Ref<RenderingBase>& renderingBase,
                                                const Ref<Transform>& transform)
{
    // double t0 = glfwGetTime();

    m_viewMatricesBuffer->bind();

    m_viewMatricesBuffer->subData("camera.projectionSpaceMatrix",
                                  glm::value_ptr(renderingBase->m_projectionSpaceMatrix), 16
    );
    m_viewMatricesBuffer->subData("camera.orthographicSpaceMatrix",
                                  glm::value_ptr(renderingBase->m_orthographicSpaceMatrix), 16
    );
    m_viewMatricesBuffer->subData("camera.orthographicMatrix",
                                  glm::value_ptr(renderingBase->m_orthographicMatrix), 16
    );
    m_viewMatricesBuffer->subData("camera.projectionMatrix",
                                  glm::value_ptr(renderingBase->m_projectionMatrix), 16
    );
    m_viewMatricesBuffer->subData("camera.viewMatrix",
                                  glm::value_ptr(renderingBase->m_viewMatrix), 16
    );

    // todo: make to final transform
    m_viewMatricesBuffer->subData("camera.position",
                                         glm::value_ptr(transform->m_finalTransform.m_position), 3
    );

    int windowWidth;
    int windowHeight;
    
    int primaryMonitorWidth;
    int primaryMonitorHeight;
    
    m_programDataBuffer->bind();
    CoreMain::getWindow().getSize(windowWidth, windowHeight);
    Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    // todo: перенести обновление в класс окна
    m_programDataBuffer->subData("programData.windowSize", { (float) windowWidth, (float) windowHeight });
    m_programDataBuffer->subData("programData.primaryMonitorSize", { (float) primaryMonitorWidth, (float) primaryMonitorHeight });
    m_programDataBuffer->subData("programData.currentTime", {(float) glfwGetTime()});

    // double t1 = glfwGetTime();

    // 0.004 ms average
    // std::cout << "time for subdata: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void SGCore::GL4Renderer::renderMeshData(const IMeshData* meshData,
                                         const MeshDataRenderInfo& meshDataRenderInfo)
{
    if(!meshData) return;

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

    if(!meshDataRenderInfo.m_useIndices)
    {
        glDrawArrays(drawMode, 0, meshData->m_positions.size() / 3);
    }
    else
    {
        glDrawElements(drawMode, meshData->m_indices.size(),
                       GL_UNSIGNED_INT, nullptr);
    }
}

void SGCore::GL4Renderer::renderArray(const SGCore::Ref<SGCore::IVertexArray>& vertexArray,
                                      const SGCore::MeshDataRenderInfo& meshDataRenderInfo,
                                      const size_t& verticesCount,
                                      const size_t& indicesCount)
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
    
    if(vertexArray)
    {
        vertexArray->bind();
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
    
    if(!meshDataRenderInfo.m_useIndices)
    {
        glDrawArrays(drawMode, 0, verticesCount);
    }
    else
    {
        glDrawElements(drawMode, indicesCount,
                       GL_UNSIGNED_INT, nullptr);
    }
}

void SGCore::GL4Renderer::renderArrayInstanced(const SGCore::Ref<SGCore::IVertexArray>& vertexArray,
                                               const SGCore::MeshDataRenderInfo& meshDataRenderInfo,
                                               const size_t& verticesCount, const size_t& indicesCount,
                                               const size_t& instancesCount)
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
    
    if(vertexArray)
    {
        vertexArray->bind();
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
    
    if(!meshDataRenderInfo.m_useIndices)
    {
        glDrawArraysInstanced(drawMode, 0, verticesCount, instancesCount);
    }
    else
    {
        glDrawElementsInstanced(drawMode, indicesCount,
                                GL_UNSIGNED_INT, nullptr, instancesCount);
    }
}

SGCore::GL46Shader* SGCore::GL4Renderer::createShader() const
{
    auto* shader = new GL46Shader;
    shader->m_version = "400 core";
    shader->addDefine(SGShaderDefineType::SGG_OTHER_DEFINE, ShaderDefine("SG_GLSL4", ""));

    // shader->setRawName("SGUnknownShader");

    return shader;
}

SGCore::GLVertexArray* SGCore::GL4Renderer::createVertexArray() const
{
    return new GLVertexArray;
}

SGCore::GLVertexBuffer* SGCore::GL4Renderer::createVertexBuffer() const
{
    return new GLVertexBuffer;
}

SGCore::GLVertexBufferLayout* SGCore::GL4Renderer::createVertexBufferLayout() const
{
    return new GLVertexBufferLayout;
}

SGCore::GLIndexBuffer* SGCore::GL4Renderer::createIndexBuffer() const
{
    return new GLIndexBuffer;
}

SGCore::GL4Texture2D* SGCore::GL4Renderer::createTexture2D() const
{
    auto* tex = new GL4Texture2D;
    // tex->setRawName("SGUnknownTexture2D");

    return tex;
}

SGCore::GL4CubemapTexture* SGCore::GL4Renderer::createCubemapTexture() const
{
    auto* tex = new GL4CubemapTexture;
    // tex->setRawName("SGUnknownCubemapTexture");

    return new GL4CubemapTexture;
}

SGCore::GL4UniformBuffer* SGCore::GL4Renderer::createUniformBuffer() const
{
    return new GL4UniformBuffer;
}

SGCore::GL4FrameBuffer* SGCore::GL4Renderer::createFrameBuffer() const
{
    auto* fb = new GL4FrameBuffer;
    // fb->setRawName("SGUnknownFrameBuffer");

    return fb;
}

SGCore::GL3MeshData* SGCore::GL4Renderer::createMeshData() const
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
