//
// Created by ilya on 24.02.24.
//

#include <glm/gtc/type_ptr.hpp>

#include "FontSpecializationRenderer.h"
#include "SGCore/Memory/Assets/FontSpecialization.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IVertexAttribute.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "SGCore/Graphics/API/ITexture2D.h"

// todo: fix rendering with indices
SGCore::FontSpecializationRenderer::FontSpecializationRenderer()
{
    // 4 rows * 4 columns
    m_charactersMatrices.resize(m_maxCharactersCount * 16);
    // 4 vertices * 4 (rgba)
    m_charactersColors.resize(m_maxCharactersCount * 16);
    // 4 vertices * 2 (xy)
    m_charactersUVs.resize(m_maxCharactersCount * 8);
    
    // 4 vertices * 3 (xyz)
    m_charactersVerticesPositions.resize(m_maxCharactersCount * 12);
    
    // 6 indices
    m_charactersVerticesIndices.resize(12);
    
    // ==================================================================
    
    m_charactersVerticesIndices[0] = 0;
    m_charactersVerticesIndices[1] = 1;
    m_charactersVerticesIndices[2] = 2;
    m_charactersVerticesIndices[3] = 0;
    m_charactersVerticesIndices[4] = 3;
    m_charactersVerticesIndices[5] = 2;
    
    // ==================================================================
    // creating buffers
    
    {
        m_charactersVertexArray = std::shared_ptr<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
        m_charactersVertexArray->create()->bind();
        
        m_charactersMatricesVertexBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        
        m_charactersMatricesVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(
                m_charactersMatrices);
        
        std::shared_ptr<IVertexBufferLayout> bufferLayout = Ref<IVertexBufferLayout>(
                CoreMain::getRenderer()->createVertexBufferLayout());
        bufferLayout
                ->addAttribute(Ref<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(0,
                                                            "characterModelMatrix",
                                                            SGGDataType::SGG_FLOAT4,
                                                            (size_t) 1))
                )
                ->addAttribute(Ref<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(1,
                                                            "characterModelMatrix",
                                                            SGGDataType::SGG_FLOAT4,
                                                            (size_t) 1))
                )
                ->addAttribute(Ref<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(2,
                                                            "characterModelMatrix",
                                                            SGGDataType::SGG_FLOAT4,
                                                            (size_t) 1))
                )
                ->addAttribute(Ref<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(3,
                                                            "characterModelMatrix",
                                                            SGGDataType::SGG_FLOAT4,
                                                            (size_t) 1))
                )
                ->prepare()->enableAttributes();
        
        // ======
        
        m_charactersColorsVertexBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        
        m_charactersColorsVertexBuffer->setUsage(SGGUsage::SGG_STATIC)->create()->bind()->putData(m_charactersColors);
        
        bufferLayout->reset();
        bufferLayout
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(4,
                                                            "characterColor",
                                                            SGGDataType::SGG_FLOAT4,
                                                            (size_t) 1))
                )
                ->prepare()->enableAttributes();
        
        // ======
        
        // =========================
        
        m_charactersUVsVertexBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        
        m_charactersUVsVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_charactersUVs);
        
        bufferLayout->reset();
        bufferLayout
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(5,
                                                            "characterUV[0]",
                                                            SGGDataType::SGG_FLOAT2,
                                                            (size_t) 1))
                )
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(6,
                                                            "characterUV[1]",
                                                            SGGDataType::SGG_FLOAT2,
                                                            (size_t) 1))
                )
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(7,
                                                            "characterUV[2]",
                                                            SGGDataType::SGG_FLOAT2,
                                                            (size_t) 1))
                )
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(8,
                                                            "characterUV[3]",
                                                            SGGDataType::SGG_FLOAT2,
                                                            (size_t) 1))
                )
                ->prepare()->enableAttributes();
        
        // =========================
        
        m_charactersPositionsBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        
        m_charactersPositionsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(
                m_charactersVerticesPositions);
        
        bufferLayout->reset();
        bufferLayout
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(9,
                                                            "characterVertexPosition[0]",
                                                            SGGDataType::SGG_FLOAT3,
                                                            (size_t) 1))
                )
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(10,
                                                            "characterVertexPosition[1]",
                                                            SGGDataType::SGG_FLOAT3,
                                                            (size_t) 1))
                )
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(11,
                                                            "characterVertexPosition[2]",
                                                            SGGDataType::SGG_FLOAT3,
                                                            (size_t) 1))
                )
                ->addAttribute(std::shared_ptr<IVertexAttribute>(
                        bufferLayout->createVertexAttribute(12,
                                                            "characterVertexPosition[3]",
                                                            SGGDataType::SGG_FLOAT3,
                                                            (size_t) 1))
                )
                ->prepare()->enableAttributes();
        
        // ======================
        
        m_charactersIndexBuffer = Ref<IIndexBuffer>(CoreMain::getRenderer()->createIndexBuffer());
        m_charactersIndexBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(
                m_charactersVerticesIndices);
    }
    
    // ==================================================================
    
    RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==================================================================
    
    m_textRenderInfo.m_useIndices = true;
    m_textRenderInfo.m_enableFacesCulling = false;
    m_textRenderInfo.m_drawMode = SGDrawMode::SGG_TRIANGLES;

    auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();

    if(renderPipeline)
    {
        m_textShader = MakeRef<IShader>();
        m_textShader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
                renderPipeline->m_shadersPaths.getByVirtualPath("StandardTextShader").getCurrentRealization()));
        
        // todo: pass uniforms
        updateUniforms();
    }
}

