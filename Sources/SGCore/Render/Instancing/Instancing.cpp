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
#include "SGCore/Graphics/API/ITexture2D.h"

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
    
    RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==================================================================
    
    m_renderInfo.m_enableFacesCulling = true;
    m_renderInfo.m_drawMode = SGG_TRIANGLES;
    
    m_shader = SGCore::MakeRef<IShader>();
    updateCurrentUsingShader();
}

void SGCore::Instancing::drawMesh(Mesh& mesh, Transform& transform) noexcept
{
    drawMeshData(mesh.m_base.m_meshData, mesh.m_base.m_meshDataRenderInfo, transform);
}

void SGCore::Instancing::drawMeshData(const Ref<IMeshData>& meshData,
                                      const MeshDataRenderInfo& meshDataRenderInfo,
                                      Transform& transform) noexcept
{
    // m_currentRenderedMeshesCount = 200'000;
    
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
                
                for(size_t uvIdx = 0; uvIdx < meshData->m_uv.size(); uvIdx += 3)
                {
                    size_t resultOffset = uvsOffset + uvIdx;
                    
                    float x;
                    float y;
                    float z;
                    
                    meshData->getVertexUV(uvIdx / 3, x, y, z);
                    
                    m_UVs[resultOffset] = x;
                    m_UVs[resultOffset + 1] = y;
                    m_UVs[resultOffset + 2] = z;
                }
            }
            
            // positions ===============================================
            if(m_updatePositions)
            {
                size_t positionsOffset = m_currentRenderedMeshesCount * m_exampleMeshVerticesPositionsCount * 3;
                
                for(size_t vIdx = 0; vIdx < meshData->m_positions.size(); vIdx += 3)
                {
                    size_t resultOffset = positionsOffset + vIdx;
                    
                    float x;
                    float y;
                    float z;
                    
                    meshData->getVertexPosition(vIdx / 3, x, y, z);
                    
                    m_verticesPositions[resultOffset] = x;
                    m_verticesPositions[resultOffset + 1] = y;
                    m_verticesPositions[resultOffset + 2] = z;
                }
            }
            
            // indices ==================================================
            if(m_updateIndices)
            {
                size_t indicesOffset = m_currentRenderedMeshesCount * m_exampleMeshIndicesCount;
                
                if(!m_isNonInstancingSingleDrawCall)
                {
                    for(size_t i = 0; i < m_exampleMeshIndicesCount; ++i)
                    {
                        m_indices[indicesOffset + i] = meshData->m_indices[i];
                    }
                }
                else
                {
                    for(size_t i = 0; i < m_exampleMeshIndicesCount; ++i)
                    {
                        m_indices[indicesOffset + i] = indicesOffset + meshData->m_indices[i];
                    }
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
    subPassShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
    
    switch(m_buffersMode)
    {
        // todo:
        case SG_USE_VERTICES_UNIQUE_DO_NOT_USE_INDICES:
            break;
        // todo:
        case SG_USE_VERTICES_NOT_UNIQUE_USE_INDICES_NOT_UNIQUE:
            break;
        case SG_USE_VERTICES_UNIQUE_USE_INDICES_UNIQUE:
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
            
            if(!m_isNonInstancingSingleDrawCall)
            {
                m_matricesVertexBuffer->bind();
                m_matricesVertexBuffer->subData(m_matrices.data(), meshesCount * 16, 0);
                
                CoreMain::getRenderer()->renderArrayInstanced(m_vertexArray, m_renderInfo,
                                                              meshesCount * m_exampleMeshVerticesPositionsCount * 3,
                                                              m_exampleMeshIndicesCount,
                                                              meshesCount);
            }
            else
            {
                m_matricesTextureBuffer->bind(0);
                m_matricesTextureBuffer->subTextureBufferData<float>(m_matrices.data(), meshesCount * 16, 0);
                subPassShader->useTextureBlock("u_matricesTextureBuffer", 0);
                
                CoreMain::getRenderer()->renderArray(m_vertexArray, m_renderInfo,
                                                              meshesCount * m_exampleMeshVerticesPositionsCount * 3,
                                                     meshesCount * m_exampleMeshIndicesCount);
            }
            
            break;
    }
    
    m_currentRenderedMeshesCount = 0;
}

void SGCore::Instancing::resetRenderer() noexcept
{
    m_currentRenderedMeshesCount = 0;
}

void SGCore::Instancing::setExampleMeshData(const Ref<IMeshData>& meshData) noexcept
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

void SGCore::Instancing::setBuffersMode(InstancingBuffersMode mode) noexcept
{
    std::string instancingBuiltState = m_built ? "The old mesh data and the old buffer mode are used." : "Instancing has not been built.";
    
    m_buffersMode = mode;
    
    rebuild();
}

void SGCore::Instancing::rebuild() noexcept
{
    std::string instancingBuiltState = m_built ? "The old mesh data and the old buffer mode are used." : "Instancing has not been built.";
    
    Ref<IMeshData> lockedExample = m_exampleMeshData.lock();
    
    if(!lockedExample)
    {
        spdlog::error("Instancing Error: cannot set buffers mode! Example mesh is not set. {0}", instancingBuiltState);
        return;
    }
    
    if(m_isNonInstancingSingleDrawCall)
    {
        size_t cnt = lockedExample->m_indices.size();
        
        m_instancesIndices.clear();
        m_instancesIndices.resize(m_maxMeshesCount * cnt);
        
        size_t currentIndex = 0;
        size_t currentMesh = 0;
        
        for(size_t i = 0; i < m_maxMeshesCount; ++i)
        {
            for(size_t j = 0; j < cnt; ++j)
            {
                m_instancesIndices[currentIndex] = currentMesh;
                ++currentIndex;
            }
            ++currentMesh;
        }
    }
    
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
                spdlog::error("Instancing Error: cannot set buffers mode! Example mesh has no vertices. {0}", instancingBuiltState);
                return;
            }
            if(lockedExample->m_indices.empty())
            {
                spdlog::error("Instancing Error: cannot set buffers mode! Example mesh has no indices. {0}", instancingBuiltState);
                return;
            }
            
            // everything is ok! let`s build mesh
            
            std::cout << "mesh : " << lockedExample.get() << std::endl;
            
            checkExampleMeshForWarnings();
            
            m_exampleMeshUVsCount = lockedExample->m_uv.size() / 3;
            m_exampleMeshVerticesPositionsCount = lockedExample->m_positions.size() / 3;
            m_exampleMeshIndicesCount = lockedExample->m_indices.size();
            
            // =========================================

            m_matrices.clear();
            m_UVs.clear();
            m_verticesPositions.clear();
            m_indices.clear();

            m_matrices.resize(m_maxMeshesCount * 16);
            m_UVs.resize(m_maxMeshesCount * m_exampleMeshUVsCount * 3);
            m_verticesPositions.resize(m_maxMeshesCount * m_exampleMeshVerticesPositionsCount * 3);
            m_indices.resize(m_maxMeshesCount * m_exampleMeshIndicesCount);
            
            {
                m_vertexArray = SGCore::Ref<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
                m_vertexArray->create()->bind();
                
                Ref<IVertexBufferLayout> bufferLayout = SGCore::Ref<IVertexBufferLayout>(
                        CoreMain::getRenderer()->createVertexBufferLayout());
                
                if(!m_isNonInstancingSingleDrawCall)
                {
                    m_matricesVertexBuffer = SGCore::Ref<IVertexBuffer>(
                            CoreMain::getRenderer()->createVertexBuffer()
                    );
                    
                    m_matricesVertexBuffer->setUsage(SGG_DYNAMIC)->create()->bind()->putData(
                            m_matrices);
                    
                    bufferLayout
                            ->addAttribute(SGCore::Ref<IVertexAttribute>(
                                    bufferLayout->createVertexAttribute(0,
                                                                        "instanceModelMatrix",
                                                                        SGG_FLOAT4,
                                                                        (size_t) 1))
                            )
                            ->addAttribute(SGCore::Ref<IVertexAttribute>(
                                    bufferLayout->createVertexAttribute(1,
                                                                        "instanceModelMatrix",
                                                                        SGG_FLOAT4,
                                                                        (size_t) 1))
                            )
                            ->addAttribute(SGCore::Ref<IVertexAttribute>(
                                    bufferLayout->createVertexAttribute(2,
                                                                        "instanceModelMatrix",
                                                                        SGG_FLOAT4,
                                                                        (size_t) 1))
                            )
                            ->addAttribute(SGCore::Ref<IVertexAttribute>(
                                    bufferLayout->createVertexAttribute(3,
                                                                        "instanceModelMatrix",
                                                                        SGG_FLOAT4,
                                                                        (size_t) 1))
                            )
                            ->prepare()->enableAttributes();
                }
                else
                {
                    m_instancesIndicesVertexBuffer = SGCore::Ref<IVertexBuffer>(
                            CoreMain::getRenderer()->createVertexBuffer()
                    );
                    
                    m_instancesIndicesVertexBuffer->setUsage(SGG_STATIC)->create()->bind()->putData(
                            m_instancesIndices);
                    
                    bufferLayout->reset()
                            ->addAttribute(SGCore::Ref<IVertexAttribute>(
                                    bufferLayout->createVertexAttribute(0,
                                                                        "instanceID",
                                                                        SGG_FLOAT,
                                                                        (size_t) 0))
                            )
                            ->prepare()->enableAttributes();
                }
                
                // ======
                
                // =========================
                
                m_UVsVertexBuffer = std::shared_ptr<IVertexBuffer>(
                        CoreMain::getRenderer()->createVertexBuffer()
                );
                
                m_UVsVertexBuffer->setUsage(SGG_DYNAMIC)->create()->bind()->putData(m_UVs);
                
                bufferLayout->reset();
                bufferLayout
                        ->addAttribute(Ref<IVertexAttribute>(
                                bufferLayout->createVertexAttribute(!m_isNonInstancingSingleDrawCall ? 4 : 1,
                                                                    "instanceVertexUV",
                                                                    SGG_FLOAT3,
                                                                    (size_t) 0))
                        )
                        ->prepare()->enableAttributes();
                
                // =========================
                
                m_positionsBuffer = Ref<IVertexBuffer>(
                        CoreMain::getRenderer()->createVertexBuffer()
                );
                
                m_positionsBuffer->setUsage(SGG_DYNAMIC)->create()->bind()->putData(
                        m_verticesPositions);
                
                bufferLayout->reset();
                bufferLayout
                        ->addAttribute(Ref<IVertexAttribute>(
                                bufferLayout->createVertexAttribute(!m_isNonInstancingSingleDrawCall ? 5 : 2,
                                                                    "instanceVertexPosition",
                                                                    SGG_FLOAT3,
                                                                    (size_t) 0))
                        )
                        ->prepare()->enableAttributes();
                
                // ===============
                
                m_indicesBuffer = Ref<IIndexBuffer>(CoreMain::getRenderer()->createIndexBuffer());
                m_indicesBuffer->setUsage(SGG_DYNAMIC)->create()->bind()->putData(m_indices);
            }
            
            m_renderInfo.m_useIndices = true;
            
            m_built = true;
            
            break;
    }
}

