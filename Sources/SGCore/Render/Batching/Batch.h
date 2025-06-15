//
// Created by stuka on 15.06.2025.
//

#ifndef SUNGEARENGINE_BATCH_H
#define SUNGEARENGINE_BATCH_H

#include "BatchInstanceTransform.h"
#include "BatchVertex.h"
#include "SGCore/Graphics/API/RenderState.h"

namespace SGCore
{
    class IVertexBuffer;
    class ITexture2D;
    class IVertexArray;

    struct Batch
    {
        Batch() noexcept;

        void addEntity(ECS::entity_t entity, const ECS::registry_t& fromRegistry) noexcept;

    private:
        MeshRenderState m_batchRenderState;
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
    };
}

#endif // SUNGEARENGINE_BATCH_H
