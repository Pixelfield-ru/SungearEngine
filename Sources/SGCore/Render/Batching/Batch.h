//
// Created by ilya on 29.02.24.
//

#ifndef SUNGEARENGINE_BATCHING_H
#define SUNGEARENGINE_BATCHING_H

#include <SGCore/pch.h>

#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/Utils/EventListener.h"
#include "SGCore/Scene/ISystem.h"
#include "BatchEntityRanges.h"

namespace SGCore
{
    class IVertexArray;
    class IVertexBuffer;
    class ITexture2D;
    class IIndexBuffer;
    class IShader;
    
    struct Transform;
    
    struct Batch
    {
        Batch(const Ref<Scene>& parentScene, const size_t& maxVerticesCount, const size_t& maxInstancesCount);
        Batch(const Ref<Scene>& parentScene) : Batch(parentScene, m_maxVerticesCount, m_maxInstancesCount) { }
        
        void renderAll() noexcept;
        
        void addEntity(const entity_t& entity) noexcept;
        void removeEntity(const entity_t& entity) noexcept;
        
    private:
        Weak<Scene> m_parentScene;
        
        size_t m_maxVerticesCount = 1024 * 1024 * 5;
        size_t m_maxIndicesCount = m_maxVerticesCount * 3;
        size_t m_maxInstancesCount = 50'000;
        
        size_t m_currentVerticesCountToRender = 0;
        size_t m_currentIndicesCountToRender = 0;
        size_t m_currentRenderingInstancesCount = 0;
        
        bool m_positionsArrayChanged = false;
        bool m_UVsArrayChanged = false;
        bool m_indicesArrayChanged = false;
        bool m_modelMatricesArrayChanged = false;
        bool m_instancesIndicesChanged = false;
        
        std::unordered_map<entity_t, size_t> m_entitiesIndices;
        std::vector<BatchEntityRanges> m_entitiesRanges;
        
        std::vector<float> m_verticesPositions;
        std::vector<float> m_UVs;
        std::vector<float> m_modelMatrices;
        std::vector<std::uint32_t> m_instancesIndices;
        std::vector<std::uint32_t> m_indices;
        
        Ref<IVertexArray> m_vertexArray;
        Ref<IVertexBuffer> m_positionsBuffer;
        // Ref<IVertexBuffer> m_matricesVertexBuffer;
        Ref<IVertexBuffer> m_UVsVertexBuffer;
        Ref<IVertexBuffer> m_instancesIndicesVertexBuffer;
        Ref<IIndexBuffer> m_indicesBuffer;
        Ref<ITexture2D> m_matricesTextureBuffer;
        
        MeshDataRenderInfo m_renderInfo;
        
        Ref<IShader> m_shader;
        
        EventListener<void(const Ref<registry_t>&, const entity_t&, Ref<const Transform>)> m_transformChangedListener =
                [this](const Ref<registry_t>& registry, const entity_t& entity, Ref<const Transform> transform) {
            onTransformUpdate(registry, entity, transform);
        };
        
        void updateArraysForEntity(const Ref<Scene>& lockedScene, const entity_t& entity) noexcept;
        
        void recalculateRanges() noexcept;

        void onMeshDestroyed(registry_t& registry, entity_t entity) noexcept;
        void onTransformDestroyed(registry_t& registry, entity_t entity) noexcept;
        
        void onMeshUpdate(registry_t& registry, entity_t entity) noexcept;
        void onTransformUpdate(const Ref<registry_t>& registry, entity_t entity, Ref<const Transform> transform) noexcept;
        
        void onRenderPipelineSet() noexcept;
        
        EventListener<void()> m_onRenderPipelineSetEventListener = [this]() {
            onRenderPipelineSet();
        };
    };
}

#endif //SUNGEARENGINE_DYNAMICBATCHING_H
