//
// Created by ilya on 24.02.24.
//

#include <glm/gtc/type_ptr.hpp>

#include "FontSpecializationRenderer.h"

#include "SGCore/UI/Elements/Text.h"
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
SGCore::UI::FontSpecializationRenderer::FontSpecializationRenderer()
{
    // 4 rows * 4 columns * 6 vertices
    m_charactersMatrices.resize(m_maxCharactersCount * 16 * 6);
    // 6 vertices * 4 (rgba)
    m_charactersColors.resize(m_maxCharactersCount * 24);
    // 6 vertices * 2 (xy)
    m_charactersUVs.resize(m_maxCharactersCount * 12);
    
    // 6 vertices * 3 (xyz)
    m_charactersVerticesPositions.resize(m_maxCharactersCount * 18);
    
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

        auto modelMatrixAttrib0 = bufferLayout->createVertexAttribute(0,
                                                               "characterModelMatrix",
                                                               SGGDataType::SGG_FLOAT4,
                                                               4,
                                                               false,
                                                               16 * 4,
                                                               0,
                                                               0);

        bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(modelMatrixAttrib0))->prepare()->enableAttributes();

        auto modelMatrixAttrib1 = bufferLayout->createVertexAttribute(1,
                                                               "characterModelMatrix",
                                                               SGGDataType::SGG_FLOAT4,
                                                               4,
                                                               false,
                                                               16 * 4,
                                                               4 * 4,
                                                               0);

        bufferLayout->reset();
        bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(modelMatrixAttrib1))->prepare()->enableAttributes();

        auto modelMatrixAttrib2 = bufferLayout->createVertexAttribute(2,
                                                                       "characterModelMatrix",
                                                                       SGGDataType::SGG_FLOAT4,
                                                                       4,
                                                                       false,
                                                                       16 * 4,
                                                                       8 * 4,
                                                                       0);

        bufferLayout->reset();
        bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(modelMatrixAttrib2))->prepare()->enableAttributes();

        auto modelMatrixAttrib3 = bufferLayout->createVertexAttribute(3,
                                                                       "characterModelMatrix",
                                                                       SGGDataType::SGG_FLOAT4,
                                                                       4,
                                                                       false,
                                                                       16 * 4,
                                                                       12 * 4,
                                                                       0);

        bufferLayout->reset();
        bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(modelMatrixAttrib3))->prepare()->enableAttributes();
        
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
                                                            (size_t) 0))
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
                                                            "characterUV",
                                                            SGGDataType::SGG_FLOAT2,
                                                            (size_t) 0))
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
                        bufferLayout->createVertexAttribute(6,
                                                            "characterVertexPosition",
                                                            SGGDataType::SGG_FLOAT3,
                                                            (size_t) 0))
                )
                ->prepare()->enableAttributes();
    }
    
    // ==================================================================
    
    RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==================================================================

    m_meshRenderState.m_useIndices = false;
    m_meshRenderState.m_useFacesCulling = false;
    m_meshRenderState.m_drawMode = SGDrawMode::SGG_TRIANGLES;

    auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();

    if(renderPipeline)
    {
        m_textShader = AssetManager::getInstance()->loadAsset<IShader>(*renderPipeline->m_shadersPaths["StandardTextShader"]);
        
        // todo: pass uniforms
        updateUniforms();
    }
}

