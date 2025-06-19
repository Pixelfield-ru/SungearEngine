//
// Created by stuka on 15.06.2025.
//

#include "Batch.h"

#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "SGCore/ECS/Registry.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexAttribute.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/RenderPipelinesManager.h"

SGCore::Batch::Batch() noexcept
{
    m_batchRenderState.m_drawMode = SGDrawMode::SGG_POINTS;
    m_batchRenderState.m_useIndices = false;

    m_fakeVertexArray = Ref<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
    m_fakeVertexArray->create()->bind();

    m_fakeVerticesBuffer = Ref<IVertexBuffer>(CoreMain::getRenderer()->createVertexBuffer());

    m_fakeVerticesBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_instanceTriangles);

    std::shared_ptr<IVertexBufferLayout> bufferLayout = Ref<IVertexBufferLayout>(CoreMain::getRenderer()->createVertexBufferLayout());
    auto instanceTriangleAttrib = bufferLayout->createVertexAttribute(0,
                                                                      "instanceTriangle",
                                                                      SGGDataType::SGG_INT2);

    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(instanceTriangleAttrib))->prepare()->enableAttributes();

    // ==============================================================
    // creating true buffers

    m_verticesBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_verticesBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_verticesBuffer->m_isTextureBuffer = true;
    m_verticesBuffer->create(reinterpret_cast<char*>(m_vertices.data()), m_vertices.size(), 1, 1,
                             SGGColorInternalFormat::SGG_RGB32_FLOAT,
                             SGGColorFormat::SGG_RGB);

    m_indicesBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_indicesBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_indicesBuffer->m_isTextureBuffer = true;
    m_indicesBuffer->create(reinterpret_cast<char*>(m_indices.data()), m_indices.size(), 1, 1,
                            SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT,
                            SGGColorFormat::SGG_RGB);

    m_instancesTransformsBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_instancesTransformsBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_instancesTransformsBuffer->m_isTextureBuffer = true;
    m_instancesTransformsBuffer->create(reinterpret_cast<char*>(m_transforms.data()), m_transforms.size(), 1, 1,
                                        SGGColorInternalFormat::SGG_RGBA32_FLOAT,
                                        SGGColorFormat::SGG_RGBA);

    auto currentRenderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();
    if(currentRenderPipeline)
    {
        m_shader = Ref<IShader>(CoreMain::getRenderer()->createShader());
        m_shader->compile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
                *currentRenderPipeline->m_shadersPaths["BatchingShader"]));
    }
}

void SGCore::Batch::addEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) noexcept
{
    const auto* tmpMesh = fromRegistry.tryGet<Mesh>(entity);
    if(!tmpMesh || !fromRegistry.allOf<Transform>(entity) || !tmpMesh->m_base.getMeshData()) return;

    const Mesh& mesh = *tmpMesh;

    const auto meshData = mesh.m_base.getMeshData();

    const size_t meshDataHash = meshData->getHash();

    /*if(mesh.m_base.getMaterial())
    {
        for(size_t i = 0; i < mesh.m_base.getMaterial()->getTextures().size(); ++i)
        {
            const auto& texturesVec = mesh.m_base.getMaterial()->getTextures()[i];
            for(const auto& texture : texturesVec)
            {
                const size_t textureHash = texture->getHash();
            }
        }
    }*/

    auto it = m_usedMeshDatas.find(meshDataHash);
    const bool meshDataMarkupFound = it != m_usedMeshDatas.end();
    if(!meshDataMarkupFound)
    {
        // adding new meshdata

        m_usedMeshDatas[meshDataHash] = {
            .m_verticesOffset = m_vertices.size(),
            .m_verticesCount = meshData->m_vertices.size(),

            .m_indicesOffset = m_indices.size(),
            .m_indicesCount = meshData->m_indices.size()
        };

        const size_t lastVertCount = m_vertices.size();

        m_vertices.resize(m_vertices.size() + meshData->m_vertices.size());

        for(size_t i = 0; i < meshData->m_vertices.size(); ++i)
        {
            const auto& meshVertex = meshData->m_vertices[i];

            const BatchVertex batchVertex {
                .m_position = meshVertex.m_position,
                .m_uv = meshVertex.m_uv,
                .m_normal = meshVertex.m_normal,
                .m_tangent = meshVertex.m_tangent,
                .m_bitangent = meshVertex.m_bitangent
            };

            m_vertices[lastVertCount + i] = batchVertex;
        }

        const bool hasIndices = !meshData->m_indices.empty();
        const size_t indicesCount = hasIndices ? meshData->m_indices.size() : meshData->m_vertices.size() / 3;

        for(size_t i = 0; i < indicesCount; ++i)
        {
            // lastVertCount is offset for indices
            m_indices.push_back(lastVertCount + meshData->m_indices[i]);
        }
    }

    auto& meshDataMarkup = m_usedMeshDatas[meshDataHash];

    for(size_t i = meshDataMarkup.m_indicesOffset; i < meshDataMarkup.m_indicesOffset + meshDataMarkup.m_indicesCount; i += 3)
    {
        const glm::ivec2 fakeVertex { m_entities.size(), i / 3 };

        m_instanceTriangles.push_back(fakeVertex);
    }

    m_entities.push_back(entity);

    updateBuffers();
}

