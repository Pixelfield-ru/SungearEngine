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

    // DO NOT SIZE COMPRESSION FOR ATLASES OF BATCH
    // todo: add aabb for batch. add camera frame receiver target layer
    struct Batch : ECS::Component<Batch, const Batch>
    {
        Ref<IShader> m_shader;

        MeshRenderState m_batchRenderState;
        BatchInstanceMaterial m_defaultMaterial { };

        // All texture types are acceptable by default.
        std::unordered_set<SGTextureType> m_acceptableTextureTypes {
            SGTextureType::SGTT_EMISSIVE,
            SGTextureType::SGTT_AMBIENT_OCCLUSION,
            SGTextureType::SGTT_AMBIENT,
            SGTextureType::SGTT_DIFFUSE_ROUGHNESS,
            SGTextureType::SGTT_DIFFUSE,
            SGTextureType::SGTT_DISPLACEMENT,
            SGTextureType::SGTT_HEIGHT,
            SGTextureType::SGTT_NORMALS,
            SGTextureType::SGTT_BASE_COLOR,
            SGTextureType::SGTT_CLEARCOAT,
            SGTextureType::SGTT_EMISSION_COLOR,
            SGTextureType::SGTT_LIGHTMAP,
            SGTextureType::SGTT_METALNESS,
            SGTextureType::SGTT_NORMAL_CAMERA,
            SGTextureType::SGTT_OPACITY,
            SGTextureType::SGTT_REFLECTION,
            SGTextureType::SGTT_SHEEN,
            SGTextureType::SGTT_SHININESS,
            SGTextureType::SGTT_SPECULAR,
            SGTextureType::SGTT_TRANSMISSION,
            SGTextureType::SGTT_SKYBOX,
            SGTextureType::SGTT_NOISE,
            SGTextureType::SGTT_SHADOW_MAP2D
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
         * @return entt::null if all entities have been successfully inserted or last unsuccessful entity.\n
         *         Entity can be not inserted if there is no free space in the batch.
         */
        [[nodiscard]] ECS::entity_t insertEntities(const std::vector<ECS::entity_t>& entities, const ECS::registry_t& fromRegistry) noexcept;

        bool hasSpaceForEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) const noexcept;

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
