//
// Created by ilya on 08.03.24.
//

#include "DebugDraw.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Camera3D.h"

SGCore::DebugDraw::DebugDraw()
{
    m_linesPositions.resize(m_maxLines * 6);
    m_linesColors.resize(m_maxLines * 8);
    m_linesIndices.resize(m_maxLines * 2);
    
    m_linesVertexArray = std::shared_ptr<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
    m_linesVertexArray->create();
    m_linesVertexArray->bind();

    // ==============================================
    
    m_linesPositionsVertexBuffer = std::shared_ptr<IVertexBuffer>(
            CoreMain::getRenderer()->createVertexBuffer()
    );
    
    m_linesPositionsVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
    m_linesPositionsVertexBuffer->create();
    m_linesPositionsVertexBuffer->bind();
    m_linesPositionsVertexBuffer->putData(m_linesPositions);

    m_linesVertexArray->addVertexBuffer(m_linesPositionsVertexBuffer.get());
    
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
    
    m_linesColorsVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
    m_linesColorsVertexBuffer->create();
    m_linesColorsVertexBuffer->bind();
    m_linesColorsVertexBuffer->putData(m_linesColors);

    m_linesVertexArray->addVertexBuffer(m_linesColorsVertexBuffer.get());

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
    m_linesIndexBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
    m_linesIndexBuffer->create();
    m_linesIndexBuffer->bind();
    m_linesIndexBuffer->putData(m_linesIndices);

    m_linesVertexArray->setIndexBuffer(m_linesIndexBuffer.get());
    
    // ==============================================
    
    RenderPipelinesManager::instance().subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==============================================

    m_meshRenderState.m_useIndices = true;
    m_meshRenderState.m_drawMode = SGDrawMode::SGG_LINES;
    
    auto currentRenderPipeline = RenderPipelinesManager::instance().getCurrentRenderPipeline();
    if(currentRenderPipeline)
    {
        m_linesShader = Ref<IShader>(CoreMain::getRenderer()->createShader());
        m_linesShader->compile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
                *currentRenderPipeline->m_shadersPaths["LinesDebugDrawShader"]));
    }
}

void SGCore::DebugDraw::drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color) noexcept
{
    if(m_mode == DebugDrawMode::NO_DEBUG) return;
    
    const size_t linePosIdx = m_currentDrawingLine * 6;
    const size_t lineColorIdx = m_currentDrawingLine * 8;
    
    if(linePosIdx < m_maxLines * 6 && lineColorIdx < m_maxLines * 8)
    {
        m_linesPositions[linePosIdx] = from.x;
        m_linesPositions[linePosIdx + 1] = from.y;
        m_linesPositions[linePosIdx + 2] = from.z;
        
        m_linesPositions[linePosIdx + 3] = to.x;
        m_linesPositions[linePosIdx + 4] = to.y;
        m_linesPositions[linePosIdx + 5] = to.z;
        
        /*m_linesPositionsVertexBuffer->bind();
        m_linesPositionsVertexBuffer->subData(
                { from.getX(), from.getY(), from.getZ(), to.getX(), to.getY(), to.getZ() },
                lineStartIdx);*/
        
        m_linesColors[lineColorIdx] = color.x;
        m_linesColors[lineColorIdx + 1] = color.y;
        m_linesColors[lineColorIdx + 2] = color.z;
        m_linesColors[lineColorIdx + 3] = color.w;
        
        m_linesColors[lineColorIdx + 4] = color.x;
        m_linesColors[lineColorIdx + 5] = color.y;
        m_linesColors[lineColorIdx + 6] = color.z;
        m_linesColors[lineColorIdx + 7] = color.w;
        
        /*m_linesColorsVertexBuffer->bind();
        m_linesColorsVertexBuffer->subData(
                { color.getX(), color.getY(), color.getZ(), color.getX(), color.getY(), color.getZ() },
                lineStartIdx);*/
        
        ++m_currentDrawingLine;
    }
}

