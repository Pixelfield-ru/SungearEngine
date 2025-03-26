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
                                                      UIElementCache& textCache) noexcept
{
    Ref<FontSpecialization> lockedSpec = m_parentSpecialization.lock();
    
    if(!lockedSpec) return;

    if(text->m_text.empty())
    {
        text->m_textSize.y = 0;
        return;
    }
    
    double curX = 0;
    double curY = 0;
    double lineHeightOffset = 0.0f;

    const auto& specMetrics = lockedSpec->getMetrics();
    const auto& specGeometry = lockedSpec->getGeometry();

    const double fsScale = (1.0 / (specMetrics.ascenderY - specMetrics.descenderY)) * lockedSpec->getSettings().m_height;
    const double descenderYPos = (-specMetrics.descenderY) * fsScale;
    const double ascenderYPos = (specMetrics.ascenderY) * fsScale;

    const double lineHeight = fsScale * specMetrics.lineHeight + lineHeightOffset;

    text->m_textSize.y = lineHeight + (ascenderYPos - descenderYPos);

    const msdf_atlas::GlyphGeometry* unknownGlyph = lockedSpec->tryGetGlyph('?');
    const msdf_atlas::GlyphGeometry* spaceGlyph = lockedSpec->tryGetGlyph(' ');

    for(size_t i = 0; i < text->m_text.size(); ++i)
    {
        auto c = text->m_text[i];

        if(m_currentDrawingCharacter >= m_maxCharactersCount - 1) return;

        if(c == '\n')
        {
            curX = 0.0f;
            curY -= lineHeight;

            text->m_textSize.y += lineHeight;

            continue;
        }

        const msdf_atlas::GlyphGeometry* glyph = lockedSpec->tryGetGlyph(c);

        if(!glyph)
        {
            c = '?';
            glyph = unknownGlyph;
        }

        if(!glyph) return;

        if(c == '\t')
        {
            glyph = spaceGlyph;
        }


        if(glyph)
        {
            double al, ab, ar, at;
            glyph->getQuadAtlasBounds(al, ab, ar, at);
            const glm::vec2 uvMin { al / lockedSpec->getSize().x, ab / lockedSpec->getSize().y };
            const glm::vec2 uvMax { ar / lockedSpec->getSize().x, at / lockedSpec->getSize().y };

            double pl, pb, pr, pt;
            glyph->getQuadPlaneBounds(pl, pb, pr, pt);
            glm::vec2 quadMin = glm::vec2 { pl, pb } * fsScale + glm::vec2(curX, curY);
            glm::vec2 quadMax = glm::vec2 { pr, pt } * fsScale + glm::vec2(curX, curY);

            const float offsetFromDescender = (quadMax.y - descenderYPos);
            const float offsetFromAscender = (quadMin.y - ascenderYPos);

            quadMin.y -= offsetFromDescender + offsetFromAscender;
            quadMax.y -= offsetFromDescender + offsetFromAscender;

            const size_t colorIdx = m_currentDrawingCharacter * 24;

            // colors =====================================================
#pragma region Colors
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
#pragma endregion Colors

            // matrices =====================================================
#pragma region Matrices

            const size_t matrixIdx = m_currentDrawingCharacter * 16 * 6;

            // for 6 vertices
            for(std::uint8_t i = 0; i < 6; ++i)
            {
                std::memcpy(&m_charactersMatrices[matrixIdx + (16 * i)],
                            glm::value_ptr(textTransform.m_finalTransform.m_modelMatrix), 16 * sizeof(float));
            }

#pragma endregion Matrices

            // uvs =====================================================
#pragma region UVs

            const size_t uvsOffset = m_currentDrawingCharacter * 12;

            m_charactersUVs[uvsOffset] = uvMin.x;
            m_charactersUVs[uvsOffset + 1] = uvMax.y;

            m_charactersUVs[uvsOffset + 2] = uvMin.x;
            m_charactersUVs[uvsOffset + 3] = uvMin.y;

            m_charactersUVs[uvsOffset + 4] = uvMax.x;
            m_charactersUVs[uvsOffset + 5] = uvMin.y;

            m_charactersUVs[uvsOffset + 6] = uvMin.x;
            m_charactersUVs[uvsOffset + 7] = uvMax.y;

            m_charactersUVs[uvsOffset + 8] = uvMax.x;
            m_charactersUVs[uvsOffset + 9] = uvMax.y;

            m_charactersUVs[uvsOffset + 10] = uvMax.x;
            m_charactersUVs[uvsOffset + 11] = uvMin.y;

#pragma endregion UVs

            // positions =====================================================
#pragma region Positions

            size_t positionsOffset = m_currentDrawingCharacter * 18;

            m_charactersVerticesPositions[positionsOffset] = quadMin.x;
            m_charactersVerticesPositions[positionsOffset + 1] = quadMin.y;
            m_charactersVerticesPositions[positionsOffset + 2] = 0.0;

            m_charactersVerticesPositions[positionsOffset + 3] = quadMin.x;
            m_charactersVerticesPositions[positionsOffset + 4] = quadMax.y;
            m_charactersVerticesPositions[positionsOffset + 5] = 0.0f;

            m_charactersVerticesPositions[positionsOffset + 6] = quadMax.x;
            m_charactersVerticesPositions[positionsOffset + 7] = quadMax.y;
            m_charactersVerticesPositions[positionsOffset + 8] = 0.0f;

            m_charactersVerticesPositions[positionsOffset + 9] = quadMin.x;
            m_charactersVerticesPositions[positionsOffset + 10] = quadMin.y;
            m_charactersVerticesPositions[positionsOffset + 11] = 0.0;

            m_charactersVerticesPositions[positionsOffset + 12] = quadMax.x;
            m_charactersVerticesPositions[positionsOffset + 13] = quadMin.y;
            m_charactersVerticesPositions[positionsOffset + 14] = 0.0f;

            m_charactersVerticesPositions[positionsOffset + 15] = quadMax.x;
            m_charactersVerticesPositions[positionsOffset + 16] = quadMax.y;
            m_charactersVerticesPositions[positionsOffset + 17] = 0.0f;

#pragma endregion Positions

            ++m_currentDrawingCharacter;

            if(i < text->m_text.size() - 1)
            {
                // float kerningOffset = 0.0f;

                double advance = 0.0f;
                // const float advance = glyph->getAdvance();
                auto nextChar = text->m_text[i + 1];
                if(!lockedSpec->tryGetGlyph(nextChar))
                {
                    nextChar = '?';
                }
                specGeometry.getAdvance(advance, c, nextChar);

                curX += fsScale * advance;
            }
            // curX += (float) (glyph->m_advance.x >> 6);
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

    subPassShader->useTextureBlock("u_fontSpecializationAtlas", 0);
    
    lockedParentSpec->m_atlasTexture->bind(0);

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
