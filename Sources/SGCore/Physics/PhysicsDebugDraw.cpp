//
// Created by ilya on 18.02.24.
//

#include "PhysicsDebugDraw.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ISubPassShader.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Camera.h"
#include "SGCore/Transformations/Transform.h"

SGCore::PhysicsDebugDraw::PhysicsDebugDraw()
{
    m_linesPositions.resize(m_maxLines * 6);
    m_linesColors.resize(m_maxLines * 6);
    
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
                                                        SGGDataType::SGG_FLOAT3))
            )
            ->prepare()->enableAttributes();
    
    // ==============================================
    
    RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==============================================
    
    m_renderInfo.m_useIndices = false;
    m_renderInfo.m_drawMode = SGDrawMode::SGG_LINES;
}

void SGCore::PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    size_t lineStartIdx = m_currentDrawingLine * 6;
    
    m_linesPositions[lineStartIdx] = from.getX();
    m_linesPositions[lineStartIdx + 1] = from.getY();
    m_linesPositions[lineStartIdx + 2] = from.getZ();
    
    m_linesPositions[lineStartIdx + 3] = to.getX();
    m_linesPositions[lineStartIdx + 4] = to.getY();
    m_linesPositions[lineStartIdx + 5] = to.getZ();
    
    m_linesPositionsVertexBuffer->bind();
    m_linesPositionsVertexBuffer->subData({ from.getX(), from.getY(), from.getZ(), to.getX(), to.getY(), to.getZ() },
                                          lineStartIdx);
    
    m_linesColors[lineStartIdx] = color.getX();
    m_linesColors[lineStartIdx + 1] = color.getY();
    m_linesColors[lineStartIdx + 2] = color.getZ();
    
    m_linesColors[lineStartIdx + 3] = color.getX();
    m_linesColors[lineStartIdx + 4] = color.getY();
    m_linesColors[lineStartIdx + 5] = color.getZ();
    
    m_linesColorsVertexBuffer->bind();
    m_linesColorsVertexBuffer->subData(
            { color.getX(), color.getY(), color.getZ(), color.getX(), color.getY(), color.getZ() },
            lineStartIdx);
    
    ++m_currentDrawingLine;
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

void SGCore::PhysicsDebugDraw::drawAll()
{
    auto subPassShader = m_linesShader->getSubPassShader("PhysicsLinesDebugPass");
    
    if(!subPassShader) return;
    
    subPassShader->bind();

    for(const auto& scene : Scene::getScenes())
    {
        auto camerasView = scene->getECSRegistry().view<RenderingBase, Camera, Transform>();
        
        camerasView.each([this](RenderingBase& renderingBase, Camera& camera, Transform& transform) {
            CoreMain::getRenderer()->prepareUniformBuffers(renderingBase, transform);
            
            CoreMain::getRenderer()->renderArray(m_linesVertexArray, m_renderInfo, m_maxLines * 6, m_maxLines * 6);
        });
    }
}

void SGCore::PhysicsDebugDraw::cleanup()
{
    m_currentDrawingLine = 0;
}

void SGCore::PhysicsDebugDraw::onRenderPipelineSet() noexcept
{
    m_linesShader = MakeRef<IShader>();
    m_linesShader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
            RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths.getByVirtualPath("PhysicsLinesDebugDrawShader").getCurrentRealization()));
}