void SGCore::FontSpecializationRenderer::drawText(SGCore::Text& text, SGCore::Transform& textTransform) noexcept
{
    Ref<FontSpecialization> lockedSpec = m_parentSpecialization.lock();
    
    if(!lockedSpec) return;
    
    float curX = 0;
    float curY = 0;
    
    for(const auto& c : text.m_text)
    {
        if(m_currentDrawingCharacter >= m_maxCharactersCount - 1) return;
        
        if(c == u' ')
        {
            curX += lockedSpec->getMaxCharacterSize().x * 0.4f;
            continue;
        }
        
        if(c == u'\n')
        {
            curY -= lockedSpec->getMaxCharacterSize().y * 1.0f;
            curX = 0.0f;
        }

        const FontGlyph* glyph = lockedSpec->tryGetGlyph(c);
        
        if(glyph)
        {
            // todo:
            
            float xpos = curX + glyph->m_bearing.x;
            float ypos = curY - (glyph->m_realSize.y - glyph->m_bearing.y);
            
            float w = (float) glyph->m_realSize.x;
            float h = (float) glyph->m_realSize.y;
            
            size_t colorOrMatrixIdx = m_currentDrawingCharacter * 16;
            
            // colors =====================================================
            m_charactersColors[colorOrMatrixIdx] = text.m_color.r;
            m_charactersColors[colorOrMatrixIdx + 1] = text.m_color.g;
            m_charactersColors[colorOrMatrixIdx + 2] = text.m_color.b;
            m_charactersColors[colorOrMatrixIdx + 3] = text.m_color.a;
            
            m_charactersColors[colorOrMatrixIdx + 4] = text.m_color.r;
            m_charactersColors[colorOrMatrixIdx + 5] = text.m_color.g;
            m_charactersColors[colorOrMatrixIdx + 6] = text.m_color.b;
            m_charactersColors[colorOrMatrixIdx + 7] = text.m_color.a;
            
            m_charactersColors[colorOrMatrixIdx + 8] = text.m_color.r;
            m_charactersColors[colorOrMatrixIdx + 9] = text.m_color.g;
            m_charactersColors[colorOrMatrixIdx + 10] = text.m_color.b;
            m_charactersColors[colorOrMatrixIdx + 11] = text.m_color.a;
            
            m_charactersColors[colorOrMatrixIdx + 12] = text.m_color.r;
            m_charactersColors[colorOrMatrixIdx + 13] = text.m_color.g;
            m_charactersColors[colorOrMatrixIdx + 14] = text.m_color.b;
            m_charactersColors[colorOrMatrixIdx + 15] = text.m_color.a;
            
            // matrices =====================================================
            
            for(std::uint8_t i = 0; i < 16; ++i)
            {
                m_charactersMatrices[colorOrMatrixIdx + i] = *(glm::value_ptr(textTransform.m_finalTransform.m_modelMatrix) + i);
            }
            
            // uvs =====================================================
            size_t uvsOffset = m_currentDrawingCharacter * 8;
            
            m_charactersUVs[uvsOffset] = glyph->m_uvMin.x;
            m_charactersUVs[uvsOffset + 1] = glyph->m_uvMax.y;
            
            m_charactersUVs[uvsOffset + 2] = glyph->m_uvMin.x;
            m_charactersUVs[uvsOffset + 3] = glyph->m_uvMin.y;
            
            m_charactersUVs[uvsOffset + 4] = glyph->m_uvMax.x;
            m_charactersUVs[uvsOffset + 5] = glyph->m_uvMin.y;
            
            m_charactersUVs[uvsOffset + 6] = glyph->m_uvMax.x;
            m_charactersUVs[uvsOffset + 7] = glyph->m_uvMax.y;
            
            // positions =====================================================
            size_t positionsOffset = m_currentDrawingCharacter * 12;
            
            m_charactersVerticesPositions[positionsOffset] = xpos;
            m_charactersVerticesPositions[positionsOffset + 1] = ypos;
            m_charactersVerticesPositions[positionsOffset + 2] = 0.0;
            
            m_charactersVerticesPositions[positionsOffset + 3] = xpos;
            m_charactersVerticesPositions[positionsOffset + 4] = ypos + h;
            m_charactersVerticesPositions[positionsOffset + 5] = 0.0f;
            
            m_charactersVerticesPositions[positionsOffset + 6] = xpos + w;
            m_charactersVerticesPositions[positionsOffset + 7] = ypos + h;
            m_charactersVerticesPositions[positionsOffset + 8] = 0.0f;
            
            m_charactersVerticesPositions[positionsOffset + 9] = xpos + w;
            m_charactersVerticesPositions[positionsOffset + 10] = ypos;
            m_charactersVerticesPositions[positionsOffset + 11] = 0.0f;
            
            ++m_currentDrawingCharacter;
            curX += (float) (glyph->m_advance.x >> 6);
        }
    }
}

