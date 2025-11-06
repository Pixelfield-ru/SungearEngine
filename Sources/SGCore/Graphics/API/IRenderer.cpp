#include "IRenderer.h"

#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ITexture2D.h"

void SGCore::IRenderer::init() noexcept
{
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
    bindScreenFrameBuffer();

    // trying to reload screen shader
    // todo: bad idea
    if(!m_screenShader)
    {
        m_screenShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");
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
}

SGCore::GAPIType SGCore::IRenderer::getGAPIType() const noexcept
{
    return m_apiType;
}

SGCore::RenderState& SGCore::IRenderer::getCachedRenderState() noexcept
{
    return m_cachedRenderState;
}
