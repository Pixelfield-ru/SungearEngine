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
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Render/Batching/BatchInstanceTransform.h"

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

    // 6 vertices * 1 int
    m_charactersAdditionalParams.resize(m_maxCharactersCount * 6);

    // creating buffers
    
    {
        m_charactersVertexArray = std::shared_ptr<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
        m_charactersVertexArray->create();
        m_charactersVertexArray->bind();

        m_charactersMatricesVertexBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        m_charactersMatricesVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
        m_charactersMatricesVertexBuffer->create();
        m_charactersMatricesVertexBuffer->bind();
        m_charactersMatricesVertexBuffer->putData(m_charactersMatrices);
        m_charactersVertexArray->addVertexBuffer(m_charactersMatricesVertexBuffer.get());

        // 1 row of model matrix
        m_charactersMatricesVertexBuffer->addAttribute(0, 4, SGGDataType::SGG_FLOAT, false, 16 * 4, 0);

        // 2 row of model matrix
        m_charactersMatricesVertexBuffer->addAttribute(1, 4, SGGDataType::SGG_FLOAT, false, 16 * 4, 4 * 4);

        // 3 row of model matrix
        m_charactersMatricesVertexBuffer->addAttribute(2, 4, SGGDataType::SGG_FLOAT, false, 16 * 4, 8 * 4);

        // 4 row of model matrix
        m_charactersMatricesVertexBuffer->addAttribute(3, 4, SGGDataType::SGG_FLOAT, false, 16 * 4, 12 * 4);

        m_charactersMatricesVertexBuffer->useAttributes();
        
        // ======
        
        m_charactersColorsVertexBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        
        m_charactersColorsVertexBuffer->setUsage(SGGUsage::SGG_STATIC);
        m_charactersColorsVertexBuffer->create();
        m_charactersColorsVertexBuffer->bind();
        m_charactersColorsVertexBuffer->putData(m_charactersColors);
        m_charactersVertexArray->addVertexBuffer(m_charactersColorsVertexBuffer.get());

        m_charactersColorsVertexBuffer->addAttribute(4, 4, SGGDataType::SGG_FLOAT, false, 4 * sizeof(float), 0);
        m_charactersColorsVertexBuffer->useAttributes();
        
        // ======
        
        // =========================
        
        m_charactersUVsVertexBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        m_charactersUVsVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
        m_charactersUVsVertexBuffer->create();
        m_charactersUVsVertexBuffer->bind();
        m_charactersUVsVertexBuffer->putData(m_charactersUVs);
        m_charactersVertexArray->addVertexBuffer(m_charactersUVsVertexBuffer.get());

        m_charactersUVsVertexBuffer->addAttribute(5, 2, SGGDataType::SGG_FLOAT, false, 2 * sizeof(float), 0);
        m_charactersUVsVertexBuffer->useAttributes();
        
        // =========================
        
        m_charactersPositionsBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );
        m_charactersPositionsBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
        m_charactersPositionsBuffer->create();
        m_charactersPositionsBuffer->bind();
        m_charactersPositionsBuffer->putData(m_charactersVerticesPositions);
        m_charactersVertexArray->addVertexBuffer(m_charactersPositionsBuffer.get());

        m_charactersPositionsBuffer->addAttribute(6, 3, SGGDataType::SGG_FLOAT, false, 3 * sizeof(float), 0);
        m_charactersPositionsBuffer->useAttributes();

        // =========================

        m_charactersAdditionalParamsVertexBuffer = std::shared_ptr<IVertexBuffer>(
                CoreMain::getRenderer()->createVertexBuffer()
        );

        m_charactersAdditionalParamsVertexBuffer->setUsage(SGGUsage::SGG_DYNAMIC);
        m_charactersAdditionalParamsVertexBuffer->create();
        m_charactersAdditionalParamsVertexBuffer->bind();
        m_charactersAdditionalParamsVertexBuffer->putData(m_charactersAdditionalParams);
        m_charactersVertexArray->addVertexBuffer(m_charactersAdditionalParamsVertexBuffer.get());

        m_charactersAdditionalParamsVertexBuffer->addAttribute(7, 2, SGGDataType::SGG_FLOAT, false, 2 * sizeof(float), 0);
        m_charactersAdditionalParamsVertexBuffer->useAttributes();
    }
    
    // ==================================================================
    
    RenderPipelinesManager::instance().subscribeToRenderPipelineSetEvent(m_onRenderPipelineSetEventListener);
    
    // ==================================================================

    m_meshRenderState.m_useIndices = false;
    m_meshRenderState.m_useFacesCulling = false;
    m_meshRenderState.m_drawMode = SGDrawMode::SGG_TRIANGLES;

    auto renderPipeline = RenderPipelinesManager::instance().getCurrentRenderPipeline();

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

    const std::u32string* usedText = text->m_text.get();

    if(!usedText || usedText->empty())
    {
        text->m_textSize.y = 0;
        return;
    }
    
    double curX = 0;
    double curY = 0;

    const auto& specGeometry = lockedSpec->getGeometry();

    const float fontScale = textCache.m_fontSize / (float) lockedSpec->getSettings().m_height;

    const float lineHeight = lockedSpec->getLineHeight() * fontScale;
    const float glyphsHeightScale = lockedSpec->getGlyphsHeightScale() * fontScale;

    text->m_textSize.y = lineHeight + (lockedSpec->getAscenderYPos() - lockedSpec->getDescenderYPos()) * fontScale;

    const FontGlyph* unknownGlyph = lockedSpec->tryGetGlyph('?');
    const FontGlyph* spaceGlyph = lockedSpec->tryGetGlyph(' ');

    for(size_t i = 0; i < usedText->size(); ++i)
    {
        auto c = (*usedText)[i];

        if(m_currentDrawingCharacter >= m_maxCharactersCount - 1) return;

        if(c == '\n' || text->m_lineBreaks[i])
        {
            curX = 0.0f;
            curY -= lineHeight;

            text->m_textSize.y += lineHeight;

            // if we will continue always then character with line break will not be drawn
            if(c == '\n')
            {
                continue;
            }
        }

        const FontGlyph* glyph = text->getGlyph(i);

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

        float* characterAdditionalParamsBuf = &m_charactersAdditionalParams[m_currentDrawingCharacter * 6 * 2];

        characterAdditionalParamsBuf[0] = textCache.m_layer;
        characterAdditionalParamsBuf[1] = textCache.m_fontSize;

        characterAdditionalParamsBuf[2] = textCache.m_layer;
        characterAdditionalParamsBuf[3] = textCache.m_fontSize;

        characterAdditionalParamsBuf[4] = textCache.m_layer;
        characterAdditionalParamsBuf[5] = textCache.m_fontSize;

        characterAdditionalParamsBuf[6] = textCache.m_layer;
        characterAdditionalParamsBuf[7] = textCache.m_fontSize;

        characterAdditionalParamsBuf[8] = textCache.m_layer;
        characterAdditionalParamsBuf[9] = textCache.m_fontSize;

        characterAdditionalParamsBuf[10] = textCache.m_layer;
        characterAdditionalParamsBuf[11] = textCache.m_fontSize;

        if(glyph)
        {
            const glm::vec2 quadMin = glyph->getQuadMin() * fontScale + glm::vec2(curX, -curY);
            const glm::vec2 quadMax = glyph->getQuadMax() * fontScale + glm::vec2(curX, -curY);

            // colors =====================================================
#pragma region Colors
            const size_t colorIdx = m_currentDrawingCharacter * 24;

            float* charsColors = &m_charactersColors[colorIdx];

            charsColors[0] = textCache.m_color.r;
            charsColors[1] = textCache.m_color.g;
            charsColors[2] = textCache.m_color.b;
            charsColors[3] = textCache.m_color.a;

            charsColors[4] = textCache.m_color.r;
            charsColors[5] = textCache.m_color.g;
            charsColors[6] = textCache.m_color.b;
            charsColors[7] = textCache.m_color.a;

            charsColors[8] = textCache.m_color.r;
            charsColors[9] = textCache.m_color.g;
            charsColors[10] = textCache.m_color.b;
            charsColors[11] = textCache.m_color.a;

            charsColors[12] = textCache.m_color.r;
            charsColors[13] = textCache.m_color.g;
            charsColors[14] = textCache.m_color.b;
            charsColors[15] = textCache.m_color.a;

            charsColors[16] = textCache.m_color.r;
            charsColors[17] = textCache.m_color.g;
            charsColors[18] = textCache.m_color.b;
            charsColors[19] = textCache.m_color.a;

            charsColors[20] = textCache.m_color.r;
            charsColors[21] = textCache.m_color.g;
            charsColors[22] = textCache.m_color.b;
            charsColors[23] = textCache.m_color.a;
#pragma endregion Colors

            // matrices =====================================================
#pragma region Matrices

            const size_t matrixIdx = m_currentDrawingCharacter * 16 * 6;

            // for 6 vertices
            for(std::uint8_t i = 0; i < 6; ++i)
            {
                std::memcpy(&m_charactersMatrices[matrixIdx + (16 * i)],
                            glm::value_ptr(textTransform.m_finalTransform.m_animatedModelMatrix), 16 * sizeof(float));
            }

#pragma endregion Matrices

            // uvs =====================================================
#pragma region UVs

            const size_t uvsOffset = m_currentDrawingCharacter * 12;

            float* uvsPos = &m_charactersUVs[uvsOffset];

            uvsPos[0] = glyph->getUVMin().x;
            uvsPos[1] = glyph->getUVMax().y;

            uvsPos[2] = glyph->getUVMin().x;
            uvsPos[3] = glyph->getUVMin().y;

            uvsPos[4] = glyph->getUVMax().x;
            uvsPos[5] = glyph->getUVMin().y;

            uvsPos[6] = glyph->getUVMin().x;
            uvsPos[7] = glyph->getUVMax().y;

            uvsPos[8] = glyph->getUVMax().x;
            uvsPos[9] = glyph->getUVMax().y;

            uvsPos[10] = glyph->getUVMax().x;
            uvsPos[11] = glyph->getUVMin().y;

#pragma endregion UVs

            // positions =====================================================
#pragma region Positions

            size_t positionsOffset = m_currentDrawingCharacter * 18;

            float* verticesPos = &m_charactersVerticesPositions[positionsOffset];

            verticesPos[0] = quadMin.x;
            verticesPos[1] = quadMin.y;
            verticesPos[2] = 0.0;

            verticesPos[3] = quadMin.x;
            verticesPos[4] = quadMax.y;
            verticesPos[5] = 0.0f;

            verticesPos[6] = quadMax.x;
            verticesPos[7] = quadMax.y;
            verticesPos[8] = 0.0f;

            verticesPos[9] = quadMin.x;
            verticesPos[10] = quadMin.y;
            verticesPos[11] = 0.0;

            verticesPos[12] = quadMax.x;
            verticesPos[13] = quadMin.y;
            verticesPos[14] = 0.0f;

            verticesPos[15] = quadMax.x;
            verticesPos[16] = quadMax.y;
            verticesPos[17] = 0.0f;

#pragma endregion Positions

            ++m_currentDrawingCharacter;

            // float kerningOffset = 0.0f;

            double advance = 0.0f;
            // const float advance = glyph->getAdvance();
            // TODO: VERY HEAVY OPERATIONS =====
            /*auto nextChar = text->m_text[i + 1];
            if(!lockedSpec->tryGetGlyph(nextChar))
            {
                nextChar = '?';
            }
            specGeometry.getAdvance(advance, c, nextChar);*/
            // =================================
            advance = glyph->m_geometry.getAdvance();

            curX += glyphsHeightScale * advance;

            if(curX > text->m_textSize.x)
            {
                text->m_textSize.x = curX;
            }
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

    m_charactersAdditionalParamsVertexBuffer->bind();
    m_charactersAdditionalParamsVertexBuffer->subData(m_charactersAdditionalParams.data(), charsCount * 12, 0);
    
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
    m_textShader = AssetManager::getInstance()->loadAsset<IShader>(*RenderPipelinesManager::instance().getCurrentRenderPipeline()->m_shadersPaths["StandardTextShader"]);
    
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
