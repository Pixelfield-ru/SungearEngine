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

    m_instancesMaterialsBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    m_instancesMaterialsBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
    m_instancesMaterialsBuffer->m_isTextureBuffer = true;
    m_instancesMaterialsBuffer->create(m_transforms.data(), 0, 1, 1,
                                       SGGColorInternalFormat::SGG_RGBA32_FLOAT,
                                       SGGColorFormat::SGG_RGBA);

    // m_atlas.m_maxSideSize = 10000;
}

bool SGCore::Batch::insertEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) noexcept
{
    if(!hasSpaceForEntity(entity, fromRegistry)) return false;

    insertEntityImpl(entity, fromRegistry, true);

    return true;
}

std::uint64_t SGCore::Batch::insertEntities(const std::vector<ECS::entity_t>& entities, const ECS::registry_t& fromRegistry) noexcept
{
    std::uint64_t lastUnsuccessfulEntityIndex = -1;

    for(std::uint64_t i = 0; i < std::ssize(entities); ++i)
    {
        const auto e = entities[i];

        if(!hasSpaceForEntity(e, fromRegistry))
        {
            lastUnsuccessfulEntityIndex = i;
            break;
        }

        insertEntityImpl(e, fromRegistry, false);
    }

    updateTextureDataInTriangles();
    updateBuffers();

    return lastUnsuccessfulEntityIndex;
}

std::uint64_t SGCore::Batch::insertEntities(const std::vector<ECS::entity_t>::iterator& entitiesBegin,
                                            const std::vector<ECS::entity_t>::iterator& entitiesEnd,
                                            const ECS::registry_t& fromRegistry) noexcept
{
    std::uint64_t lastUnsuccessfulEntityIndex = -1;

    const std::uint64_t vecSize = entitiesEnd - entitiesBegin;

    for(std::uint64_t i = 0; i < vecSize; ++i)
    {
        const auto e = *(entitiesBegin + i);

        if(!hasSpaceForEntity(e, fromRegistry))
        {
            lastUnsuccessfulEntityIndex = i;
            break;
        }

        insertEntityImpl(e, fromRegistry, false);
    }

    updateTextureDataInTriangles();
    updateBuffers();

    return lastUnsuccessfulEntityIndex;
}

void SGCore::Batch::removeEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry, bool immediateRemove) noexcept
{
    if(entity == entt::null) return;

    if(!immediateRemove)
    {
        m_entitiesToRemove.push_back(entity);
        return;
    }

    std::erase(m_entities, entity);

    // clearing buffers
    m_instanceTriangles.clear();
    m_vertices.clear();
    m_indices.clear();
    m_usedMeshDatas.clear();
    m_trianglesMarkup.clear();

    const std::vector<ECS::entity_t> newEntities = std::move(m_entities);
    m_entities = { };

    // adding all entities
    insertEntities(newEntities, fromRegistry);
}

void SGCore::Batch::removeEntities(const std::vector<ECS::entity_t>& entities, const ECS::registry_t& fromRegistry, bool immediateRemove) noexcept
{
    if(entities.empty()) return;

    if(!immediateRemove)
    {
        for(const auto e : entities)
        {
            m_entitiesToRemove.push_back(e);
        }
        return;
    }

    for(const auto e : entities)
    {
        std::erase(m_entities, e);
    }

    // clearing buffers
    m_instanceTriangles.clear();
    m_vertices.clear();
    m_indices.clear();
    m_usedMeshDatas.clear();
    m_trianglesMarkup.clear();

    const std::vector<ECS::entity_t> newEntities = std::move(m_entities);
    m_entities = { };

    // adding all entities
    insertEntities(newEntities, fromRegistry);
}

