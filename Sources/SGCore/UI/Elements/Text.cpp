//
// Created by stuka on 16.03.2025.
//

#include "Text.h"

#include "SGCore/UI/FontSpecializationRenderer.h"

SGCore::UI::Text::Text() noexcept
{
    m_selector = AssetManager::getInstance()->getOrAddAssetByAlias<CSSSelector>("sgui_default_text_style");
}

bool SGCore::UI::Text::draw(const LayeredFrameReceiver::reg_t& cameraReceiver,
                            const Transform& elementTransform,
                            UIElementCache& elementCache) noexcept
{
    if(!m_selector) return true;

    AssetRef<Font> font = m_selector->m_font.lock();

    if(font)
    {
        Ref<FontSpecialization> fontSpec = font->getSpecialization(m_selector->getFontSpecializationSettings());

        if(fontSpec)
        {
            // filling the glyphs vector if vector does not match ===
            if(m_glyphs.size() != m_text.size())
            {
                m_glyphs.clear();

                for(size_t i = 0; i < m_text.size(); ++i)
                {
                    m_glyphs.push_back(fontSpec->tryGetGlyph(m_text[i]));
                }
            }
            // ======================================================

            fontSpec->getRenderer()->drawText(this, elementTransform, elementCache);
        }
        else
        {
            // if font specialization was deleted than clearing the glyphs vector
            m_glyphs.clear();
        }
    }

    return true;
}

const SGCore::UI::FontGlyph* SGCore::UI::Text::getGlyph(std::size_t glyphIndex) const noexcept
{
    return m_glyphs[glyphIndex];
}

size_t SGCore::UI::Text::getGlyphsCount() const noexcept
{
    return m_glyphs.size();
}

void SGCore::UI::Text::doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                                         const Transform* parentTransform, Transform& ownTransform) noexcept
{
    if(m_selector)
    {
        m_selector->calculateCache(parentElementCache, thisElementCache);

        thisElementCache.m_finalSize = m_textSize;
    }
    else
    {

    }
}

void SGCore::UI::Text::doGenerateMeshBaseSelector(const UIElementCache* parentElementCache,
                                                  UIElementCache& thisElementCache) noexcept
{

}

void SGCore::UI::Text::doGenerateBasicMesh() noexcept
{

}