void SGCore::DebugDraw::drawAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color) noexcept
{
    if(m_mode == DebugDrawMode::NO_DEBUG) return;
    
    // face
    drawLine(min, { min.x, max.y, min.z }, color);
    drawLine({ min.x, max.y, min.z }, { max.x, max.y, min.z }, color);
    drawLine({ max.x, max.y, min.z }, { max.x, min.y, min.z }, color);
    drawLine({ max.x, min.y, min.z }, min, color);
    
    // backface
    drawLine({ min.x, min.y, max.z }, { min.x, max.y, max.z }, color);
    drawLine({ min.x, max.y, max.z }, { max.x, max.y, max.z }, color);
    drawLine({ max.x, max.y, max.z }, { max.x, min.y, max.z }, color);
    drawLine({ max.x, min.y, max.z }, { min.x, min.y, max.z }, color);
    
    // left face
    drawLine({ min.x, max.y, min.z }, { min.x, max.y, max.z }, color);
    drawLine({ min.x, min.y, max.z }, { min.x, min.y, min.z }, color);
    
    // right face
    drawLine({ max.x, max.y, min.z }, { max.x, max.y, max.z }, color);
    drawLine({ max.x, min.y, max.z }, { max.x, min.y, min.z }, color);
}

void SGCore::DebugDraw::render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) noexcept
{
    if(!m_linesShader || !scene || m_mode == DebugDrawMode::NO_DEBUG) return;

    auto subPassShader = m_linesShader;

    if(!subPassShader || subPassShader->getAnalyzedFile()->getSubPassName() != "BatchedLinesPass") return;

    size_t vCnt = std::min(m_currentDrawingLine * 6, m_maxLines * 6);
    size_t cCnt = std::min(m_currentDrawingLine * 8, m_maxLines * 8);
    size_t iCnt = std::min(m_currentDrawingLine * 2, m_maxLines * 2);

    m_linesPositionsVertexBuffer->bind();
    m_linesPositionsVertexBuffer->subData(m_linesPositions.data(), vCnt, 0);

    m_linesColorsVertexBuffer->bind();
    m_linesColorsVertexBuffer->subData(m_linesColors.data(), cCnt, 0);

    subPassShader->bind();

    auto camerasView = scene->getECSRegistry()->view<Camera3D, RenderingBase, Transform, LayeredFrameReceiver>();

    camerasView.each([this, &subPassShader, &vCnt, &iCnt](Camera3D::reg_t& camera3D,
                                                          RenderingBase::reg_t& renderingBase,
                                                          Transform::reg_t& transform,
                                                          const LayeredFrameReceiver& cameraLayeredFrameReceiver) {
        // todo: make get receiver (postprocess or default) and render in them
        cameraLayeredFrameReceiver.m_layersFrameBuffer->bind();
        cameraLayeredFrameReceiver.m_layersFrameBuffer->bindAttachmentsToDrawIn(cameraLayeredFrameReceiver.m_attachmentToRenderIn);

        CoreMain::getRenderer()->prepareUniformBuffers(renderingBase, transform);
        subPassShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);

        CoreMain::getRenderer()->renderArray(m_linesVertexArray, m_meshRenderState, vCnt, iCnt);

        cameraLayeredFrameReceiver.m_layersFrameBuffer->unbind();
    });

    m_currentDrawingLine = 0;
}

void SGCore::DebugDraw::resetRenderer() noexcept
{
    m_currentDrawingLine = 0;
}

void SGCore::DebugDraw::onRenderPipelineSet() noexcept
{
    m_linesShader = Ref<IShader>(CoreMain::getRenderer()->createShader());
    m_linesShader->compile(AssetManager::getInstance()->loadAsset<TextFileAsset>(
            *RenderPipelinesManager::instance().getCurrentRenderPipeline()->m_shadersPaths["LinesDebugDrawShader"]));
}
