//
// Created by ilya on 29.02.24.
//

#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Render/RenderPipelinesManager.h>
#include <glm/gtc/type_ptr.hpp>
#include "Batch.h"

#include "SGCore/Scene/Scene.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Graphics/API/IVertexAttribute.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Transformations/TransformationsUpdater.h"

SGCore::Batch::Batch(const Ref<Scene>& parentScene, const size_t& maxVerticesCount, const size_t& maxInstancesCount)
{
    m_parentScene = parentScene;

    m_maxVerticesCount = maxVerticesCount;
    m_maxIndicesCount = m_maxVerticesCount * 3;
    m_maxInstancesCount = maxInstancesCount;
    
    parentScene->getECSRegistry().on_destroy<Mesh>().connect<&Batch::onMeshDestroyed>(*this);
    parentScene->getECSRegistry().on_destroy<Ref<Transform>>().connect<&Batch::onTransformDestroyed>(*this);
    
    parentScene->getECSRegistry().on_update<Mesh>().connect<&Batch::onMeshUpdate>(*this);
    // parentScene->getECSRegistry().on_update<Ref<Transform>>().connect<&Batch::onTransformUpdate>(*this);
    
    (*parentScene->getSystem<TransformationsUpdater>()->m_transformChangedEvent) += m_transformChangedListener;
    
    /*m_meshUpdateObserver.connect(parentScene->getECSRegistry(), entt::basic_collector<>::update<Mesh>());
    m_transformUpdateObserver.connect(parentScene->getECSRegistry(), entt::basic_collector<>::update<Transform>());*/
    
    m_verticesPositions.resize(m_maxVerticesCount * 3);
    m_UVs.resize(m_maxVerticesCount * 3);
    m_modelMatrices.resize(m_maxInstancesCount * 16);
    m_instancesIndices.resize(m_maxVerticesCount);
    m_indices.resize(m_maxIndicesCount);
    
    m_vertexArray = SGCore::Ref<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
    m_vertexArray->create()->bind();
    
    {
        Ref<IVertexBufferLayout> bufferLayout = Ref<IVertexBufferLayout>(
                CoreMain::getRenderer()->createVertexBufferLayout());
        
        m_instancesIndicesVertexBuffer = Ref<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        
        m_instancesIndicesVertexBuffer->setUsage(SGG_STATIC)->create()->bind()->putData(
                m_instancesIndices);
        
        bufferLayout->reset()
                ->addAttribute(SGCore::Ref<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(0,
                                                            "instanceID",
                                                            SGG_INT,
                                                            (size_t) 0))
                )
                ->prepare()->enableAttributes();
        
        // =================
        
        m_UVsVertexBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        
        m_UVsVertexBuffer->setUsage(SGG_DYNAMIC)->create()->bind()->putData(m_UVs);
        
        bufferLayout->reset();
        bufferLayout
                ->addAttribute(Ref<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(1,
                                                            "vertexUV",
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
                        bufferLayout->createVertexAttribute(2,
                                                            "vertexPosition",
                                                            SGG_FLOAT3,
                                                            (size_t) 0))
                )
                ->prepare()->enableAttributes();
        
        // ===============
        
        m_indicesBuffer = Ref<IIndexBuffer>(CoreMain::getRenderer()->createIndexBuffer());
        m_indicesBuffer->setUsage(SGG_DYNAMIC)->create()->bind()->putData(m_indices);
        
        // ===============
        
        m_matricesTextureBuffer = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
        m_matricesTextureBuffer->m_textureBufferUsage = SGGUsage::SGG_DYNAMIC;
        m_matricesTextureBuffer->m_isTextureBuffer = true;
        m_matricesTextureBuffer->create<float>(m_modelMatrices.data(), m_modelMatrices.size(), 1, 1,
                                               SGGColorInternalFormat::SGG_RGBA32_FLOAT,
                                               SGGColorFormat::SGG_RGBA);
    }
    // ==================================================================
    
    SGCore::RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==================================================================
    
    m_renderInfo.m_useIndices = true;
    m_renderInfo.m_enableFacesCulling = true;
    m_renderInfo.m_drawMode = SGDrawMode::SGG_TRIANGLES;
    
    m_shader = SGCore::MakeRef<SGCore::IShader>();
    
    onRenderPipelineSet();
}