void SGCore::Batch::shrinkBuffersToFit() noexcept
{
    if(m_verticesBuffer->getWidth() > m_vertices.size() * BatchVertex::components_count)
    {
        m_verticesBuffer->resizeDataBuffer(m_vertices.size() * BatchVertex::components_count, 1, false);
    }
    m_verticesBuffer->bind(0);
    m_verticesBuffer->subTextureBufferData(reinterpret_cast<std::uint8_t*>(m_vertices.data()), m_vertices.size() * BatchVertex::components_count, 0);

    if(m_indicesBuffer->getWidth() > m_indices.size())
    {
        m_indicesBuffer->resizeDataBuffer(m_indices.size(), 1, false);
    }
    m_indicesBuffer->bind(1);
    m_indicesBuffer->subTextureBufferData(reinterpret_cast<std::uint8_t*>(m_indices.data()), m_indices.size(), 0);

    if(m_instancesTransformsBuffer->getWidth() > m_entities.size() * BatchInstanceTransform::components_count)
    {
        m_instancesTransformsBuffer->resizeDataBuffer(m_entities.size() * BatchInstanceTransform::components_count, 1, false);
    }

    if(m_instancesMaterialsBuffer->getWidth() > m_entities.size() * BatchInstanceMaterial::components_count)
    {
        m_instancesMaterialsBuffer->resizeDataBuffer(m_entities.size() * BatchInstanceMaterial::components_count, 1, false);
    }
}

bool SGCore::Batch::hasSpaceForEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) const noexcept
{
    const auto* tmpMesh = fromRegistry.tryGet<Mesh>(entity);
    if(!tmpMesh || !fromRegistry.allOf<Transform>(entity) || !tmpMesh->m_base.getMeshData()) return true;

    const Mesh& mesh = *tmpMesh;

    const auto meshData = mesh.m_base.getMeshData();

    // checking available space for textures
    if(mesh.m_base.getMaterial())
    {
        for(std::uint8_t i = 0; i < mesh.m_base.getMaterial()->getTextures().size(); ++i)
        {
            const auto& texturesVec = mesh.m_base.getMaterial()->getTextures()[i];

            for(const auto& texture : texturesVec)
            {
                if(!texture) continue;

                if(!m_usedTextures.contains(texture->getHash()) && m_atlas.getSize().x + texture->getWidth() > m_atlas.m_maxSideSize && m_atlas.getSize().y + texture->getHeight() > m_atlas.m_maxSideSize)
                {
                    return false;
                }
            }
        }
    }

    // checking available space for buffers

    const bool hasIndices = !meshData->m_indices.empty();
    const size_t indicesCount = hasIndices ? meshData->m_indices.size() : meshData->m_vertices.size() / 3;

    if(!m_usedMeshDatas.contains(meshData->getHash()))
    {
        if(sizeof(BatchVertex) * (m_vertices.size() + meshData->m_vertices.size()) > GPUDeviceInfo::getMaxTextureBufferSize())
        {
            return false;
        }

        if(sizeof(glm::u32vec3) * (m_indices.size() + indicesCount) > GPUDeviceInfo::getMaxTextureBufferSize())
        {
            return false;
        }
    }

    if(sizeof(BatchTriangle) * (m_instanceTriangles.size() + indicesCount / 3) > GPUDeviceInfo::getMaxTextureBufferSize())
    {
        return false;
    }

    if(sizeof(BatchInstanceTransform) * (m_transforms.size() + 1) > GPUDeviceInfo::getMaxTextureBufferSize())
    {
        return false;
    }

    if(sizeof(BatchInstanceMaterial) * (m_materials.size() + 1) > GPUDeviceInfo::getMaxTextureBufferSize())
    {
        return false;
    }

    return true;
}

void SGCore::Batch::setAtlasMaxSide(std::uint32_t maxSide) noexcept
{
    m_atlas.m_maxSideSize = maxSide;
}

