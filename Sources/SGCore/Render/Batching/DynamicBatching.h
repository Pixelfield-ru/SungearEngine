//
// Created by ilya on 29.02.24.
//

#ifndef SUNGEARENGINE_DYNAMICBATCHING_H
#define SUNGEARENGINE_DYNAMICBATCHING_H

#include <stack>
#include <SGCore/ImportedScenesArch/MeshDataRenderInfo.h>
#include <SGUtils/EventListener.h>

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class IVertexArray;
    class IVertexBuffer;
    class ITexture2D;
    class IIndexBuffer;
    class IShader;
    
    struct DynamicBatching : public ISystem
    {
        DynamicBatching();
        
        void update(const double& dt) noexcept override;
        
        void onAddToScene() noexcept override;

    private:
        size_t m_maxVerticesCount = 1024 * 1024 * 5;
        size_t m_maxIndicesCount = 1024 * 1024 * 5;
        size_t m_maxInstancesCount = 200'000;
        
        std::vector<float> m_verticesPositions;
        std::vector<float> m_UVs;
        std::vector<float> m_modelMatrices;
        std::vector<float> m_instancesIndices;
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

        std::unordered_map<entt::entity, size_t> m_entitiesIndices;
        
        size_t m_currentEntityIndex = 0;
        
        void onTargetConstruct(entt::registry& registry, entt::entity targetEntity) noexcept;
        void onTargetDestroy(entt::registry& registry, entt::entity targetEntity) noexcept;
        
        void onRenderPipelineSet() noexcept;
        
        EventListener<void()> m_onRenderPipelineSetEventListener = MakeEventListener<void()>([this]() {
            onRenderPipelineSet();
        });
    };
}

#endif //SUNGEARENGINE_DYNAMICBATCHING_H
