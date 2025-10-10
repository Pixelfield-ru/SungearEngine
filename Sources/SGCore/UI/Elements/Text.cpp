//
// Created by stuka on 16.03.2025.
//

#include "Text.h"

#include "SGCore/UI/FontSpecializationRenderer.h"

SGCore::UI::Text::Text() noexcept
{
    m_mainStyle = AssetManager::getInstance()->getOrAddAssetByAlias<CSSStyle>("sgui_default_text_style");
}

bool SGCore::UI::Text::draw(const LayeredFrameReceiver::reg_t& cameraReceiver,
                            const Transform& elementTransform,
                            UIElementCache& elementCache) noexcept
{
    if(elementCache.m_currentFrameStyles.empty()) return true;

    auto* lastStyle = elementCache.m_currentFrameStyles.back();

    AssetRef<Font> font = lastStyle->m_font.lock();

    if(font)
    {
        Ref<FontSpecialization> fontSpec = font->getSpecialization(lastStyle->getFontSpecializationSettings());

        if(fontSpec)
        {
            // filling the glyphs vector if vector does not match ===
            if(m_glyphs.size() != m_text.size())
            {
                clearGlyphs();

                for(size_t i = 0; i < m_text.size(); ++i)
                {
                    m_glyphs.push_back(fontSpec->tryGetGlyph(m_text[i]));
                    m_lineBreaks.push_back(false);
                }
            }
            // ======================================================

            fontSpec->getRenderer()->drawText(this, elementTransform, elementCache);
        }
        else
        {
            clearGlyphs();
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

void SGCore::UI::Text::clearGlyphs() noexcept
{
    m_glyphs.clear();
    m_lineBreaks.clear();
}

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::Text::copy() const noexcept
{
    auto element = MakeRef<Text>();
    UIElement::doCopy(element);
    doCopy(element);

    return element;
}

void SGCore::UI::Text::doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                                         const Transform* parentTransform, Transform& ownTransform) noexcept
{
    for(auto* style : thisElementCache.m_currentFrameStyles)
    {
        style->calculateCache(parentElementCache, thisElementCache);
    }

    if(!thisElementCache.m_currentFrameStyles.empty())
    {
        thisElementCache.m_finalSize = m_textSize;
    }
}

void SGCore::UI::Text::doGenerateMesh(const UIElementCache* parentElementCache,
                                      UIElementCache& thisElementCache) noexcept
{

}

void SGCore::UI::Text::doCopy(const Ref<UIElement>& to) const noexcept
{
    auto* text = static_cast<Text*>(to.get());
    text->m_text = m_text;
}
