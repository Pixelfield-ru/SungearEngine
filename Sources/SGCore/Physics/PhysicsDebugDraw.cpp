//
// Created by ilya on 18.02.24.
//

#include "PhysicsDebugDraw.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ISubPassShader.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Camera3D.h"

SGCore::PhysicsDebugDraw::PhysicsDebugDraw()
{
    m_linesPositions.resize(m_maxLines * 6);
    m_linesColors.resize(m_maxLines * 8);
    m_linesIndices.resize(m_maxLines * 2);
    
    m_linesVertexArray = std::shared_ptr<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
    m_linesVertexArray->create()->bind();
    
    // ==============================================
    
    m_linesPositionsVertexBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
    );
    
    m_linesPositionsVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_linesPositions);
    
    std::shared_ptr<IVertexBufferLayout> bufferLayout = Ref<IVertexBufferLayout>(CoreMain::getRenderer()->createVertexBufferLayout());
    bufferLayout
            ->addAttribute(std::shared_ptr<IVertexAttribute>(
                    bufferLayout->createVertexAttribute(0,
                                                        "linesPositionsAttribute",
                                                        SGGDataType::SGG_FLOAT3))
            )
            ->prepare()->enableAttributes();
    
    // ==============================================
    
    m_linesColorsVertexBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
    );
    
    m_linesColorsVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_linesColors);
    
    bufferLayout->reset();
    bufferLayout
            ->addAttribute(std::shared_ptr<IVertexAttribute>(
                    bufferLayout->createVertexAttribute(1,
                                                        "linesColorsAttribute",
                                                        SGGDataType::SGG_FLOAT4))
            )
            ->prepare()->enableAttributes();
    
    // ==============================================
    
    for(std::uint32_t i = 0; i < m_maxLines; i += 2)
    {
        m_linesIndices[i] = i;
        m_linesIndices[i + 1] = i + 1;
    }
    
    m_linesIndexBuffer = Ref<IIndexBuffer>(CoreMain::getRenderer()->createIndexBuffer());
    m_linesIndexBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_linesIndices);
    
    // ==============================================
    
    RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==============================================
    
    m_linesRenderInfo.m_useIndices = true;
    m_linesRenderInfo.m_drawMode = SGDrawMode::SGG_LINES;

    auto currentRenderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();
    if(currentRenderPipeline)
    {
        m_linesShader = MakeRef<IShader>();
        m_linesShader->addSubPassShadersAndCompile(AssetManager::loadAsset<TextFileAsset>(
                currentRenderPipeline->m_shadersPaths.getByVirtualPath("LinesDebugDrawShader").getCurrentRealization()));
    }
}

void SGCore::PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    const size_t linePosIdx = m_currentDrawingLine * 6;
    const size_t lineColorIdx = m_currentDrawingLine * 8;
    
    if(linePosIdx < m_maxLines * 6 && lineColorIdx < m_maxLines * 8)
    {
        m_linesPositions[linePosIdx] = from.x();
        m_linesPositions[linePosIdx + 1] = from.y();
        m_linesPositions[linePosIdx + 2] = from.z();
        
        m_linesPositions[linePosIdx + 3] = to.x();
        m_linesPositions[linePosIdx + 4] = to.y();
        m_linesPositions[linePosIdx + 5] = to.z();
        
        /*m_linesPositionsVertexBuffer->bind();
        m_linesPositionsVertexBuffer->subData(
                { from.getX(), from.getY(), from.getZ(), to.getX(), to.getY(), to.getZ() },
                lineStartIdx);*/
        
        m_linesColors[lineColorIdx] = color.x();
        m_linesColors[lineColorIdx + 1] = color.y();
        m_linesColors[lineColorIdx + 2] = color.z();
        m_linesColors[lineColorIdx + 3] = 1.0;
        
        m_linesColors[lineColorIdx + 4] = color.x();
        m_linesColors[lineColorIdx + 5] = color.y();
        m_linesColors[lineColorIdx + 6] = color.z();
        m_linesColors[lineColorIdx + 7] = 1.0;
        
        /*m_linesColorsVertexBuffer->bind();
        m_linesColorsVertexBuffer->subData(
                { color.getX(), color.getY(), color.getZ(), color.getX(), color.getY(), color.getZ() },
                lineStartIdx);*/
        
        ++m_currentDrawingLine;
    }
}

void SGCore::PhysicsDebugDraw::drawContactPoint(
        const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    // todo: impl
}

void SGCore::PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
    // todo: impl
}

void SGCore::PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
    // todo: impl
}

void SGCore::PhysicsDebugDraw::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

int SGCore::PhysicsDebugDraw::getDebugMode() const
{
    return m_debugMode;
}

void SGCore::PhysicsDebugDraw::drawAll(const Ref<Scene>& scene)
{
    if(!m_linesShader) return;

    auto subPassShader = m_linesShader->getSubPassShader("BatchedLinesPass");
    
    if(!subPassShader) return;
    
    size_t vCnt = std::min(m_currentDrawingLine * 6, m_maxLines * 6);
    size_t cCnt = std::min(m_currentDrawingLine * 8, m_maxLines * 8);
    size_t iCnt = std::min(m_currentDrawingLine * 2, m_maxLines * 2);
    
    m_linesPositionsVertexBuffer->bind();
    m_linesPositionsVertexBuffer->subData(m_linesPositions.data(), vCnt, 0);
    
    m_linesColorsVertexBuffer->bind();
    m_linesColorsVertexBuffer->subData(m_linesColors.data(), cCnt, 0);
    
    subPassShader->bind();
    
    auto camerasView = scene->getECSRegistry()->view<Ref<Camera3D>, Ref<RenderingBase>, Ref<Transform>>();
    
    camerasView.each([this, &subPassShader, &vCnt, &iCnt](Ref<Camera3D>& camera3D, Ref<RenderingBase>& renderingBase, Ref<Transform>& transform) {
        // todo: make get receiver (postprocess or default) and render in them
        
        CoreMain::getRenderer()->prepareUniformBuffers(renderingBase, transform);
        subPassShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
        
        CoreMain::getRenderer()->renderArray(m_linesVertexArray, m_linesRenderInfo, vCnt, iCnt);
    });
    
    m_currentDrawingLine = 0;
}

void SGCore::PhysicsDebugDraw::resetRenderer() noexcept
{
    m_currentDrawingLine = 0;
}

void SGCore::PhysicsDebugDraw::onRenderPipelineSet() noexcept
{
    m_linesShader = MakeRef<IShader>();
    m_linesShader->addSubPassShadersAndCompile(AssetManager::loadAsset<TextFileAsset>(
            RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths.getByVirtualPath("LinesDebugDrawShader").getCurrentRealization()));
}