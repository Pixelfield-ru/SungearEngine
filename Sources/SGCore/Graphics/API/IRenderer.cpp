#include "IRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Memory/AssetManager.h"

void SGCore::IRenderer::init() noexcept
{
    // Shared uniform blocks of the engine (camera and per-frame program data). They are backend
    // agnostic — a layout of IShaderUniform plus an IUniformBuffer the backend implements — so every
    // renderer gets them here instead of each backend's init().
    // TODO: make defines for uniforms names
    m_viewMatricesBuffer = Ref<IUniformBuffer>(createUniformBuffer());
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

    m_programDataBuffer = Ref<IUniformBuffer>(createUniformBuffer());
    m_programDataBuffer->m_blockName = "ProgramDataBlock";
    m_programDataBuffer->putUniforms({
        IShaderUniform("programData.windowSize", SGGDataType::SGG_FLOAT2),
        IShaderUniform("programData.primaryMonitorSize", SGGDataType::SGG_FLOAT2),
        IShaderUniform("programData.p0", SGGDataType::SGG_FLOAT),
        IShaderUniform("programData.currentTime", SGGDataType::SGG_FLOAT)
    });
    m_programDataBuffer->setLayoutLocation(2);
    m_programDataBuffer->prepare();

    m_screenQuadMeshRenderState.m_useFacesCulling = false;

    m_screenShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");

    m_screenQuadMesh = Ref<IMeshData>(createMeshData());

    m_screenQuadMesh->m_vertices.resize(4);

    m_screenQuadMesh->m_vertices[0] = {
        .m_position = glm::vec3 { -1, -1, 0.0f },
        .m_uv = glm::vec3 { 0.0f, 0.0f, 0.0f },
        .m_normal = { 0.0f, 1.0f, 0.0f }
    };

    m_screenQuadMesh->m_vertices[1] = {
        .m_position = glm::vec3 { -1, 1, 0.0f },
        .m_uv = glm::vec3 { 0.0f, 1.0f, 0.0f },
        .m_normal = { 0.0f, 1.0f, 0.0f }
    };

    m_screenQuadMesh->m_vertices[2] = {
        .m_position = glm::vec3 { 1, 1, 0.0f },
        .m_uv = glm::vec3 { 1.0f, 1.0f, 0.0f },
        .m_normal = { 0.0f, 1.0f, 0.0f }
    };

    m_screenQuadMesh->m_vertices[3] = {
        .m_position = glm::vec3 { 1, -1, 0.0f },
        .m_uv = glm::vec3 { 1.0f, 0.0f, 0.0f },
        .m_normal = { 0.0f, 1.0f, 0.0f }
    };

    m_screenQuadMesh->m_indices.resize(6);

    m_screenQuadMesh->m_indices[0] = 0;
    m_screenQuadMesh->m_indices[1] = 2;
    m_screenQuadMesh->m_indices[2] = 1;
    m_screenQuadMesh->m_indices[3] = 0;
    m_screenQuadMesh->m_indices[4] = 3;
    m_screenQuadMesh->m_indices[5] = 2;

    m_screenQuadMesh->prepare();
}

void SGCore::IRenderer::renderTextureOnScreen(const ITexture2D* texture, bool flipOutput) noexcept
{
    if(!texture)
    {
        return;
    }

    int wndWidth = 0;
    int wndHeight = 0;
    CoreMain::getWindow().getSize(wndWidth, wndHeight);
    renderTextureOnScreen(texture, flipOutput, 0, 0, wndWidth, wndHeight);
}

void SGCore::IRenderer::renderTextureOnScreen(const ITexture2D* texture,
                                              bool flipOutput,
                                              int x,
                                              int y,
                                              int width,
                                              int height) noexcept
{
    if(!texture || width <= 0 || height <= 0)
    {
        return;
    }

    bindScreenFrameBuffer();
    setViewport(x, y, width, height);

    // trying to reload screen shader
    // todo: bad idea
    if(!m_screenShader)
    {
        m_screenShader = AssetManager::getInstance()->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");
    }

    m_screenShader->bind();

    m_screenShader->useInteger("u_flipOutput", flipOutput);

    texture->bind(0);
    m_screenShader->useTextureBlock("u_bufferToDisplay", 0);

    renderArray(
        m_screenQuadMesh->getVertexArray(),
        m_screenQuadMeshRenderState,
        m_screenQuadMesh->m_vertices.size(),
        m_screenQuadMesh->m_indices.size()
    );

    int wndWidth = 0;
    int wndHeight = 0;
    CoreMain::getWindow().getSize(wndWidth, wndHeight);
    setViewport(0, 0, wndWidth, wndHeight);
}

SGCore::GAPIType SGCore::IRenderer::getGAPIType() const noexcept
{
    return m_apiType;
}

SGCore::RenderState& SGCore::IRenderer::getCachedRenderState() noexcept
{
    return m_cachedRenderState;
}

SGCore::MeshRenderState& SGCore::IRenderer::getCachedMeshRenderState() noexcept
{
    return m_cachedMeshRenderState;
}

void SGCore::IRenderer::prepareUniformBuffers(const RenderingBase& renderingBase,
                                                const Transform& transform)
{
    // double t0 = glfwGetTime();

    m_viewMatricesBuffer->bind();

    m_viewMatricesBuffer->subData("camera.projectionSpaceMatrix",
                                  glm::value_ptr(renderingBase.m_projectionSpaceMatrix), 16
    );
    m_viewMatricesBuffer->subData("camera.orthographicSpaceMatrix",
                                  glm::value_ptr(renderingBase.m_orthographicSpaceMatrix), 16
    );
    m_viewMatricesBuffer->subData("camera.orthographicMatrix",
                                  glm::value_ptr(renderingBase.m_orthographicMatrix), 16
    );
    m_viewMatricesBuffer->subData("camera.projectionMatrix",
                                  glm::value_ptr(renderingBase.m_projectionMatrix), 16
    );
    m_viewMatricesBuffer->subData("camera.viewMatrix",
                                  glm::value_ptr(renderingBase.m_viewMatrix), 16
    );

    m_viewMatricesBuffer->subData("camera.position",
                                         glm::value_ptr(transform.m_worldTransform.m_position), 3
    );

    m_viewMatricesBuffer->subData("camera.zFar",
                                  &renderingBase.m_zFar, 1
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