void SGCore::Batch::renderAll() noexcept
{
    if(!m_shader) return;
    
    auto subPassShader = m_shader->getSubPassShader("BatchingPass");
    
    if(!subPassShader) return;
    
    size_t indicesCount = std::min<size_t>(m_currentIndicesCountToRender, m_maxIndicesCount);
    size_t verticesCount = std::min<size_t>(m_currentVerticesCountToRender, m_maxVerticesCount);
    size_t instanceCnt = std::min<size_t>(m_currentRenderingInstancesCount, m_maxInstancesCount);
    
    // std::cout << "indicesCount : " << indicesCount << ", verticesCount: " << verticesCount << std::endl;
    
    subPassShader->bind();
    subPassShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
    
    if(m_positionsArrayChanged)
    {
        m_positionsBuffer->bind();
        m_positionsBuffer->subData(m_verticesPositions.data(), verticesCount * 3, 0);
    }
    
    if(m_UVsArrayChanged)
    {
        m_UVsVertexBuffer->bind();
        m_UVsVertexBuffer->subData(m_UVs.data(), verticesCount * 3, 0);
    }
    
    if(m_indicesArrayChanged)
    {
        m_indicesBuffer->bind();
        m_indicesBuffer->subData(m_indices.data(), indicesCount, 0);
    }
    
    if(m_instancesIndicesChanged)
    {
        m_instancesIndicesVertexBuffer->bind();
        m_instancesIndicesVertexBuffer->subData(m_instancesIndices.data(), indicesCount, 0);
    }
    
    m_matricesTextureBuffer->bind(0);
    if(m_modelMatricesArrayChanged)
    {
        m_matricesTextureBuffer->subTextureBufferData<float>(m_modelMatrices.data(), instanceCnt * 16, 0);
    }
    subPassShader->useTextureBlock("u_matricesTextureBuffer", 0);
    
    CoreMain::getRenderer()->renderArray(m_vertexArray, m_renderInfo,
                                         verticesCount,
                                         indicesCount);
    
    m_positionsArrayChanged = false;
    m_UVsArrayChanged = false;
    m_indicesArrayChanged = false;
    m_modelMatricesArrayChanged = false;
    m_instancesIndicesChanged = false;
}

void SGCore::Batch::addEntity(const entity_t& entity) noexcept
{
    auto lockedScene = m_parentScene.lock();
    
    if(!lockedScene)
    {
        spdlog::error("Batching error: can not add entity. Parent scene is not set.");
        return;
    }
    
    if(!lockedScene->getECSRegistry().all_of<Mesh, Ref<Transform>>(entity))
    {
        spdlog::error("Batching error: can not add entity. One of Mesh or Transform components is not set.");
        return;
    }
    
    Mesh& entityMesh = lockedScene->getECSRegistry().get<Mesh>(entity);
    
    if(entityMesh.m_base.m_meshData->m_positions.empty())
    {
        spdlog::error("Batching error: can not add entity. Entity`s mesh does not have vertices.");
        return;
    }
    
    BatchEntityRanges* lastRanges = nullptr;
    if(!m_entitiesRanges.empty())
    {
        lastRanges = &*m_entitiesRanges.rbegin();
    }
    
    size_t positionsCount = entityMesh.m_base.m_meshData->m_positions.size();
    size_t uvsCount = entityMesh.m_base.m_meshData->m_uv.size();
    size_t indicesCount = entityMesh.m_base.m_meshData->m_indices.size();
    
    /*std::cout << "========================" << std::endl;
    for(size_t i = 0; i < entityMesh.m_base.m_meshData->m_indices.size(); ++i)
    {
        std::cout << "idx : " << entityMesh.m_base.m_meshData->m_indices[i] << std::endl;
    }
    std::cout << "========================" << std::endl;*/
    
    if(indicesCount == 0)
    {
        spdlog::warn("Batching warning: Entity`s mesh does not have indices.");
    }
    
    BatchEntityRanges newRanges;
    
    if(!lastRanges)
    {
        newRanges.m_verticesRange.x = 0;
        newRanges.m_verticesRange.y = positionsCount;
        
        newRanges.m_UVsRange.x = 0;
        newRanges.m_UVsRange.y = (uvsCount == 0 ? positionsCount : uvsCount);
        
        newRanges.m_indicesRange.x = 0;
        newRanges.m_indicesRange.y = (indicesCount == 0 ? (positionsCount / 3) : indicesCount);
    }
    else
    {
        newRanges.m_verticesRange.x = lastRanges->m_verticesRange.y;
        newRanges.m_verticesRange.y = newRanges.m_verticesRange.x + positionsCount;
        
        newRanges.m_UVsRange.x = lastRanges->m_UVsRange.y;
        newRanges.m_UVsRange.y = newRanges.m_UVsRange.x + (uvsCount == 0 ? positionsCount : uvsCount);
        
        newRanges.m_indicesRange.x = lastRanges->m_indicesRange.y;
        newRanges.m_indicesRange.y = newRanges.m_indicesRange.x + (indicesCount == 0 ? (positionsCount / 3) : indicesCount);
    }
    
    if(newRanges.m_verticesRange.y > m_maxVerticesCount * 3)
    {
        spdlog::error("Batching error: can not add entity. Not enough space for vertices.");
        return;
    }
    
    if(newRanges.m_indicesRange.y > m_maxIndicesCount)
    {
        spdlog::error("Batching error: can not add entity. Not enough space for indices.");
        return;
    }
    
    m_entitiesIndices[entity] = m_entitiesRanges.size();
    m_entitiesRanges.push_back(newRanges);
    
    updateArraysForEntity(lockedScene, entity);
    
    m_currentVerticesCountToRender += (newRanges.m_verticesRange.y - newRanges.m_verticesRange.x) / 3;
    m_currentIndicesCountToRender += (newRanges.m_indicesRange.y - newRanges.m_indicesRange.x);
    
    const size_t vOffset = newRanges.m_verticesRange.x;
    const size_t uvOffset = newRanges.m_UVsRange.x;
    const size_t iOffset = newRanges.m_indicesRange.x;
    
    m_positionsBuffer->bind();
    m_positionsBuffer->subData(m_verticesPositions.data() + vOffset, newRanges.m_verticesRange.y - vOffset, vOffset);
    
    m_UVsVertexBuffer->bind();
    m_UVsVertexBuffer->subData(m_UVs.data() + uvOffset, newRanges.m_UVsRange.y - uvOffset, uvOffset);
    
    m_indicesBuffer->bind();
    m_indicesBuffer->subData(m_indices.data() + iOffset, newRanges.m_indicesRange.y - iOffset, iOffset);
    
    m_instancesIndicesVertexBuffer->bind();
    m_instancesIndicesVertexBuffer->subData(m_instancesIndices.data() + vOffset / 3, (newRanges.m_verticesRange.y - vOffset) / 3,  vOffset / 3);
    
    m_matricesTextureBuffer->bind(0);
    m_matricesTextureBuffer->subTextureBufferData<float>(m_modelMatrices.data() + m_currentRenderingInstancesCount * 16, 16,
                                                         m_currentRenderingInstancesCount * 16);
    
    ++m_currentRenderingInstancesCount;
    /*m_positionsArrayChanged = true;
    m_UVsArrayChanged = true;
    m_indicesArrayChanged = true;
    m_modelMatricesArrayChanged = true;
    m_instancesIndicesChanged = true;*/
}

