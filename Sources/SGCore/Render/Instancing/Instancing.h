//
// Created by ilya on 28.02.24.
//

#ifndef OCEANSEDGE_INSTANCING_H
#define OCEANSEDGE_INSTANCING_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/UI/Text.h"
#include "SGCore/Render/Mesh.h"

// режимы:
// уникальные вершины для каждого меша. индексы не используются
// не уникальные вершины. не уникальные индексы
// уникальные вершины. уникальные индексы

namespace SGCore
{
    struct FontSpecialization;
    class IShader;
    
    class IVertexArray;
    class IVertexBuffer;
    
    enum InstancingBuffersMode
    {
        // getting vertices from example mesh data by indices but do not use indices
        SG_USE_VERTICES_UNIQUE_DO_NOT_USE_INDICES,
        // vertices are not unique for each mesh data and indices are the same for each mesh data
        SG_USE_VERTICES_NOT_UNIQUE_USE_INDICES_NOT_UNIQUE,
        // vertices are the same for each mesh data but indices are unique for each mesh data
        SG_USE_VERTICES_UNIQUE_USE_INDICES_UNIQUE
    };
    
    struct Instancing
    {
        bool m_updateUVs = true;
        bool m_updatePositions = true;
        bool m_updateIndices = true;
        
        std::vector<entt::entity> m_entitiesToRender;
        
        Instancing();
        Instancing(const size_t& maxMeshesCount);
        
        void drawMesh(Mesh& mesh, Transform& transform) noexcept;
        void drawMeshData(const Ref<IMeshData>& meshData, const MeshDataRenderInfo& meshDataRenderInfo, Transform& transform) noexcept;
        
        void drawAll() noexcept;
        
        void resetRenderer() noexcept;
        
        void setExampleMeshData(const Ref<IMeshData>& meshData) noexcept;
    
        void setBuffersMode(InstancingBuffersMode mode) noexcept;
        
        void rebuild() noexcept;
        
        void fillArraysByExample() noexcept;
        
        void updateBuffersEntirely() noexcept;
        
    private:
        void updateUniforms() noexcept;
        
        Ref<IVertexArray> m_vertexArray;
        Ref<IVertexBuffer> m_positionsBuffer;
        Ref<IVertexBuffer> m_matricesVertexBuffer;
        Ref<IVertexBuffer> m_UVsVertexBuffer;
        Ref<IIndexBuffer> m_indicesBuffer;
        
        Ref<IShader> m_shader;
        
        // ------------------
        Weak<IMeshData> m_exampleMeshData;
        
        // vec3
        std::size_t m_exampleMeshUVsCount = 0;
        // vec3
        std::size_t m_exampleMeshVerticesPositionsCount = 0;
        std::size_t m_exampleMeshIndicesCount = 0;
        // ------------------
        
        SGCore::MeshDataRenderInfo m_renderInfo;
        
        std::uint32_t m_maxMeshesCount = 50'000;
        std::uint32_t m_currentRenderedMeshesCount = 0;
        
        // buffers
        std::vector<float> m_matrices;
        std::vector<float> m_UVs;
        std::vector<float> m_verticesPositions;
        std::vector<std::uint32_t> m_indices;
        // ------------------------------------
        
        InstancingBuffersMode m_buffersMode = InstancingBuffersMode::SG_USE_VERTICES_UNIQUE_USE_INDICES_UNIQUE;
        
        bool m_built = false;
        
        void checkExampleMeshForWarnings() const noexcept;
        
        EventListener<void()> m_onRenderPipelineSetEventListener = MakeEventListener<void()>([this]() {
            onRenderPipelineSet();
        });
        
        void onRenderPipelineSet() noexcept;
    };
}

#endif //OCEANSEDGE_INSTANCING_H
