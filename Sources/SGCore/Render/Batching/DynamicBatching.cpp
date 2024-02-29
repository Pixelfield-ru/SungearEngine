//
// Created by ilya on 29.02.24.
//

#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Render/RenderPipelinesManager.h>
#include "DynamicBatching.h"

#include "SGCore/Scene/Scene.h"
#include "DynamicBatchingTarget.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Graphics/API/IVertexAttribute.h"
#include "SGCore/Graphics/API/IShader.h"

SGCore::DynamicBatching::DynamicBatching()
{
    m_verticesPositions.resize(m_maxVerticesCount * 3);
    m_UVs.resize(m_maxVerticesCount * 2);
    m_modelMatrices.resize(m_maxInstancesCount * 16);
    m_instancesIndices.resize(m_maxIndicesCount);
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
                                                            SGG_FLOAT,
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
    
    m_renderInfo.m_enableFacesCulling = true;
    m_renderInfo.m_drawMode = SGDrawMode::SGG_TRIANGLES;
    
    m_shader = SGCore::MakeRef<SGCore::IShader>();
    
    onRenderPipelineSet();
}

void SGCore::DynamicBatching::update(const double& dt) noexcept
{

}

void SGCore::DynamicBatching::onAddToScene() noexcept
{
    auto lockedScene = m_scene.lock();
    
    lockedScene->getECSRegistry().on_construct<DynamicBatchingTarget>().connect<&DynamicBatching::onTargetConstruct>(*this);
    lockedScene->getECSRegistry().on_destroy<DynamicBatchingTarget>().connect<&DynamicBatching::onTargetDestroy>(*this);
}

void SGCore::DynamicBatching::onTargetConstruct(entt::registry& registry, entt::entity targetEntity) noexcept
{
    m_entitiesIndices[targetEntity] = m_currentEntityIndex;
    
    ++m_currentEntityIndex;
}

void SGCore::DynamicBatching::onTargetDestroy(entt::registry& registry, entt::entity targetEntity) noexcept
{

}

void SGCore::DynamicBatching::onRenderPipelineSet() noexcept
{
    auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();
    
    if(!renderPipeline) return;
    
    m_shader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
            renderPipeline->m_shadersPaths.getByVirtualPath("DynamicBatchingShader").getCurrentRealization()));
}