void SGCore::Batch::removeEntity(const entity_t& entity) noexcept
{
    auto lockedScene = m_parentScene.lock();
    
    if(!lockedScene)
    {
        spdlog::error("Batching error: can not remove entity. Parent scene is not set.");
        return;
    }
    
    auto foundEntityInter = m_entitiesIndices.find(entity);
    
    if(foundEntityInter == m_entitiesIndices.end())
    {
        return;
    }

    const BatchEntityRanges& entityRanges = m_entitiesRanges[foundEntityInter->second];
    
    m_currentVerticesCountToRender -= (entityRanges.m_verticesRange.y - entityRanges.m_verticesRange.x) / 3;
    m_currentIndicesCountToRender -= (entityRanges.m_indicesRange.y - entityRanges.m_indicesRange.x);
    
    --m_currentRenderingInstancesCount;
    
    m_entitiesRanges.erase(m_entitiesRanges.begin() + foundEntityInter->second);
    m_entitiesIndices.erase(entity);
    
    size_t idx = 0;
    
    for(auto& it : m_entitiesIndices)
    {
        it.second = idx;
        ++idx;
    }

    recalculateRanges();
    
    // then update all arrays
    
    for(const auto& eIt : m_entitiesIndices)
    {
        const auto& e = eIt.first;
        
        updateArraysForEntity(lockedScene, e);
    }
    
    m_positionsArrayChanged = true;
    m_UVsArrayChanged = true;
    m_indicesArrayChanged = true;
    m_modelMatricesArrayChanged = true;
    m_instancesIndicesChanged = true;
}

