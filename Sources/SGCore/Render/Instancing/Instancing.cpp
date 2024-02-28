//
// Created by ilya on 28.02.24.
//

#include <glm/gtc/type_ptr.hpp>

#include "Instancing.h"

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"

SGCore::Instancing::Instancing()
{
    // ==================================================================
    
    SGCore::RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==================================================================
    
    m_renderInfo.m_enableFacesCulling = true;
    m_renderInfo.m_drawMode = SGDrawMode::SGG_TRIANGLES;
    
    auto renderPipeline = SGCore::RenderPipelinesManager::getCurrentRenderPipeline();
    
    if(renderPipeline)
    {
        m_shader = SGCore::MakeRef<SGCore::IShader>();
        m_shader->addSubPassShadersAndCompile(SGCore::AssetManager::loadAsset<SGCore::FileAsset>(
                renderPipeline->m_shadersPaths.getByVirtualPath("InstancingShader").getCurrentRealization()));
        
        // todo: pass uniforms
        updateUniforms();
    }
}

SGCore::Instancing::Instancing(const size_t& maxMeshesCount)
{
    m_maxMeshesCount = maxMeshesCount;
    
    // ==================================================================
    
    SGCore::RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==================================================================
    
    m_renderInfo.m_enableFacesCulling = true;
    m_renderInfo.m_drawMode = SGDrawMode::SGG_TRIANGLES;
    
    auto renderPipeline = SGCore::RenderPipelinesManager::getCurrentRenderPipeline();
    
    if(renderPipeline)
    {
        m_shader = SGCore::MakeRef<SGCore::IShader>();
        m_shader->addSubPassShadersAndCompile(SGCore::AssetManager::loadAsset<SGCore::FileAsset>(
                renderPipeline->m_shadersPaths.getByVirtualPath("InstancingShader").getCurrentRealization()));
        
        // todo: pass uniforms
        updateUniforms();
    }
}

void SGCore::Instancing::drawMesh(SGCore::Mesh& mesh, SGCore::Transform& transform) noexcept
{
    drawMeshData(mesh.m_base.m_meshData, mesh.m_base.m_meshDataRenderInfo, transform);
}

void SGCore::Instancing::drawMeshData(const SGCore::Ref<SGCore::IMeshData>& meshData,
                                      const SGCore::MeshDataRenderInfo& meshDataRenderInfo,
                                      SGCore::Transform& transform) noexcept
{
    if(m_currentRenderedMeshesCount >= m_maxMeshesCount - 1) return;
    
    switch(m_buffersMode)
    {
        // todo:
        case SG_USE_VERTICES_UNIQUE_DO_NOT_USE_INDICES:
            break;
            // todo:
        case SG_USE_VERTICES_NOT_UNIQUE_USE_INDICES_NOT_UNIQUE:
            break;
        case SG_USE_VERTICES_UNIQUE_USE_INDICES_UNIQUE:
            // cannot render if counts are not equal
            if(meshData->m_indices.size() != m_exampleMeshIndicesCount ||
               meshData->m_positions.size() != m_exampleMeshVerticesPositionsCount * 3 ||
               meshData->m_uv.size() != m_exampleMeshUVsCount * 3) return;
            
            size_t matrixIdx = m_currentRenderedMeshesCount * 16;
            
            // matrices =====================================================
            
            for(std::uint8_t i = 0; i < 16; ++i)
            {
                m_matrices[matrixIdx + i] = *(glm::value_ptr(transform.m_finalTransform.m_modelMatrix) + i);
            }
            
            // uvs =====================================================
            if(m_updateUVs)
            {
                size_t uvsOffset = m_currentRenderedMeshesCount * m_exampleMeshUVsCount * 3;
                
                for(size_t uvIdx = 0; uvIdx < meshData->m_uv.size(); ++uvIdx)
                {
                    size_t resultOffset = uvsOffset + uvIdx * 3;
                    
                    float x;
                    float y;
                    float z;
                    
                    meshData->getVertexUV(uvIdx, x, y, z);
                    
                    m_UVs[resultOffset] = x;
                    m_UVs[resultOffset + 1] = y;
                    m_UVs[resultOffset + 2] = z;
                }
            }
            
            // positions ===============================================
            if(m_updatePositions)
            {
                size_t positionsOffset = m_currentRenderedMeshesCount * m_exampleMeshVerticesPositionsCount * 3;
                
                for(size_t vIdx = 0; vIdx < meshData->m_positions.size(); ++vIdx)
                {
                    size_t resultOffset = positionsOffset + vIdx * 3;
                    
                    float x;
                    float y;
                    float z;
                    
                    meshData->getVertexPosition(vIdx, x, y, z);
                    
                    m_verticesPositions[resultOffset] = x;
                    m_verticesPositions[resultOffset + 1] = y;
                    m_verticesPositions[resultOffset + 2] = z;
                }
            }
            
            // indices ==================================================
            if(m_updateIndices)
            {
                size_t indicesOffset = m_currentRenderedMeshesCount * m_exampleMeshIndicesCount;
                
                for(size_t i = 0; i < meshData->m_indices.size(); ++i)
                {
                    m_indices[indicesOffset + i] = meshData->m_indices[i];
                }
            }
            
            // ==========================================================
            
            ++m_currentRenderedMeshesCount;
            break;
    }
}