void SGCore::UI::FontSpecializationRenderer::drawText(Text* text, const Transform& textTransform,
                                                      const UIElementCache& textCache) noexcept
{
    Ref<FontSpecialization> lockedSpec = m_parentSpecialization.lock();
    
    if(!lockedSpec) return;
    
    float curX = 0;
    float curY = 0;
    
    for(const auto& c : text->m_text)
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
            
            const size_t colorIdx = m_currentDrawingCharacter * 24;
            
            // colors =====================================================
            m_charactersColors[colorIdx] = textCache.m_color.r;
            m_charactersColors[colorIdx + 1] = textCache.m_color.g;
            m_charactersColors[colorIdx + 2] = textCache.m_color.b;
            m_charactersColors[colorIdx + 3] = textCache.m_color.a;
            
            m_charactersColors[colorIdx + 4] = textCache.m_color.r;
            m_charactersColors[colorIdx + 5] = textCache.m_color.g;
            m_charactersColors[colorIdx + 6] = textCache.m_color.b;
            m_charactersColors[colorIdx + 7] = textCache.m_color.a;
            
            m_charactersColors[colorIdx + 8] = textCache.m_color.r;
            m_charactersColors[colorIdx + 9] = textCache.m_color.g;
            m_charactersColors[colorIdx + 10] = textCache.m_color.b;
            m_charactersColors[colorIdx + 11] = textCache.m_color.a;
            
            m_charactersColors[colorIdx + 12] = textCache.m_color.r;
            m_charactersColors[colorIdx + 13] = textCache.m_color.g;
            m_charactersColors[colorIdx + 14] = textCache.m_color.b;
            m_charactersColors[colorIdx + 15] = textCache.m_color.a;
            
            m_charactersColors[colorIdx + 16] = textCache.m_color.r;
            m_charactersColors[colorIdx + 17] = textCache.m_color.g;
            m_charactersColors[colorIdx + 18] = textCache.m_color.b;
            m_charactersColors[colorIdx + 19] = textCache.m_color.a;
            
            m_charactersColors[colorIdx + 20] = textCache.m_color.r;
            m_charactersColors[colorIdx + 21] = textCache.m_color.g;
            m_charactersColors[colorIdx + 22] = textCache.m_color.b;
            m_charactersColors[colorIdx + 23] = textCache.m_color.a;
            
            // matrices =====================================================
            
            const size_t matrixIdx = m_currentDrawingCharacter * 16 * 6;

            // for 6 vertices
            for(std::uint8_t i = 0; i < 6; ++i)
            {
                std::memcpy(&m_charactersMatrices[matrixIdx + (16 * i)], glm::value_ptr(textTransform.m_finalTransform.m_modelMatrix), 16 * sizeof(float));
            }
            /*for(std::uint8_t i = 0; i < 6; ++i)
            {
                for(std::uint8_t j = 0; j < 16; ++j)
                {
                    m_charactersMatrices[matrixIdx * (16 + i) + j] = *(glm::value_ptr(textTransform.m_finalTransform.m_modelMatrix) + i);
                }
            }*/

            /*for(std::uint8_t i = 0; i < 16; ++i)
            {
                m_charactersMatrices[matrixIdx + i] = *(glm::value_ptr(textTransform.m_finalTransform.m_modelMatrix) + i);
            }*/
            
            // uvs =====================================================
            const size_t uvsOffset = m_currentDrawingCharacter * 12;
            
            m_charactersUVs[uvsOffset] = glyph->m_uvMin.x;
            m_charactersUVs[uvsOffset + 1] = glyph->m_uvMax.y;
            
            m_charactersUVs[uvsOffset + 2] = glyph->m_uvMin.x;
            m_charactersUVs[uvsOffset + 3] = glyph->m_uvMin.y;
            
            m_charactersUVs[uvsOffset + 4] = glyph->m_uvMax.x;
            m_charactersUVs[uvsOffset + 5] = glyph->m_uvMin.y;
            
            m_charactersUVs[uvsOffset + 6] = glyph->m_uvMin.x;
            m_charactersUVs[uvsOffset + 7] = glyph->m_uvMax.y;
            
            m_charactersUVs[uvsOffset + 8] = glyph->m_uvMax.x;
            m_charactersUVs[uvsOffset + 9] = glyph->m_uvMax.y;
            
            m_charactersUVs[uvsOffset + 10] = glyph->m_uvMax.x;
            m_charactersUVs[uvsOffset + 11] = glyph->m_uvMin.y;
            
            // positions =====================================================
            size_t positionsOffset = m_currentDrawingCharacter * 18;
            
            m_charactersVerticesPositions[positionsOffset] = xpos;
            m_charactersVerticesPositions[positionsOffset + 1] = ypos;
            m_charactersVerticesPositions[positionsOffset + 2] = 0.0;
            
            m_charactersVerticesPositions[positionsOffset + 3] = xpos;
            m_charactersVerticesPositions[positionsOffset + 4] = ypos + h;
            m_charactersVerticesPositions[positionsOffset + 5] = 0.0f;
            
            m_charactersVerticesPositions[positionsOffset + 6] = xpos + w;
            m_charactersVerticesPositions[positionsOffset + 7] = ypos + h;
            m_charactersVerticesPositions[positionsOffset + 8] = 0.0f;
            
            m_charactersVerticesPositions[positionsOffset + 9] = xpos;
            m_charactersVerticesPositions[positionsOffset + 10] = ypos;
            m_charactersVerticesPositions[positionsOffset + 11] = 0.0;
            
            m_charactersVerticesPositions[positionsOffset + 12] = xpos + w;
            m_charactersVerticesPositions[positionsOffset + 13] = ypos;
            m_charactersVerticesPositions[positionsOffset + 14] = 0.0f;
            
            m_charactersVerticesPositions[positionsOffset + 15] = xpos + w;
            m_charactersVerticesPositions[positionsOffset + 16] = ypos + h;
            m_charactersVerticesPositions[positionsOffset + 17] = 0.0f;
            
            ++m_currentDrawingCharacter;
            curX += (float) (glyph->m_advance.x >> 6);
        }
    }
}

