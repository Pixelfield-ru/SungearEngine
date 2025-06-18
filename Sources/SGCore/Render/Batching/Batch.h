//
// Created by stuka on 15.06.2025.
//

#ifndef SUNGEARENGINE_BATCH_H
#define SUNGEARENGINE_BATCH_H

#include "BatchInstanceTransform.h"
#include "BatchVertex.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Memory/AssetsPackage.h"
#include "SGCore/Utils/Slot.h"

namespace SGCore
{
    class IShader;
    class IVertexBuffer;
    class ITexture2D;
    class IVertexArray;

    // todo: add aabb for batch. add camera frame receiver target layer
    struct Batch : ECS::Component<Batch, const Batch>
    {
        Ref<IShader> m_shader;

        MeshRenderState m_batchRenderState;

        Batch() noexcept;

        void addEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) noexcept;

        void update(const ECS::registry_t& inRegistry) noexcept;

        void bind() const noexcept;

        Ref<IVertexArray> getVertexArray() const noexcept;

        size_t getTrianglesCount() const noexcept;

    private:
        struct MeshDataMarkup
        {
            size_t m_verticesOffset = 0;
            size_t m_verticesCount = 0;

            size_t m_indicesOffset = 0;
            size_t m_indicesCount = 0;
        };

        Ref<IVertexArray> m_fakeVertexArray;
        Ref<IVertexBuffer> m_fakeVerticesBuffer;

        Ref<ITexture2D> m_verticesBuffer;
        Ref<ITexture2D> m_indicesBuffer;
        Ref<ITexture2D> m_instancesTransformsBuffer;

        // vertices for fake vertex buffer
        std::vector<glm::ivec2> m_instanceTriangles;

        // all vertices of all instances
        std::vector<BatchVertex> m_vertices;
        // all indices of all instances
        std::vector<std::uint32_t> m_indices;
        std::vector<BatchInstanceTransform> m_transforms;

        // first - meshdata hash, second - data offset and size (in bytes)
        std::unordered_map<size_t, MeshDataMarkup> m_usedMeshDatas;

        std::vector<ECS::entity_t> m_entities;

        void onRenderPipelineSet() noexcept;

        Slot<void()> m_onRenderPipelineSetEventListener = [this]() {
            onRenderPipelineSet();
        };
    };
}

#endif // SUNGEARENGINE_BATCH_H