void SGCore::Instancing::drawAll() noexcept
{
    if(!m_shader) return;
    
    auto subPassShader = m_shader->getSubPassShader("InstancingPass");
    
    if(!subPassShader) return;
    
    size_t meshesCount = std::min(m_currentRenderedMeshesCount, m_maxMeshesCount);
    
    subPassShader->bind();
    subPassShader->useUniformBuffer(SGCore::CoreMain::getRenderer()->m_viewMatricesBuffer);
    
    switch(m_buffersMode)
    {
        // todo:
        case SG_USE_VERTICES_UNIQUE_DO_NOT_USE_INDICES:
            break;
        // todo:
        case SG_USE_VERTICES_NOT_UNIQUE_USE_INDICES_NOT_UNIQUE:
            break;
        case SG_USE_VERTICES_UNIQUE_USE_INDICES_UNIQUE:
            m_matricesVertexBuffer->bind();
            m_matricesVertexBuffer->subData(m_matrices.data(), meshesCount * 16, 0);
            
            if(m_updateUVs)
            {
                m_UVsVertexBuffer->bind();
                m_UVsVertexBuffer->subData(m_UVs.data(), meshesCount * m_exampleMeshUVsCount * 3, 0);
            }
            
            if(m_updatePositions)
            {
                m_positionsBuffer->bind();
                m_positionsBuffer->subData(m_verticesPositions.data(),
                                           meshesCount * m_exampleMeshVerticesPositionsCount * 3, 0);
            }
            
            if(m_updateIndices)
            {
                m_indicesBuffer->bind();
                m_indicesBuffer->subData(m_indices.data(), meshesCount * m_exampleMeshIndicesCount, 0);
            }
            
            SGCore::CoreMain::getRenderer()->renderArrayInstanced(m_vertexArray, m_renderInfo, meshesCount * m_exampleMeshVerticesPositionsCount * 3, m_exampleMeshIndicesCount,
                                                                  meshesCount);
            
            break;
    }
    
    m_currentRenderedMeshesCount = 0;
}

void SGCore::Instancing::resetRenderer() noexcept
{
    m_currentRenderedMeshesCount = 0;
}

void SGCore::Instancing::setExampleMeshData(const SGCore::Ref<SGCore::IMeshData>& meshData) noexcept
{
    std::string instancingBuiltState = m_built ? "The old mesh data and the old buffer mode are used." : "Instancing has not been built.";
    
    if(!meshData)
    {
        spdlog::error("Instancing Error: cannot set example mesh! New example mesh is nullptr. {0}", instancingBuiltState);
        return;
    }
    
    m_exampleMeshData = meshData;
    
    rebuild();
}

void SGCore::Instancing::setBuffersMode(SGCore::InstancingBuffersMode mode) noexcept
{
    std::string instancingBuiltState = m_built ? "The old mesh data and the old buffer mode are used." : "Instancing has not been built.";
    
    m_buffersMode = mode;
    
    SGCore::Ref<SGCore::IMeshData> lockedExample = m_exampleMeshData.lock();
    
    if(!lockedExample)
    {
        spdlog::error("Instancing Error: cannot set buffers mode! Example mesh is not set. {0}", instancingBuiltState);
        return;
    }
    
    rebuild();
}