void SGCore::UI::FontSpecializationRenderer::drawAll() noexcept
{
    Ref<FontSpecialization> lockedParentSpec = m_parentSpecialization.lock();
    
    if(!m_textShader || !lockedParentSpec) return;
    
    auto subPassShader = m_textShader;
    
    if(!subPassShader) return;
    
    const size_t charsCount = std::min(m_currentDrawingCharacter, m_maxCharactersCount);

    m_charactersVertexArray->bind();

    m_charactersMatricesVertexBuffer->bind();
    m_charactersMatricesVertexBuffer->subData(m_charactersMatrices.data(), charsCount * 16 * 6, 0);
    
    m_charactersColorsVertexBuffer->bind();
    m_charactersColorsVertexBuffer->subData(m_charactersColors.data(), charsCount * 24, 0);
    
    m_charactersUVsVertexBuffer->bind();
    m_charactersUVsVertexBuffer->subData(m_charactersUVs.data(), charsCount * 12, 0);
    
    m_charactersPositionsBuffer->bind();
    m_charactersPositionsBuffer->subData(m_charactersVerticesPositions.data(), charsCount * 18, 0);
    
    subPassShader->bind();
    subPassShader->useUniformBuffer(CoreMain::getRenderer()->m_viewMatricesBuffer);

    subPassShader->useVectorf("u_maxCharacterSize", lockedParentSpec->getMaxCharacterSize());

    subPassShader->useTextureBlock("u_fontSpecializationAtlas", 0);
    subPassShader->useTextureBlock("u_fontSpecializationAtlasSDF", 1);
    
    lockedParentSpec->m_atlas->bind(0);
    lockedParentSpec->m_atlasSDF.m_texture->bind(1);

    CoreMain::getRenderer()->renderArray(m_charactersVertexArray, m_meshRenderState, charsCount * 6, 6);
}

void SGCore::UI::FontSpecializationRenderer::resetRenderer() noexcept
{
    m_currentDrawingCharacter = 0;
}

void SGCore::UI::FontSpecializationRenderer::onRenderPipelineSet() noexcept
{
    m_textShader = AssetManager::getInstance()->loadAsset<IShader>(*RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths["StandardTextShader"]);
    
    updateUniforms();
}

void SGCore::UI::FontSpecializationRenderer::updateUniforms() noexcept
{
    /*auto subPassShader = m_textShader;
    if(subPassShader)
    {
        subPassShader->bind();

        subPassShader->useTextureBlock("u_fontSpecializationAtlas", 0);
        subPassShader->useTextureBlock("u_fontSpecializationAtlasSDF", 1);
    }*/
}
