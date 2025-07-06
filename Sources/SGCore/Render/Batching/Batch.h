//
// Created by stuka on 15.06.2025.
//

#ifndef SUNGEARENGINE_BATCH_H
#define SUNGEARENGINE_BATCH_H

#include "BatchInstanceTransform.h"
#include "BatchTriangle.h"
#include "BatchVertex.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Memory/AssetsPackage.h"
#include "SGCore/Memory/Assets/Atlas/Atlas.h"
#include "SGCore/Utils/Slot.h"

namespace SGCore
{
    class IShader;
    class IVertexBuffer;
    class ITexture2D;
    class IVertexArray;

    // DO NOT SIZE COMPRESSION FOR ATLASES OF BATCH
    // todo: add aabb for batch. add camera frame receiver target layer
    struct Batch : ECS::Component<Batch, const Batch>
    {
        Ref<IShader> m_shader;

        MeshRenderState m_batchRenderState;

        Batch() noexcept;

        void insertEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) noexcept;
        void insertEntities(const std::vector<ECS::entity_t>& entities, const ECS::registry_t& fromRegistry) noexcept;

        void update(const ECS::registry_t& inRegistry) noexcept;

        void bind() const noexcept;

        Ref<IVertexArray> getVertexArray() const noexcept;

        const Atlas& getAtlas() const noexcept;

        size_t getTrianglesCount() const noexcept;

    private:
        struct MeshDataMarkup
        {
            size_t m_verticesOffset = 0;
            size_t m_verticesCount = 0;

            size_t m_indicesOffset = 0;
            size_t m_indicesCount = 0;

            // first - hash of texture in m_usedTextures map
            std::array<size_t, texture_types_count> m_textures = makeFilledArray<size_t, texture_types_count>(0);
        };

        struct TextureDataMarkup
        {
            std::uint8_t m_textureType = 0;

            // in atlas. can be uv offset
            glm::u32vec2 m_insertionPosition { };
            glm::u32vec2 m_insertionSize { };
        };

        struct TrianglesMarkup
        {
            size_t m_trianglesOffset = 0;
            size_t m_trianglesCount = 0;
        };

        Ref<IVertexArray> m_fakeVertexArray;
        Ref<IVertexBuffer> m_fakeVerticesBuffer;

        Ref<ITexture2D> m_verticesBuffer;
        Ref<ITexture2D> m_indicesBuffer;
        Ref<ITexture2D> m_instancesTransformsBuffer;

        std::array<Ref<ITexture2D>, texture_types_count> m_texturesBuffers;

        // vertices for fake vertex buffer
        std::vector<BatchTriangle> m_instanceTriangles;

        // all vertices of all instances
        std::vector<BatchVertex> m_vertices;
        // all indices of all instances. every instance contains 3 indices of one triangle
        std::vector<glm::u32vec3> m_indices;
        std::vector<BatchInstanceTransform> m_transforms;

        // first - meshdata hash, second - markup
        std::unordered_map<size_t, MeshDataMarkup> m_usedMeshDatas;

        // first - meshdata hash, second - vector of triangles ranges in triangles buffer
        std::unordered_map<size_t, std::vector<TrianglesMarkup>> m_trianglesMarkup;

        // first - texture hash, second - texture
        std::unordered_map<size_t, AssetRef<ITexture2D>> m_usedTextures;
        // std::array<std::unordered_map<size_t, TextureDataMarkup>, texture_types_count> m_usedTextures;
        // std::array<Atlas, texture_types_count> m_atlases;
        Atlas m_atlas;

        std::vector<ECS::entity_t> m_entities;

        void insertEntityImpl(ECS::entity_t entity, const ECS::registry_t& fromRegistry, bool isUpdateBuffers) noexcept;

        void updateTextureDataInTriangles() noexcept;

        void onRenderPipelineSet() noexcept;

        void updateBuffers() noexcept;

        static std::uint32_t pack2UInt16ToUInt32(uint16_t x, uint16_t y) noexcept;

        Slot<void()> m_onRenderPipelineSetEventListener = [this]() {
            onRenderPipelineSet();
        };
    };
}

#endif // SUNGEARENGINE_BATCH_H