void SGCore::Batch::update(const ECS::registry_t& inRegistry) noexcept
{
    m_transforms.clear();
    m_materials.clear();

    for(size_t i = 0; i < m_entities.size(); ++i)
    {
        const auto entity = m_entities[i];

        if(!inRegistry.valid(entity))
        {
            m_entitiesToRemove.push_back(entity);
            continue;
        }

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

        const auto* mesh = inRegistry.tryGet<Mesh>(entity);
        if(!mesh)
        {
            m_materials.push_back(m_defaultMaterial);
            continue;
        }

        const auto meshMaterial = mesh->m_base.getMaterial();
        if(!meshMaterial)
        {
            m_materials.push_back(m_defaultMaterial);
            continue;
        }

        const BatchInstanceMaterial instanceMaterial {
            .m_diffuseColor = meshMaterial->getDiffuseColor(),
            .m_specularColor = meshMaterial->getSpecularColor(),
            .m_ambientColor = meshMaterial->getAmbientColor(),
            .m_emissionColor = meshMaterial->getEmissionColor(),
            .m_transparentColor = meshMaterial->getTransparentColor(),
            .m_shininessMetallicRoughness = { meshMaterial->getShininess(), meshMaterial->getMetallicFactor(), meshMaterial->getRoughnessFactor() }
        };

        m_materials.push_back(instanceMaterial);
    }

    // deferred remove
    removeEntities(m_entitiesToRemove, inRegistry, true);
    m_entitiesToRemove.clear();

    if(m_instancesTransformsBuffer->getWidth() < m_transforms.size() * BatchInstanceTransform::components_count)
    {
        m_instancesTransformsBuffer->resizeDataBuffer(m_transforms.size() * BatchInstanceTransform::components_count, 1, false);
    }
    m_instancesTransformsBuffer->bind(2);
    m_instancesTransformsBuffer->subTextureBufferData(reinterpret_cast<std::uint8_t*>(m_transforms.data()), m_transforms.size() * BatchInstanceTransform::components_count, 0);

    // ============================================================

    if(m_instancesMaterialsBuffer->getWidth() < m_materials.size() * BatchInstanceMaterial::components_count)
    {
        m_instancesMaterialsBuffer->resizeDataBuffer(m_materials.size() * BatchInstanceMaterial::components_count, 1, false);
    }
    m_instancesMaterialsBuffer->bind(3);
    m_instancesMaterialsBuffer->subTextureBufferData(reinterpret_cast<std::uint8_t*>(m_materials.data()), m_materials.size() * BatchInstanceMaterial::components_count, 0);
}