void SGCore::Instancing::rebuild() noexcept
{
    std::string instancingBuiltState = m_built ? "The old mesh data and the old buffer mode are used." : "Instancing has not been built.";
    
    SGCore::Ref<SGCore::IMeshData> lockedExample = m_exampleMeshData.lock();
    
    switch(m_buffersMode)
    {
        // todo:
        case SG_USE_VERTICES_UNIQUE_DO_NOT_USE_INDICES:
            break;
        // todo:
        case SG_USE_VERTICES_NOT_UNIQUE_USE_INDICES_NOT_UNIQUE:
            break;
        case SG_USE_VERTICES_UNIQUE_USE_INDICES_UNIQUE:
            if(lockedExample->m_positions.empty())
            {
                spdlog::error("Instancing Error: cannot set buffers mode! Example mash has no vertices. {0}", instancingBuiltState);
                return;
            }
            if(lockedExample->m_indices.empty())
            {
                spdlog::error("Instancing Error: cannot set buffers mode! Example mash has no indices. {0}", instancingBuiltState);
                return;
            }
            
            // everything is ok! let`s build mesh
            
            checkExampleMeshForWarnings();
            
            m_exampleMeshUVsCount = lockedExample->m_uv.size() / 3;
            m_exampleMeshVerticesPositionsCount = lockedExample->m_positions.size() / 3;
            m_exampleMeshIndicesCount = lockedExample->m_indices.size();
            
            // =========================================
            
            m_matrices.resize(m_maxMeshesCount * 16);
            m_UVs.resize(m_maxMeshesCount * m_exampleMeshUVsCount * 3);
            m_verticesPositions.resize(m_maxMeshesCount * m_exampleMeshVerticesPositionsCount * 3);
            m_indices.resize(m_maxMeshesCount * m_exampleMeshIndicesCount);
            
            {
                m_vertexArray = SGCore::Ref<SGCore::IVertexArray>(SGCore::CoreMain::getRenderer()->createVertexArray());
                m_vertexArray->create()->bind();
                
                m_matricesVertexBuffer = SGCore::Ref<SGCore::IVertexBuffer>(
                        SGCore::CoreMain::getRenderer()->createVertexBuffer()
                );
                
                m_matricesVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(
                        m_matrices);
                
                SGCore::Ref<SGCore::IVertexBufferLayout> bufferLayout = SGCore::Ref<SGCore::IVertexBufferLayout>(
                        SGCore::CoreMain::getRenderer()->createVertexBufferLayout());
                bufferLayout
                        ->addAttribute(SGCore::Ref<SGCore::IVertexAttribute>(
                                bufferLayout->createVertexAttribute(0,
                                                                    "instanceModelMatrix",
                                                                    SGGDataType::SGG_FLOAT4,
                                                                    (size_t) 1))
                        )
                        ->addAttribute(SGCore::Ref<SGCore::IVertexAttribute>(
                                bufferLayout->createVertexAttribute(1,
                                                                    "instanceModelMatrix",
                                                                    SGGDataType::SGG_FLOAT4,
                                                                    (size_t) 1))
                        )
                        ->addAttribute(SGCore::Ref<SGCore::IVertexAttribute>(
                                bufferLayout->createVertexAttribute(2,
                                                                    "instanceModelMatrix",
                                                                    SGGDataType::SGG_FLOAT4,
                                                                    (size_t) 1))
                        )
                        ->addAttribute(SGCore::Ref<SGCore::IVertexAttribute>(
                                bufferLayout->createVertexAttribute(3,
                                                                    "instanceModelMatrix",
                                                                    SGGDataType::SGG_FLOAT4,
                                                                    (size_t) 1))
                        )
                        ->prepare()->enableAttributes();
                
                // ======
                
                // =========================
                
                m_UVsVertexBuffer = std::shared_ptr<SGCore::IVertexBuffer>(
                        SGCore::CoreMain::getRenderer()->createVertexBuffer()
                );
                
                m_UVsVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_UVs);
                
                bufferLayout->reset();
                bufferLayout
                        ->addAttribute(Ref<SGCore::IVertexAttribute>(
                                bufferLayout->createVertexAttribute(4,
                                                                    "instanceVertexUV",
                                                                    SGGDataType::SGG_FLOAT3,
                                                                    (size_t) 0))
                        )
                        ->prepare()->enableAttributes();
                
                // =========================
                
                m_positionsBuffer = Ref<SGCore::IVertexBuffer>(
                        SGCore::CoreMain::getRenderer()->createVertexBuffer()
                );
                
                m_positionsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(
                        m_verticesPositions);
                
                bufferLayout->reset();
                bufferLayout
                        ->addAttribute(Ref<SGCore::IVertexAttribute>(
                                bufferLayout->createVertexAttribute(5,
                                                                    "instanceVertexPosition",
                                                                    SGGDataType::SGG_FLOAT3,
                                                                    (size_t) 0))
                        )
                        ->prepare()->enableAttributes();
                
                // ===============
                
                m_indicesBuffer = Ref<IIndexBuffer>(CoreMain::getRenderer()->createIndexBuffer());
                m_indicesBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_indices);
            }
            
            m_renderInfo.m_useIndices = true;
            
            m_built = true;
            
            break;
    }
}