void SGCore::FontSpecializationRenderer::drawAll() noexcept
{
    Ref<FontSpecialization> lockedParentSpec = m_parentSpecialization.lock();
    
    if(!m_textShader || !lockedParentSpec) return;
    
    auto subPassShader = m_textShader->getSubPassShader("TextRenderPass");
    
    if(!subPassShader) return;
    
    size_t vCnt = std::min(m_currentDrawingCharacter, m_maxCharactersCount);
    
    m_charactersMatricesVertexBuffer->bind();
    m_charactersMatricesVertexBuffer->subData(m_charactersMatrices.data(), vCnt * 16, 0);
    
    m_charactersColorsVertexBuffer->bind();
    m_charactersColorsVertexBuffer->subData(m_charactersColors.data(), vCnt * 16, 0);
    
    m_charactersUVsVertexBuffer->bind();
    m_charactersUVsVertexBuffer->subData(m_charactersUVs.data(), vCnt * 8, 0);
    
    m_charactersPositionsBuffer->bind();
    m_charactersPositionsBuffer->subData(m_charactersVerticesPositions.data(), vCnt * 12, 0);
    
    subPassShader->bind();
    subPassShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);
    
    lockedParentSpec->m_atlas->bind(0);
    
    // todo: do for each camera
    CoreMain::getRenderer()->renderArrayInstanced(m_charactersVertexArray, m_textRenderInfo, 6, 6,
                                                  vCnt);
}

void SGCore::FontSpecializationRenderer::resetRenderer() noexcept
{
    m_currentDrawingCharacter = 0;
}

void SGCore::FontSpecializationRenderer::onRenderPipelineSet() noexcept
{
    m_textShader = MakeRef<IShader>();
    m_textShader->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(
            RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths.getByVirtualPath("StandardTextShader").getCurrentRealization()));
    
    updateUniforms();
}

void SGCore::FontSpecializationRenderer::updateUniforms() noexcept
{
    auto subPass = m_textShader->getSubPassShader("TextRenderPass");
    if(subPass)
    {
        subPass->bind();
        
        subPass->useTextureBlock("u_fontSpecializationAtlas", 0);
    }
}