void SGCore::Batch::updateArraysForEntity(const Ref<Scene>& lockedScene, const entity_t& entity) noexcept
{
    const size_t& entityIdx = m_entitiesIndices[entity];
    Mesh& entityMesh = lockedScene->getECSRegistry().get<Mesh>(entity);
    Ref<Transform>& entityTransform = lockedScene->getECSRegistry().get<Ref<Transform>>(entity);
    BatchEntityRanges& ranges = m_entitiesRanges[entityIdx];
    
    size_t positionsCount = entityMesh.m_base.m_meshData->m_positions.size();
    size_t uvsCount = entityMesh.m_base.m_meshData->m_uv.size();
    size_t indicesCount = entityMesh.m_base.m_meshData->m_indices.size();
    
    for(size_t v = ranges.m_verticesRange.x; v < ranges.m_verticesRange.y; ++v)
    {
        m_verticesPositions[v] = entityMesh.m_base.m_meshData->m_positions[v - ranges.m_verticesRange.x];
        
        if(v % 2 == 0)
        {
            m_instancesIndices[v / 3] = entityIdx;
        }
    }
    
    /*for(size_t v = ranges.m_verticesRange.x; v < ranges.m_verticesRange.y; v += 3)
    {
        float x;
        float y;
        float z;
        
        entityMesh.m_base.m_meshData->getVertexPosition((v - ranges.m_verticesRange.x) / 3, x, y, z);
        
        m_verticesPositions[v] = x;
        m_verticesPositions[v + 1] = y;
        m_verticesPositions[v + 2] = z;
    }*/
    
    for(size_t uv = ranges.m_UVsRange.x; uv < ranges.m_UVsRange.y; ++uv)
    {
        if(uvsCount == 0)
        {
            m_UVs[uv] = 0.0f;
        }
        else
        {
            m_UVs[uv] = entityMesh.m_base.m_meshData->m_uv[uv - ranges.m_UVsRange.x];
        }
    }
    
    for(size_t i = ranges.m_indicesRange.x; i < ranges.m_indicesRange.y; ++i)
    {
        if(indicesCount == 0)
        {
            m_indices[i] = (ranges.m_verticesRange.x / 3) + i;
        }
        else
        {
            m_indices[i] = (ranges.m_verticesRange.x / 3) + entityMesh.m_base.m_meshData->m_indices[i - ranges.m_indicesRange.x];
        }
    }
    
    const float* matPtr = glm::value_ptr(entityTransform->m_finalTransform.m_modelMatrix);
    
    const size_t matrixStartIdx = entityIdx * 16;
    const size_t matrixEndIdx = matrixStartIdx + 16;
    
    for(size_t m = matrixStartIdx; m < matrixEndIdx; ++m)
    {
        m_modelMatrices[m] = *(matPtr + (m - matrixStartIdx));
    }
}

void SGCore::Batch::recalculateRanges() noexcept
{
    size_t curVertexOffset = 0;
    size_t curUVOffset = 0;
    size_t curIndexOffset = 0;
    
    for(auto& ranges : m_entitiesRanges)
    {
        size_t vDif = ranges.m_verticesRange.y - ranges.m_verticesRange.x;
        size_t uvDif = ranges.m_UVsRange.y - ranges.m_UVsRange.x;
        size_t iDif = ranges.m_indicesRange.y - ranges.m_indicesRange.x;
        
        ranges.m_verticesRange.x = curVertexOffset;
        ranges.m_verticesRange.y = curVertexOffset + vDif;
        
        ranges.m_UVsRange.x = curUVOffset;
        ranges.m_UVsRange.y = curUVOffset + uvDif;
        
        ranges.m_indicesRange.x = curIndexOffset;
        ranges.m_indicesRange.y = curIndexOffset + iDif;
        
        curVertexOffset += vDif;
        curUVOffset += uvDif;
        curIndexOffset += iDif;
    }
}

void SGCore::Batch::onMeshDestroyed(entt::basic_registry<entity_t>& registry, entity_t entity) noexcept
{
    if(m_entitiesIndices.contains(entity))
    {
        removeEntity(entity);
    }
}

void SGCore::Batch::onTransformDestroyed(entt::basic_registry<entity_t>& registry, entity_t entity) noexcept
{
    if(m_entitiesIndices.contains(entity))
    {
        removeEntity(entity);
    }
}

void SGCore::Batch::onMeshUpdate(entt::basic_registry<entity_t>& registry, entity_t entity) noexcept
{
    auto it = m_entitiesIndices.find(entity);
    if(it == m_entitiesIndices.end()) return;
    
    const size_t& entityIdx = it->second;
    
    BatchEntityRanges& ranges = m_entitiesRanges[entityIdx];
    
    // todo:
}

void SGCore::Batch::onTransformUpdate(entt::basic_registry<entity_t>& registry, entity_t entity, Ref<const Transform> transform) noexcept
{
    auto it = m_entitiesIndices.find(entity);
    if(it == m_entitiesIndices.end()) return;
    
    const size_t& entityIdx = it->second;
    
    const size_t startIdx = entityIdx * 16;
    const size_t endIdx = startIdx + 16;
    
    const float* matPtr = glm::value_ptr(transform->m_finalTransform.m_modelMatrix);
    
    // TRUE
    // std::cout << "this: " << this << ", " << transform->m_finalTransform.m_position.x << ", " << transform->m_finalTransform.m_position.y << ", " << transform->m_finalTransform.m_position.z << std::endl;
    
    for(size_t m = startIdx; m < endIdx; ++m)
    {
        m_modelMatrices[m] = *(matPtr + (m - startIdx));
    }
    
    m_modelMatricesArrayChanged = true;
}

void SGCore::Batch::onRenderPipelineSet() noexcept
{
    auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();
    
    if(!renderPipeline) return;
    
    m_shader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
            renderPipeline->m_shadersPaths.getByVirtualPath("BatchingShader").getCurrentRealization()));
}