void SGCore::Instancing::fillArraysByExample() noexcept
{
    SGCore::Ref<SGCore::IMeshData> lockedExample = m_exampleMeshData.lock();
    
    if(!lockedExample)
    {
        spdlog::error("Instancing Error: cannot fill buffers! Example mesh is not set.");
        return;
    }
    
    auto& meshData = lockedExample;
    
    for(size_t m = 0; m < m_maxMeshesCount; ++m)
    {
        switch(m_buffersMode)
        {
            // todo:
            case SG_USE_VERTICES_UNIQUE_DO_NOT_USE_INDICES:
                break;
                // todo:
            case SG_USE_VERTICES_NOT_UNIQUE_USE_INDICES_NOT_UNIQUE:
                break;
            case SG_USE_VERTICES_UNIQUE_USE_INDICES_UNIQUE:
                // uvs =====================================================
                size_t uvsOffset = m * m_exampleMeshUVsCount * 3;
                
                for(size_t uvIdx = 0; uvIdx < meshData->m_uv.size(); ++uvIdx)
                {
                    size_t resultOffset = uvsOffset + uvIdx * 3;
                    
                    float x;
                    float y;
                    float z;
                    
                    meshData->getVertexUV(uvIdx, x, y, z);
                    
                    m_UVs[resultOffset] = x;
                    m_UVs[resultOffset + 1] = y;
                    m_UVs[resultOffset + 2] = z;
                }
                
                // positions ===============================================
                size_t positionsOffset = m * m_exampleMeshVerticesPositionsCount * 3;
                
                for(size_t vIdx = 0; vIdx < meshData->m_positions.size(); ++vIdx)
                {
                    size_t resultOffset = positionsOffset + vIdx * 3;
                    
                    float x;
                    float y;
                    float z;
                    
                    meshData->getVertexPosition(vIdx, x, y, z);
                    
                    m_verticesPositions[resultOffset] = x;
                    m_verticesPositions[resultOffset + 1] = y;
                    m_verticesPositions[resultOffset + 2] = z;
                }
                
                // indices ==================================================
                
                size_t indicesOffset = m * m_exampleMeshIndicesCount;
                
                for(size_t i = 0; i < meshData->m_indices.size(); ++i)
                {
                    m_indices[indicesOffset + i] = meshData->m_indices[i];
                }
                
                // ==========================================================
                break;
        }
    }
}

void SGCore::Instancing::updateBuffersEntirely() noexcept
{
    m_matricesVertexBuffer->bind();
    m_matricesVertexBuffer->subData(m_matrices.data(), m_matrices.size(), 0);
    
    m_UVsVertexBuffer->bind();
    m_UVsVertexBuffer->subData(m_UVs.data(), m_UVs.size(), 0);
    
    m_positionsBuffer->bind();
    m_positionsBuffer->subData(m_verticesPositions.data(), m_verticesPositions.size(), 0);
    
    m_indicesBuffer->bind();
    m_indicesBuffer->subData(m_indices.data(), m_indices.size(), 0);
}

void SGCore::Instancing::checkExampleMeshForWarnings() const noexcept
{
    Ref<IMeshData> lockedExample = m_exampleMeshData.lock();
    
    if(lockedExample->m_uv.empty())
    {
        spdlog::warn("Instancing Warning while set buffers mode. Example mash has no UV.");
        return;
    }
    
    if(lockedExample->m_bitangents.empty())
    {
        spdlog::warn("Instancing Warning while set buffers mode. Example mash has no bitangents.");
        return;
    }
    
    if(lockedExample->m_tangents.empty())
    {
        spdlog::warn("Instancing Warning while set buffers mode. Example mash has no tangents.");
        return;
    }
    
    if(lockedExample->m_normals.empty())
    {
        spdlog::warn("Instancing Warning while set buffers mode. Example mash has no normals.");
        return;
    }
}

void SGCore::Instancing::updateUniforms() noexcept
{

}

void SGCore::Instancing::onRenderPipelineSet() noexcept
{
    m_shader = MakeRef<IShader>();
    m_shader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
            RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths.getByVirtualPath("InstancingShader").getCurrentRealization()));
    
    updateUniforms();
}