void SGCore::Batch::update(const ECS::registry_t& inRegistry) noexcept
{
    m_transforms.clear();

    for(size_t i = 0; i < m_entities.size(); ++i)
    {
        const auto entity = m_entities[i];

        const auto* tmpTransform = inRegistry.tryGet<Transform>(entity);
        if(!tmpTransform) continue; // todo: maybe delete from batch?

        const auto& transform = *tmpTransform;

        const BatchInstanceTransform instanceTransform {
            .m_modelMatrix = transform->m_finalTransform.m_animatedModelMatrix,
            .m_position = transform->m_finalTransform.m_position,
            .m_rotation = glm::degrees(glm::eulerAngles(transform->m_finalTransform.m_rotation)),
            .m_scale = transform->m_finalTransform.m_scale
        };

        m_transforms.push_back(instanceTransform);
    }

    if(m_instancesTransformsBuffer->getWidth() < m_transforms.size() * sizeof(BatchInstanceTransform))
    {
        m_instancesTransformsBuffer->resize(m_transforms.size() * sizeof(BatchInstanceTransform), 1);
    }
    m_instancesTransformsBuffer->bind(2);
    m_instancesTransformsBuffer->subTextureBufferData(reinterpret_cast<char*>(m_transforms.data()), m_transforms.size() * sizeof(BatchInstanceTransform), 0);
}

void SGCore::Batch::bind() const noexcept
{
    if(!m_shader) return;

    m_shader->bind();

    m_shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
    m_shader->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);

    m_verticesBuffer->bind(0);
    m_indicesBuffer->bind(1);
    m_instancesTransformsBuffer->bind(2);

    m_shader->useTextureBlock("u_verticesTextureBuffer", 0);
    m_shader->useTextureBlock("u_indicesTextureBuffer", 1);
    m_shader->useTextureBlock("u_transformsTextureBuffer", 2);
}

SGCore::Ref<SGCore::IVertexArray> SGCore::Batch::getVertexArray() const noexcept
{
    return m_fakeVertexArray;
}

size_t SGCore::Batch::getTrianglesCount() const noexcept
{
    return m_instanceTriangles.size();
}

void SGCore::Batch::onRenderPipelineSet() noexcept
{
    m_shader = Ref<IShader>(CoreMain::getRenderer()->createShader());
    m_shader->compile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
            *RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths["BatchingShader"]));
}

void SGCore::Batch::updateBuffers() noexcept
{
    m_fakeVerticesBuffer->bind();
    m_fakeVerticesBuffer->putData(m_instanceTriangles);

    if(m_verticesBuffer->getWidth() < m_vertices.size() * sizeof(BatchVertex))
    {
        m_verticesBuffer->resize(m_vertices.size() * sizeof(BatchVertex), 1);
    }
    m_verticesBuffer->bind(0);
    m_verticesBuffer->subTextureBufferData(reinterpret_cast<char*>(m_vertices.data()), m_vertices.size() * sizeof(BatchVertex), 0);

    if(m_indicesBuffer->getWidth() < m_indices.size() * sizeof(float))
    {
        m_indicesBuffer->resize(m_indices.size() * sizeof(float), 1);
    }
    m_indicesBuffer->bind(1);
    m_indicesBuffer->subTextureBufferData(reinterpret_cast<char*>(m_indices.data()), m_indices.size() * sizeof(float), 0);
}
