//
// Created by stuka on 21.07.2025.
//

#ifndef SUNGEARENGINE_INSTANCING_H
#define SUNGEARENGINE_INSTANCING_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Render/Batching/BatchInstanceTransform.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Graphics/API/RenderState.h"

namespace SGCore
{
    class IVertexArray;
    class IVertexBuffer;
    class IMeshData;
    class IMaterial;

    struct Instancing : ECS::Component<Instancing, const Instancing>
    {
        std::vector<ECS::entity_t> m_entities;

        MeshRenderState m_renderState;

        [[nodiscard]] AssetRef<IMeshData> getBaseMeshData() const noexcept;
        void setBaseMeshData(const AssetRef<IMeshData>& meshData) noexcept;

        [[nodiscard]] AssetRef<IMaterial> getBaseMaterial() const noexcept;
        void setBaseMaterial(const AssetRef<IMaterial>& material) noexcept;

        [[nodiscard]] Ref<IVertexArray> getVertexArray() const noexcept;

        size_t getActiveEntitiesCount() const noexcept;

        void update(const ECS::registry_t& inRegistry) noexcept;

    private:
        size_t m_activeEntitiesCount = 0;

        Ref<IVertexArray> m_vertexArray;
        Ref<IVertexBuffer> m_instancesTransformsBuffer;

        std::vector<BatchInstanceTransform> m_instancesTransforms;

        AssetRef<IMeshData> m_baseMeshData;
        AssetRef<IMaterial> m_baseMaterial;

        std::vector<ECS::entity_t> m_entitiesToRemove;
    };
}

#endif // SUNGEARENGINE_INSTANCING_H