void SGCore::Instancing::fillArraysByExample() noexcept
{
    Ref<IMeshData> lockedExample = m_exampleMeshData.lock();
    
    if(!lockedExample)
    {
        spdlog::error("Instancing Error: cannot fill buffers! Example mesh is not set.");
        return;
    }
    
    size_t lockedExampleIndicesCnt = lockedExample->m_indices.size();

    switch (m_buffersMode)
    {
    // todo:
    case SG_USE_VERTICES_UNIQUE_DO_NOT_USE_INDICES:
        break;
    // todo:
    case SG_USE_VERTICES_NOT_UNIQUE_USE_INDICES_NOT_UNIQUE:
        break;
    case SG_USE_VERTICES_UNIQUE_USE_INDICES_UNIQUE:
        for (size_t m = 0; m < m_maxMeshesCount; ++m)
        {
            // uvs =====================================================
            size_t uvsOffset = m * m_exampleMeshUVsCount * 3;

            for (size_t uvIdx = 0; uvIdx < lockedExample->m_uv.size(); uvIdx += 3)
            {
                size_t resultOffset = uvsOffset + uvIdx;

                float x;
                float y;
                float z;
                
                lockedExample->getVertexUV(uvIdx / 3, x, y, z);

                m_UVs[resultOffset] = x;
                m_UVs[resultOffset + 1] = y;
                m_UVs[resultOffset + 2] = z;
            }

            // positions ===============================================
            size_t positionsOffset = m * m_exampleMeshVerticesPositionsCount * 3;

            for (size_t vIdx = 0; vIdx < lockedExample->m_positions.size(); vIdx += 3)
            {
                size_t resultOffset = positionsOffset + vIdx;

                float x;
                float y;
                float z;
                
                lockedExample->getVertexPosition(vIdx / 3, x, y, z);

                m_verticesPositions[resultOffset] = x;
                m_verticesPositions[resultOffset + 1] = y;
                m_verticesPositions[resultOffset + 2] = z;
            }

            // indices ==================================================

            size_t indicesOffset = m * m_exampleMeshIndicesCount;
            
            if(!m_isNonInstancingSingleDrawCall)
            {
                for(size_t i = 0; i < lockedExampleIndicesCnt; ++i)
                {
                    m_indices[indicesOffset + i] = lockedExample->m_indices[i];
                }
            }
            else
            {
                for(size_t i = 0; i < lockedExampleIndicesCnt; ++i)
                {
                    m_indices[indicesOffset + i] = indicesOffset + lockedExample->m_indices[i];
                }
            }

            // ==========================================================
        }
        break;
    }
}

