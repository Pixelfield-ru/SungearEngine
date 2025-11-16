#include "GL4Renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "SGCore/Main/CoreMain.h"

#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Transformations/TransformBase.h"

#include "SGCore/Graphics/API/GL/GLGraphicsTypesCaster.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"

#include "SGCore/Graphics/API/GL/DeviceGLInfo.h"
#include "SGCore/Memory/AssetManager.h"

void SGCore::GL4Renderer::init() noexcept
{
    LOG_I(SGCORE_TAG, "-----------------------------------");
    LOG_I(SGCORE_TAG, "GLRenderer initializing...");

#if SG_PLATFORM_PC
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        LOG_I(SGCORE_TAG, "Failed to initialize GLAD.");
    }
    else
    {
        LOG_I(SGCORE_TAG, "GLAD initialized!");
    }
#elif SG_PLATFORM_OS_ANDROID
    // -------------------------------------

    if(!gladLoadGLES2Loader((GLADloadproc) eglGetProcAddress))
    {
        LOG_I(SGCORE_TAG, "Failed to initialize GLAD.");
    }
    else
    {
        LOG_I(SGCORE_TAG, "GLAD initialized!");
    }
#endif

    printInfo();
    LOG_I(SGCORE_TAG, "-----------------------------------");

    DeviceGLInfo::init();

    if(!confirmSupport()) { CoreMain::getWindow().setShouldClose(true); }

    useState(m_cachedRenderState, true);
    useMeshRenderState(m_cachedMeshRenderState, true);

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
        IShaderUniform("camera.zFar", SGGDataType::SGG_FLOAT),
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

    IRenderer::init();
}

