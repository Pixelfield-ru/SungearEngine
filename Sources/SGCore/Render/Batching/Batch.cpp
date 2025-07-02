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
}

void SGCore::Batch::addEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) noexcept
{
    const auto* tmpMesh = fromRegistry.tryGet<Mesh>(entity);
    if(!tmpMesh || !fromRegistry.allOf<Transform>(entity) || !tmpMesh->m_base.getMeshData()) return;

    const Mesh& mesh = *tmpMesh;

    const auto meshData = mesh.m_base.getMeshData();

    const size_t meshDataHash = meshData->getHash();

    // first - insertion position, second - insertion size
    std::array<glm::u32vec2, texture_types_count> atlasesInsertionsPos { };

    // =====================================================================
    // packing textures

    if(mesh.m_base.getMaterial())
    {
        m_shader->bind();

        for(std::uint8_t i = 0; i < mesh.m_base.getMaterial()->getTextures().size(); ++i)
        {
            const auto textureType = static_cast<SGTextureType>(i);
            const auto& texturesVec = mesh.m_base.getMaterial()->getTextures()[i];

            auto& atlas = m_atlases[i];

            for(const auto& texture : texturesVec)
            {
                if(!texture) continue;

                const size_t textureHash = texture->getHash();

                if(!m_usedTextures[i].contains(textureHash))
                {
                    AtlasRect rect;
                    atlas.findBestRect({ texture->getWidth(), texture->getHeight() }, rect);
                    atlas.packTexture(rect, texture.get());

                    m_usedTextures[i][textureHash] = {
                        .m_textureType = i,
                        .m_insertionPosition = rect.m_position,
                        .m_insertionSize = rect.m_size
                    };

                    std::cout << fmt::format("adding texture to atlas: texture type: {}, rect pos: {}, {}, rect size: {}, {}, texture path: '{}'",
                        sgStandardTextureTypeToString(textureType),
                        rect.m_position.x,
                        rect.m_position.y,
                        rect.m_size.x,
                        rect.m_size.y,
                        Utils::toUTF8(texture->getPath().resolved().u16string())) << std::endl;

                    m_shader->useInteger(sgStandardTextureTypeNameToStandardUniformName(textureType) + "_CURRENT_COUNT", 1);
                    m_shader->useTextureBlock(sgStandardTextureTypeNameToStandardUniformName(textureType) + "[0]", 3 + i);
                    m_shader->useVectorf(sgStandardTextureTypeNameToStandardUniformName(textureType) + "Sizes[0]", glm::vec2 { atlas.getTexture()->getWidth(), atlas.getTexture()->getHeight() });
                }

                const auto& textureMarkup = m_usedTextures[i][textureHash];

                const glm::u32vec2 texSize { texture->getWidth(), texture->getHeight() };

                /*atlasesInsertionsPos[i].x = textureMarkup.m_insertionPosition.x;
                atlasesInsertionsPos[i].y = textureMarkup.m_insertionPosition.y;*/

                static auto packPosition = [](uint16_t x, uint16_t y) -> std::uint32_t {
                    // Little-endian: y в младшие, x в старшие
                    return (static_cast<uint32_t>(x) << 16) + y;
                };

                atlasesInsertionsPos[i].x = packPosition(
                    static_cast<std::uint16_t>(textureMarkup.m_insertionPosition.x),
                    static_cast<std::uint16_t>(textureMarkup.m_insertionPosition.y)
                );

                atlasesInsertionsPos[i].y = packPosition(
                    static_cast<std::uint16_t>(texSize.x),
                    static_cast<std::uint16_t>(texSize.y)
                );

                /*std::memcpy(&atlasesInsertionsPos[i].x, &textureMarkup.m_insertionPosition.x, 2);
                std::memcpy(reinterpret_cast<std::uint8_t*>(&atlasesInsertionsPos[i].x) + 2, &textureMarkup.m_insertionPosition.y, 2);

                std::memcpy(&atlasesInsertionsPos[i].y, &texSize.x, 2);
                std::memcpy(reinterpret_cast<std::uint8_t*>(&atlasesInsertionsPos[i].y) + 2, &texSize.y, 2);*/
            }
        }
    }

    // =====================================================================

    const bool meshDataMarkupFound = m_usedMeshDatas.contains(meshDataHash);
    if(!meshDataMarkupFound)
    {
        // adding new meshdata

        glm::vec3 diffuseTexSize { };
        if(mesh.m_base.getMaterial())
        {
            const auto diffuseTex = mesh.m_base.getMaterial()->getTexture(SGTextureType::SGTT_DIFFUSE, 0);
            if(diffuseTex)
            {
                diffuseTexSize.x = diffuseTex->getWidth();
                diffuseTexSize.y = diffuseTex->getHeight();
            }
        }

        m_usedMeshDatas[meshDataHash] = {
            .m_verticesOffset = m_vertices.size(),
            .m_verticesCount = meshData->m_vertices.size(),

            .m_indicesOffset = m_indices.size() * 3,
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

    for(std::int32_t i = meshDataMarkup.m_indicesOffset; i < meshDataMarkup.m_indicesOffset + meshDataMarkup.m_indicesCount; i += 3)
    {
        BatchTriangle batchTriangle {
            .m_meshInstanceID = (std::int32_t) m_entities.size(),
            .m_triangleID = i / 3,
            .m_atlasesUVsOffset = atlasesInsertionsPos
        };

        m_instanceTriangles.push_back(batchTriangle);
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

    for(std::uint8_t i = 0; i < m_atlases.size(); ++i)
    {
        const auto& atlas = m_atlases[i];

        if(atlas.getTexture())
        {
            atlas.getTexture()->bind(3 + i);
        }
    }
}

SGCore::Ref<SGCore::IVertexArray> SGCore::Batch::getVertexArray() const noexcept
{
    return m_fakeVertexArray;
}

const std::array<SGCore::Atlas, texture_types_count>& SGCore::Batch::getAtlases() const noexcept
{
    return m_atlases;
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