void SGCore::Instancing::updateBuffersEntirely() noexcept
{
    if(!m_isNonInstancingSingleDrawCall)
    {
        m_matricesVertexBuffer->bind();
        m_matricesVertexBuffer->subData(m_matrices.data(), m_matrices.size(), 0);
    }
    else
    {
        m_instancesIndicesVertexBuffer->bind();
        m_instancesIndicesVertexBuffer->subData(m_instancesIndices.data(), m_instancesIndices.size(), 0);
    }
    
    m_UVsVertexBuffer->bind();
    m_UVsVertexBuffer->subData(m_UVs.data(), m_UVs.size(), 0);
    
    m_positionsBuffer->bind();
    m_positionsBuffer->subData(m_verticesPositions.data(), m_verticesPositions.size(), 0);
    
    m_indicesBuffer->bind();
    m_indicesBuffer->subData(m_indices.data(), m_indices.size(), 0);
}

void SGCore::Instancing::setIsNonInstancingSingleDrawCall(bool isNonInstancingSingleDrawCall) noexcept
{
    if(m_buffersMode == InstancingBuffersMode::SG_USE_VERTICES_NOT_UNIQUE_USE_INDICES_NOT_UNIQUE)
    {
        spdlog::error("Instancing Error while set single draw call mode. Can not use single draw call mode with buffers mode equals to SG_USE_VERTICES_NOT_UNIQUE_USE_INDICES_NOT_UNIQUE.");
        
        return;
    }
    
    m_isNonInstancingSingleDrawCall = isNonInstancingSingleDrawCall;

    if(m_isNonInstancingSingleDrawCall)
    {
        m_matricesVertexBuffer = nullptr;
        
        m_shader->removeSubPass("InstancingPass");
        
        updateCurrentUsingShader();
        
        rebuild();
        
        m_matricesTextureBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
        m_matricesTextureBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
        m_matricesTextureBuffer->m_isTextureBuffer = true;
        m_matricesTextureBuffer->create<float>(m_matrices.data(), m_matrices.size(), 1, 1,
                                               SGGColorInternalFormat::SGG_RGBA32_FLOAT,
                                               SGGColorFormat::SGG_RGBA);
    }
    else
    {
        m_shader->removeSubPass("InstancingPass");
        
        updateCurrentUsingShader();
        
        m_matricesTextureBuffer = nullptr;
    }
}