void SGCore::Batch::bind(IShader* shader) const noexcept
{
    if(!shader) return;

    shader->bind();

    shader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
    shader->useUniformBuffer(CoreMain::getRenderer()->m_programDataBuffer);

    m_verticesBuffer->bind(0);
    m_indicesBuffer->bind(1);
    m_instancesTransformsBuffer->bind(2);
    m_instancesMaterialsBuffer->bind(3);

    shader->useTextureBlock("u_verticesTextureBuffer", 0);
    shader->useTextureBlock("u_indicesTextureBuffer", 1);
    shader->useTextureBlock("u_transformsTextureBuffer", 2);
    shader->useTextureBlock("u_materialsTextureBuffer", 3);

    shader->useVectorf("batchAtlasSize", glm::vec2 { m_atlas.getSize().x, m_atlas.getSize().y });

    shader->useTextureBlock("batchAtlas", 4);

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

const std::vector<SGCore::ECS::entity_t>& SGCore::Batch::getEntities() const noexcept
{
    return m_entities;
}

size_t SGCore::Batch::getTrianglesCount() const noexcept
{
    return m_instanceTriangles.size();
}

void SGCore::Batch::insertEntityImpl(ECS::entity_t entity, const ECS::registry_t& fromRegistry, bool isUpdateBuffers) noexcept
{
    if(!hasSpaceForEntity(entity, fromRegistry)) return;

    const auto* tmpMesh = fromRegistry.tryGet<Mesh>(entity);
    if(!tmpMesh || !fromRegistry.allOf<Transform>(entity) || !tmpMesh->m_base.getMeshData()) return;

    const Mesh& mesh = *tmpMesh;

    const auto meshData = mesh.m_base.getMeshData();

    const size_t meshDataHash = meshData->getHash();

    // two bytes of x component - texture insertion position, two bytes of y component - texture size
    std::array<glm::u32vec2, texture_types_count> atlasTextureInfo = makeFilledArray<glm::u32vec2, texture_types_count>({ -1, -1 });
    std::array<size_t, texture_types_count> usedTexturesOfMesh { };

    // =====================================================================
    // packing textures

    if(mesh.m_base.getMaterial())
    {
        for(std::uint8_t i = 0; i < mesh.m_base.getMaterial()->getTextures().size(); ++i)
        {
            const auto textureType = static_cast<SGTextureType>(i);
            const auto& texturesVec = mesh.m_base.getMaterial()->getTextures()[i];

            // auto& atlas = m_atlases[i];

            for(const auto& texture : texturesVec)
            {
                if(!texture) continue;

                // if texture is not acceptable
                if(!m_acceptableTextureTypes.contains(textureType))
                {
                    atlasTextureInfo[i] = { -1, -1 };

                    continue;
                }

                const size_t textureHash = texture->getHash();

                rectpack2D::rect_xywh rect;

                // inserting new texture...
                if(!m_atlas.contains(textureHash))
                {
                    m_atlas.findBestRect({ texture->getWidth(), texture->getHeight() }, rect, textureHash);
                    m_usedTextures[textureHash] = texture;

                    std::cout << fmt::format("adding texture to atlas: texture type: {}, rect pos: {}, {}, rect size: {}, {}, texture path: '{}'",
                        sgStandardTextureTypeToString(textureType),
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h,
                        Utils::toUTF8(texture->getPath().resolved().u16string())) << std::endl;
                }

                rect = *m_atlas.getRectByHash(textureHash);

                atlasTextureInfo[i].x = pack2UInt16ToUInt32(
                    static_cast<std::uint16_t>(rect.x),
                    static_cast<std::uint16_t>(rect.y)
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
    for(auto& [meshDataHash, meshDataMarkup] : m_usedMeshDatas)
    {
        std::array<glm::u32vec2, texture_types_count> atlasTextureInfo = makeFilledArray<glm::u32vec2, texture_types_count>({ -1, -1 });;

        // collecting info about textures...
        for(std::uint8_t i = 0; i < texture_types_count; ++i)
        {
            // if texture of this type is not acceptable
            if(!m_acceptableTextureTypes.contains(static_cast<SGTextureType>(i)))
            {
                atlasTextureInfo[i] = { -1, -1 };
                continue;
            }

            const auto rectPtr = m_atlas.getRectByHash(meshDataMarkup.m_textures[i]);
            if(!rectPtr) continue;

            const auto& rect = *rectPtr;

            atlasTextureInfo[i].x = pack2UInt16ToUInt32(
                static_cast<std::uint16_t>(rect.x),
                static_cast<std::uint16_t>(rect.y)
            );

            atlasTextureInfo[i].y = pack2UInt16ToUInt32(
                static_cast<std::uint16_t>(rect.w),
                static_cast<std::uint16_t>(rect.h)
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

void SGCore::Batch::updateBuffers() noexcept
{
    // packing textures
    for(const auto& [texHash, texture] : m_usedTextures)
    {
        m_atlas.packTexture(*m_atlas.getRectByHash(texHash), texture.get());
    }

    m_fakeVerticesBuffer->bind();
    m_fakeVerticesBuffer->putData(m_instanceTriangles);

    if(m_verticesBuffer->getWidth() < m_vertices.size() * BatchVertex::components_count)
    {
        m_verticesBuffer->resizeDataBuffer(m_vertices.size() * BatchVertex::components_count, 1, false);
    }
    m_verticesBuffer->bind(0);
    m_verticesBuffer->subTextureBufferData(reinterpret_cast<std::uint8_t*>(m_vertices.data()), m_vertices.size() * BatchVertex::components_count, 0);

    if(m_indicesBuffer->getWidth() < m_indices.size())
    {
        m_indicesBuffer->resizeDataBuffer(m_indices.size(), 1, false);
    }
    m_indicesBuffer->bind(1);
    m_indicesBuffer->subTextureBufferData(reinterpret_cast<std::uint8_t*>(m_indices.data()), m_indices.size(), 0);
}

std::uint32_t SGCore::Batch::pack2UInt16ToUInt32(uint16_t x, uint16_t y) noexcept
{
    return (static_cast<uint32_t>(x) << 16) + y;
}
