//
// Created by stuka on 15.06.2025.
//

#include "Batch.h"

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
    m_batchRenderState.m_useFacesCulling = false;
    m_batchRenderState.m_useIndices = false;

    m_fakeVertexArray = Ref<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
    m_fakeVertexArray->create()->bind();

    m_fakeVerticesBuffer = Ref<IVertexBuffer>(CoreMain::getRenderer()->createVertexBuffer());

    m_fakeVerticesBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_instanceTriangles);

    std::shared_ptr<IVertexBufferLayout> bufferLayout = Ref<IVertexBufferLayout>(CoreMain::getRenderer()->createVertexBufferLayout());

    // ---------- preparing uvs offsets0 ---------------

    auto instanceTriangleAttrib = bufferLayout->createVertexAttribute(0,
                                                               "instanceTriangle",
                                                               SGGDataType::SGG_INT2,
                                                               2,
                                                               false,
                                                               sizeof(BatchTriangle),
                                                               0,
                                                               0);

    bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(instanceTriangleAttrib))->prepare()->enableAttributes();

    std::uint8_t currentVertexAttribID = 1;

    // ---------- preparing uvs offsets ---------------
    for(std::uint8_t i = 0; i < texture_types_count / 2; ++i)
    {
        auto uvOffsetsAttrib = bufferLayout->createVertexAttribute(currentVertexAttribID,
                                                               "uvOffsets" + std::to_string(i),
                                                               SGGDataType::SGG_INT4,
                                                               4,
                                                               false,
                                                               sizeof(BatchTriangle),
                                                               offsetof(BatchTriangle, m_atlasesUVsOffset) + i * sizeof(glm::u32vec4),
                                                               0);

        bufferLayout->reset();
        bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(uvOffsetsAttrib))->prepare()->enableAttributes();

        ++currentVertexAttribID;
    }

    // ==============================================================
    // creating true buffers

    m_verticesBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_verticesBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_verticesBuffer->m_isTextureBuffer = true;
    m_verticesBuffer->create(m_vertices.data(), 0, 1, 1,
                             SGGColorInternalFormat::SGG_RGB32_FLOAT,
                             SGGColorFormat::SGG_RGB);

    m_indicesBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_indicesBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_indicesBuffer->m_isTextureBuffer = true;
    m_indicesBuffer->create(m_indices.data(), 0, 1, 1,
                            SGGColorInternalFormat::SGG_RGB32_UNSIGNED_INT,
                            SGGColorFormat::SGG_RGB_INTEGER);

    m_instancesTransformsBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_instancesTransformsBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_instancesTransformsBuffer->m_isTextureBuffer = true;
    m_instancesTransformsBuffer->create(m_transforms.data(), 0, 1, 1,
                                        SGGColorInternalFormat::SGG_RGBA32_FLOAT,
                                        SGGColorFormat::SGG_RGBA);

    auto currentRenderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();
    if(currentRenderPipeline)
    {
        m_shader = Ref<IShader>(CoreMain::getRenderer()->createShader());
        m_shader->compile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
                *currentRenderPipeline->m_shadersPaths["BatchingShader"]));
    }

    // m_atlas.m_maxSideSize = 10000;
}

void SGCore::Batch::insertEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) noexcept
{
    insertEntityImpl(entity, fromRegistry, true);
}

void SGCore::Batch::insertEntities(const std::vector<ECS::entity_t>& entities, const ECS::registry_t& fromRegistry) noexcept
{
    for(auto e : entities)
    {
        insertEntityImpl(e, fromRegistry, false);
    }

    updateTextureDataInTriangles();
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

    if(m_instancesTransformsBuffer->getWidth() < m_transforms.size() * BatchInstanceTransform::components_count)
    {
        m_instancesTransformsBuffer->resizeDataBuffer(m_transforms.size() * BatchInstanceTransform::components_count, 1);
    }
    m_instancesTransformsBuffer->bind(2);
    m_instancesTransformsBuffer->subTextureBufferData(reinterpret_cast<std::uint8_t*>(m_transforms.data()), m_transforms.size() * BatchInstanceTransform::components_count, 0);
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

    m_atlas.getTexture()->bind(4);
    /*for(std::uint8_t i = 0; i < m_atlases.size(); ++i)
    {
        const auto& atlas = m_atlases[i];

        if(atlas.getTexture())
        {
            atlas.getTexture()->bind(3 + i);
        }
    }*/
}