bool SGCore::Instancing::isNonInstancingSingleDrawCall() const noexcept
{
    return m_isNonInstancingSingleDrawCall;
}

void SGCore::Instancing::checkExampleMeshForWarnings() const noexcept
{
    Ref<IMeshData> lockedExample = m_exampleMeshData.lock();
    
    if(lockedExample->m_uv.empty())
    {
        spdlog::warn("Instancing Warning while set buffers mode. Example mesh has no UV.");
        return;
    }
    
    if(lockedExample->m_bitangents.empty())
    {
        spdlog::warn("Instancing Warning while set buffers mode. Example mesh has no bitangents.");
        return;
    }
    
    if(lockedExample->m_tangents.empty())
    {
        spdlog::warn("Instancing Warning while set buffers mode. Example mesh has no tangents.");
        return;
    }
    
    if(lockedExample->m_normals.empty())
    {
        spdlog::warn("Instancing Warning while set buffers mode. Example mesh has no normals.");
        return;
    }
}

void SGCore::Instancing::updateUniforms() noexcept
{

}

void SGCore::Instancing::updateCurrentUsingShader() noexcept
{
    auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();
    
    if(!renderPipeline) return;
    
    if(!m_isNonInstancingSingleDrawCall)
    {
        m_shader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
                renderPipeline->m_shadersPaths.getByVirtualPath("InstancingShader").getCurrentRealization()));
    }
    else
    {
        m_shader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
                renderPipeline->m_shadersPaths.getByVirtualPath("SingleDrawCallShader").getCurrentRealization()));
    }
    
    updateUniforms();
}