bool SGCore::GL4Renderer::confirmSupport() noexcept
{
    // because gl version always has this pattern
    int glMajorVersion = DeviceGLInfo::getVersion()[0] - '0';
    int glMinorVersion = DeviceGLInfo::getVersion()[2] - '0';

    /*if(glMajorVersion < 4)
    {
        LOG_E(SGCORE_TAG, "OpengGL 4.0 is not supported!\n{}", SG_CURRENT_LOCATION_STR);

        return false;
    }*/

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
        std::cerr << fmt::format("OpenGL error (code: {}): {}", errCode, errStr) << std::endl;
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

    m_viewMatricesBuffer->subData("camera.position",
                                         glm::value_ptr(transform->m_finalTransform.m_position), 3
    );

    m_viewMatricesBuffer->subData("camera.zFar",
                                  &renderingBase->m_zFar, 1
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
    m_programDataBuffer->subData("programData.currentTime", {(float) Utils::getTimeSecondsAsDouble() });

    // double t1 = glfwGetTime();

    // 0.004 ms average
    // std::cout << "time for subdata: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void SGCore::GL4Renderer::renderMeshData(const IMeshData* meshData,
                                         const MeshRenderState& meshRenderState)
{
    if(!meshData) return;

    useMeshRenderState(meshRenderState);

    if(meshData->getVertexArray())
    {
        meshData->getVertexArray()->bind();
    }

    auto drawMode = GLGraphicsTypesCaster::sggDrawModeToGL(meshRenderState.m_drawMode);

    if(!meshRenderState.m_useIndices)
    {
        glDrawArrays(drawMode, 0, meshData->m_vertices.size());
    }
    else
    {
        glDrawElements(drawMode, meshData->m_indices.size(),
                       GL_UNSIGNED_INT, nullptr);
    }

    glBindVertexArray(0);
}

void SGCore::GL4Renderer::renderArray(const Ref<IVertexArray>& vertexArray,
                                      const MeshRenderState& meshRenderState,
                                      const int& verticesCount,
                                      const int& indicesCount)
{
    useMeshRenderState(meshRenderState);

    if(vertexArray)
    {
        vertexArray->bind();
    }

    auto drawMode = GLGraphicsTypesCaster::sggDrawModeToGL(meshRenderState.m_drawMode);

    if(!meshRenderState.m_useIndices)
    {
        glDrawArrays(drawMode, 0, (GLuint64) verticesCount);
    }
    else
    {
        glDrawElements(drawMode, (GLuint64) indicesCount,
                       GL_UNSIGNED_INT, nullptr);
    }

    glBindVertexArray(0);
}

void SGCore::GL4Renderer::renderArrayInstanced(const Ref<IVertexArray>& vertexArray,
                                               const MeshRenderState& meshRenderState,
                                               const int& verticesCount,
                                               const int& indicesCount,
                                               const int& instancesCount)
{
    useMeshRenderState(meshRenderState);

    if(vertexArray)
    {
        vertexArray->bind();
    }

    auto drawMode = GLGraphicsTypesCaster::sggDrawModeToGL(meshRenderState.m_drawMode);

    if(!meshRenderState.m_useIndices)
    {
        glDrawArraysInstanced(drawMode, 0, verticesCount, instancesCount);
    }
    else
    {
        glDrawElementsInstanced(drawMode, indicesCount,
                                GL_UNSIGNED_INT, nullptr, instancesCount);
    }

    glBindVertexArray(0);
}

SGCore::GL46Shader* SGCore::GL4Renderer::createShader()
{
    auto* shader = new GL46Shader;
#if SG_PLATFORM_PC
    shader->m_version = "400 core";
    shader->addDefine(SGShaderDefineType::SGG_OTHER_DEFINE, ShaderDefine("SG_GLSL4", ""));
#elif SG_PLATFORM_OS_ANDROID
    shader->m_version = "320 es";
    shader->addDefine(SGShaderDefineType::SGG_OTHER_DEFINE, ShaderDefine("SG_GLES32", ""));
#endif

    // shader->setRawName("SGUnknownShader");

    m_storage.m_shaders.insert(shader);

    return shader;
}

SGCore::GLVertexArray* SGCore::GL4Renderer::createVertexArray()
{
    auto* vao = new GLVertexArray;
    m_storage.m_vertexArrays.insert(vao);

    return vao;
}

SGCore::GLVertexBuffer* SGCore::GL4Renderer::createVertexBuffer()
{
    return new GLVertexBuffer;
}

SGCore::GLIndexBuffer* SGCore::GL4Renderer::createIndexBuffer()
{
    return new GLIndexBuffer;
}

SGCore::GL4Texture2D* SGCore::GL4Renderer::createTexture2D()
{
    auto* tex = new GL4Texture2D;
    m_storage.m_textures.insert(tex);

    return tex;
}

SGCore::GL4CubemapTexture* SGCore::GL4Renderer::createCubemapTexture()
{
    auto* tex = new GL4CubemapTexture;
    m_storage.m_textures.insert(tex);

    return tex;
}

SGCore::GL4UniformBuffer* SGCore::GL4Renderer::createUniformBuffer()
{
    auto* ub = new GL4UniformBuffer;
    m_storage.m_uniformBuffers.insert(ub);

    return ub;
}

SGCore::GL4FrameBuffer* SGCore::GL4Renderer::createFrameBuffer()
{
    auto* fb = new GL4FrameBuffer;
    m_storage.m_frameBuffers.insert(fb);

    return fb;
}

SGCore::IMeshData* SGCore::GL4Renderer::createMeshData() const
{
    return new IMeshData;
}

void SGCore::GL4Renderer::bindScreenFrameBuffer() const noexcept
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SGCore::IGPUObjectsStorage& SGCore::GL4Renderer::storage() noexcept
{
    return m_storage;
}

const SGCore::IGPUObjectsStorage& SGCore::GL4Renderer::storage() const noexcept
{
    return m_storage;
}

const SGCore::Ref<SGCore::GL4Renderer>& SGCore::GL4Renderer::getInstance() noexcept
{
    static Ref<GL4Renderer> s_instancePointer(new GL4Renderer);
// #if SG_PLATFORM_PC
    s_instancePointer->m_apiType = SG_API_TYPE_GL4;
/*#else
    s_instancePointer->m_apiType = SG_API_TYPE_GLES3;
#endif*/

    return s_instancePointer;
}

void SGCore::GL4Renderer::useState(const SGCore::RenderState& newRenderState, bool forceState) noexcept
{
    if(forceState)
    {
        if(newRenderState.m_useDepthTest)
        {
            glEnable(GL_DEPTH_TEST);
            m_cachedRenderState.m_useDepthTest = true;
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
            m_cachedRenderState.m_useDepthTest = false;
        }

        if(newRenderState.m_useDepthTest)
        {
            glDepthFunc(GLGraphicsTypesCaster::sgDepthStencilFuncToGL(newRenderState.m_depthFunc));
            glDepthMask(newRenderState.m_depthMask ? GL_TRUE : GL_FALSE);

            m_cachedRenderState.m_depthFunc = newRenderState.m_depthFunc;
            m_cachedRenderState.m_depthMask = newRenderState.m_depthMask;
        }

        if(newRenderState.m_useStencilTest)
        {
            glEnable(GL_STENCIL_TEST);
            glStencilOp(GLGraphicsTypesCaster::sgStencilOpToGL(newRenderState.m_stencilFailOp),
                        GLGraphicsTypesCaster::sgStencilOpToGL(newRenderState.m_stencilZFailOp),
                        GLGraphicsTypesCaster::sgStencilOpToGL(newRenderState.m_stencilZPassOp));
            glStencilFunc(GLGraphicsTypesCaster::sgDepthStencilFuncToGL(newRenderState.m_stencilFunc),
                          newRenderState.m_stencilFuncRef,
                          newRenderState.m_stencilFuncMask);
            glStencilMask(newRenderState.m_stencilMask);

            m_cachedRenderState.m_useStencilTest = true;
            m_cachedRenderState.m_stencilFailOp = newRenderState.m_stencilFailOp;
            m_cachedRenderState.m_stencilZFailOp = newRenderState.m_stencilZFailOp;
            m_cachedRenderState.m_stencilZPassOp = newRenderState.m_stencilZPassOp;
            m_cachedRenderState.m_stencilFunc = newRenderState.m_stencilFunc;
            m_cachedRenderState.m_stencilFuncRef = newRenderState.m_stencilFuncRef;
            m_cachedRenderState.m_stencilFuncMask = newRenderState.m_stencilFuncMask;
            m_cachedRenderState.m_stencilMask = newRenderState.m_stencilMask;
        }
        else
        {
            glDisable(GL_STENCIL_TEST);

            m_cachedRenderState.m_useStencilTest = false;
        }

        useBlendingState(newRenderState.m_globalBlendingState, true);
    }
    else
    {
        if(m_cachedRenderState == newRenderState) return;

        if(m_cachedRenderState.m_useDepthTest != newRenderState.m_useDepthTest)
        {
            if(newRenderState.m_useDepthTest)
            {
                glEnable(GL_DEPTH_TEST);
                m_cachedRenderState.m_useDepthTest = true;
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
                m_cachedRenderState.m_useDepthTest = false;
            }
        }

        if(newRenderState.m_useDepthTest)
        {
            if(m_cachedRenderState.m_depthFunc != newRenderState.m_depthFunc)
            {
                glDepthFunc(GLGraphicsTypesCaster::sgDepthStencilFuncToGL(newRenderState.m_depthFunc));
                m_cachedRenderState.m_depthFunc = newRenderState.m_depthFunc;
            }

            if(m_cachedRenderState.m_depthMask != newRenderState.m_depthMask)
            {
                glDepthMask(newRenderState.m_depthMask ? GL_TRUE : GL_FALSE);
                m_cachedRenderState.m_depthMask = newRenderState.m_depthMask;
            }
        }

        if(m_cachedRenderState.m_useStencilTest != newRenderState.m_useStencilTest)
        {
            if(newRenderState.m_useStencilTest)
            {
                glEnable(GL_STENCIL_TEST);
                m_cachedRenderState.m_useStencilTest = true;
            }
            else
            {
                glDisable(GL_STENCIL_TEST);
                m_cachedRenderState.m_useStencilTest = false;
            }
        }

        if(newRenderState.m_useStencilTest)
        {
            if(m_cachedRenderState.m_stencilFailOp != newRenderState.m_stencilFailOp ||
               m_cachedRenderState.m_stencilZFailOp != newRenderState.m_stencilZFailOp ||
               m_cachedRenderState.m_stencilZPassOp != newRenderState.m_stencilZPassOp)
            {
                glStencilOp(GLGraphicsTypesCaster::sgStencilOpToGL(newRenderState.m_stencilFailOp),
                            GLGraphicsTypesCaster::sgStencilOpToGL(newRenderState.m_stencilZFailOp),
                            GLGraphicsTypesCaster::sgStencilOpToGL(newRenderState.m_stencilZPassOp));

                m_cachedRenderState.m_stencilFailOp = newRenderState.m_stencilFailOp;
                m_cachedRenderState.m_stencilZFailOp = newRenderState.m_stencilZFailOp;
                m_cachedRenderState.m_stencilZPassOp = newRenderState.m_stencilZPassOp;
            }

            if(m_cachedRenderState.m_stencilFunc != newRenderState.m_stencilFunc ||
               m_cachedRenderState.m_stencilFuncRef != newRenderState.m_stencilFuncRef ||
               m_cachedRenderState.m_stencilMask != newRenderState.m_stencilMask)
            {
                glStencilFunc(GLGraphicsTypesCaster::sgDepthStencilFuncToGL(newRenderState.m_stencilFunc),
                              newRenderState.m_stencilFuncRef,
                              newRenderState.m_stencilFuncMask);

                m_cachedRenderState.m_stencilFunc = newRenderState.m_stencilFunc;
                m_cachedRenderState.m_stencilFuncRef = newRenderState.m_stencilFuncRef;
                m_cachedRenderState.m_stencilFuncMask = newRenderState.m_stencilFuncMask;
            }

            if(m_cachedRenderState.m_stencilMask != newRenderState.m_stencilMask)
            {
                glStencilMask(newRenderState.m_stencilMask);
                m_cachedRenderState.m_stencilMask = newRenderState.m_stencilMask;
            }
        }

        // if(newRenderState.m_globalBlendingState.m_useBlending)
        {
            // if(m_cachedRenderState.m_globalBlendingState != newRenderState.m_globalBlendingState)
            {
                useBlendingState(newRenderState.m_globalBlendingState);
            }
        }
    }
}

void SGCore::GL4Renderer::useBlendingState(const SGCore::BlendingState& newBlendingState,
                                           bool forceState) noexcept
{
    {
        const std::string assertMsg =
                fmt::format("Can not apply blending state for attachment '{}'. Attachment index must LESS than 32",
                            newBlendingState.m_forAttachment);
        SG_ASSERT(newBlendingState.m_forAttachment < 32, assertMsg.c_str());
    }

    if(m_cachedRenderState.m_globalBlendingState.m_useBlending != newBlendingState.m_useBlending || forceState)
    {
        if(newBlendingState.m_useBlending)
        {
            glEnable(GL_BLEND);
            m_cachedRenderState.m_globalBlendingState.m_useBlending = true;
        }
        else
        {
            glDisable(GL_BLEND);
            m_cachedRenderState.m_globalBlendingState.m_useBlending = false;
        }
    }

    if(!newBlendingState.m_useBlending) return;

    auto& oldBlendingState = newBlendingState.m_forAttachment >= 0 ?
                             m_cachedColorAttachmentsBlendingStates[newBlendingState.m_forAttachment] :
                             m_cachedRenderState.m_globalBlendingState;

    /*if(oldBlendingState.m_sFactor != newBlendingState.m_sFactor ||
       oldBlendingState.m_dFactor != newBlendingState.m_dFactor ||
       m_cachedRenderState.m_globalBlendingState.m_sFactor != newBlendingState.m_sFactor ||
       m_cachedRenderState.m_globalBlendingState.m_dFactor != newBlendingState.m_dFactor ||
       forceState)*/
    {
        if(newBlendingState.m_forAttachment >= 0)
        {
            // std::cout << std::format("setting m_sFactor and m_dFactor for attachment {}", newBlendingState.m_forAttachment) << std::endl;
            glBlendFunci(newBlendingState.m_forAttachment,
                         GLGraphicsTypesCaster::sgBlendingFactorToGL(newBlendingState.m_sFactor),
                         GLGraphicsTypesCaster::sgBlendingFactorToGL(newBlendingState.m_dFactor));
        }
        else
        {
            // std::cout << std::format("setting global m_sFactor and m_dFactor") << std::endl;
            glBlendFunc(GLGraphicsTypesCaster::sgBlendingFactorToGL(newBlendingState.m_sFactor),
                        GLGraphicsTypesCaster::sgBlendingFactorToGL(newBlendingState.m_dFactor));
        }

        oldBlendingState.m_sFactor = newBlendingState.m_sFactor;
        oldBlendingState.m_dFactor = newBlendingState.m_dFactor;
    }

    /*if(oldBlendingState.m_blendingEquation != newBlendingState.m_blendingEquation ||
       m_cachedRenderState.m_globalBlendingState.m_blendingEquation != newBlendingState.m_blendingEquation ||
       forceState)*/
    {
        if(newBlendingState.m_forAttachment >= 0)
        {
            glBlendEquationi(newBlendingState.m_forAttachment,
                             GLGraphicsTypesCaster::sgEquationToGL(newBlendingState.m_blendingEquation));
        }
        else
        {
            glBlendEquation(GLGraphicsTypesCaster::sgEquationToGL(newBlendingState.m_blendingEquation));
        }

        oldBlendingState.m_blendingEquation = newBlendingState.m_blendingEquation;
    }
}

void
SGCore::GL4Renderer::useMeshRenderState(const SGCore::MeshRenderState& newMeshRenderState, bool forceState) noexcept
{
    if(forceState)
    {
        if(newMeshRenderState.m_useFacesCulling)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GLGraphicsTypesCaster::sggFaceTypeToGL(newMeshRenderState.m_facesCullingFaceType));
            glFrontFace(GLGraphicsTypesCaster::sggPolygonsOrderToGL(
                    newMeshRenderState.m_facesCullingPolygonsOrder)
            );

            m_cachedMeshRenderState.m_useFacesCulling = true;
            m_cachedMeshRenderState.m_facesCullingFaceType = newMeshRenderState.m_facesCullingFaceType;
            m_cachedMeshRenderState.m_facesCullingPolygonsOrder = newMeshRenderState.m_facesCullingPolygonsOrder;
        }
        else
        {
            glDisable(GL_CULL_FACE);

            m_cachedMeshRenderState.m_useFacesCulling = false;
        }

        glLineWidth(newMeshRenderState.m_linesWidth);
#ifdef GL_VERSION_1_0
        glPointSize(newMeshRenderState.m_pointsSize);
#else
#warning "Please, implement point size set"
#endif

        m_cachedMeshRenderState.m_linesWidth = newMeshRenderState.m_linesWidth;
        m_cachedMeshRenderState.m_pointsSize = newMeshRenderState.m_pointsSize;

        glPatchParameteri(GL_PATCH_VERTICES, newMeshRenderState.m_patchVerticesCount);

        m_cachedMeshRenderState.m_patchVerticesCount = newMeshRenderState.m_patchVerticesCount;
    }
    else
    {
        if(m_cachedMeshRenderState == newMeshRenderState) return;

        if(m_cachedMeshRenderState.m_useFacesCulling != newMeshRenderState.m_useFacesCulling)
        {
            if(newMeshRenderState.m_useFacesCulling)
            {
                glEnable(GL_CULL_FACE);
                m_cachedMeshRenderState.m_useFacesCulling = true;
            }
            else
            {
                glDisable(GL_CULL_FACE);
                m_cachedMeshRenderState.m_useFacesCulling = false;
            }
        }

        if(newMeshRenderState.m_useFacesCulling)
        {
            if(m_cachedMeshRenderState.m_facesCullingFaceType != newMeshRenderState.m_facesCullingFaceType)
            {
                glCullFace(GLGraphicsTypesCaster::sggFaceTypeToGL(newMeshRenderState.m_facesCullingFaceType));
                m_cachedMeshRenderState.m_facesCullingFaceType = newMeshRenderState.m_facesCullingFaceType;
            }

            if(m_cachedMeshRenderState.m_facesCullingPolygonsOrder != newMeshRenderState.m_facesCullingPolygonsOrder)
            {
                glFrontFace(GLGraphicsTypesCaster::sggPolygonsOrderToGL(newMeshRenderState.m_facesCullingPolygonsOrder));
                m_cachedMeshRenderState.m_facesCullingPolygonsOrder = newMeshRenderState.m_facesCullingPolygonsOrder;
            }
        }

        if(m_cachedMeshRenderState.m_linesWidth != newMeshRenderState.m_linesWidth)
        {
            glLineWidth(newMeshRenderState.m_linesWidth);
            m_cachedMeshRenderState.m_linesWidth = newMeshRenderState.m_linesWidth;
        }
        if(m_cachedMeshRenderState.m_pointsSize != newMeshRenderState.m_pointsSize)
        {
#ifdef GL_VERSION_1_0
            glPointSize(newMeshRenderState.m_pointsSize);
#else
#warning "Please, implement point size set"
#endif
            m_cachedMeshRenderState.m_pointsSize = newMeshRenderState.m_pointsSize;
        }

        if(newMeshRenderState.m_drawMode == SGDrawMode::SGG_PATCHES)
        {
            if(m_cachedMeshRenderState.m_patchVerticesCount != newMeshRenderState.m_patchVerticesCount)
            {
                glPatchParameteri(GL_PATCH_VERTICES, newMeshRenderState.m_patchVerticesCount);
                m_cachedMeshRenderState.m_patchVerticesCount = newMeshRenderState.m_patchVerticesCount;
            }
        }
    }
}
