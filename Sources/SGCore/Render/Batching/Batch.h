//
// Created by stuka on 15.06.2025.
//

#ifndef SUNGEARENGINE_BATCH_H
#define SUNGEARENGINE_BATCH_H

#include "BatchInstanceMaterial.h"
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

    // DO NOT COMPRESS SIZE OF ATLASES OF BATCH
    // todo: add aabb for batch. add camera frame receiver target layer
    struct Batch : ECS::Component<Batch, const Batch>
    {
        MeshRenderState m_batchRenderState;
        BatchInstanceMaterial m_defaultMaterial { };

        // All texture types are acceptable by default.
        std::unordered_set<SGTextureSlot> m_acceptableTextureTypes {
            SGTextureSlot::SGTT_EMISSIVE,
            SGTextureSlot::SGTT_AMBIENT_OCCLUSION,
            SGTextureSlot::SGTT_AMBIENT,
            SGTextureSlot::SGTT_DIFFUSE_ROUGHNESS,
            SGTextureSlot::SGTT_DIFFUSE,
            SGTextureSlot::SGTT_DISPLACEMENT,
            SGTextureSlot::SGTT_HEIGHT,
            SGTextureSlot::SGTT_NORMALS,
            SGTextureSlot::SGTT_BASE_COLOR,
            SGTextureSlot::SGTT_CLEARCOAT,
            SGTextureSlot::SGTT_EMISSION_COLOR,
            SGTextureSlot::SGTT_LIGHTMAP,
            SGTextureSlot::SGTT_METALNESS,
            SGTextureSlot::SGTT_NORMAL_CAMERA,
            SGTextureSlot::SGTT_OPACITY,
            SGTextureSlot::SGTT_REFLECTION,
            SGTextureSlot::SGTT_SHEEN,
            SGTextureSlot::SGTT_SHININESS,
            SGTextureSlot::SGTT_SPECULAR,
            SGTextureSlot::SGTT_TRANSMISSION,
            SGTextureSlot::SGTT_SKYBOX,
            SGTextureSlot::SGTT_NOISE,
            SGTextureSlot::SGTT_SHADOW_MAP2D
        };

        Batch() noexcept;

        /**
         * Inserts one entity into batch.
         * @param entity Entity. Must contain Mesh and Transform components.
         * @param fromRegistry The registry in which an entity is located.
         * @return True if entity was successfully inserted. False if was not inserted (no free space in batch).
         */
        [[nodiscard]] bool insertEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) noexcept;

        /**
         * Inserts vector of entities into batch.
         * @param entities Vector of entities. All entities must contain Mesh and Transform components.
         * @param fromRegistry The registry in which the entities are located.
         * @return -1 if all entities have been successfully inserted or index of last unsuccessful entity in entities vector.\n
         *         Entity can be not inserted if there is no free space in the batch.
         */
        [[nodiscard]] std::uint64_t insertEntities(const std::vector<ECS::entity_t>& entities, const ECS::registry_t& fromRegistry) noexcept;

        /**
         * Inserts vector of entities into batch.
         * @param entitiesBegin Begin of range.
         * @param entitiesEnd End of range.
         * @param fromRegistry The registry in which the entities are located.
         * @return -1 if all entities have been successfully inserted or index (between begin and end) of last unsuccessful entity in entities vector.\n
         *         Entity can be not inserted if there is no free space in the batch.
         */
        [[nodiscard]] std::uint64_t insertEntities(const std::vector<ECS::entity_t>::iterator& entitiesBegin, const std::vector<ECS::entity_t>::iterator& entitiesEnd, const ECS::registry_t& fromRegistry) noexcept;

        void removeEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry, bool immediateRemove = true) noexcept;

        void removeEntities(const std::vector<ECS::entity_t>& entities, const ECS::registry_t& fromRegistry, bool immediateRemove = true) noexcept;

        void shrinkBuffersToFit() noexcept;

        bool hasSpaceForEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) const noexcept;

        void setAtlasMaxSide(std::uint32_t maxSide) noexcept;

        void update(const ECS::registry_t& inRegistry) noexcept;

        void bind(IShader* shader) const noexcept;

        Ref<IVertexArray> getVertexArray() const noexcept;

        const Atlas& getAtlas() const noexcept;

        const std::vector<ECS::entity_t>& getEntities() const noexcept;

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
        Ref<ITexture2D> m_instancesMaterialsBuffer;

        std::array<Ref<ITexture2D>, texture_types_count> m_texturesBuffers;

        // vertices for fake vertex buffer
        std::vector<BatchTriangle> m_instanceTriangles;

        // all vertices of all instances
        std::vector<BatchVertex> m_vertices;
        // all indices of all instances. every instance contains 3 indices of one triangle
        std::vector<glm::u32vec3> m_indices;
        std::vector<BatchInstanceTransform> m_transforms;
        std::vector<BatchInstanceMaterial> m_materials;

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
        std::vector<ECS::entity_t> m_entitiesToRemove;

        void insertEntityImpl(ECS::entity_t entity, const ECS::registry_t& fromRegistry, bool isUpdateBuffers) noexcept;

        void updateTextureDataInTriangles() noexcept;

        void updateBuffers() noexcept;

        static std::uint32_t pack2UInt16ToUInt32(uint16_t x, uint16_t y) noexcept;
    };
}

#endif // SUNGEARENGINE_BATCH_H
