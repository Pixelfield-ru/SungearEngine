//
// Created by ilya on 29.02.24.
//

#ifndef SUNGEARENGINE_BATCHING_H
#define SUNGEARENGINE_BATCHING_H

#include <stack>
#include <unordered_set>
#include <vector>
#include <entt/entity/observer.hpp>

#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGUtils/EventListener.h"
#include "SGCore/Scene/ISystem.h"
#include "BatchEntityRanges.h"

namespace SGCore
{
    class IVertexArray;
    class IVertexBuffer;
    class ITexture2D;
    class IIndexBuffer;
    class IShader;
    
    struct Batch
    {
        Batch(const Ref<Scene>& parentScene);
        
        void renderAll() noexcept;
        
        void addEntity(const entt::entity& entity) noexcept;
        void removeEntity(const entt::entity& entity) noexcept;

    private:
        Weak<Scene> m_parentScene;
        
        size_t m_maxVerticesCount = 1024 * 1024 * 12;
        size_t m_maxIndicesCount = 1024 * 1024 * 12;
        size_t m_maxInstancesCount = 1'000'000;
        
        size_t m_currentVerticesCountToRender = 0;
        size_t m_currentIndicesCountToRender = 0;
        size_t m_currentRenderingInstancesCount = 0;
        
        bool m_positionsArrayChanged = false;
        bool m_UVsArrayChanged = false;
        bool m_indicesArrayChanged = false;
        bool m_modelMatricesArrayChanged = false;
        bool m_instancesIndicesChanged = false;
        
        std::unordered_map<entt::entity, size_t> m_entitiesIndices;
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
        
        void updateArraysForEntity(const Ref<Scene>& lockedScene, const entt::entity& entity) noexcept;
        
        void recalculateRanges() noexcept;

        void onMeshDestroyed(entt::registry& registry, entt::entity entity) noexcept;
        void onTransformDestroyed(entt::registry& registry, entt::entity entity) noexcept;
        
        void onMeshUpdate(entt::registry& registry, entt::entity entity) noexcept;
        void onTransformUpdate(entt::registry& registry, entt::entity entity) noexcept;
        
        void onRenderPipelineSet() noexcept;
        
        EventListener<void()> m_onRenderPipelineSetEventListener = MakeEventListener<void()>([this]() {
            onRenderPipelineSet();
        });
    };
}

#endif //SUNGEARENGINE_DYNAMICBATCHING_H