SGCore::Ref<SGCore::IVertexArray> SGCore::Batch::getVertexArray() const noexcept
{
    return m_fakeVertexArray;
}

const SGCore::Atlas& SGCore::Batch::getAtlas() const noexcept
{
    return m_atlas;
}

size_t SGCore::Batch::getTrianglesCount() const noexcept
{
    return m_instanceTriangles.size();
}

void SGCore::Batch::insertEntityImpl(ECS::entity_t entity, const ECS::registry_t& fromRegistry, bool isUpdateBuffers) noexcept
{
    const auto* tmpMesh = fromRegistry.tryGet<Mesh>(entity);
    if(!tmpMesh || !fromRegistry.allOf<Transform>(entity) || !tmpMesh->m_base.getMeshData()) return;

    const Mesh& mesh = *tmpMesh;

    const auto meshData = mesh.m_base.getMeshData();

    const size_t meshDataHash = meshData->getHash();

    // two bytes of x component - texture insertion position, two bytes of y component - texture size
    std::array<glm::u32vec2, texture_types_count> atlasTextureInfo { };
    std::array<size_t, texture_types_count> usedTexturesOfMesh { };

    // =====================================================================
    // packing textures

    if(mesh.m_base.getMaterial())
    {
        m_shader->bind();

        for(std::uint8_t i = 0; i < mesh.m_base.getMaterial()->getTextures().size(); ++i)
        {
            const auto textureType = static_cast<SGTextureType>(i);
            const auto& texturesVec = mesh.m_base.getMaterial()->getTextures()[i];

            // auto& atlas = m_atlases[i];

            for(const auto& texture : texturesVec)
            {
                if(!texture) continue;

                const size_t textureHash = texture->getHash();

                rectpack2D::rect_xywh rect;

                // inserting new texture...
                if(!m_usedTextures.contains(textureHash))
                {
                    m_atlas.findBestRect({ texture->getWidth(), texture->getHeight() }, rect, textureHash);
                    m_atlas.packTexture(rect, texture.get());

                    m_usedTextures[textureHash] = {
                        .m_textureType = i,
                        .m_insertionPosition = { rect.x, rect.y },
                        .m_insertionSize = { rect.w, rect.h }
                    };

                    std::cout << fmt::format("adding texture to atlas: texture type: {}, rect pos: {}, {}, rect size: {}, {}, texture path: '{}'",
                        sgStandardTextureTypeToString(textureType),
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h,
                        Utils::toUTF8(texture->getPath().resolved().u16string())) << std::endl;

                    m_shader->useTextureBlock("batchAtlas", 4);
                    m_shader->useVectorf("batchAtlasSize", glm::vec2 { m_atlas.getTexture()->getWidth(), m_atlas.getTexture()->getHeight() });
                }

                rect = *m_atlas.getRectByHash(textureHash);

                const auto& textureMarkup = m_usedTextures[textureHash];

                atlasTextureInfo[i].x = pack2UInt16ToUInt32(
                    static_cast<std::uint16_t>(textureMarkup.m_insertionPosition.x),
                    static_cast<std::uint16_t>(textureMarkup.m_insertionPosition.y)
                );

                atlasTextureInfo[i].y = pack2UInt16ToUInt32(
                    static_cast<std::uint16_t>(rect.w),
                    static_cast<std::uint16_t>(rect.h)
                );

                usedTexturesOfMesh[i] = textureHash;
            }
        }
    }

    // =====================================================================

    const bool meshDataMarkupFound = m_usedMeshDatas.contains(meshDataHash);
    if(!meshDataMarkupFound)
    {
        // adding new meshdata

        m_usedMeshDatas[meshDataHash] = {
            .m_verticesOffset = m_vertices.size(),
            .m_verticesCount = meshData->m_vertices.size(),

            .m_indicesOffset = m_indices.size() * 3,
            .m_indicesCount = meshData->m_indices.size(),

            .m_textures = usedTexturesOfMesh
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

        for(size_t i = 0; i < indicesCount; i += 3)
        {
            // lastVertCount is offset for indices
            m_indices.push_back(glm::u32vec3 {
                lastVertCount + meshData->m_indices[i],
                lastVertCount + meshData->m_indices[i + 1],
                lastVertCount + meshData->m_indices[i + 2]
            });
        }
    }

    auto& meshDataMarkup = m_usedMeshDatas[meshDataHash];

    const auto lastTrianglesCount = m_instanceTriangles.size();

    for(std::int32_t i = meshDataMarkup.m_indicesOffset; i < meshDataMarkup.m_indicesOffset + meshDataMarkup.m_indicesCount; i += 3)
    {
        BatchTriangle batchTriangle {
            .m_meshInstanceID = (std::int32_t) m_entities.size(),
            .m_triangleID = i / 3,
            .m_atlasesUVsOffset = atlasTextureInfo
        };

        m_instanceTriangles.push_back(batchTriangle);
    }

    m_trianglesMarkup[meshDataHash].push_back({
        .m_trianglesOffset = lastTrianglesCount,
        .m_trianglesCount = meshDataMarkup.m_indicesCount / 3,
    });

    m_entities.push_back(entity);

    if(isUpdateBuffers)
    {
        updateTextureDataInTriangles();
        updateBuffers();
    }
}

void SGCore::Batch::updateTextureDataInTriangles() noexcept
{
    // updating cached textures data markup
    for(auto& [hash, markup] : m_usedTextures)
    {
        const auto& atlasRect = m_atlas.getRectByHash(hash);
        if(!atlasRect) continue;

        markup.m_insertionPosition = { atlasRect->x, atlasRect->y };
        markup.m_insertionSize = { atlasRect->w, atlasRect->h };
    }

    for(auto& [meshDataHash, meshDataMarkup] : m_usedMeshDatas)
    {
        std::array<glm::u32vec2, texture_types_count> atlasTextureInfo { };

        // collecting info about textures...
        for(std::uint8_t i = 0; i < texture_types_count; ++i)
        {
            const auto it = m_usedTextures.find(meshDataMarkup.m_textures[i]);
            if(it == m_usedTextures.end()) continue;

            const auto& textureDataMarkup = it->second;

            atlasTextureInfo[i].x = pack2UInt16ToUInt32(
                static_cast<std::uint16_t>(textureDataMarkup.m_insertionPosition.x),
                static_cast<std::uint16_t>(textureDataMarkup.m_insertionPosition.y)
            );

            atlasTextureInfo[i].y = pack2UInt16ToUInt32(
                static_cast<std::uint16_t>(textureDataMarkup.m_insertionSize.x),
                static_cast<std::uint16_t>(textureDataMarkup.m_insertionSize.y)
            );
        }

        // updating info of texture in triangles of batch
        const auto& trianglesMarkups = m_trianglesMarkup[meshDataHash];
        for(const auto& trianglesMarkup : trianglesMarkups)
        {
            for(size_t j = trianglesMarkup.m_trianglesOffset; j < trianglesMarkup.m_trianglesOffset + trianglesMarkup.m_trianglesCount; ++j)
            {
                m_instanceTriangles[j].m_atlasesUVsOffset = atlasTextureInfo;
            }
        }
    }
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

    if(m_verticesBuffer->getWidth() < m_vertices.size() * BatchVertex::components_count)
    {
        m_verticesBuffer->resizeDataBuffer(m_vertices.size() * BatchVertex::components_count, 1);
    }
    m_verticesBuffer->bind(0);
    m_verticesBuffer->subTextureBufferData(reinterpret_cast<std::uint8_t*>(m_vertices.data()), m_vertices.size() * BatchVertex::components_count, 0);

    if(m_indicesBuffer->getWidth() < m_indices.size())
    {
        m_indicesBuffer->resizeDataBuffer(m_indices.size(), 1);
    }
    m_indicesBuffer->bind(1);
    m_indicesBuffer->subTextureBufferData(reinterpret_cast<std::uint8_t*>(m_indices.data()), m_indices.size(), 0);
}

std::uint32_t SGCore::Batch::pack2UInt16ToUInt32(uint16_t x, uint16_t y) noexcept
{
    return (static_cast<uint32_t>(x) << 16) + y;
}
